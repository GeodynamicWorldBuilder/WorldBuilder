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

#include "world_builder/features/plume.h"


#include "world_builder/features/plume_models/composition/interface.h"
#include "world_builder/features/plume_models/grains/interface.h"
#include "world_builder/features/plume_models/temperature/interface.h"
#include "world_builder/nan.h"
#include "world_builder/types/array.h"
#include "world_builder/types/double.h"
#include "world_builder/types/object.h"
#include "world_builder/types/one_of.h"
#include "world_builder/types/plugin_system.h"
#include "world_builder/types/value_at_points.h"
#include "world_builder/world.h"

#include "world_builder/kd_tree.h"

#include <iostream>


namespace WorldBuilder
{
  using namespace Utilities;

  namespace Features
  {
    Plume::Plume(WorldBuilder::World *world_)
      :
      min_depth(NaN::DSNAN),
      max_depth(NaN::DSNAN)
    {
      this->world = world_;
      this->name = "plume";
    }

    Plume::~Plume()
      = default;


    void
    Plume::declare_entries(Parameters &prm,
                                      const std::string & /*unused*/,
                                      const std::vector<std::string> &required_entries)
    {
      prm.declare_entry("", Types::Object(required_entries), "Plume object. Requires properties `model` and `coordinates`.");

      prm.declare_entry("min depth", Types::OneOf(Types::Double(0),Types::Array(Types::ValueAtPoints(0.))),
                        "The depth from which this feature is present. Units: m.");
      prm.declare_entry("max depth", Types::OneOf(Types::Double(std::numeric_limits<double>::max()),Types::Array(Types::ValueAtPoints(std::numeric_limits<double>::max()))),
                        "The depth to which this feature is present. Units: m.");
      prm.declare_entry("cross section depths", Types::Array(Types::Double(0)),
                        "The depths of the elliptic cross section of the plume. Units: m.");
      prm.declare_entry("semi-major axis", Types::Array(Types::Double(100.e3)),
                        "The lengths of the semi-major axes of the elliptic cross sections of the plume. "
                        "In spherical coordinates, this is in degrees, otherwise in meters.");
      prm.declare_entry("eccentricity", Types::Array(Types::Double(0)),
                        "The eccentricities of the cross sections.");
      prm.declare_entry("rotation angles", Types::Array(Types::Double(0)),
                        "The directions that the semi-major axis of the elliptic cross-sections "
                        "are pointing to, as an angle from geographic North in degrees. "
                        "The angle should be between 0 and 360 degrees.");

      prm.declare_entry("temperature models",
                        Types::PluginSystem("", Features::PlumeModels::Temperature::Interface::declare_entries, {"model"}),
                        "A list of temperature models.");
      prm.declare_entry("composition models",
                        Types::PluginSystem("", Features::PlumeModels::Composition::Interface::declare_entries, {"model"}),
                        "A list of composition models.");
      prm.declare_entry("grains models",
                        Types::PluginSystem("", Features::PlumeModels::Grains::Interface::declare_entries, {"model"}),
                        "A list of grains models.");
    }

    void
    Plume::parse_entries(Parameters &prm)
    {
      const CoordinateSystem coordinate_system = prm.coordinate_system->natural_coordinate_system();

      this->name = prm.get<std::string>("name");
      this->get_coordinates("coordinates", prm, coordinate_system);

      min_depth_surface = Objects::Surface(prm.get("min depth",coordinates));
      min_depth = min_depth_surface.minimum;

      max_depth_surface = Objects::Surface(prm.get("max depth",coordinates));
      max_depth = max_depth_surface.maximum;

      depths = prm.get_vector<double>("cross section depths");
      semi_major_axis_lengths = prm.get_vector<double>("semi-major axis");
      eccentricities = prm.get_vector<double>("eccentricity");
      rotation_angles = prm.get_vector<double>("rotation angles");

      for (unsigned int i = 0; i < depths.size()-1; ++i)
        WBAssert(depths[i] < depths[i+1],
                 "The depths of the elliptic cross sections of the plume need to be listed in ascending order.");

      WBAssert(depths.size() == coordinates.size(), 
               "The cross section depths array needs to have the same number of entries as there are coordinates. At the moment there are: " 
                << depths.size()
                << " depth entries but " 
                << coordinates.size() 
                << " coordinates!");

      WBAssert(semi_major_axis_lengths.size() == coordinates.size(), 
               "The semi-major axis array needs to have the same number of entries as there are coordinates. At the moment there are: " 
                << semi_major_axis_lengths.size()
                << " semi-major axis entries but " 
                << coordinates.size() 
                << " coordinates!");

      WBAssert(eccentricities.size() == coordinates.size(), 
               "The eccentricity array needs to have the same number of entries as there are coordinates. At the moment there are: " 
                << eccentricities.size()
                << " eccentricity entries but " 
                << coordinates.size() 
                << " coordinates!");

      WBAssert(rotation_angles.size() == coordinates.size(), 
               "The rotation angles array needs to have the same number of entries as there are coordinates. At the moment there are: " 
                << rotation_angles.size()
                << " rotation angle entries but " 
                << coordinates.size() 
                << " coordinates!");

      
      // Convert degrees to radians, convert from geographical to mathematical
      // TODO: convert semi_major_axis_lengths as well for spherical coordinates
      for (unsigned int i = 0; i < rotation_angles.size(); ++i)
        rotation_angles[i] = Consts::PI/2. - rotation_angles[i] * Consts::PI/180.;

      if (world->parameters.coordinate_system->natural_coordinate_system() == CoordinateSystem::spherical)
        for (unsigned int i = 0; i < semi_major_axis_lengths.size(); ++i)
      {
        semi_major_axis_lengths[i] *= Consts::PI/180.;
      }

      prm.get_unique_pointers<Features::PlumeModels::Temperature::Interface>("temperature models", temperature_models);

      prm.enter_subsection("temperature models");
      {
        for (unsigned int i = 0; i < temperature_models.size(); ++i)
          {
            prm.enter_subsection(std::to_string(i));
            {
              temperature_models[i]->parse_entries(prm,coordinates);
            }
            prm.leave_subsection();
          }
      }
      prm.leave_subsection();


      prm.get_unique_pointers<Features::PlumeModels::Composition::Interface>("composition models", composition_models);

      prm.enter_subsection("composition models");
      {
        for (unsigned int i = 0; i < composition_models.size(); ++i)
          {
            prm.enter_subsection(std::to_string(i));
            {
              composition_models[i]->parse_entries(prm, coordinates);
            }
            prm.leave_subsection();
          }
      }
      prm.leave_subsection();


      prm.get_unique_pointers<Features::PlumeModels::Grains::Interface>("grains models", grains_models);

      prm.enter_subsection("grains models");
      {
        for (unsigned int i = 0; i < grains_models.size(); ++i)
          {
            prm.enter_subsection(std::to_string(i));
            {
              grains_models[i]->parse_entries(prm,coordinates);
            }
            prm.leave_subsection();
          }
      }
      prm.leave_subsection();

    }



    void
    Plume::properties(const Point<3> &position_in_cartesian_coordinates,
                                 const Objects::NaturalCoordinate &position_in_natural_coordinates,
                                 const double depth,
                                 const std::vector<std::array<unsigned int,3>> &properties,
                                 const double gravity_norm,
                                 const std::vector<size_t> &entry_in_output,
                                 std::vector<double> &output) const
    {
      // Figure out if the point is within the plume
      auto upper = std::upper_bound(depths.begin(), depths.end(), depth);

      Point<2> plume_center(coordinates[0]);
      double semi_major_axis_length;
      double eccentricity;
      double rotation_angle;

      if (upper == depths.begin())
        return;
      else if (upper == depths.end())
      {
        plume_center = coordinates.back();
        semi_major_axis_length = semi_major_axis_lengths.back();
        eccentricity = eccentricities.back();
        rotation_angle = rotation_angles.back();
      }
      else
      {
        const unsigned int index = std::distance(depths.begin(), upper);
        const double fraction = (depth - depths[index-1]) / (depths[index] - depths[index-1]);

        plume_center[0] = (1-fraction) * coordinates[index-1][0] + fraction * (coordinates[index][0]);
        plume_center[1] = (1-fraction) * coordinates[index-1][1] + fraction * (coordinates[index][1]);

        semi_major_axis_length = (1-fraction) * semi_major_axis_lengths[index-1] + fraction * semi_major_axis_lengths[index];
        eccentricity = (1-fraction) * eccentricities[index-1] + fraction * eccentricities[index];

        // For the angles, we only want to go between zero and pi, and we have to make sure we
        // interpolate the values close to zero/pi correctly: 
        // TODO: make utilities function
        double angle_1 = rotation_angles[index-1];
        double angle_2 = rotation_angles[index];
        if (std::abs(angle_2 - angle_1) > Consts::PI)
        {
          if (angle_2 > angle_1)
            angle_1 += 2.*Consts::PI;
          else
            angle_2 += 2.*Consts::PI;
        }
        rotation_angle = (1-fraction) * angle_1 + fraction * angle_2;

        // make sure angle is between 0 and 360 degrees
        rotation_angle = rotation_angle - 2*Consts::PI*std::floor(rotation_angle/(2 * Consts::PI));
      }

      if (depth <= depths.back() && depth >= depths.front() &&
          WorldBuilder::Utilities::ellipse_contains_point(plume_center,
                                                          semi_major_axis_length,
                                                          eccentricity,
                                                          rotation_angle,
                                                          Point<2>(position_in_natural_coordinates.get_surface_coordinates(),
                                                                   world->parameters.coordinate_system->natural_coordinate_system())))
        {
          const double min_depth_local = min_depth_surface.constant_value ? min_depth : min_depth_surface.local_value(position_in_natural_coordinates.get_surface_point()).interpolated_value;
          const double max_depth_local = max_depth_surface.constant_value ? max_depth : max_depth_surface.local_value(position_in_natural_coordinates.get_surface_point()).interpolated_value;
          if (depth <= max_depth_local &&  depth >= min_depth_local)
            {
              for (unsigned int i_property = 0; i_property < properties.size(); ++i_property)
                {
                  switch (properties[i_property][0])
                    {
                      case 1:  // temperature
                      {
                        for (const auto &temperature_model: temperature_models)
                          {
                            output[entry_in_output[i_property]] = temperature_model->get_temperature(position_in_cartesian_coordinates,
                                                                                                     position_in_natural_coordinates,
                                                                                                     depth,
                                                                                                     gravity_norm,
                                                                                                     output[entry_in_output[i_property]],
                                                                                                     min_depth_local,
                                                                                                     max_depth_local);

                            WBAssert(!std::isnan(output[entry_in_output[i_property]]), "Temparture is not a number: " << output[entry_in_output[i_property]]
                                     << ", based on a temperature model with the name " << temperature_model->get_name() << ", in feature " << this->name);
                            WBAssert(std::isfinite(output[entry_in_output[i_property]]), "Temparture is not finite: " << output[entry_in_output[i_property]]
                                     << ", based on a temperature model with the name " << temperature_model->get_name() << ", in feature " << this->name);

                          }
                        break;
                        case 2: // composition

                          for (const auto &composition_model: composition_models)
                            {
                              output[entry_in_output[i_property]] = composition_model->get_composition(position_in_cartesian_coordinates,
                                                                                                       position_in_natural_coordinates,
                                                                                                       depth,
                                                                                                       properties[i_property][1],
                                                                                                       output[entry_in_output[i_property]],
                                                                                                       min_depth_local,
                                                                                                       max_depth_local);

                              WBAssert(!std::isnan(output[entry_in_output[i_property]]), "Composition is not a number: " << output[entry_in_output[i_property]]
                                       << ", based on a composition model with the name " << composition_model->get_name() << ", in feature " << this->name);
                              WBAssert(std::isfinite(output[entry_in_output[i_property]]), "Composition is not finite: " << output[entry_in_output[i_property]]
                                       << ", based on a composition model with the name " << composition_model->get_name() << ", in feature " << this->name);

                            }

                          break;
                        }
                      case 3: // grains
                      {
                        WorldBuilder::grains  grains(output,properties[i_property][2],entry_in_output[i_property]);
                        for (const auto &grains_model: grains_models)
                          {
                            grains = grains_model->get_grains(position_in_cartesian_coordinates,
                                                              position_in_natural_coordinates,
                                                              depth,
                                                              properties[i_property][1],
                                                              grains,
                                                              min_depth_local,
                                                              max_depth_local);

                          }
                        grains.unroll_into(output,entry_in_output[i_property]);
                      }
                      break;
                      default:
                        WBAssertThrow(false, "Internal error: Unimplemented property provided. Only temperature (1), composition (2) or grains (3) are allowed.");
                    }
                }
            }
        }
    }

    WB_REGISTER_FEATURE(Plume, plume)

  } // namespace Features
} // namespace WorldBuilder
