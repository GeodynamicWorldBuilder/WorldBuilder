/*
  Copyright (C) 2018-2024 by the authors of the World Builder code.

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

#include "world_builder/features/oceanic_plate_models/velocity/uniform_raw.h"

#include "world_builder/nan.h"
#include "world_builder/types/array.h"
#include "world_builder/types/double.h"
#include "world_builder/types/object.h"
#include "world_builder/types/one_of.h"
#include "world_builder/types/value_at_points.h"


namespace WorldBuilder
{

  using namespace Utilities;

  namespace Features
  {
    namespace OceanicPlateModels
    {
      namespace Velocity
      {
        UniformRaw::UniformRaw(WorldBuilder::World *world_)
          :
          min_depth(NaN::DSNAN),
          max_depth(NaN::DSNAN),
          velocity({{NaN::DSNAN,NaN::DSNAN,NaN::DSNAN}}),
        operation(Operations::REPLACE)
        {
          this->world = world_;
          this->name = "uniform raw";
        }

        UniformRaw::~UniformRaw()
          = default;

        void
        UniformRaw::declare_entries(Parameters &prm, const std::string & /*unused*/)
        {
          // Document plugin and require entries if needed.
          // Add `velocity` and to the required parameters.
          prm.declare_entry("", Types::Object({"velocity"}),
                            "Uniform velocity model. Set the velocity to a constant value.");

          // Declare entries of this plugin
          prm.declare_entry("min depth", Types::OneOf(Types::Double(0),
                                                      Types::Array(Types::ValueAtPoints(0., 2.)),
                                                      Types::String("")),
                            "The depth in meters from which the composition of this feature is present.");

          prm.declare_entry("max depth", Types::OneOf(Types::Double(std::numeric_limits<double>::max()),
                                                      Types::Array(Types::ValueAtPoints(std::numeric_limits<double>::max(), 2.)),
                                                      Types::String("")),
                            "The depth in meters to which the composition of this feature is present.");


          prm.declare_entry("velocity", Types::Array(Types::Double(0.0),3,3),
                            "The velocity in meter per year");

        }

        void
        UniformRaw::parse_entries(Parameters &prm, const std::vector<Point<2>> &coordinates)
        {

          min_depth_surface = Objects::Surface(prm.get("min depth",coordinates));
          min_depth = min_depth_surface.minimum;
          max_depth_surface = Objects::Surface(prm.get("max depth",coordinates));
          max_depth = max_depth_surface.maximum;
          operation = string_operations_to_enum(prm.get<std::string>("operation"));
          std::vector<double> velocity_vec = prm.get_vector<double>("velocity");
          velocity[0] = velocity_vec[0];
          velocity[1] = velocity_vec[1];
          velocity[2] = velocity_vec[2];
        }


        std::array<double,3>
        UniformRaw::get_velocity(const Point<3> & /*position_in_cartesian_coordinates*/,
                                 const Objects::NaturalCoordinate &position_in_natural_coordinates,
                                 const double depth,
                                 const double  /*gravity*/,
                                 std::array<double,3> velocity_,
                                 const double /*feature_min_depth*/,
                                 const double /*feature_max_depth*/) const
        {

          if (depth <= max_depth && depth >= min_depth)
            {
              const double min_depth_local = min_depth_surface.constant_value ? min_depth : min_depth_surface.local_value(position_in_natural_coordinates.get_surface_point()).interpolated_value;
              const double max_depth_local = max_depth_surface.constant_value ? max_depth : max_depth_surface.local_value(position_in_natural_coordinates.get_surface_point()).interpolated_value;
              if (depth <= max_depth_local &&  depth >= min_depth_local)
                {
                  return {{
                      apply_operation(operation,velocity_[0],velocity[0]),
                      apply_operation(operation,velocity_[1],velocity[1]),
                      apply_operation(operation,velocity_[2],velocity[2])
                    }
                  };
                }
            }

          return velocity_;
        }

        WB_REGISTER_FEATURE_OCEANIC_PLATE_VELOCITY_MODEL(UniformRaw, uniform raw)
      } // namespace Velocity
    } // namespace OceanicPlateModels
  } // namespace Features
} // namespace WorldBuilder

