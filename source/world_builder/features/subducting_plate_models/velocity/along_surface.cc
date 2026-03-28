/*
  Copyright (C) 2018-2026 by the authors of the World Builder code.

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

#include "world_builder/features/subducting_plate_models/velocity/along_surface.h"


#include "world_builder/nan.h"
#include "world_builder/types/array.h"
#include "world_builder/types/double.h"
#include "world_builder/types/object.h"
#include "world_builder/utilities.h"


namespace WorldBuilder
{

  using namespace Utilities;

  namespace Features
  {
    namespace SubductingPlateModels
    {
      namespace Velocity
      {
        AlongSurface::AlongSurface(WorldBuilder::World *world_)
          :
          min_depth(NaN::DSNAN),
          max_depth(NaN::DSNAN),
          velocity_magnitude(NaN::DSNAN),
          operation(Operations::REPLACE)
        {
          this->world = world_;
          this->name = "along surface";
        }

        AlongSurface::~AlongSurface()
          = default;

        void
        AlongSurface::declare_entries(Parameters &prm, const std::string & /*unused*/)
        {
          // Document plugin and require entries if needed.
          // Add `velocity magnitude` and to the required parameters.
          prm.declare_entry("", Types::Object({"velocity magnitude"}),
                            "Uniform velocity model. Set the velocity to a constant value.");

          // Declare entries of this plugin
          prm.declare_entry("min distance slab top", Types::Double(0),
                            "todo The depth in meters from which the composition of this feature is present.");

          prm.declare_entry("max distance slab top", Types::Double(std::numeric_limits<double>::max()),
                            "todo The depth in meters to which the composition of this feature is present.");

          prm.declare_entry("velocity magnitude", Types::Double(0),
                            "The velocity in meter per year");

        }

        void
        AlongSurface::parse_entries(Parameters &prm)
        {

          min_depth = prm.get<double>("min distance slab top");
          max_depth = prm.get<double>("max distance slab top");
          operation = string_operations_to_enum(prm.get<std::string>("operation"));
          velocity_magnitude = prm.get<double>("velocity magnitude");
        }


        std::array<double,3>
        AlongSurface::get_velocity(const Point<3> & /*position_in_cartesian_coordinates*/,
                                   const double  /*depth*/,
                                   const double  /*gravity*/,
                                   std::array<double,3> velocity_,
                                   const double  /*feature_min_depth*/,
                                   const double  /*feature_max_depth*/,
                                   const WorldBuilder::Utilities::PointDistanceFromCurvedPlanes &distance_from_plane,
                                   const AdditionalParameters & /*additional_parameters*/) const
        {

          if (distance_from_plane.distance_from_plane <= max_depth && distance_from_plane.distance_from_plane >= min_depth)
            {
              const double angle = distance_from_plane.angle;

              const double sign_vx = -1.0; // place holder for modify vx direction
              const double vx = sign_vx * velocity_magnitude * std::cos(angle);
              const double vy = 0.0;
              const double vz = -velocity_magnitude * std::sin(angle);

              return {{
                  apply_operation(operation,velocity_[0],vx),
                  apply_operation(operation,velocity_[1],vy),
                  apply_operation(operation,velocity_[2],vz)
                }
              };
            }

          return velocity_;
        }

        WB_REGISTER_FEATURE_SUBDUCTING_PLATE_VELOCITY_MODEL(AlongSurface, along surface)
      } // namespace Velocity
    } // namespace SubductingPlateModels
  } // namespace Features
} // namespace WorldBuilder

