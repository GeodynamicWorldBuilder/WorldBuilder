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

#include "world_builder/features/subducting_plate_models/temperature/mass_conserving.h"

#include "world_builder/nan.h"
#include "world_builder/types/bool.h"
#include "world_builder/types/double.h"
#include "world_builder/types/object.h"
#include "world_builder/utilities.h"
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
        MassConserving::MassConserving(WorldBuilder::World *world_)
          :
          min_depth(NaN::DSNAN),
          max_depth(NaN::DSNAN),
          density(NaN::DSNAN),
          plate_velocity(NaN::DSNAN),
          age_at_trench(NaN::DSNAN),
          overriding_plate_age_above_slab(NaN::DSNAN),
          mantle_coupling_depth(NaN::DSNAN),
          shallow_average_dip(NaN::DSNAN),
          thermal_conductivity(NaN::DSNAN),
          thermal_expansion_coefficient(NaN::DSNAN),
          specific_heat(NaN::DSNAN),
          thermal_diffusivity(NaN::DSNAN),
          potential_mantle_temperature(NaN::DSNAN),
          surface_temperature(NaN::DSNAN),
          adiabatic_heating(true),
          operation(Utilities::Operations::REPLACE)
        {
          this->world = world_;
          this->name = "mass conserving";
        }

        MassConserving::~MassConserving()
          = default;

        void
        MassConserving::declare_entries(Parameters &prm, const std::string & /*unused*/)
        {

          // Add temperature to the required parameters.
          prm.declare_entry("", Types::Object({"plate velocity"}), "Temperature model object");


          prm.declare_entry("min distance slab top", Types::Double(0),
                            "The distance in meters from the top surface of the slab over which the temperature is "
                            "determined by this feature. This parameter should be negative and should be 1.5-2 times "
                            "larger thatn the nominal slab thickness to allow the diffusion of cold "
                            "temperatures from in the slab into the mantle above the slab surface."
                            " Also note that the top truncation value for the slab segment needs to have a value "
                            " of -1, otherwise the temperature above the slab will be cut off at a distance less than"
                            " the value set here.");

          prm.declare_entry("max distance slab top", Types::Double(std::numeric_limits<double>::max()),
                            "The distance in meters from the top surface of the slab over which the temperature is "
                            "determined by this feature. This parameter should be positive and approximately 2.5-3.0 times "
                            "larger than the nominal slab thickness to allow the diffusion of cold"
                            "temperatures from in the slab into the mantle below the slab surface."
                            "For example if the slab starts with cold temperatures over a 100 km wide region, this"
                            "parameters should be about 250 km.");

          prm.declare_entry("density", Types::Double(3300),
                            "The reference density of the subducting plate in $kg/m^3$");

          prm.declare_entry("plate velocity", Types::Double(NaN::DQNAN),
                            "The velocity with which the plate subducts in meters per year.");

          prm.declare_entry("age at trench", Types::Double(NaN::DQNAN),
                            "The age of the slab at the trench in millions of years.");

          prm.declare_entry("overriding plate age", Types::Double(NaN::DQNAN),
                            "The age of the overriding plate immediately above the slab  in millions of years");

          prm.declare_entry("coupling depth", Types::Double(NaN::DQNAN),
                            "The depth at which the slab surface first comes in contact with the hot mantle wedge "
                            "in meters.");

          prm.declare_entry("shallow dip", Types::Double(NaN::DQNAN),
                            "The average dip of the slab at depths shallower than the coupling depth "
                            "in degrees. If the shallow slab dip changes with distance along the slab surface, "
                            "then this will need to be adjusted to get the temperature above and below "
                            "the coupling depth to match.");

          prm.declare_entry("thermal conductivity", Types::Double(3.3),
                            "The thermal conductivity of the subducting plate material in $W m^{-1} K^{-1}$.");

          prm.declare_entry("thermal expansion coefficient", Types::Double(-1),
                            "The thermal expansivity of the subducting plate material in $K^{-1}$. If smaller than zero, the global value is used.");

          prm.declare_entry("specific heat", Types::Double(-1),
                            "The specific heat of the subducting plate material in $J kg^{-1} K^{-1}$. If smaller than zero, the global value is used.");

          prm.declare_entry("thermal diffusivity", Types::Double(-1),
                            "The thermal conductivity of the subducting plate material in $W m^{-1} K^{-1}$.");

          prm.declare_entry("adiabatic heating", Types::Bool(true),
                            "Whether adiabatic heating should be used for the slab.");

          prm.declare_entry("potential mantle temperature", Types::Double(-1),
                            "The potential temperature of the mantle at the surface in Kelvin. If smaller than zero, the global value is used.");
        }

        void
        MassConserving::parse_entries(Parameters &prm)
        {

          min_depth = prm.get<double>("min distance slab top");
          max_depth = prm.get<double>("max distance slab top");
          operation = Utilities::string_operations_to_enum(prm.get<std::string>("operation"));

          density = prm.get<double>("density");
          thermal_conductivity = prm.get<double>("thermal conductivity");
          plate_velocity = prm.get<double>("plate velocity");

          age_at_trench = prm.get<double>("age at trench");
          overriding_plate_age_above_slab = prm.get<double>("overriding plate age");
          mantle_coupling_depth = prm.get<double>("coupling depth");
          shallow_average_dip = prm.get<double>("shallow dip");

          thermal_expansion_coefficient = prm.get<double>("thermal expansion coefficient");
          if (thermal_expansion_coefficient < 0 )
            thermal_expansion_coefficient = this->world->thermal_expansion_coefficient;

          specific_heat = prm.get<double>("specific heat");
          if (specific_heat < 0)
            specific_heat = this->world->specific_heat;

          thermal_diffusivity = prm.get<double>("thermal diffusivity");
          if (thermal_diffusivity < 0)
            thermal_diffusivity = this->world->thermal_diffusivity;

          adiabatic_heating = prm.get<bool>("adiabatic heating");

          potential_mantle_temperature = this->world->potential_mantle_temperature >= 0
                                         ?
                                         this->world->potential_mantle_temperature
                                         :
                                         prm.get<double>("potential mantle temperature");

          surface_temperature = this->world->surface_temperature;

        }


        double
        MassConserving::get_temperature(const Point<3> & /*position_in_cartesian_coordinates*/,
                                        const double depth,
                                        const double gravity_norm,
                                        double temperature_,
                                        const double /*feature_min_depth*/,
                                        const double /*feature_max_depth*/,
                                        const WorldBuilder::Utilities::PointDistanceFromCurvedPlanes &distance_from_planes) const
        {

          const double distance_from_plane = distance_from_planes.distance_from_plane;
          const double distance_along_plane = distance_from_planes.distance_along_plane;
          const double depth_slab_surface = distance_from_planes.depth_reference_surface;

          const double seconds_in_year = 60.0*60.0*24.0*365.25;
          const double plate_age_sec = age_at_trench*1e6*seconds_in_year;  // my --> seconds
          const double over_plate_age_sec = overriding_plate_age_above_slab*1e6*seconds_in_year;  // my --> seconds

          if (distance_from_plane <= max_depth && distance_from_plane >= min_depth)
            {

              // 1. Determine initial heat content of the slab based on age of plate at trench
              //    This uses the integral of the half-space temperature profile
              const double initial_heat_content = 2*thermal_conductivity*(surface_temperature - potential_mantle_temperature) *
                                                  std::sqrt(plate_age_sec/(thermal_diffusivity*const_pi));

              //  2. Get Tmin and distance_offset given distance along slab and depth of point on the slab.
              //  These equations are empirical based on fitting the temperature profiles from dynamic subduction models.
              double min_temperature = 0.0;
              double distance_offset = 0.0;

              const double mantle_coupling_length = mantle_coupling_depth/std::sin(shallow_average_dip*const_pi/180.0); //m

              /* Empirical model parameters */
              double upper_mantle_depth = 660e3; // m
              double upper_mantle_length = 1000e3; // m
              /* Distance offset parameters */
              double offset_distance_min = 25*1e3; // m
              double offset_distance_max = 35*1e3; // m
              double slope_distance_min = 25e3/(1.5*660e3);
              double slope_distance_max = 20e3/(2.5*100e3);
              double sink_velocity_min = 0.025; // m/yr
              double sink_velocity_max = 0.10; // m/yr
              /* minimum temperature parameters */
              double slope_temperature_min = 250/(2.5*100*1e3); // deg/m (2.5 because dip in ref. model is 20-25 deg)
              double slope_temperature_max = 500/(2.5*100*1e3); // deg/m
              double temperature_min = 750;
              double temperature_max = 950;
              double vsubfact = (1 - (plate_velocity - sink_velocity_min)/sink_velocity_max);  // vsubfact = 0 when vel=sink_velocity_max

              double slope_distance_shallow = slope_distance_min;
              double slope_temperature_shallow = slope_temperature_min;

              if (plate_velocity < sink_velocity_min)
                {
                  slope_distance_shallow = slope_distance_max;
                  slope_temperature_shallow = slope_temperature_max;
                }
              else if ( plate_velocity > sink_velocity_max)
                {
                  slope_distance_shallow = slope_distance_min;
                  slope_temperature_shallow = slope_temperature_min;
                }
              else
                {
                  slope_distance_shallow = slope_distance_min + vsubfact*(slope_distance_max - slope_distance_min);
                  slope_temperature_shallow = slope_temperature_min + vsubfact*(slope_temperature_max - slope_temperature_min);
                }
              double offset_coupling_depth = slope_distance_shallow*mantle_coupling_depth; // m  mantle_coupling_length
              double offset_660 = offset_distance_min + vsubfact*(offset_distance_max - offset_distance_min); // m

              double slope_distance_deep = (offset_660 - offset_coupling_depth)/(upper_mantle_length - mantle_coupling_length);
              double intercept_dist_deep = (slope_distance_shallow - slope_distance_deep)*mantle_coupling_length;

              double temperature_min_coupling_depth = slope_temperature_shallow*mantle_coupling_length;
              double temperature_min_660 = temperature_min + vsubfact*(temperature_max - temperature_min);

              double slope_temperature_deep = (temperature_min_660 - temperature_min_coupling_depth)/(upper_mantle_length - mantle_coupling_length);
              double intercept_temperature_deep = (slope_temperature_shallow - slope_temperature_deep)*(mantle_coupling_length);

              if (distance_along_plane <= mantle_coupling_length)
                {
                  distance_offset = slope_distance_shallow*distance_along_plane;
                  min_temperature = surface_temperature + slope_temperature_shallow*distance_along_plane;
                }
              else
                {
                  distance_offset = slope_distance_deep*distance_along_plane + intercept_dist_deep;
                  min_temperature = surface_temperature + slope_temperature_deep*distance_along_plane + intercept_temperature_deep;
                }

              double temperature = 0.0;

              // Need adiabatic temperature at position of grid point
              double background_temperature = adiabatic_heating ?
                                              potential_mantle_temperature*std::exp(((thermal_expansion_coefficient * gravity_norm * depth) / specific_heat))
                                              : potential_mantle_temperature;

              WBAssert(!std::isnan(background_temperature), "Internal error: temp is not a number: " << background_temperature << ". In exponent: "
                       << std::exp(((thermal_expansion_coefficient * gravity_norm) / specific_heat) * depth)
                       << ", thermal_expansion_coefficient = " << thermal_expansion_coefficient << ", gravity_norm = " << gravity_norm
                       << ", specific_heat = "<< specific_heat << ", depth = " << depth );

              if (min_temperature < background_temperature)
                {

                  // Need temperature in plate/mantle immediately above the slab to create smooth
                  // transition to the overriding plate temperature at shallower depth.

                  double overriding_plate_temperature = background_temperature + (surface_temperature - background_temperature) *
                                                        std::erfc(depth/(2*std::sqrt(thermal_diffusivity*over_plate_age_sec)));

                  // Adjust distance for the offset of the minimum temperature from the top of the slab
                  double adjusted_distance = distance_from_plane - distance_offset;


                  // 3. Determine the heat content for side 1 (bottom) of the slab

                  double time_since_subducting = (distance_along_plane/plate_velocity)*seconds_in_year;  // m/(m/y) = y(seconds_in_year)
                  double bottom_heat_content = 2*thermal_conductivity*(min_temperature - potential_mantle_temperature) *
                                               std::sqrt((plate_age_sec+time_since_subducting)/(thermal_diffusivity*const_pi));

                  // 4. The difference in heat content goes into the temperature above where Tmin occurs.

                  double top_heat_content = initial_heat_content - bottom_heat_content;

                  // Assign the temperature depending on whether distance is negative (above) or positive (below) the slab

                  if (adjusted_distance < 0)
                    {
                      // use 1D infinite space solution for top (side 2) of slab the slab
                      // 2 times the "top_heat_content" because all this heat needs to be on one side of the Gaussian
                      double time_top_slab = (1/(const_pi*thermal_diffusivity))*pow(((2*top_heat_content)/
                                                                                     (2*density*specific_heat*(min_temperature - overriding_plate_temperature + 1e-16))),2) + 1e-16;

                      temperature  = overriding_plate_temperature + (2*top_heat_content/(2*density*specific_heat*std::sqrt(const_pi*thermal_diffusivity*time_top_slab)))*
                                     std::exp(-(adjusted_distance*adjusted_distance)/(4*thermal_diffusivity*time_top_slab));
                    }
                  else
                    {
                      // use half-space cooling model for the bottom (side 1) of the slab
                      temperature = background_temperature + (min_temperature - background_temperature)*
                                    std::erfc(adjusted_distance/(2*std::sqrt(thermal_diffusivity*(plate_age_sec+time_since_subducting))));
                    }
                }
              else
                {
                  // slab temperature anomaly is gone.
                  temperature = background_temperature;
                }

              WBAssert(!std::isnan(temperature), "Internal error: temperature is not a number: " << temperature << ".");
              WBAssert(std::isfinite(temperature), "Internal error: temperature is not finite: " << temperature << ".");

              return Utilities::apply_operation(operation,temperature_,temperature);
            }

          return temperature_;
        }

        WB_REGISTER_FEATURE_SUBDUCTING_PLATE_TEMPERATURE_MODEL(MassConserving, mass conserving)
      } // namespace Temperature
    } // namespace SubductingPlateModels
  } // namespace Features
} // namespace WorldBuilder

