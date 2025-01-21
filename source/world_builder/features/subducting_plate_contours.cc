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

#include "world_builder/features/subducting_plate_contours.h"


#include "glm/glm.h"
#include "world_builder/types/array.h"
#include "world_builder/types/double.h"
#include "world_builder/types/object.h"
#include "world_builder/types/point.h"
#include "world_builder/types/segment.h"
#include "world_builder/types/contour.h"
#include "world_builder/types/unsigned_int.h"
#include "world_builder/world.h"
#include <algorithm>
#include <array>

/*
The general for of the contours geometry type will probably look like this.

{
  "model": "subducting plate",
  "name": "My SLAB",
  "geometry type": "contours",

  "contours": [[0, 0],
               [1,2]
               {"coordinate":[1, 1]},
               {"coordinate":[0, 0], "thickness": 10e3},
               [3,5]
               .
               .
               }], "thickness": 10e3

  OR 

  "contours": [{"coordinate":[0, 0], "thickness": 10e3},
               {"coordinate":[1, 1], "thickness": 20e3},
               .
               .
               .
               }]

  "depth contours": [{"depth":0.0, "contours":[{"coord":[0, 0],"thickness":10e3}, [1, 0], [1, 1], [0, 1]], "thickness":1000, "temperature models":[]},
                     {"depth":100.0, "contours":[[0, 0], [1, 0], [1, 1], [0, 1]], "thickness":1000}],
  "temperature models":[{"model":"uniform", "temperature":600.0}],
  "composition models":[{"model":"uniform", "compositions": [0], "fractions":[1.0]}]
}

This should result in a slab that has a temperature of 600.0 and a composition of 0.0 at all depths. 
This can then be extended to have variable temperature and composition models at different depths and
different along strike points.

The workflow will be to first fit the contour with a bezier curve, determine the arc-length of the curve,
and then divide the arc-length into even intervals. All contours will be divided into the same number of
intervals. Then the corresponding indices of the contours will be used to connect the contour and create 
a 3D surface.
*/

using namespace std;

namespace WorldBuilder
{
  using namespace Utilities;

  namespace Features
  {
    SubductingPlateContours::SubductingPlateContours(WorldBuilder::World *world_)
      :
      reference_point(0,0,cartesian)
    {
      this->world = world_;
      this->name = "subducting plate contours";
    }

    SubductingPlateContours::~SubductingPlateContours()
      = default;



    void SubductingPlateContours::make_snippet(Parameters &prm)
    {
      using namespace rapidjson;
      Document &declarations = prm.declarations;

      const std::string path = prm.get_full_json_path();

      /*
      ideally:
       {
        "model": "fault",
        "name": "${1:default name}",
        "dip point":[0.0,0.0],
        "coordinates": [[0.0,0.0]],
        "segments": [],
        "sections": [],
        "temperature models":[{"model":"uniform", "temperature":600.0}],
        "composition models":[{"model":"uniform", "compositions": [0], "fractions":[1.0]}]
       }
       */

      Pointer((path + "/body").c_str()).Set(declarations,"object");
      Pointer((path + "/body/model").c_str()).Set(declarations,"subducting plate");
      Pointer((path + "/body/name").c_str()).Set(declarations,"${1:My Subducting Plate}");
      Pointer((path + "/body/coordinates").c_str()).Create(declarations).SetArray();
      Pointer((path + "/body/temperature models").c_str()).Create(declarations).SetArray();
      Pointer((path + "/body/composition models").c_str()).Create(declarations).SetArray();
    }



    void
    SubductingPlateContours::declare_entries(Parameters &prm,
                                     const std::string &parent_name,
                                     const std::vector<std::string> &required_entries)
    {
      prm.declare_entry("geometry type", Types::String("contours"),
                        "The model geometry type");
      prm.declare_entry("temperature models",
                        Types::PluginSystem("", Features::SubductingPlateModels::Temperature::Interface::declare_entries, {"model"}),
                        "A list of temperature models.");
      prm.declare_entry("composition models",
                        Types::PluginSystem("", Features::SubductingPlateModels::Composition::Interface::declare_entries, {"model"}),
                        "A list of composition models.");
      prm.declare_entry("grains models",
                        Types::PluginSystem("", Features::SubductingPlateModels::Grains::Interface::declare_entries, {"model"}),
                        "A list of grains models.");
      prm.declare_entry("velocity models",
                        Types::PluginSystem("", Features::SubductingPlateModels::Velocity::Interface::declare_entries, {"model"}),
                        "A list of velocity models.");
    }

    void
    SubductingPlateContours::parse_entries(Parameters &prm)
    {
      const CoordinateSystem coordinate_system = prm.coordinate_system->natural_coordinate_system();

      this->name = prm.get<std::string>("name");
      std::string geometry_type = prm.get<std::string>("geometry type");

      std::string tag = prm.get<std::string>("tag");
      if (tag == "")
        {
          tag = "subducting plate";
        }
      this->tag_index = FeatureUtilities::add_vector_unique(this->world->feature_tags,tag);

      // this->get_coordinates("coordinates", prm, coordinate_system);

      // Divide the bezier_curve into intervals, and use the operator function to generate the points on the curve.
      // I need to check the length of each bezier curve and compare it with the interval distance that I calculate.
      // This will then tell me what the index of the pair of points is that I need to deal with, and then I can
      // compute the t value that I need to use to get the point on the bezier curve.

      // This line will need to be adjusted so that it reads in the size of the vector "depth value"
      std::vector<double> depths_of_contours = {0, 100e3}; // prm.get_vector<Objects::Segment<Features::SubductingPlateModels::Temperature::Interface;

      const unsigned int number_of_intervals = 100;
      std::vector<std::vector<Point<2>>> regularized_points;
      regularized_points.resize(depths_of_contours.size());
      slab_segment_lengths.resize(depths_of_contours.size());
      slab_segment_thickness.resize(depths_of_contours.size());
      slab_segment_top_truncation.resize(depths_of_contours.size());
      slab_segment_angles.resize(depths_of_contours.size());
      const double total_arclength = 5000e3;




      for (unsigned int i = 0; i < depths_of_contours.size(); ++i)
        {
          // Somehow here I need to specify which coordinates I am using. This should be possible by using the 
          // index i which corresponds to the index of the depth value within the array of depth points.
          this->get_coordinates("contours", prm, coordinate_system);
          const double contour_arc_length = this->bezier_curve.get_bezier_arclength(); // not implemented in this branch
          std::vector<double> bezier_sub_lengths = this->bezier_curve.get_bezier_segment_lengths();

          for (unsigned int t = 0; t < number_of_intervals; ++t)
            {
              // The distance along the current bezier curve
              const double distance_along_contour = t * contour_arc_length / static_cast<double>(number_of_intervals);
              double sum_of_bezier_curves = 0; // The sum of all the previous bezier curves
              unsigned int q = 0; // The index of the bezier curve. This is supposed to start from 0 and
              // then increase until the sum of the bezier curves is greater than the distance along the contour.
              // however, in this case there is only one bezier curve, so this will always be 0.
              while (sum_of_bezier_curves < total_arclength)
                {
                  sum_of_bezier_curves += distance_along_contour;
                  ++q;
                }
              const double remaining_distance = distance_along_contour - sum_of_bezier_curves;
              const double t_value = remaining_distance / bezier_sub_lengths[q];
              regularized_points[i].emplace_back(this->bezier_curve(q, t_value));
            }
          }

      for (unsigned int i = 0; i < depths_of_contours.size() - 1; ++i)
        {
          slab_segment_lengths[i].resize(number_of_intervals);
          slab_segment_thickness[i].resize(number_of_intervals);
          slab_segment_top_truncation[i].resize(number_of_intervals);
          slab_segment_angles[i].resize(number_of_intervals, Point<2>(invalid));
          for (unsigned int t = 0; t < number_of_intervals; ++t)
            {
              std::array<double, 3> P1 = {regularized_points[i][t][0], regularized_points[i][t][1], depths_of_contours[i]};
              std::array<double, 3> P2 = {regularized_points[i + 1][t][0], regularized_points[i + 1][t][1], depths_of_contours[i + 1]};
              std::array<double, 3> P1P2 = {P2[0] - P1[0], P2[1] - P1[1], P2[2] - P1[2]};

              slab_segment_lengths[i][t] = std::sqrt(P1P2[0] * P1P2[0] + P1P2[1] * P1P2[1] + P1P2[2] * P1P2[2]);
              slab_segment_thickness[i][t] = slab_thickness;

              // Angle with the Z-axis
              slab_segment_angles[i][t][0] = std::acos(P1P2[2] / slab_segment_lengths[i][t]);
              slab_segment_top_truncation[i][t] = top_truncation;
            }
        }



// Hypothetically I have calculate the angles, filled the thicknesses, the top truncation values, and the lengths.
// Maybe the next step is just to check if this actually creates a slab that is a ramp connecting from 0 depth to 100
// km depth.




      if (coordinate_system == spherical)
        {
          // When spherical, input is in degrees, so change to radians for internal use.
          reference_point *= (Consts::PI/180.);
        }

      default_temperature_models.resize(0);
      default_composition_models.resize(0);
      default_grains_models.resize(0);
      default_velocity_models.resize(0);
      prm.get_shared_pointers<Features::SubductingPlateModels::Temperature::Interface>("temperature models", default_temperature_models);
      prm.get_shared_pointers<Features::SubductingPlateModels::Composition::Interface>("composition models", default_composition_models);
      prm.get_shared_pointers<Features::SubductingPlateModels::Grains::Interface>("grains models", default_grains_models);
      prm.get_shared_pointers<Features::SubductingPlateModels::Velocity::Interface>("velocity models", default_velocity_models);

      // Here, we compute the spherical bounding box using the two extreme points of the box containing all the surface
      // coordinates and an additional buffer zone that accounts for the slab thickness and length. The first and second
      // points correspond to the lower left and the upper right corners of the bounding box, respectively (see the
      // documentation in include/bounding_box.h).
      // For the spherical system, the buffer zone along the longitudal direction is calculated using the
      // corresponding latitude points.

      // Find minimal and maximal coordinates. Do this by finding the
      // leftmost/rightmost point with regard to either the [0] or [1]
      // coordinate, and then takes its [0] or [1] element.
      auto compare_x_coordinate = [](auto p1, auto p2)
      {
        return p1[0]<p2[0];
      };

      min_along_x = (*std::min_element(coordinates.begin(), coordinates.end(), compare_x_coordinate)) [0];
      max_along_x = (*std::max_element(coordinates.begin(), coordinates.end(), compare_x_coordinate)) [0];


      auto compare_y_coordinate = [](auto p1, auto p2)
      {
        return p1[1]<p2[1];
      };

      min_along_y = (*std::min_element(coordinates.begin(), coordinates.end(), compare_y_coordinate)) [1];
      max_along_y = (*std::max_element(coordinates.begin(), coordinates.end(), compare_y_coordinate)) [1];

      min_lat_cos_inv = 1. / std::cos(min_along_y);
      max_lat_cos_inv = 1. / std::cos(max_along_y);

      buffer_around_slab_cartesian = (maximum_slab_thickness + maximum_total_slab_length);

      // Compute the surface bounding box
      buffer_around_slab_cartesian = (maximum_slab_thickness + maximum_total_slab_length);
      if (world->parameters.coordinate_system->natural_coordinate_system() == CoordinateSystem::spherical)
        {
          const double starting_radius_inv = 1 / (world->parameters.coordinate_system->max_model_depth());
          std::pair<Point<2>, Point<2> > &spherical_bounding_box = surface_bounding_box.get_boundary_points();

          const double buffer_around_slab_spherical = 2 * Consts::PI * buffer_around_slab_cartesian * starting_radius_inv;

          spherical_bounding_box.first = {(min_along_x - buffer_around_slab_spherical * min_lat_cos_inv) ,
                                          (min_along_y - buffer_around_slab_spherical), spherical
                                         } ;

          spherical_bounding_box.second = {(max_along_x + buffer_around_slab_spherical * max_lat_cos_inv) ,
                                           (max_along_y + buffer_around_slab_spherical), spherical
                                          };
        }
      else if (world->parameters.coordinate_system->natural_coordinate_system() == CoordinateSystem::cartesian)
        {
          std::pair<Point<2>, Point<2> > &bounding_box = surface_bounding_box.get_boundary_points();
          bounding_box.first = {min_along_x, min_along_y, cartesian};
          bounding_box.second = {max_along_x, max_along_y, cartesian};
          surface_bounding_box.extend(buffer_around_slab_cartesian);
        }
    }



    const BoundingBox<2> &
    SubductingPlateContours::get_surface_bounding_box () const
    {
      return surface_bounding_box;
    }


    void
    SubductingPlateContours::properties(const Point<3> &position_in_cartesian_coordinates,
                                const Objects::NaturalCoordinate &position_in_natural_coordinates,
                                const double depth,
                                const std::vector<std::array<unsigned int,3>> &properties,
                                const double gravity_norm,
                                const std::vector<size_t> &entry_in_output,
                                std::vector<double> &output) const
    {
      // The depth variable is the distance from the surface to the position, the depth
      // coordinate is the distance from the bottom of the model to the position and
      // the starting radius is the distance from the bottom of the model to the surface.
      const double starting_radius = position_in_natural_coordinates.get_depth_coordinate() + depth - starting_depth;

      WBAssert(std::abs(starting_radius) > std::numeric_limits<double>::epsilon(), "World Builder error: starting_radius can not be zero. "
               << "Position = " << position_in_cartesian_coordinates[0] << ':' << position_in_cartesian_coordinates[1] << ':' << position_in_cartesian_coordinates[2]
               << ", position_in_natural_coordinates.get_depth_coordinate() = " << position_in_natural_coordinates.get_depth_coordinate()
               << ", depth = " << depth
               << ", starting_depth " << starting_depth
              );

      // todo: explain and check -starting_depth
      if (depth <= maximum_depth && depth >= starting_depth && depth <= maximum_total_slab_length + maximum_slab_thickness &&
          get_surface_bounding_box().point_inside(Point<2>(position_in_natural_coordinates.get_surface_coordinates(),
                                                           world->parameters.coordinate_system->natural_coordinate_system())))
        {
          const WorldBuilder::Utilities::PointDistanceFromCurvedPlanes distance_from_planes =
            WorldBuilder::Utilities::distance_point_from_curved_planes(position_in_cartesian_coordinates,
                                                                       position_in_natural_coordinates,
                                                                       reference_point,
                                                                       coordinates,
                                                                       slab_segment_lengths,
                                                                       slab_segment_angles,
                                                                       starting_radius,
                                                                       this->world->parameters.coordinate_system,
                                                                       false,
                                                                       this->bezier_curve);

          const double distance_from_plane = distance_from_planes.distance_from_plane;
          const double distance_along_plane = distance_from_planes.distance_along_plane;
          const double section_fraction = distance_from_planes.fraction_of_section;
          const size_t current_section = distance_from_planes.section;
          const size_t next_section = current_section + 1;
          const size_t current_segment = distance_from_planes.segment; // the original value was a unsigned in, converting it back.
          //const size_t next_segment = current_segment + 1;
          const double segment_fraction = distance_from_planes.fraction_of_segment;

          if (abs(distance_from_plane) < std::numeric_limits<double>::infinity() || (distance_along_plane) < std::numeric_limits<double>::infinity())
            {
              // We want to do both section (horizontal) and segment (vertical) interpolation.
              // first for thickness
              const double thickness_up = slab_thickness;
              const double thickness_down = slab_thickness;
              const double thickness_local = slab_thickness;

              // if the thickness is zero, we don't need to compute anything, so return.
              if (std::fabs(thickness_local) < 2.0 * std::numeric_limits<double>::epsilon())
                return;

              // secondly for top truncation
              const double top_truncation_up = top_truncation;
              const double top_truncation_down = top_truncation;
              const double top_truncation_local = top_truncation;

              // if the thickness is smaller than what is truncated off at the top, we don't need to compute anything, so return.
              if (thickness_local < top_truncation_local)
                return;

              const double max_slab_length = total_slab_length[current_section] +
                                             section_fraction *
                                             (total_slab_length[next_section] - total_slab_length[current_section]);

              if (distance_from_plane >= top_truncation_local &&
                  distance_from_plane <= thickness_local &&
                  distance_along_plane >= 0 &&
                  distance_along_plane <= max_slab_length)
                {
                  // Inside the slab!
                  const Features::AdditionalParameters additional_parameters = {max_slab_length,thickness_local};
                  for (unsigned int i_property = 0; i_property < properties.size(); ++i_property)
                    {
                      switch (properties[i_property][0])
                        {
                          case 1: // temperature
                          {

                            for (const auto &temperature_model: segment_vector[current_section][current_segment].temperature_systems)
                              {
                                double temp_1 = 1;
                              }

                            for (const auto &temperature_model: segment_vector[next_section][current_segment].temperature_systems)
                              {
                                double temp_2 = 1;
                              }

                            // linear interpolation between current and next section temperatures
                            break;
                          }
                          case 2: // composition
                          {
                            for (const auto &composition_model: segment_vector[current_section][current_segment].composition_systems)
                              {
                                double test_2 = 1;
                              }

                            for (const auto &composition_model: segment_vector[next_section][current_segment].composition_systems)
                              {
                                double test_1 = 1;
                              }

                            // linear interpolation between current and next section temperatures
                            break;
                          }
                          case 3: // grains
                          {
                            WorldBuilder::grains grains(output,properties[i_property][2],entry_in_output[i_property]);
                            WorldBuilder::grains  grains_current_section = grains;
                            WorldBuilder::grains  grains_next_section = grains;

                            for (const auto &grains_model: segment_vector[current_section][current_segment].grains_systems)
                              {
                                grains_current_section = grains_model->get_grains(position_in_cartesian_coordinates,
                                                                                  depth,
                                                                                  properties[i_property][1],
                                                                                  grains_current_section,
                                                                                  starting_depth,
                                                                                  maximum_depth,
                                                                                  distance_from_planes,
                                                                                  additional_parameters);

                              }

                            for (const auto &grains_model: segment_vector[next_section][current_segment].grains_systems)
                              {
                                grains_next_section = grains_model->get_grains(position_in_cartesian_coordinates,
                                                                               depth,
                                                                               properties[i_property][1],
                                                                               grains_next_section,
                                                                               starting_depth,
                                                                               maximum_depth,
                                                                               distance_from_planes,
                                                                               additional_parameters);

                              }

                            // linear interpolation between current and next section temperatures
                            for (size_t i = 0; i < grains.sizes.size(); i++)
                              {
                                grains.sizes[i] = grains_current_section.sizes[i] + section_fraction * (grains_next_section.sizes[i] - grains_current_section.sizes[i]);
                              }

                            // average two rotations matrices through quaternions.
                            for (size_t i = 0; i < grains_current_section.rotation_matrices.size(); i++)
                              {
                                const glm::quaternion::quat quat_current = glm::quaternion::quat_cast(grains_current_section.rotation_matrices[i]);
                                const glm::quaternion::quat quat_next = glm::quaternion::quat_cast(grains_next_section.rotation_matrices[i]);

                                const glm::quaternion::quat quat_average = glm::quaternion::slerp(quat_current,quat_next,section_fraction);

                                grains.rotation_matrices[i] = glm::quaternion::mat3_cast(quat_average);
                              }

                            grains.unroll_into(output,entry_in_output[i_property]);
                            break;
                          }
                          case 4:
                          {
                            output[entry_in_output[i_property]] = static_cast<double>(tag_index);
                            break;
                          }
                          case 5: // velocity
                          {
                            std::array<double,3> velocity_current_section;// = Point<3>(cartesian);
                            velocity_current_section[0] = output[entry_in_output[i_property]];
                            velocity_current_section[1] = output[entry_in_output[i_property]+1];
                            velocity_current_section[2] = output[entry_in_output[i_property]]+2;
                            std::array<double,3> velocity_next_section = velocity_current_section;// output[entry_in_output[i_property]];

                            for (const auto &velocity_model: segment_vector[current_section][current_segment].velocity_systems)
                              {
                                velocity_current_section = velocity_model->get_velocity(position_in_cartesian_coordinates,
                                                                                        depth,
                                                                                        gravity_norm,
                                                                                        velocity_current_section,
                                                                                        starting_depth,
                                                                                        maximum_depth,
                                                                                        distance_from_planes,
                                                                                        additional_parameters);

                                //WBAssert(!std::isnan(velocity_current_section), "Velocity is not a number: " << velocity_current_section
                                //         << ", based on a velocity model with the name " << velocity_model->get_name() << ", in feature " << this->name);
                                //WBAssert(std::isfinite(velocity_current_section), "Velocity is not a finite: " << velocity_current_section
                                //         << ", based on a velocity model with the name " << velocity_model->get_name() << ", in feature " << this->name);

                              }

                            for (const auto &velocity_model: segment_vector[next_section][current_segment].velocity_systems)
                              {
                                velocity_next_section = velocity_model->get_velocity(position_in_cartesian_coordinates,
                                                                                     depth,
                                                                                     gravity_norm,
                                                                                     velocity_next_section,
                                                                                     starting_depth,
                                                                                     maximum_depth,
                                                                                     distance_from_planes,
                                                                                     additional_parameters);

                                //WBAssert(!std::isnan(velocity_next_section), "Velocity is not a number: " << velocity_next_section
                                //         << ", based on a velocity model with the name " << velocity_model->get_name() << ", in feature " << this->name);
                                //WBAssert(std::isfinite(velocity_next_section), "Velocity is not a finite: " << velocity_next_section
                                //         << ", based on a velocity model with the name " << velocity_model->get_name() << ", in feature " << this->name);

                              }

                            // linear interpolation between current and next section velocitys
                            output[entry_in_output[i_property]] = velocity_current_section[0] + section_fraction * (velocity_next_section[0] - velocity_current_section[0]);
                            output[entry_in_output[i_property]+1] = velocity_current_section[1] + section_fraction * (velocity_next_section[1] - velocity_current_section[1]);
                            output[entry_in_output[i_property]+2] = velocity_current_section[2] + section_fraction * (velocity_next_section[2] - velocity_current_section[2]);
                            break;
                          }
                          default:
                          {
                            WBAssertThrow(false,
                                          "Internal error: Unimplemented property provided. " <<
                                          "Only temperature (1), composition (2), grains (3), tag (4) or velocity (5) are allowed. "
                                          "Provided property number was: " << properties[i_property][0]);
                          }
                        }
                    }

                }
            }
        }

    }

    Objects::PlaneDistances
    SubductingPlateContours::distance_to_feature_plane(const Point<3> &position_in_cartesian_coordinates,
                                               const Objects::NaturalCoordinate &position_in_natural_coordinates,
                                               const double depth) const
    {
      // The depth variable is the distance from the surface to the position, the depth
      // coordinate is the distance from the bottom of the model to the position and
      // the starting radius is the distance from the bottom of the model to the surface.
      const double starting_radius = position_in_natural_coordinates.get_depth_coordinate() + depth - starting_depth;

      WBAssert(std::abs(starting_radius) > std::numeric_limits<double>::epsilon(), "World Builder error: starting_radius can not be zero. "
               << "Position = " << position_in_cartesian_coordinates[0] << ':' << position_in_cartesian_coordinates[1] << ':' << position_in_cartesian_coordinates[2]
               << ", natural_coordinate.get_depth_coordinate() = " << position_in_natural_coordinates.get_depth_coordinate()
               << ", depth = " << depth
               << ", starting_depth " << starting_depth
              );

      const WorldBuilder::Utilities::PointDistanceFromCurvedPlanes distance_from_planes =
        WorldBuilder::Utilities::distance_point_from_curved_planes(position_in_cartesian_coordinates,
                                                                   position_in_natural_coordinates,
                                                                   reference_point,
                                                                   coordinates,
                                                                   slab_segment_lengths,
                                                                   slab_segment_angles,
                                                                   starting_radius,
                                                                   this->world->parameters.coordinate_system,
                                                                   false,
                                                                   this->bezier_curve);

      Objects::PlaneDistances plane_distances(distance_from_planes.distance_from_plane, distance_from_planes.distance_along_plane);
      return plane_distances;
    }

    /**
     * Register plugin
     */
    WB_REGISTER_FEATURE(SubductingPlateContours, subducting plate contours)
  } // namespace Features
} // namespace WorldBuilder

