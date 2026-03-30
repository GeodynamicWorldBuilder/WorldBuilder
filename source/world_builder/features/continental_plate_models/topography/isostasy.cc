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

#include "world_builder/features/continental_plate_models/topography/isostasy.h"


#include "world_builder/features/continental_plate_models/topography/interface.h"
#include "world_builder/nan.h"
#include "world_builder/types/array.h"
#include "world_builder/types/double.h"
#include "world_builder/types/object.h"
#include "world_builder/types/one_of.h"
#include "world_builder/types/value_at_points.h"
#include "world_builder/world.h"
#include "world_builder/gravity_model/interface.h"


namespace WorldBuilder
{

  using namespace Utilities;

  namespace Features
  {
    namespace ContinentalPlateModels
    {
      namespace Topography
      {
        Isostasy::Isostasy(WorldBuilder::World *world_)
          :
          min_depth(NaN::DSNAN),
          max_depth(NaN::DSNAN),
          operation(Operations::REPLACE)
        {
          this->world = world_;
          this->name = "isostasy";
        }

        Isostasy::~Isostasy()
          = default;

        void
        Isostasy::declare_entries(Parameters &prm, const std::string & /*unused*/)
        {
          prm.declare_entry("", Types::Object(),
                            "Isostatic topography model based on vertical density-column integration.");

          prm.declare_entry("min depth", Types::OneOf(Types::Double(0),
                                                      Types::Array(Types::ValueAtPoints(0.,2)),
                                                      Types::String("")),
                            "The depth in meters from which this feature is present.");

          prm.declare_entry("max depth", Types::OneOf(Types::Double(std::numeric_limits<double>::max()),
                                                      Types::Array(Types::ValueAtPoints(std::numeric_limits<double>::max(),2)),
                                                      Types::String("")),
                            "The depth in meters to which this feature is present.");
        }

        void
        Isostasy::parse_entries(Parameters &prm, const std::vector<Point<2>> &coordinates)
        {
          min_depth_surface = Objects::Surface(prm.get("min depth",coordinates));
          min_depth = min_depth_surface.minimum;
          max_depth_surface = Objects::Surface(prm.get("max depth",coordinates));
          max_depth = max_depth_surface.maximum;
          operation = string_operations_to_enum(prm.get<std::string>("operation"));
        }

        double
        Isostasy::get_topography(const Point<3> &position_in_cartesian_coordinates,
                                 const Objects::NaturalCoordinate &position_in_natural_coordinates,
                                 double topography_) const
        {
          (void) position_in_natural_coordinates;

          WBAssertThrow(world->number_integration_points > 1,
                        "The number of integration points for isostasy must be > 1.");

          const double gravity = 
            this->world->parameters.gravity_model->gravity_norm(position_in_cartesian_coordinates);

          WBAssertThrow(std::fabs(gravity) > 0.0,
                        "Gravity norm must be non-zero for isostasy calculation.");

          const double dz =
            this->world->compensation_depth / static_cast<double>(this->world->number_integration_points - 1);

          double pressure_local = 0.0;

          double density_prev =
            world->properties(position_in_cartesian_coordinates.get_array(),
                              0.0,
                              {{{7,0,0}}})[0];

          for (unsigned int i = 1; i < world->number_integration_points; ++i)
            {
              const double z = i * dz;

              const double density =
                world->properties(position_in_cartesian_coordinates.get_array(),
                                  z,
                                  {{{7,0,0}}})[0];

              pressure_local += 0.5 * (density + density_prev) * dz * gravity;

              density_prev = density;
 
            }
          const double new_topography =
            (world->compensation_pressure - pressure_local) /
            (world->background_density * gravity);

          std::cout << "Isostasy: compensation_pressure = " << world->compensation_pressure
                    << ", pressure_local = " << pressure_local
                    << ", new_topography = " << new_topography
                    << ", background_density = " << world->background_density 
                    << ", gravity = " << gravity << std::endl;

          return apply_operation(operation, topography_, new_topography);
        }

        WB_REGISTER_FEATURE_CONTINENTAL_PLATE_TOPOGRAPHY_MODEL(Isostasy, isostasy)
      } // namespace Topography
    } // namespace ContinentalPlateModels
  } // namespace Features
} // namespace WorldBuilder

