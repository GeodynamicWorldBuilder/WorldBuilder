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


   Note that the empirical model used to define how Tmin increases with depth
   and how the position of Tmin shift with depth is expected to change somewhat
   after better calibrating with further tests.
*/

#include "world_builder/features/subducting_plate_models/temperature/mass_conserving.h"

#include "world_builder/nan.h"
#include "world_builder/types/array.h"
#include "world_builder/types/bool.h"
#include "world_builder/types/double.h"
#include "world_builder/types/object.h"
#include "world_builder/types/point.h"
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
          mantle_coupling_depth(NaN::DSNAN),
          shallow_average_dip(NaN::DSNAN),
          thermal_conductivity(NaN::DSNAN),
          thermal_expansion_coefficient(NaN::DSNAN),
          specific_heat(NaN::DSNAN),
          thermal_diffusivity(NaN::DSNAN),
          potential_mantle_temperature(NaN::DSNAN),
          surface_temperature(NaN::DSNAN),
          taper_distance(NaN::DSNAN),
          adiabatic_heating(true),
          operation(Utilities::Operations::REPLACE)
        {
          this->world = world_;
          this->name = "mass conserving";
        }

        MassConserving::~MassConserving() = default;

        void
        MassConserving::declare_entries(Parameters &prm, const std::string & /*unused*/)
        {
          // Document plugin and require entries if needed.
          // Add `plate velocity` to the required parameters.
          prm.declare_entry("", Types::Object({"plate velocity"}),
                            "Mass conserving temperature model. The temperature "
                            "model uses the heat content (proportional to to thermal mass anomaly) to "
                            "define a smooth temperature profile that conserves mass along the slab length. "
                            "An empirical (linear) model is used to define how the minimum temperature "
                            "increases with depth and how the location of the minimum temperature shifts "
                            "into the slab interior. The slab is divided in to top and bottom parts, "
                            "which meet at the location where the minimum temperature occurs in the slab. "
                            "For the bottom slab the temperature is defined by a half-space cooling model. "
                            "For the top of the slab the temperature is defined by one side of a 1D infinite "
                            "space cooling model. The age of the overriding plate is used so the slab temperature "
                            "at shallow depth smoothly transitions to the temperature of the overriding plate: "
                            "this is not perfect, and is affected by the value of \"top truncation\" parameter "
                            "subducting plate. Also note that the parameter \"thickness\" for the subducting plate "
                            "segments needs to be defined but is not used. "
                            "Note that the empirical model used to define how Tmin increases with depth "
                            "and how the position of Tmin shift with depth is expected to change somewhat "
                            "after better calibrating with further tests.");

          // Declare entries of this plugin
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

          prm.declare_entry("taper distance", Types::Double(100e3),
                            "Distance over which to taper the slab tip."
                            "tapers the initial heat content to zero and the minimum temperature to the background temperature.");

          prm.declare_entry("potential mantle temperature", Types::Double(-1),
                            "The potential temperature of the mantle at the surface in Kelvin. If smaller than zero, the global value is used.");

          prm.declare_entry("ridge coordinates", Types::Array(Types::Array(Types::Point<2>(), 2),1),
                            "An list of ridges. Each ridge is a lists of at least 2 2d points which "
                            "define the location of the ridge. You need to define at least one ridge."
                            "So the an exmple with two ridges is "
                            "[[[10,20],[20,30],[10,40]],[[50,10],[60,10]]].");
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

          mantle_coupling_depth = prm.get<double>("coupling depth");
          shallow_average_dip = prm.get<double>("shallow dip");

          taper_distance = prm.get<double>("taper distance");

          thermal_expansion_coefficient = prm.get<double>("thermal expansion coefficient");
          if (thermal_expansion_coefficient < 0)
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

          mid_oceanic_ridges = prm.get_vector<std::vector<Point<2>>>("ridge coordinates");
          const double dtr = prm.coordinate_system->natural_coordinate_system() == spherical ? Consts::PI / 180.0 : 1.0;
          for (auto &ridge_coordinates : mid_oceanic_ridges)
            for (auto &ridge_coordinate : ridge_coordinates)
              {
                ridge_coordinate *= dtr;
              }
        }

        double
        MassConserving::get_temperature(const Point<3> & /*position_in_cartesian_coordinates*/,
                                        const double depth,
                                        const double gravity_norm,
                                        double temperature_,
                                        const double /*feature_min_depth*/,
                                        const double /*feature_max_depth*/,
                                        const WorldBuilder::Utilities::PointDistanceFromCurvedPlanes &distance_from_planes,
                                        const Utilities::AdditionalParameters &additional_parameters) const
        {

          const double distance_from_plane = distance_from_planes.distance_from_plane;
          const double distance_along_plane = distance_from_planes.distance_along_plane;
          const double total_segment_length = additional_parameters.total_local_segment_length;

          if (distance_from_plane <= max_depth && distance_from_plane >= min_depth)
            {

              const CoordinateSystem coordinate_system = world->parameters.coordinate_system->natural_coordinate_system();
              double distance_ridge = std::numeric_limits<double>::max();
              const Point<3> trench_point = distance_from_planes.closest_trench_point;
              const Objects::NaturalCoordinate trench_point_natural = Objects::NaturalCoordinate(trench_point,
                                                                      *(world->parameters.coordinate_system));
              const Point<2> trench_point_2d(trench_point_natural.get_surface_coordinates(),trench_point_natural.get_coordinate_system());
              // find the distance between the trench and ridge


              // first find if the coordinate is on this side of a ridge
              unsigned int relevant_ridge = 0;


              // if there is only one ridge, there is no transform
              if (mid_oceanic_ridges.size() > 1)
                {
                  // There are more than one ridge, so there are transform faults
                  // Find the first which is on the same side
                  for (relevant_ridge = 0; relevant_ridge < mid_oceanic_ridges.size()-1; relevant_ridge++)
                    {
                      const Point<2> transform_point_0 = mid_oceanic_ridges[relevant_ridge+1][0];
                      const Point<2> transform_point_1 = mid_oceanic_ridges[relevant_ridge][mid_oceanic_ridges[relevant_ridge].size()-1];
                      const Point<2> reference_point   = mid_oceanic_ridges[relevant_ridge][0];

                      const bool reference_on_side_of_line = (transform_point_1[0] - transform_point_0[0])
                                                             * (reference_point[1] - transform_point_0[1])
                                                             - (transform_point_1[1] - transform_point_0[1])
                                                             * (reference_point[0] - transform_point_0[0])
                                                             < 0;
                      const bool checkpoint_on_side_of_line = (transform_point_1[0] - transform_point_0[0])
                                                              * (trench_point_2d[1] - transform_point_0[1])
                                                              - (transform_point_1[1] - transform_point_0[1])
                                                              * (trench_point_2d[0] - transform_point_0[0])
                                                              < 0;

                      if (reference_on_side_of_line == checkpoint_on_side_of_line)
                        {
                          break;
                        }

                    }
                }

              for (unsigned int i_coordinate = 0; i_coordinate < mid_oceanic_ridges[relevant_ridge].size() - 1; i_coordinate++)
                {
                  const Point<2> segment_point0 = mid_oceanic_ridges[relevant_ridge][i_coordinate];
                  const Point<2> segment_point1 = mid_oceanic_ridges[relevant_ridge][i_coordinate + 1];

                  // based on http://geomalgorithms.com/a02-_lines.html
                  const Point<2> v = segment_point1 - segment_point0;
                  const Point<2> w = trench_point_2d - segment_point0;

                  const double c1 = (w[0] * v[0] + w[1] * v[1]);
                  const double c2 = (v[0] * v[0] + v[1] * v[1]);

                  Point<2> Pb(coordinate_system);
                  // This part is needed when we want to consider segments instead of lines
                  // If you want to have infinite lines, use only the else statement.

                  if (c1 <= 0)
                    Pb = segment_point0;
                  else if (c2 <= c1)
                    Pb = segment_point1;
                  else
                    Pb = segment_point0 + (c1 / c2) * v;

                  Point<3> compare_point(coordinate_system);

                  compare_point[0] = coordinate_system == cartesian ? Pb[0] : trench_point_natural.get_depth_coordinate();
                  compare_point[1] = coordinate_system == cartesian ? Pb[1] : Pb[0];
                  compare_point[2] = coordinate_system == cartesian ? trench_point_natural.get_depth_coordinate() : Pb[1];

                  distance_ridge = std::min(distance_ridge, this->world->parameters.coordinate_system->distance_between_points_at_same_depth(Point<3>(trench_point_natural.get_coordinates(),trench_point_natural.get_coordinate_system()), compare_point));
                }

              const double age_at_trench = distance_ridge / plate_velocity; // yr

              const double seconds_in_year = 60.0 * 60.0 * 24.0 * 365.25;
              const double plate_age_sec = age_at_trench * seconds_in_year; // y --> seconds

              // 1. Determine initial heat content of the slab based on age of plate at trench
              //    This uses the integral of the half-space temperature profile
              double initial_heat_content = 2 * thermal_conductivity * (surface_temperature - potential_mantle_temperature) *
                                            std::sqrt(plate_age_sec / (thermal_diffusivity * Consts::PI));

              //  2. Get Tmin and distance_offset given distance along slab and depth of point on the slab.
              //  These equations are empirical based on fitting the temperature profiles from dynamic subduction models.
              double min_temperature = 0.0;
              double distance_offset = 0.0;

              const double mantle_coupling_length = mantle_coupling_depth / std::sin(shallow_average_dip * Consts::PI / 180.0); //m

              /* Empirical model parameters */
              //double upper_mantle_depth = 660e3;   // m
              double upper_mantle_length = 1000e3; // m
              /* Distance offset parameters */
              double offset_distance_min = 25 * 1e3; // m
              double offset_distance_max = 35 * 1e3; // m
              double slope_distance_min = 25e3 / (1.5 * 660e3);
              double slope_distance_max = 20e3 / (2.5 * 100e3);
              double sink_velocity_min = 0.025; // m/yr
              double sink_velocity_max = 0.10;  // m/yr
              /* minimum temperature parameters */
              double slope_temperature_min = 250 / (2.5 * 100 * 1e3); // deg/m (2.5 because dip in ref. model is 20-25 deg)
              double slope_temperature_max = 500 / (2.5 * 100 * 1e3); // deg/m
              double temperature_min = 750;
              double temperature_max = 950;

              double zero = 0.0;
              double one = 1.0;
             
              double vsubfact = (1 - (plate_velocity - sink_velocity_min) / sink_velocity_max);
              vsubfact = std::max(zero, std::min(vsubfact,one));
              
              double ref_age = 40e6; // yr
              double age_con = 4.0;
              double agefact = - age_con*(age_at_trench - ref_age)/ref_age;
              agefact = std::max(zero, std::min(agefact,age_con));

              double slope_distance_shallow = slope_distance_min + vsubfact * (slope_distance_max - slope_distance_min);
              double slope_temperature_shallow = slope_temperature_min + (vsubfact + agefact) * (slope_temperature_max - slope_temperature_min);

              double offset_coupling_depth = slope_distance_shallow * mantle_coupling_depth;                    // m  mantle_coupling_length
              double offset_660 = offset_distance_min + vsubfact * (offset_distance_max - offset_distance_min); // m

              double slope_distance_deep = (offset_660 - offset_coupling_depth) / (upper_mantle_length - mantle_coupling_length);
              double intercept_dist_deep = (slope_distance_shallow - slope_distance_deep) * mantle_coupling_length;

              double temperature_min_coupling_depth = slope_temperature_shallow * mantle_coupling_length;
              double temperature_min_660 = temperature_min + vsubfact * (temperature_max - temperature_min);

              double slope_temperature_deep = (temperature_min_660 - temperature_min_coupling_depth) / (upper_mantle_length - mantle_coupling_length);
              double intercept_temperature_deep = (slope_temperature_shallow - slope_temperature_deep) * (mantle_coupling_length);

              if (distance_along_plane <= mantle_coupling_length)
                {
                  distance_offset = slope_distance_shallow * distance_along_plane;
                  min_temperature = surface_temperature + slope_temperature_shallow * distance_along_plane;
                }
              else
                {
                  distance_offset = slope_distance_deep * distance_along_plane + intercept_dist_deep;
                  min_temperature = surface_temperature + slope_temperature_deep * distance_along_plane + intercept_temperature_deep;
                }

              double temperature = 0.0;

              // Need adiabatic temperature at position of grid point
              double background_temperature = adiabatic_heating ? potential_mantle_temperature * std::exp(((thermal_expansion_coefficient * gravity_norm * depth) / specific_heat))
                                              : potential_mantle_temperature;

              WBAssert(!std::isnan(background_temperature), "Internal error: temp is not a number: " << background_temperature << ". In exponent: "
                       << std::exp(((thermal_expansion_coefficient * gravity_norm) / specific_heat) * depth)
                       << ", thermal_expansion_coefficient = " << thermal_expansion_coefficient << ", gravity_norm = " << gravity_norm
                       << ", specific_heat = " << specific_heat << ", depth = " << depth);

              // Taper the heat_content and min temperature to create a smooth slab tip
              const double start_taper_distance =  total_segment_length -  taper_distance;

              if ((distance_along_plane > start_taper_distance) )
                {
                  initial_heat_content = initial_heat_content * (total_segment_length - distance_along_plane)/taper_distance;

                  min_temperature = surface_temperature + slope_temperature_deep * start_taper_distance + intercept_temperature_deep;
                  min_temperature =  min_temperature + (( background_temperature - min_temperature)/(total_segment_length - start_taper_distance)) *
                                     (distance_along_plane - start_taper_distance);

                }
              if (min_temperature < background_temperature)
                {
                  // Adjust distance for the offset of the minimum temperature from the top of the slab
                  const double adjusted_distance = distance_from_plane - distance_offset;

                  // 3. Determine the heat content for side 1 (bottom) of the slab
                  // Comes from integrating the half-space cooling model temperature

                  double effective_plate_age = plate_age_sec + (distance_along_plane / plate_velocity) * seconds_in_year; // m/(m/y) = y(seconds_in_year)
                  if (distance_along_plane > start_taper_distance)
                    {
                      effective_plate_age = effective_plate_age * (total_segment_length - distance_along_plane)/ (taper_distance);
                    }

                  const double bottom_heat_content = 2 * thermal_conductivity * (min_temperature - potential_mantle_temperature) *
                                                     std::sqrt(effective_plate_age /(thermal_diffusivity * Consts::PI));

                  // 4. The difference in heat content goes into the temperature above where Tmin occurs.
                  double top_heat_content = initial_heat_content - bottom_heat_content;

                  // Also need to taper the top_heat_content otherwise slab top will continue to thicken to the tip.
                  if (distance_along_plane > start_taper_distance)
                    {
                      top_heat_content = top_heat_content * (total_segment_length - distance_along_plane)/ (taper_distance);
                    }
                  // Assign the temperature depending on whether distance is negative (above) or positive (below) the slab
                  if (adjusted_distance < 0)
                    {
                      // use 1D infinite space solution for top (side 2) of slab the slab
                      // 2 times the "top_heat_content" because all this heat needs to be on one side of the Gaussian
                      double time_top_slab = (1/(Consts::PI*thermal_diffusivity))*pow(((2*top_heat_content)/
                                                                                       (2*density*specific_heat*(min_temperature - temperature_ + 1e-16))),2) + 1e-16;

                      // for overriding plate region where plate temperature is less the minimum slab temperature
                      // need to set temperature = temperature_ otherwise end up with temperature less than surface temperature ;
                      if (temperature_ < min_temperature)
                        {
                          temperature = temperature_;
                        }
                      else
                        {
                          temperature  = temperature_ + (2*top_heat_content/(2*density*specific_heat*std::sqrt(Consts::PI*thermal_diffusivity*time_top_slab)))*
                                         std::exp(-(adjusted_distance*adjusted_distance)/(4*thermal_diffusivity*time_top_slab));
                        }
                      // temperature = temperature_ + (2 * top_heat_content / (2 * density * specific_heat * std::sqrt(Consts::PI * thermal_diffusivity * time_top_slab))) *
                      //              std::exp(-(adjusted_distance * adjusted_distance) / (4 * thermal_diffusivity * time_top_slab));
                    }
                  else
                    {
                      // use half-space cooling model for the bottom (side 1) of the slab
                      temperature = background_temperature + (min_temperature - background_temperature) *
                                    std::erfc(adjusted_distance / (2 * std::sqrt(thermal_diffusivity * effective_plate_age)));
                    }
                }
              else
                {
                  // slab temperature anomaly is gone.
                  temperature = temperature_;
                }

              WBAssert(!std::isnan(temperature), "Internal error: temperature is not a number: " << temperature << '.');
              WBAssert(std::isfinite(temperature), "Internal error: temperature is not finite: " << temperature << '.');

              return Utilities::apply_operation(operation, temperature_, temperature);
            }

          return temperature_;
        }

        WB_REGISTER_FEATURE_SUBDUCTING_PLATE_TEMPERATURE_MODEL(MassConserving, mass conserving)
      } // namespace Temperature
    } // namespace SubductingPlateModels
  } // namespace Features
} // namespace WorldBuilder
