/*
  Copyright (C) 2018 - 2021 by the authors of the World Builder code.

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

#include "world_builder/features/plume_models/temperature/gaussian.h"


#include "world_builder/nan.h"
#include "world_builder/types/double.h"
#include "world_builder/types/object.h"
#include "world_builder/utilities.h"
#include "world_builder/world.h"


namespace WorldBuilder
{
  using namespace Utilities;

  namespace Features
  {
    namespace PlumeModels
    {
      namespace Temperature
      {
        Gaussian::Gaussian(WorldBuilder::World *world_)
          :
          min_depth(NaN::DSNAN),
          max_depth(NaN::DSNAN),
          operation(Operations::REPLACE)
        {
          this->world = world_;
          this->name = "gaussian";
        }

        Gaussian::~Gaussian()
          = default;

        void
        Gaussian::declare_entries(Parameters &prm, const std::string & /*unused*/)
        {
          // Document plugin and require entries if needed.
          // Add `max distance fault` center to the required parameters.
          prm.declare_entry("", Types::Object({"centerline temperatures"}),
                            "Gaussian temperature model. Can be set to use an adiabatic temperature at the boundaries.");

          // Declare entries of this plugin
          prm.declare_entry("min depth", Types::Double(0),
                            "The depth in meters from which the temperature of the plume is present.");
          prm.declare_entry("max depth", Types::Double(std::numeric_limits<double>::max()),
                            "The depth in meters to which the temperature of the plume is present.");

          prm.declare_entry("depths", Types::Array(Types::Double(0)),
                            "The temperature at the center of this feature in degree Kelvin."
                            "If the value is below zero, the an adiabatic temperature is used.");
          prm.declare_entry("centerline temperatures", Types::Array(Types::Double(0)),
                            "The temperature at the center of this feature in degree Kelvin."
                            "If the value is below zero, the an adiabatic temperature is used.");
          prm.declare_entry("gaussian sigmas", Types::Array(Types::Double(-1)),
                            "The sigma (standard deviation) of the Gaussian function used to compute the "
                            "temperature distribution within the plume.");

          // TODO: assert that the three have the same length

        }

        void
        Gaussian::parse_entries(Parameters &prm)
        {
          min_depth = prm.get<double>("min depth");
          max_depth = prm.get<double>("max depth");
          WBAssert(max_depth >= min_depth, "max depth needs to be larger or equal to min depth.");

          operation = string_operations_to_enum(prm.get<std::string>("operation"));

          depths = prm.get_vector<double>("depths");
          center_temperatures = prm.get_vector<double>("centerline temperatures");
          gaussian_sigmas = prm.get_vector<double>("gaussian sigmas");
        }


        double
        Gaussian::get_temperature(const Point<3> & /*position_in_cartesian_coordinates*/,
                                  const Objects::NaturalCoordinate & /*position_in_natural_coordinates*/,
                                  const double depth,
                                  const double gravity_norm,
                                  double temperature_,
                                  const double /*feature_min_depth*/,
                                  const double /*feature_max_depth*/,
                                  const double relative_distance_from_center) const
        {
          if (depth <= max_depth && depth >= min_depth && relative_distance_from_center <= 1.)
            {
              // Figure out if the point is within the plume
              auto upper = std::upper_bound(depths.begin(), depths.end(), depth);

              double center_temperature_local;
              double gaussian_sigma;

              if (upper - depths.begin() == 0)
                {
                  // interpolate to make the top of the plume spherical
                  center_temperature_local = center_temperatures.front();
                  gaussian_sigma = gaussian_sigmas.front();

                  // TODO: deal with spherical plume tip
                }
              else if (upper - depths.end() == 0)
                {
                  center_temperature_local = center_temperatures.back();
                  gaussian_sigma = gaussian_sigmas.back();
                }
              else
                {
                  const unsigned int index = std::distance(depths.begin(), upper);
                  const double fraction = (depth - depths[index-1]) / (depths[index] - depths[index-1]);

                  center_temperature_local = (1-fraction) * center_temperatures[index-1] + fraction * center_temperatures[index];
                  gaussian_sigma = (1-fraction) * gaussian_sigmas[index-1] + fraction * gaussian_sigmas[index];
                }

              if (center_temperature_local < 0)
                {
                  center_temperature_local =  this->world->potential_mantle_temperature *
                                              std::exp(((this->world->thermal_expansion_coefficient * gravity_norm) /
                                                        this->world->specific_heat) * depth);
                }

              const double new_temperature =   center_temperature_local * std::exp(-relative_distance_from_center/(2.*std::pow(gaussian_sigma, 2)));

              return apply_operation(operation,temperature_,new_temperature);

            }

          return temperature_;
        }

        WB_REGISTER_FEATURE_PLUME_TEMPERATURE_MODEL(Gaussian, gaussian)
      } // namespace Temperature
    } // namespace PlumeModels
  } // namespace Features
} // namespace WorldBuilder

