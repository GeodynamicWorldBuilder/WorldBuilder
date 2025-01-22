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

#include "world_builder/features/fault_models/density/uniform.h"


#include "world_builder/nan.h"
#include "world_builder/types/array.h"
#include "world_builder/types/double.h"
#include "world_builder/types/object.h"
#include "world_builder/types/unsigned_int.h"
#include "world_builder/utilities.h"


namespace WorldBuilder
{

  using namespace Utilities;

  namespace Features
  {
    namespace FaultModels
    {
      namespace Density
      {
        Uniform::Uniform(WorldBuilder::World *world_)
          :
          min_depth(NaN::DSNAN),
          max_depth(NaN::DSNAN)
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
          // Add `densities` to the required parameters.
          prm.declare_entry("", Types::Object({"densities"}),
                            "Uniform density model. Set the density to a constant value.");

          prm.declare_entry("densities", Types::Array(Types::Double(3300)),
                            "list of compositionalk densities");

          prm.declare_entry("compositions", Types::Array(Types::UnsignedInt(),0),
                            "A list with the labels of the composition which are present there.");

          prm.declare_entry("operation", Types::String("replace", std::vector<std::string> {"replace", "replace defined only", "add", "subtract"}),
                            "Whether the value should replace any value previously defined at this location (replace) or "
                            "add the value to the previously define value. Replacing implies that all compositions not "
                            "explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.");

        }

        void
        Uniform::parse_entries(Parameters &prm)
        {
          operation = string_operations_to_enum(prm.get<std::string>("operation"));
          densities = prm.get_vector<double>("densities");
          compositions = prm.get_vector<unsigned int>("compositions");
        }


        double
        Uniform::get_density(const Point<3> &position_in_cartesian_coordinates,
                             const double depth,
                             const unsigned int composition_number,
                             double density_,
                             const double  /*feature_min_depth*/,
                             const double  /*feature_max_depth*/,
                             const WorldBuilder::Utilities::PointDistanceFromCurvedPlanes &distance_from_plane,
                             const AdditionalParameters & /*additional_parameters*/) const
        {
          // If the composition is greater than 0, average it into the density.
          // By callign the world property for composition, fractions will be included.
          double compositional_density = 0.;
          for (unsigned int i = 0; i < compositions.size(); ++i)
            if (world->properties(position_in_cartesian_coordinates.get_array(), depth, {{{2, compositions[i], 0}}})[0] > 0.0)
          compositional_density += world->properties(position_in_cartesian_coordinates.get_array(), depth, {{{2, compositions[i], 0}}})[0] * densities[i];

          return apply_operation(operation,density_,compositional_density);
        }

        WB_REGISTER_FEATURE_FAULT_DENSITY_MODEL(Uniform, uniform)
      } // namespace Density
    } // namespace FaultModels
  } // namespace Features
} // namespace WorldBuilder


