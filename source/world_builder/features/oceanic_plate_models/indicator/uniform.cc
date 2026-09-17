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

#include "world_builder/features/oceanic_plate_models/indicator/uniform.h"

#include "world_builder/nan.h"
#include "world_builder/types/array.h"
#include "world_builder/types/double.h"
#include "world_builder/types/object.h"
#include "world_builder/types/one_of.h"
#include "world_builder/types/string.h"
#include "world_builder/types/unsigned_int.h"
#include "world_builder/types/value_at_points.h"
#include "world_builder/world.h"


namespace WorldBuilder
{
  using namespace Utilities;

  namespace Features
  {
    namespace OceanicPlateModels
    {
      namespace Indicator
      {
        Uniform::Uniform(WorldBuilder::World *world_)
          :
          min_depth(NaN::DSNAN),
          max_depth(NaN::DSNAN),
          operation(Operations::REPLACE)
        {
          this->world = world_;
          this->name = "uniform";
        }

        Uniform::~Uniform()
          = default;

        void
        Uniform::declare_entries(Parameters &prm, const std::string & /*unused*/)
        {
          // Document plugin and require entries if needed.
          prm.declare_entry("", Types::Object(),
                            "A depth-dependent indicator model. Sets a constant indicator value of 1 "
                            "for the selected user-defined indicators.");

          // Declare entries of this plugin
          prm.declare_entry("min depth", Types::OneOf(Types::Double(0),
                                                      Types::Array(Types::ValueAtPoints(0.,2)),
                                                      Types::String("")),
                            "The depth in meters from which the composition of this feature is present.");

          prm.declare_entry("max depth", Types::OneOf(Types::Double(std::numeric_limits<double>::max()),
                                                      Types::Array(Types::ValueAtPoints(std::numeric_limits<double>::max(),2)),
                                                      Types::String("")),
                            "The depth in meters to which the composition of this feature is present.");

          prm.declare_entry("indicators", Types::Array(Types::OneOf(Types::UnsignedInt(0),Types::String("")),0),
                            "A list of user-defined indicator indices or names. If omitted, indicator 0 is selected. "
                            "An explicitly empty list selects no indicators.");

          prm.declare_entry("operation", Types::String("replace", std::vector<std::string> {"replace", "replace defined only"}),
                            "Whether the value should replace any value previously defined at this location (replace) or "
                            "add the value to the previously define value. Replacing implies that all indicators not "
                            "explicitly defined are set to zero. To only replace the defined indicators use the replace only defined option.");
        }

        void
        Uniform::parse_entries(Parameters &prm, const std::vector<Point<2>> &coordinates)
        {
          min_depth_surface = Objects::Surface(prm.get("min depth",coordinates));
          min_depth = min_depth_surface.minimum;
          max_depth_surface = Objects::Surface(prm.get("max depth",coordinates));
          max_depth = max_depth_surface.maximum;
          WBAssert(max_depth >= min_depth, "max depth needs to be larger or equal to min depth.");
          if (prm.check_entry("indicators"))
            indicators = prm.get_vector<unsigned int>("indicators", world->indicator_properties);
          else
            {
              constexpr unsigned int default_indicator = 0;
              WBAssertThrow(world->indicator_properties.find(default_indicator) != world->indicator_properties.end(),
                            "Invalid default indicator index " << default_indicator
                            << ": no matching entry exists in indicator properties.");
              indicators = {default_indicator};
            }
          operation = string_operations_to_enum(prm.get<std::string>("operation"));
        }


        double
        Uniform::get_indicator(const Point<3> & /*position_in_cartesian_coordinates*/,
                               const Objects::NaturalCoordinate &position_in_natural_coordinates,
                               const double depth,
                               const unsigned int indicator_number,
                               double indicator_,
                               const double feature_min_depth,
                               const double feature_max_depth) const
        {
          if (depth <= max_depth && depth >= min_depth)
            {
              const double min_depth_local = min_depth_surface.constant_value ? min_depth : min_depth_surface.local_value(position_in_natural_coordinates.get_surface_point()).interpolated_value;
              const double max_depth_local = max_depth_surface.constant_value ? max_depth : max_depth_surface.local_value(position_in_natural_coordinates.get_surface_point()).interpolated_value;

              const double min_depth_local_local = std::max(feature_min_depth, min_depth_local);
              const double max_depth_local_local = std::min(feature_max_depth, max_depth_local);
              if (depth <= max_depth_local_local &&  depth >= min_depth_local_local)
                {
                  for (unsigned int i =0; i < indicators.size(); ++i)
                    {
                      if (indicators[i] == indicator_number)
                        {
                          return apply_operation(operation,indicator_,1.0);
                        }
                    }

                  if (operation == Operations::REPLACE)
                    return 0.0;
                }
            }
          else
            {
              if (operation == Operations::REPLACE)
                return 0.0;
            }

          return indicator_;
        }

        WB_REGISTER_FEATURE_OCEANIC_PLATE_INDICATOR_MODEL(Uniform, uniform)
      } // namespace Indicator
    } // namespace OceanicPlateModels
  } // namespace Features
} // namespace WorldBuilder
