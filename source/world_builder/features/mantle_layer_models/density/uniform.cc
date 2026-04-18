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

#include "world_builder/features/mantle_layer_models/density/uniform.h"

#include "world_builder/nan.h"
#include "world_builder/types/array.h"
#include "world_builder/types/double.h"
#include "world_builder/types/object.h"
#include "world_builder/types/one_of.h"
#include "world_builder/types/value_at_points.h"
#include "world_builder/world.h"

namespace WorldBuilder
{

  using namespace Utilities;

  namespace Features
  {
    namespace MantleLayerModels
    {
      namespace Density
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
          prm.declare_entry("compositions", Types::Array(Types::OneOf(Types::UnsignedInt(), Types::String("")),0),
                            "A list of indices or names of the composition which are present there.");

          // Declare entries of this plugin
          prm.declare_entry("min depth", Types::OneOf(Types::Double(0),
                                                      Types::Array(Types::ValueAtPoints(0.,2)),
                                                      Types::String("")),
                            "The depth in meters from which the composition of this feature is present.");

          prm.declare_entry("max depth", Types::OneOf(Types::Double(std::numeric_limits<double>::max()),
                                                      Types::Array(Types::ValueAtPoints(std::numeric_limits<double>::max(),2)),
                                                      Types::String("")),
                            "The depth in meters to which the composition of this feature is present.");

          prm.declare_entry("operation", Types::String("replace", std::vector<std::string> {"replace", "replace defined only", "add", "subtract"}),
                            "Whether the value should replace any value previously defined at this location (replace) or "
                            "add the value to the previously define value. Replacing implies that all compositions not "
                            "explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.");

        }

        void
        Uniform::parse_entries(Parameters &prm, const std::vector<Point<2>> &coordinates)
        {
          min_depth_surface = Objects::Surface(prm.get("min depth",coordinates));
          min_depth = min_depth_surface.minimum;
          max_depth_surface = Objects::Surface(prm.get("max depth",coordinates));
          max_depth = max_depth_surface.maximum;
          operation = string_operations_to_enum(prm.get<std::string>("operation"));
          compositions = prm.get_vector<unsigned int>("compositions", this->world->composition_properties);
        }

        double
        Uniform::get_density(const Point<3> &position_in_cartesian_coordinates,
                             const Objects::NaturalCoordinate &position_in_natural_coordinates,
                             const double depth,
                             const double  /*gravity*/,
                             double density_,
                             const double /*feature_min_depth*/,
                             const double /*feature_max_depth*/) const
        {
          // If the composition is greater than 0, average it into the density.
          // By calling the world property for composition, fractions will be included.
          const double min_depth_local = min_depth_surface.constant_value ? min_depth : min_depth_surface.local_value(position_in_natural_coordinates.get_surface_point()).interpolated_value;
          const double max_depth_local = max_depth_surface.constant_value ? max_depth : max_depth_surface.local_value(position_in_natural_coordinates.get_surface_point()).interpolated_value;
          if (depth <= max_depth_local &&  depth >= min_depth_local)
            {
              double compositional_density = 0.0;
              double sum_compositions = 0.0;
              for (unsigned int i = 0; i < compositions.size(); ++i)
                {
                  if (world->properties(position_in_cartesian_coordinates.get_array(), depth, {{{2, compositions[i], 0}}})[0] > 0.0)
                  {
                    const double density = this->world->composition_properties[compositions[i]].reference_density;
                    compositional_density += world->properties(position_in_cartesian_coordinates.get_array(), depth, {{{2, compositions[i], 0}}})[0] * density;
                    sum_compositions += world->properties(position_in_cartesian_coordinates.get_array(), depth, {{{2, compositions[i], 0}}})[0];
                  }
                }

              // Add in the background_density component.
              compositional_density += this->world->background_density * (1 - sum_compositions);
              return apply_operation(operation,density_,compositional_density);
            }

          return density_;
        }

        WB_REGISTER_FEATURE_MANTLE_LAYER_DENSITY_MODEL(Uniform, uniform)
      } // namespace Density
    } // namespace MantleLayerModels
  } // namespace Features
} // namespace WorldBuilder

