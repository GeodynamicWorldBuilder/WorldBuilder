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

#include "world_builder/features/oceanic_plate_models/topography/half_space_cooling.h"

#include <world_builder/world.h>
#include "world_builder/features/oceanic_plate_models/topography/interface.h"
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
    namespace OceanicPlateModels
    {
      namespace Topography
      {
        HalfSpaceCooling::HalfSpaceCooling(WorldBuilder::World *world_):
          min_depth(NaN::DSNAN),
          max_depth(NaN::DSNAN),
          min_ocean_depth(NaN::DSNAN),
          operation(Operations::REPLACE),
          top_temperature(NaN::DSNAN),
          bottom_temperature(NaN::DSNAN),
          top_density(NaN::DSNAN),
          bottom_density(NaN::DSNAN)
        {
          this->world = world_;
          this->name = "half space cooling";
        }

        HalfSpaceCooling::~HalfSpaceCooling()
          = default;

        void
        HalfSpaceCooling::declare_entries(Parameters &prm, const std::string & /*unused*/)
        {
          // Document plugin and require entries if needed.
          // Add `topography` and half space model params to the required parameters.

          prm.declare_entry("", Types::Object({"ridge coordinates", "spreading velocity",
                                               "max depth", "min ocean depth", "bottom density", 
                                               "top density"
                                              }), "Half space cooled topography");

          prm.declare_entry("min depth", Types::OneOf(Types::Double(0),
                                                      Types::Array(Types::ValueAtPoints(0.,2)),
                                                      Types::String("")),
                            "The depth in meters from which the composition of this feature is present.");

          prm.declare_entry("max depth", Types::OneOf(Types::Double(std::numeric_limits<double>::max()),
                                                      Types::Array(Types::ValueAtPoints(std::numeric_limits<double>::max(),2)),
                                                      Types::String("")),
                            "The depth in meters to which the composition of this feature is present.");

          prm.declare_entry("min ocean depth", Types::Double(0), "The minimum depth of the ocean or depth"
                            "of the ridge below the ocean surface in meters.");

          prm.declare_entry("top temperature", Types::Double(293.15),
                            "The actual surface temperature in degree Kelvin for this feature.");

          prm.declare_entry("bottom temperature", Types::Double(-1),
                            "The mantle temperature for the half-space cooling model"
                            "in degree Kelvin for this feature. If the model has an adiabatic gradient"
                            "this should be the mantle potential temperature, and T = Tad + Thalf. ");

          prm.declare_entry("top density", Types::Double(0.0),
                            "The density of the top or surface of the ridge in kg/m3");

          prm.declare_entry("bottom density", Types::Double(0.0),
                            "The density of the bottom of the ridge in kg/m3");

          prm.declare_entry("spreading velocity", Types::OneOf(Types::Double(0.05),Types::Array(Types::ValueAtPoints(0.05, std::numeric_limits<uint64_t>::max()))),
                            "The spreading velocity of the plate in meter per year. "
                            "This is the velocity with which one side moves away from the ridge.");

          prm.declare_entry("ridge coordinates", Types::Array(Types::Array(Types::Point<2>(), 2),1),
                            "An list of ridges. Each ridge is a lists of at least 2 2d points which "
                            "define the location of the ridge. You need to define at least one ridge."
                            "So the an example with two ridges is "
                            "[[[10,20],[20,30],[10,40]],[[50,10],[60,10]]].");
        }

        void
        HalfSpaceCooling::parse_entries(Parameters &prm,
                                          const std::vector<Point<2>> &coordinates)
        {
          min_depth_surface = Objects::Surface(prm.get("min depth",coordinates));
          min_depth = min_depth_surface.minimum;
          max_depth_surface = Objects::Surface(prm.get("max depth",coordinates));
          max_depth = max_depth_surface.maximum;

          operation = string_operations_to_enum(prm.get<std::string>("operation"));
          min_ocean_depth = prm.get<double>("min ocean depth");
          top_temperature = prm.get<double>("top temperature");
          bottom_temperature = prm.get<double>("bottom temperature");

          top_density = prm.get<double>("top density");
          bottom_density = prm.get<double>("bottom density");

          spreading_velocities = prm.get_value_at_array("spreading velocity");

          mid_oceanic_ridges = prm.get_vector<std::vector<Point<2>>>("ridge coordinates");
          const double dtr = prm.coordinate_system->natural_coordinate_system() == spherical ? Consts::PI / 180.0 : 1.0;
          for (auto &ridge_coordinates : mid_oceanic_ridges)
            for (auto &ridge_coordinate : ridge_coordinates)
              {
                ridge_coordinate *= dtr;
              }

          unsigned int ridge_point_index = 0;
          for (const auto &mid_oceanic_ridge : mid_oceanic_ridges)
            {
              std::vector<double> spreading_rates_for_ridge;
              for (unsigned int index_y = 0; index_y < mid_oceanic_ridge.size(); index_y++)
                {
                  if (spreading_velocities.second.size() <= 1)
                    {
                      spreading_rates_for_ridge.push_back(spreading_velocities.second[0]);
                    }
                  else
                    {
                      spreading_rates_for_ridge.push_back(spreading_velocities.second[ridge_point_index]);
                    }
                  ridge_point_index += 1;
                }
              spreading_velocities_at_each_ridge_point.push_back(spreading_rates_for_ridge);
            }
        }

        double
        HalfSpaceCooling::get_topography(const Point<3> &position_in_cartesian_coordinates,
                                           const Objects::NaturalCoordinate &position_in_natural_coordinates,
                                           double /*topo*/) const
        {
          (void) position_in_natural_coordinates;
          Objects::NaturalCoordinate position_in_natural_coordinates_at_min_depth = Objects::NaturalCoordinate(position_in_cartesian_coordinates,
                                                                                    *(this->world->parameters.coordinate_system));
          std::vector<std::vector<double>> subducting_plate_velocities = {{0}};
          std::vector<double> ridge_migration_times = {0.0};

          // compute ridge params like spreading velocity and distance from ridge
          std::vector<double> ridge_parameters = Utilities::calculate_ridge_distance_and_spreading(
                                                   mid_oceanic_ridges,
                                                   spreading_velocities_at_each_ridge_point,
                                                   this->world->parameters.coordinate_system,
                                                   position_in_natural_coordinates_at_min_depth,
                                                   subducting_plate_velocities,
                                                   ridge_migration_times);
          double ridge_distance = ridge_parameters[1];
          double spreading_velocity = ridge_parameters[0];
          double age = ridge_distance / spreading_velocity; // in sec

          const double thermal_diffusivity = this->world->thermal_diffusivity;
          const double alpha = this->world->thermal_expansion_coefficient;

          // age of half space cooling model is used to compute the heights and added with initial depth and topo
          double half_space_cooling_height = (2.0 * bottom_density * alpha * (bottom_temperature - top_temperature) / top_density) *
                                            std::sqrt(thermal_diffusivity * age / Consts::PI);

          return -(half_space_cooling_height + min_ocean_depth);

        }

        WB_REGISTER_FEATURE_OCEANIC_PLATE_TOPOGRAPHY_MODEL(HalfSpaceCooling, half space cooling)
      } // namespace Topography
    } // namespace OceanicPlateModels
  } // namespace Features
} // namespace WorldBuilder
