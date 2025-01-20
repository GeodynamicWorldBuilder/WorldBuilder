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

#include "world_builder/features/continental_plate_models/density/uniform.h"


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
    namespace ContinentalPlateModels
    {
      namespace Density
      {
        Uniform::Uniform(WorldBuilder::World *world_)
          :
          min_depth(NaN::DSNAN),
          max_depth(NaN::DSNAN),
          //density(NaN::DSNAN),
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
          // Add `temperature` to the required parameters.

          // Is this object necessary?
          //prm.declare_entry("", Types::Object({"densities"}),
          //                  "Uniform density model. Set the density to a constant value.");

          // Declare entries of this plugin
          //prm.declare_entry("min depth", Types::OneOf(Types::Double(0),Types::Array(Types::ValueAtPoints(0., 2.))),
          //                  "The depth in meters from which the composition of this feature is present.");

          //prm.declare_entry("max depth", Types::OneOf(Types::Double(std::numeric_limits<double>::max()),Types::Array(Types::ValueAtPoints(std::numeric_limits<double>::max(), 2.))),
          //                  "The depth in meters to which the composition of this feature is present.");

          //prm.declare_entry("density", Types::Double(3300),
          //                 "The density in kg/m3 which this feature should have");

          prm.declare_entry("number of compositions", Types::UnsignedInt(1),
                            "The density in kg/m3 which this feature should have");

          prm.declare_entry("densities", Types::Array(Types::Double(3000)),
                            "list of compositionalk densities");

        }

        void
        Uniform::parse_entries(Parameters &prm, const std::vector<Point<2>> &coordinates)
        {

          //min_depth_surface = Objects::Surface(prm.get("min depth",coordinates));
          //min_depth = min_depth_surface.minimum;
          //max_depth_surface = Objects::Surface(prm.get("max depth",coordinates));
          //max_depth = max_depth_surface.maximum;
          operation = string_operations_to_enum(prm.get<std::string>("operation"));
          //density = prm.get<double>("density");
          densities = prm.get_vector<double>("densities");
          n_comps = prm.get<int>("number of compositions");
        }


        double
        Uniform::get_density(const Point<3> & position_in_cartesian_coordinates,
                                 const Objects::NaturalCoordinate &position_in_natural_coordinates,
                                 const double depth,
                                 const double  /*gravity*/,
                                 double density_,
                                 const unsigned int composition_number,
                                 const double /*feature_min_depth*/,
                                 const double /*feature_max_depth*/) const
        {
          std::vector<double> compositions(n_comps, 0.0);
          double density = 0.;
          for (unsigned int i = 0; i < n_comps; ++i)
              density += world->properties(position_in_cartesian_coordinates.get_array(), depth, {{{2, i, 0}}})[0] * densities[i];

          return apply_operation(operation,density_,density);
        }

        WB_REGISTER_FEATURE_CONTINENTAL_PLATE_TEMPERATURE_MODEL(Uniform, uniform)
      } // namespace Temperature
    } // namespace ContinentalPlateModels
  } // namespace Features
} // namespace WorldBuilder

