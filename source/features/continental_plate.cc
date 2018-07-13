/*
  Copyright (C) 2018 by the authors of the World Builder code.

  This file is part of the World Builder.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation, either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <boost/algorithm/string.hpp>

#include <world_builder/features/continental_plate.h>
#include <world_builder/utilities.h>
#include <world_builder/assert.h>
#include <world_builder/nan.h>
#include <world_builder/parameters.h>


namespace WorldBuilder
{
  using namespace Utilities;

  namespace Features
  {
    ContinentalPlate::ContinentalPlate(WorldBuilder::World *world_)
      :
      temperature_submodule_depth(NaN::DSNAN),
      temperature_submodule_temperature(NaN::DSNAN),
      composition_submodule_depth(NaN::DSNAN),
      composition_submodule_composition(NaN::ISNAN)
    {
      this->world = world_;
      this->name = "continental plate";
    }

    ContinentalPlate::~ContinentalPlate()
    { }


    void
    ContinentalPlate::decare_entries(std::string &path)
    {
      Parameters &prm = this->world->parameters;
      prm.enter_subsection("continental plate");
      {

        prm.load_entry("name", true, Types::String("","The name which the user has given to the feature."));
        name = prm.get_string("name");
        bool set = prm.load_entry("coordinates", true, Types::Array(
                                    Types::Point<2>(Point<2>(0,0),"desciption point cross section"),
                                    "An array of Points representing an array of coordinates where the feature is located."));

        WBAssertThrow(set == true, "A list of coordinates is required for every feature.");

        std::vector<const Types::Point<2>* > typed_coordinates =  prm.get_array<const Types::Point<2> >("coordinates");

        coordinates.resize(typed_coordinates.size());
        for (unsigned int i = 0; i < typed_coordinates.size(); ++i)
          {
            coordinates[i] = typed_coordinates[i]->value;
          }

        prm.enter_subsection("temperature submodule");
        {
          prm.load_entry("name", true, Types::String("","The name of the temperature submodule."));
          temperature_submodule_name = prm.get_string("name");

          if (temperature_submodule_name == "constant")
            {
              prm.load_entry("depth", true, Types::Double(NaN::DSNAN,"The depth to which the temperature of this feature is present."));
              temperature_submodule_depth = prm.get_double("depth");

              prm.load_entry("temperature", true, Types::Double(0,"The temperature which this feature should have"));
              temperature_submodule_temperature = prm.get_double("temperature");
            }

        }
        prm.leave_subsection();

        prm.enter_subsection("composition submodule");
        {
          prm.load_entry("name", true, Types::String("",""));
          composition_submodule_name = prm.get_string("name");

          if (composition_submodule_name == "constant")
            {
              prm.load_entry("depth", true, Types::Double(NaN::DSNAN,"The depth to which the composition of this feature is present."));
              composition_submodule_depth = prm.get_double("depth");

              prm.load_entry("composition", true, Types::UnsignedInt(0,"The number of the composition that is present there."));
              composition_submodule_composition = prm.get_unsigned_int("composition");
            }
        }
        prm.leave_subsection();

      }
      prm.leave_subsection();
    }


    double
    ContinentalPlate::temperature(const Point<3> &position,
                                  const double depth,
                                  const double /*gravity*/,
                                  double temperature) const
    {
      if (temperature_submodule_name == "constant")
        {
          WorldBuilder::Utilities::NaturalCoordinate natural_coordinate = WorldBuilder::Utilities::NaturalCoordinate(position,*(world->parameters.coordinate_system));
          // The constant temperature module should be used for this.
          if (depth <= temperature_submodule_depth &&
              Utilities::polygon_contains_point(coordinates, natural_coordinate.get_surface_coordinates()))
            {
              // We are in the the area where the contintal plate is defined. Set the constant temperature.
              return temperature_submodule_temperature;
            }

        }
      else if (temperature_submodule_name == "none")
        {
          return temperature;
        }
      else
        {
          WBAssertThrow(false,"Given temperature module does not exist: " + temperature_submodule_name);
        }

      return temperature;
    }

    bool
    ContinentalPlate::composition(const Point<3> &position,
                                  const double depth,
                                  const unsigned int composition_number,
                                  bool composition) const
    {
      if (composition_submodule_name == "constant")
        {
          WorldBuilder::Utilities::NaturalCoordinate natural_coordinate = WorldBuilder::Utilities::NaturalCoordinate(position,*(world->parameters.coordinate_system));
          // The constant temperature module should be used for this.
          if (depth <= composition_submodule_depth &&
              Utilities::polygon_contains_point(coordinates, natural_coordinate.get_surface_coordinates()))
            {
              // We are in the the area where the contintal plate is defined. Set the constant temperature.
              if (composition_submodule_composition == composition_number)
                {
                  return true;
                }
            }

        }
      else if (composition_submodule_name == "none")
        {
          return composition;
        }
      else
        {
          WBAssertThrow(false,"Given composition module does not exist: " + composition_submodule_name);
        }

      return composition;
    }
  }
}

