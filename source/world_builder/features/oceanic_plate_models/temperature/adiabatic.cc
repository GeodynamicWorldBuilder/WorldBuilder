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

#include "world_builder/features/oceanic_plate_models/temperature/adiabatic.h"


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
    namespace OceanicPlateModels
    {
      namespace Temperature
      {
        Adiabatic::Adiabatic(WorldBuilder::World *world_)
          :
          min_depth_entry(NaN::DSNAN),
          max_depth_entry(NaN::DSNAN),
          potential_mantle_temperature(NaN::DSNAN),
          thermal_expansion_coefficient(NaN::DSNAN),
          specific_heat(NaN::DSNAN),
          operation(Operations::REPLACE)
        {
          this->world = world_;
          this->name = "adiabatic";
        }

        Adiabatic::~Adiabatic()
          = default;

        void
        Adiabatic::declare_entries(Parameters &prm, const std::string & /*unused*/)
        {
          // Document plugin and require entries if needed.
          prm.declare_entry("", Types::Object(),
                            "Adiabatic temperature model. Uses global values by default.");

          // Declare entries of this plugin
          prm.declare_entry("min depth", Types::OneOf(Types::Double(-std::numeric_limits<double>::max()),Types::Array(Types::ValueAtPoints(-std::numeric_limits<double>::max(), 2.))),
                            "The depth in m from which the temperature of this feature is present.");

          prm.declare_entry("max depth", Types::OneOf(Types::Double(std::numeric_limits<double>::max()),Types::Array(Types::ValueAtPoints(std::numeric_limits<double>::max(), 2.))),
                            "The depth in m to which the temperature of this feature is present.");

          prm.declare_entry("potential mantle temperature", Types::Double(-1),
                            "The potential temperature in K of the mantle at the surface. "
                            "If the value is lower then zero, the global value is used.");

          prm.declare_entry("thermal expansion coefficient", Types::Double(-1),
                            "The thermal expansion coefficient in $K^{-1}$. "
                            "If the value is lower then zero, the global value is used.");

          prm.declare_entry("specific heat", Types::Double(-1),
                            "The specific heat in $J kg^{-1} K^{-1}$. "
                            "If the value is lower then zero, the global value is used.");

        }

        void
        Adiabatic::parse_entries(Parameters &prm, const std::vector<Point<2>> &coordinates)
        {

          min_depth_surface_entry = Objects::Surface(prm.get("min depth",coordinates));
          min_depth_entry = min_depth_surface_entry.minimum;
          max_depth_surface_entry = Objects::Surface(prm.get("max depth",coordinates));
          max_depth_entry = max_depth_surface_entry.maximum;
          operation = string_operations_to_enum(prm.get<std::string>("operation"));

          potential_mantle_temperature = prm.get<double>("potential mantle temperature");
          if (potential_mantle_temperature < 0)
            potential_mantle_temperature =  this->world->potential_mantle_temperature;


          thermal_expansion_coefficient = prm.get<double>("thermal expansion coefficient");
          if (thermal_expansion_coefficient < 0)
            thermal_expansion_coefficient =  this->world->thermal_expansion_coefficient;

          specific_heat = prm.get<double>("specific heat");
          if (specific_heat < 0)
            specific_heat =  this->world->specific_heat;

          // Some assertions in debug mode can't hurt and have helped before.
          WBAssert(!std::isnan(potential_mantle_temperature),
                   "potential_mantle_temperature is not a number: " << potential_mantle_temperature << '.');
          WBAssert(std::isfinite(potential_mantle_temperature),
                   "potential_mantle_temperature is not a finite: " << potential_mantle_temperature << '.');

          WBAssert(!std::isnan(thermal_expansion_coefficient),
                   "thermal_expansion_coefficient is not a number: " << thermal_expansion_coefficient << '.');
          WBAssert(std::isfinite(thermal_expansion_coefficient),
                   "thermal_expansion_coefficient is not a finite: " << thermal_expansion_coefficient << '.');

          WBAssert(!std::isnan(specific_heat),
                   "specific_heat is not a number: " << specific_heat << '.');
          WBAssert(std::isfinite(thermal_expansion_coefficient),
                   "specific_heat is not a finite: " << specific_heat << '.');

        }


        double
        Adiabatic::get_temperature(const Point<3> & /*position_in_cartesian_coordinates*/,
                                   const Objects::NaturalCoordinate &position_in_natural_coordinates,
                                   const double depth,
                                   const double gravity_norm,
                                   double temperature_,
                                   const double min_depth_feature,
                                   const double max_depth_feature) const
        {

          if (depth <= max_depth_entry && depth >= min_depth_entry && depth <= max_depth_feature && depth >= min_depth_feature)
            {
              // check if the user defined min_depth and max_depth are constant values
              // use those values if that is the case, find the surface point if not
              double min_depth_point = min_depth_surface_entry.constant_value ? min_depth_entry : min_depth_surface_entry.local_value(position_in_natural_coordinates.get_surface_point()).interpolated_value;
              double max_depth_point = max_depth_surface_entry.constant_value ? max_depth_entry : max_depth_surface_entry.local_value(position_in_natural_coordinates.get_surface_point()).interpolated_value;

              // constrain the depth to the feature min and max depth
              double min_depth_local = std::max(min_depth_feature, min_depth_point);
              double max_depth_local = std::min(max_depth_feature, max_depth_point);

              if (depth <= max_depth_local &&  depth >= min_depth_local)
                {
                  const double temperature_calculated = potential_mantle_temperature *
                                                        std::exp(((thermal_expansion_coefficient * gravity_norm) /
                                                                  specific_heat) * depth);


                  WBAssert(!std::isnan(temperature_calculated), "Temperature is not a number: " << temperature_calculated
                           << ", based on a temperature model with the name " << this->name);

                  WBAssert(std::isfinite(temperature_calculated), "Temperature is not a finite: " << temperature_calculated
                           << ", based on a temperature model with the name " << this->name
                           << ", potential_mantle_temperature = " << potential_mantle_temperature
                           << ", thermal_expansion_coefficient = " << thermal_expansion_coefficient
                           << ", gravity_norm = " << gravity_norm
                           << ", specific_heat = " << specific_heat
                           << ", depth = " << depth);

                  return apply_operation(operation,temperature_,temperature_calculated);
                }
            }


          return temperature_;
        }

        WB_REGISTER_FEATURE_OCEANIC_PLATE_TEMPERATURE_MODEL(Adiabatic, adiabatic)
      } // namespace Temperature
    } // namespace OceanicPlateModels
  } // namespace Features
} // namespace WorldBuilder

