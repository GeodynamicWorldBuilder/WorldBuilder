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
                            "The minimum distance in meters from the slab top at which this velocity model is applied.");

          prm.declare_entry("max distance slab top", Types::Double(std::numeric_limits<double>::max()),
                            "The maximum distance in meters from the slab top at which this velocity model is applied.");

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
        AlongSurface::get_velocity(const Point<3> &/*position_in_cartesian_coordinates*/,
                                   const Objects::NaturalCoordinate &position_in_natural_coordinates,
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
              const double angle_x_axis = distance_from_plane.angle_x_axis;

              WBAssertThrow(std::isfinite(angle), "Invalid angle value: angle = " << angle);
              WBAssertThrow(std::isfinite(angle_x_axis), "Invalid angle value: angle_x_axis = " << angle_x_axis);

              double vx = 0.0;
              double vy = 0.0;
              double vz = 0.0;
              if (position_in_natural_coordinates.get_coordinate_system() == CoordinateSystem::cartesian)
                {
                  vx = -velocity_magnitude * std::cos(angle) * std::cos(angle_x_axis);
                  vy = -velocity_magnitude * std::cos(angle) * std::sin(angle_x_axis);
                  vz = -velocity_magnitude * std::sin(angle);
                }
              else
                {
                  const double vn = -velocity_magnitude * std::cos(angle) * std::cos(angle_x_axis);
                  const double ve = -velocity_magnitude * std::cos(angle) * std::sin(angle_x_axis);
                  const double vr = -velocity_magnitude * std::sin(angle);

                  const std::array<double, 3> V_cartesian = local_spherical_vector_to_cartesian(position_in_natural_coordinates.get_coordinates(),
                  std::array<double, 3> {{vr, vn, ve}});

                  vx = V_cartesian[0];
                  vy = V_cartesian[1];
                  vz = V_cartesian[2];
                }

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
