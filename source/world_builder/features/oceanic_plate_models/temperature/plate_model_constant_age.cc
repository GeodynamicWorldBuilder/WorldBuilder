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

#include "world_builder/features/oceanic_plate_models/temperature/plate_model_constant_age.h"

#include "world_builder/nan.h"
#include "world_builder/types/array.h"
#include "world_builder/types/double.h"
#include "world_builder/types/object.h"
#include "world_builder/types/one_of.h"
#include "world_builder/types/point.h"
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
        PlateModelConstantAge::PlateModelConstantAge(WorldBuilder::World *world_)
          :
          min_depth_entry(NaN::DSNAN),
          max_depth_entry(NaN::DSNAN),
          top_temperature(NaN::DSNAN),
          bottom_temperature(NaN::DSNAN),
          operation(Operations::REPLACE)
        {
          this->world = world_;
          this->name = "plate model constant age";
        }

        PlateModelConstantAge::~PlateModelConstantAge()
          = default;

        void
        PlateModelConstantAge::declare_entries(Parameters &prm, const std::string & /*unused*/)
        {
          // Document plugin and require entries if needed.
          // Add temperature to the required parameters.
          prm.declare_entry("", Types::Object({"max depth"}),
                            "Plate model, but with a fixed age.");

          // Declare entries of this plugin
          prm.declare_entry("min depth", Types::OneOf(Types::Double(-std::numeric_limits<double>::max()),Types::Array(Types::ValueAtPoints(-std::numeric_limits<double>::max(), 2.))),
                            "The depth in m from which the temperature of this feature is present.");

          prm.declare_entry("max depth", Types::OneOf(Types::Double(std::numeric_limits<double>::max()),Types::Array(Types::ValueAtPoints(std::numeric_limits<double>::max(), 2.))),
                            "The depth in m to which the temperature of this feature is present.");

          prm.declare_entry("top temperature", Types::Double(293.15),
                            "The temperature in K at the top surface of this feature.");

          prm.declare_entry("bottom temperature", Types::Double(-1),
                            "The temperature in K at the bottom surface of this feature.");

          prm.declare_entry("plate age", Types::Double(80e3),
                            "The age of the plate in year. "
                            "This age is assigned to the whole plate. ");
        }

        void
        PlateModelConstantAge::parse_entries(Parameters &prm, const std::vector<Point<2>> &coordinates)
        {

          min_depth_surface_entry = Objects::Surface(prm.get("min depth",coordinates));
          min_depth_entry = min_depth_surface_entry.minimum;
          max_depth_surface_entry = Objects::Surface(prm.get("max depth",coordinates));
          max_depth_entry = max_depth_surface_entry.maximum;
          operation = string_operations_to_enum(prm.get<std::string>("operation"));
          top_temperature = prm.get<double>("top temperature");
          bottom_temperature = prm.get<double>("bottom temperature");
          plate_age = prm.get<double>("plate age")*31557600;
        }


        double
        PlateModelConstantAge::get_temperature(const Point<3> & /*position*/,
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
                  double bottom_temperature_local = bottom_temperature;
                  double top_temperature_local = top_temperature;

                  if (bottom_temperature_local < 0)
                    {
                      bottom_temperature_local =  this->world->potential_mantle_temperature *
                                                  std::exp(((this->world->thermal_expansion_coefficient* gravity_norm) /
                                                            this->world->specific_heat) * depth);
                    }
                  const int sommation_number = 100;

                  // some aliases
                  //const double top_temperature = top_temperature;
                  const double thermal_diffusivity = this->world->thermal_diffusivity;
                  double temperature_calculated = top_temperature_local + (bottom_temperature_local - top_temperature_local) * (depth / max_depth_local);

                  // This formula ignores the horizontal heat transfer by just having the age of the plate in it.
                  // (Chapter 7 Heat, Fowler M. The solid earth: an introduction to global geophysics[M]. Cambridge University Press, 1990)
                  for (int i = 1; i<sommation_number+1; ++i)
                    {
                      temperature_calculated = temperature_calculated + (bottom_temperature_local - top_temperature_local) *
                                               ((2 / (double(i) * Consts::PI)) * std::sin((double(i) * Consts::PI * depth) / max_depth_local) *
                                                std::exp(-1.0 * i * i * Consts::PI * Consts::PI * thermal_diffusivity * plate_age / (max_depth_local * max_depth_local)));
                    }


                  WBAssert(!std::isnan(temperature_calculated), "Temperature is not a number: " << temperature_calculated
                           << ", based on a temperature model with the name " << this->name);

                  WBAssert(std::isfinite(temperature_calculated), "Temperature is not a finite: " << temperature_calculated
                           << ", based on a temperature model with the name " << this->name
                           << ", depth = " << depth
                           << ", min_depth_local = " << min_depth_local
                           << ", max_depth_local = " << max_depth_local
                           << ", min_depth_feature = " << min_depth_feature
                           << ", max_depth_feature = " << max_depth_feature
                           << ", top_temperature_local=" << top_temperature_local
                           << ", bottom_temperature_local= " << bottom_temperature_local
                           << ", thermal_diffusivity = " << thermal_diffusivity
                           << ", plate_age = " << plate_age << '.');

                  return apply_operation(operation,temperature_,temperature_calculated);

                }
            }
          return temperature_;
        }

        WB_REGISTER_FEATURE_OCEANIC_PLATE_TEMPERATURE_MODEL(PlateModelConstantAge, plate model constant age)
      } // namespace Temperature
    } // namespace OceanicPlateModels
  } // namespace Features
} // namespace WorldBuilder

