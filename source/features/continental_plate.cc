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
      temperature_submodule_constant_depth(NaN::DSNAN),
      temperature_submodule_constant_temperature(NaN::DSNAN),
      composition_submodule_constant_depth(NaN::DSNAN),
      composition_submodule_constant_composition(NaN::ISNAN)
    {
      this->world = world_;
      this->name = "continental plate";
    }

    ContinentalPlate::~ContinentalPlate()
    { }


    void
    ContinentalPlate::decare_entries()
    {
      Parameters &prm = this->world->parameters;

      const CoordinateSystem coordinate_system = prm.coordinate_system->natural_coordinate_system();

      prm.load_entry("name", true, Types::String("","The name which the user has given to the feature."));
      name = prm.get_string("name");
      bool set = prm.load_entry("coordinates", true, Types::Array(
                                  Types::Point<2>(Point<2>(0,0, coordinate_system),"desciption point cross section"),
                                  "An array of Points representing an array of coordinates where the feature is located."));

      WBAssertThrow(set == true, "A list of coordinates is required for every feature.");

      std::vector<const Types::Point<2>* > typed_coordinates =  prm.get_array<const Types::Point<2> >("coordinates");

      coordinates.resize(typed_coordinates.size(),Point<2>(coordinate_system));
      for (unsigned int i = 0; i < typed_coordinates.size(); ++i)
        {
          coordinates[i] = typed_coordinates[i]->value * (coordinate_system == CoordinateSystem::spherical ? M_PI / 180.0 : 1.0);
        }

      prm.enter_subsection("temperature submodule");
      {
        prm.load_entry("name", true, Types::String("","The name of the temperature submodule."));
        temperature_submodule_name = prm.get_string("name");

        if (temperature_submodule_name == "constant")
          {
            prm.load_entry("depth", true, Types::Double(NaN::DSNAN,"The depth in meters to which the temperature of this feature is present."));
            temperature_submodule_constant_depth = prm.get_double("depth");

            prm.load_entry("temperature", true, Types::Double(0,"The temperature in degree Kelvin which this feature should have"));
            temperature_submodule_constant_temperature = prm.get_double("temperature");
          }
        else if (temperature_submodule_name == "linear")
          {
            prm.load_entry("depth", true, Types::Double(NaN::DSNAN,"The depth in meters to which the temperature rises (or lowers) to."));
            temperature_submodule_linear_depth = prm.get_double("depth");

            prm.load_entry("top temperature", false, Types::Double(293.15,"The temperature in degree Kelvin a the top of this block. If this value is not set, the "));
            temperature_submodule_linear_top_temperature = prm.get_double("top temperature");


            prm.load_entry("bottom temperature", false, Types::Double(NaN::DQNAN,"The temperature in degree Kelvin a the bottom of this block."));
            temperature_submodule_linear_bottom_temperature = prm.get_double("bottom temperature");
          }

      }
      prm.leave_subsection();

      prm.enter_subsection("composition submodule");
      {
        prm.load_entry("name", true, Types::String("","The name of the composition submodule used."));
        composition_submodule_name = prm.get_string("name");

        if (composition_submodule_name == "constant")
          {
            prm.load_entry("depth", true, Types::Double(NaN::DSNAN,"The depth in meters to which the composition of this feature is present."));
            composition_submodule_constant_depth = prm.get_double("depth");

            prm.load_entry("value", false, Types::Double(1.0,"The value between 0 and 1 of how much this composition is present."));
            composition_submodule_constant_value = prm.get_double("value");

            prm.load_entry("composition", true, Types::UnsignedInt(0,"The number of the composition that is present there."));
            composition_submodule_constant_composition = prm.get_unsigned_int("composition");
          }
        else if (composition_submodule_name == "constant layers")
          {
            // Load the layers.
            prm.load_entry("layers", true, Types::Array(Types::ConstantLayer(NaN::ISNAN,1.0,NaN::DSNAN,
                                                                             "A plate constant layer with a certain composition and thickness."),
                                                        "A list of layers."));

            std::vector<const Types::ConstantLayer *> constant_layers = prm.get_array<const Types::ConstantLayer>("layers");

            composition_submodule_constant_layers_compositions.resize(constant_layers.size());
            composition_submodule_constant_layers_thicknesses.resize(constant_layers.size());
            composition_submodule_constant_layers_value.resize(constant_layers.size());

            for (unsigned int i = 0; i < constant_layers.size(); ++i)
              {
                composition_submodule_constant_layers_compositions[i] = constant_layers[i]->value_composition;
                composition_submodule_constant_layers_thicknesses[i] = constant_layers[i]->value_thickness;
                composition_submodule_constant_layers_value[i] = constant_layers[i]->value;
              }
          }
        else
          {
            WBAssertThrow(composition_submodule_name == "none","Subducting plate temperature model '" << temperature_submodule_name << "' not found.");
          }
      }
      prm.leave_subsection();
    }


    double
    ContinentalPlate::temperature(const Point<3> &position,
                                  const double depth,
                                  const double gravity_norm,
                                  double temperature) const
    {
      if (temperature_submodule_name == "constant")
        {
          WorldBuilder::Utilities::NaturalCoordinate natural_coordinate = WorldBuilder::Utilities::NaturalCoordinate(position,
                                                                          *(world->parameters.coordinate_system));

          // The constant temperature module should be used for this.
          if (depth <= temperature_submodule_constant_depth &&
              Utilities::polygon_contains_point(coordinates, Point<2>(natural_coordinate.get_surface_coordinates(),
                                                                      world->parameters.coordinate_system->natural_coordinate_system())))
            {
              // We are in the the area where the contintal plate is defined. Set the constant temperature.
              return temperature_submodule_constant_temperature;
            }
        }
      else if (temperature_submodule_name == "linear")
        {
          WorldBuilder::Utilities::NaturalCoordinate natural_coordinate = WorldBuilder::Utilities::NaturalCoordinate(position,
                                                                          *(world->parameters.coordinate_system));

          // The linear temperature module should be used for this.
          if (depth <= temperature_submodule_linear_depth &&
              Utilities::polygon_contains_point(coordinates, Point<2>(natural_coordinate.get_surface_coordinates(),
                                                                      world->parameters.coordinate_system->natural_coordinate_system())))
            {
              double bottom_temperature = temperature_submodule_linear_bottom_temperature;
              if (std::isnan(temperature_submodule_linear_bottom_temperature))
                {
                  bottom_temperature =  this->world->parameters.get_double("potential mantle temperature") +
                                        (((this->world->parameters.get_double("potential mantle temperature") * this->world->parameters.get_double("Thermal expansion coefficient alpha") * gravity_norm) /
                                          this->world->parameters.get_double("specific heat Cp")) * 1000.0) * ((depth) / 1000.0);
                }

              return temperature_submodule_linear_top_temperature +
                     depth * ((bottom_temperature - temperature_submodule_linear_top_temperature) / temperature_submodule_linear_depth);
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

    double
    ContinentalPlate::composition(const Point<3> &position,
                                  const double depth,
                                  const unsigned int composition_number,
                                  double composition) const
    {
      if (composition_submodule_name == "constant")
        {
          WorldBuilder::Utilities::NaturalCoordinate natural_coordinate = WorldBuilder::Utilities::NaturalCoordinate(position,*(world->parameters.coordinate_system));
          // The constant temperature module should be used for this.
          if (depth <= composition_submodule_constant_depth &&
              Utilities::polygon_contains_point(coordinates, Point<2>(natural_coordinate.get_surface_coordinates(),world->parameters.coordinate_system->natural_coordinate_system())))
            {
              // We are in the the area where the contintal plate is defined. Set the constant temperature.
              if (composition_submodule_constant_composition == composition_number)
                {
                  return composition_submodule_constant_value;
                }
            }

        }
      else if (composition_submodule_name == "constant layers")
        {
          // find out what layer we are in.
          double total_thickness = 0;
          for (unsigned int i = 0; i < composition_submodule_constant_layers_compositions.size(); ++i)
            {
              WorldBuilder::Utilities::NaturalCoordinate natural_coordinate = WorldBuilder::Utilities::NaturalCoordinate(position,*(world->parameters.coordinate_system));

              // Check wether we are in the correct layer
              if (depth >= total_thickness
                  && depth < total_thickness + composition_submodule_constant_layers_thicknesses[i]
                  && Utilities::polygon_contains_point(coordinates,
                                                       Point<2>(natural_coordinate.get_surface_coordinates(),
                                                                world->parameters.coordinate_system->natural_coordinate_system())))
                {
                  // We are in a layer. Check whether this is the correct composition.
                  if (composition_submodule_constant_layers_compositions[i] == composition_number)
                    {
                      return composition_submodule_constant_layers_value[i];
                    }
                }
              total_thickness += composition_submodule_constant_layers_thicknesses[i];
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

