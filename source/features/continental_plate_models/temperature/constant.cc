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

#include <world_builder/features/continental_plate_models/temperature/constant.h>

#include <boost/algorithm/string.hpp>

#include <world_builder/utilities.h>
#include <world_builder/assert.h>
#include <world_builder/nan.h>
#include <world_builder/parameters.h>

#include <world_builder/types/double.h>
#include <world_builder/types/string.h>


namespace WorldBuilder
{
  using namespace Utilities;

  namespace Features
  {
    namespace ContinentalPlateModels
    {
      namespace Temperature
      {
        Constant::Constant(WorldBuilder::World *world_)
          :
          depth(NaN::DSNAN),
          temperature(NaN::DSNAN),
          operation("")
        {
          this->world = world_;
          this->name = "continental plate";
        }

        Constant::~Constant()
        { }

        void
        Constant::declare_entries(Parameters &prm)
        {

          prm.declare_entry("temperature","",true,Types::Double(),
                            "The depth in meters to which the temperature of this feature is present.");
          prm.declare_entry("depth","",true,Types::Double(),
                            "The temperature in degree Kelvin which this feature should have");
          prm.declare_entry("operation","replace",false,Types::String(),
                            "Wheter the value should replace any value previously defined at this location (replace) or "
                            "add the value to the previously define value (add).");

        }

        void
        Constant::parse_entries(Parameters &prm)
        {
          //Parameters &prm = this->world->parameters;

          prm.load_entry("depth", true, Types::Double(NaN::DSNAN,"The depth in meters to which the temperature of this feature is present."));
          depth = prm.get_double("depth");

          prm.load_entry("temperature", true, Types::Double(0,"The temperature in degree Kelvin which this feature should have"));
          temperature = prm.get_double("temperature");
        }


        double
        Constant::get_temperature(const Point<3> &position,
                                  const double depth,
                                  const double gravity_norm,
                                  double temperature_) const
        {

          WorldBuilder::Utilities::NaturalCoordinate natural_coordinate = WorldBuilder::Utilities::NaturalCoordinate(position,
                                                                          *(world->parameters.coordinate_system));

          // The constant temperature module should be used for this.
          //if (depth <= constant_depth &&
          //  Utilities::polygon_contains_point(coordinates, Point<2>(natural_coordinate.get_surface_coordinates(),
          //                                                        world->parameters.coordinate_system->natural_coordinate_system())))
          {
            // We are in the the area where the contintal plate is defined. Set the constant temperature.
            return temperature;
          }


          return temperature_;
        }

        WB_REGISTER_FEATURE_CONTINENTAL_TEMPERATURE_MODEL(Constant, constant)
      }
    }
  }
}

