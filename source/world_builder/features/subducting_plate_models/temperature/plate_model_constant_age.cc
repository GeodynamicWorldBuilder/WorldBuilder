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

#include "world_builder/features/subducting_plate_models/temperature/plate_model_constant_age.h"

#include "world_builder/nan.h"
#include "world_builder/types/array.h"
#include "world_builder/types/double.h"
#include "world_builder/types/bool.h"
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
    namespace SubductingPlateModels
    {
      namespace Temperature
      {
        PlateModelConstantAge::PlateModelConstantAge(WorldBuilder::World *world_)
          :
          min_depth(NaN::DSNAN),
          max_depth(NaN::DSNAN),
          top_temperature(NaN::DSNAN),
          bottom_temperature(NaN::DSNAN),
          plate_age(NaN::DSNAN),
          adiabatic_heating(true),
          thermal_expansion_coefficient(NaN::DSNAN),
          specific_heat(NaN::DSNAN),
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
          prm.declare_entry("", Types::Object({"max distance slab top", "plate age"}),
                            "Plate model, but with a fixed age.");

          // Declare entries of this plugin
          prm.declare_entry("min distance slab top", Types::Double(0),
                            "The depth in meters from which the temperature of this feature is present.");

          prm.declare_entry("max distance slab top", Types::Double(std::numeric_limits<double>::max()),
                            "The depth in meters to which the temperature of this feature is present.");

          prm.declare_entry("top temperature", Types::Double(293.15),
                            "The temperature in degree Kelvin which this feature should have");

          prm.declare_entry("bottom temperature", Types::Double(-1),
                            "The temperature in degree Kelvin which this feature should have. "
                            "If smaller than zero, the temperature is calculated using the potential mantle temperature and adiabatic heating.");

          prm.declare_entry("plate age", Types::Double(80e3),
                            "The age of the plate in year. "
                            "This age is assigned to the whole plate. ");

          prm.declare_entry("adiabatic heating", Types::Bool(true),
                            "Whether adiabatic heating should be used for the slab.");

          prm.declare_entry("thermal expansion coefficient", Types::Double(-1),
                            "The thermal expansivity of the subducting plate material in $K^{-1}$. If smaller than zero, the global value is used.");

          prm.declare_entry("specific heat", Types::Double(-1),
                            "The specific heat of the subducting plate material in $J kg^{-1} K^{-1}$. If smaller than zero, the global value is used.");
        }

        void
        PlateModelConstantAge::parse_entries(Parameters &prm)
        {
          min_depth = prm.get<double>("min distance slab top");
          max_depth = prm.get<double>("max distance slab top");
          operation = string_operations_to_enum(prm.get<std::string>("operation"));
          top_temperature = prm.get<double>("top temperature");
          bottom_temperature = prm.get<double>("bottom temperature");
          plate_age = prm.get<double>("plate age") * 31557600; // Convert years to seconds
          
          adiabatic_heating = prm.get<bool>("adiabatic heating");
          
          thermal_expansion_coefficient = prm.get<double>("thermal expansion coefficient");
          if (thermal_expansion_coefficient < 0 )
             thermal_expansion_coefficient = this->world->thermal_expansion_coefficient;

          specific_heat = prm.get<double>("specific heat");
          if (specific_heat < 0)
             specific_heat = this->world->specific_heat;
        }


        double
        PlateModelConstantAge::get_temperature(const Point<3> & /*position*/,
                                               const double depth,
                                               const double gravity_norm,
                                               double temperature_,
                                               const double /*feature_min_depth*/,
                                               const double /*feature_max_depth*/,
                                               const WorldBuilder::Utilities::PointDistanceFromCurvedPlanes &distance_from_planes,
                                               const AdditionalParameters & /*additional_parameters*/) const
        {
           // In subducting plate models, the coordinate used for the profile across the slab is distance_from_planes.distance_from_plane.
           // min_depth and max_depth are configured as "min distance slab top" and "max distance slab top".
           double depth_local = distance_from_planes.distance_from_plane;

           // If the user sets max_depth to a large value (default max double), we might want to clamp it or treat it carefully.
           // However, standard WorldBuilder usage for features involves strict bounds. 
           
           if (depth_local <= max_depth && depth_local >= min_depth)
            {
               double bottom_temperature_local = bottom_temperature;

               if (bottom_temperature_local < 0)
                 {
                   // Use adiabatic heating calculation
                   // Note: 'depth' here is the global z-depth, which is correct for adiabatic heating.
                   
                    if (adiabatic_heating) {
                        bottom_temperature_local =  this->world->potential_mantle_temperature *
                                               std::exp(((thermal_expansion_coefficient * gravity_norm) /
                                                         specific_heat) * depth);
                    } else {
                        bottom_temperature_local = this->world->potential_mantle_temperature;
                    }
                 }
                 
               const int sommation_number = 100;

               const double thermal_diffusivity = this->world->thermal_diffusivity;
               
               // Linear interpolation part (steady state)
               // Note: depth_local is used as the coordinate within the plate (0 to max_depth)
               // If max_depth is infinite/large, this linear part might be problematic if not careful, but typically for plate models max_depth is the plate thickness.
               // Users should set "max distance slab top" to the plate thickness.
               
               double temperature = top_temperature + (bottom_temperature_local - top_temperature) * (depth_local / max_depth);

               // This formula ignores the horizontal heat transfer by just having the age of the plate in it.
               // (Chapter 7 Heat, Fowler M. The solid earth: an introduction to global geophysics[M]. Cambridge University Press, 1990)
               // Summation part for time-dependence
               for (int i = 1; i < sommation_number + 1; ++i)
                 {
                   temperature = temperature + (bottom_temperature_local - top_temperature) *
                                 ((2 / (double(i) * Consts::PI)) * std::sin((double(i) * Consts::PI * depth_local) / max_depth) *
                                  std::exp(-1.0 * i * i * Consts::PI * Consts::PI * thermal_diffusivity * plate_age / (max_depth * max_depth)));
                 }

               WBAssert(!std::isnan(temperature), "Temperature inside plate model constant age is not a number: " << temperature
                        << ". Relevant variables: bottom_temperature_local = " << bottom_temperature_local
                        << ", top_temperature = " << top_temperature
                        << ", max_depth = " << max_depth
                        << ", thermal_diffusivity = " << thermal_diffusivity
                        << ", age = " << plate_age << '.');
               WBAssert(std::isfinite(temperature), "Temperature inside plate model constant age is not a finite: " << temperature                           << ". Relevant variables: bottom_temperature_local = " << bottom_temperature_local
                        << ", top_temperature = " << top_temperature
                        << ", thermal_diffusivity = " << thermal_diffusivity
                        << ", age = " << plate_age << '.');


               return apply_operation(operation, temperature_, temperature);
            }
          return temperature_;
        }

        WB_REGISTER_FEATURE_SUBDUCTING_PLATE_TEMPERATURE_MODEL(PlateModelConstantAge, plate model constant age)
      } // namespace Temperature
    } // namespace SubductingPlateModels
  } // namespace Features
} // namespace WorldBuilder
