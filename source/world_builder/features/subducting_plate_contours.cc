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
The general form of the contours geometry type will probably look like this.

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

      regularized_points.resize(depths_of_contours.size());
      slab_segment_lengths.resize(depths_of_contours.size());
      slab_segment_thickness.resize(depths_of_contours.size());
      slab_segment_top_truncation.resize(depths_of_contours.size());
      slab_segment_angles.resize(depths_of_contours.size());

      // In reality, the variable below should be calculated like this:
      // std::vector<double> bezier_sub_lengths = this->bezier_curve.get_bezier_segment_lengths();
      // This is a VECTOR equal to N-1, where N is the number of coordinates which defines the feature.
      // This is because for N points, you need N-1 bezier curves, and each bezier curve can be a different
      // length.
      const double bezier_sublengths = 2500e3;

      // In reality, the variable below should be calculated like this:
      // const double total_arclength = this->bezier_curve.get_bezier_arclength();
      const double total_arclength = 5000e3;


      std::vector<Point<2>> coordinates_1 = {Point<2>(1500e3, 0.0, cartesian), Point<2>(1500e3, 2500e3, cartesian), Point<2>(1500e3, 5000e3, cartesian)};
      // std::vector<Point<2>> coordinates_2 = {Point<2>(500e3, 0.0, cartesian), Point<2>(500e3, 2500e3, cartesian), Point<2>(500e3, 5000e3, cartesian)};
      std::vector<Point<2>> coordinates_2 = {Point<2>(1200e3, 0.0, cartesian), Point<2>(1200e3, 2500e3, cartesian), Point<2>(1200e3, 5000e3, cartesian)};
      std::vector<Point<2>> coordinates_3 = {Point<2>(1200e3, 0.0, cartesian), Point<2>(1200e3, 2500e3, cartesian), Point<2>(1200e3, 5000e3, cartesian)};
      // std::vector<Point<2>> coordinates_3 = {Point<2>(1000e3, 0.0, cartesian), Point<2>(1000e3, 2500e3, cartesian), Point<2>(1000e3, 5000e3, cartesian)};
      Objects::BezierCurve bezier_curve;
      for (unsigned int i = 0; i < depths_of_contours.size(); ++i)
        {
          // Somehow here I need to specify which coordinates I am using. This should be possible by using the
          // index i which corresponds to the index of the depth value within the array of depth points.
          if (i == 0)
            {
              // The first bezier curve
              bezier_curve = Objects::BezierCurve(coordinates_1);
            }

          if (i == 1)
            {
              // The second bezier curve
              bezier_curve = Objects::BezierCurve(coordinates_2);
            }

          if (i == 2)
            {
              // The second bezier curve
              bezier_curve = Objects::BezierCurve(coordinates_3);
            }
          // The sum of all the previous bezier curves WITHIN the current contour
          double distance_along_contour = 0;
          double distance_along_current_bezier_curve = 0;

          // The index of the bezier curve. Currently this doesnt do too much, but this will be used
          // to index the length of the bezier curve as well.
          unsigned int which_bezier_curve = 0;

          for (unsigned int current_interval = 0; current_interval < number_of_intervals; ++current_interval)
            {
              // The increment in distance along the current bezier curve
              const double distance_increment = total_arclength / static_cast<double>(number_of_intervals);
              distance_along_current_bezier_curve += distance_increment;

              if (distance_along_current_bezier_curve <= bezier_sublengths)
                {
                  const double t_value = distance_along_current_bezier_curve / bezier_sublengths;
                  regularized_points[i].emplace_back(bezier_curve(which_bezier_curve, t_value));
                  distance_along_contour += distance_increment;
                }

              else
                {
                  ++which_bezier_curve;
                  const double remaining_distance = distance_along_current_bezier_curve -
                                                    bezier_sublengths;

                  const double t_value = remaining_distance / bezier_sublengths;
                  regularized_points[i].emplace_back(bezier_curve(which_bezier_curve, t_value));
                  distance_along_contour += distance_increment;
                  distance_along_current_bezier_curve = remaining_distance;
                }
            }
        }

      for (unsigned int i = 0; i < depths_of_contours.size(); ++i)
        {
          slab_segment_lengths[i].resize(number_of_intervals);
          slab_segment_thickness[i].resize(number_of_intervals);
          slab_segment_top_truncation[i].resize(number_of_intervals);
          slab_segment_angles[i].resize(number_of_intervals, Point<2>(invalid));
          for (unsigned int t = 0; t < number_of_intervals; ++t)
            {
              std::array<double, 3> P1 = {regularized_points[i][t][0], regularized_points[i][t][1], depths_of_contours[i]};
              std::array<double, 3> P2;
              if (i == depths_of_contours.size() - 1)
                {
                  P2 = {regularized_points[i][t][0], regularized_points[i][t][1], depths_of_contours[i]};
                }
              else
                {
                  P2 = {regularized_points[i + 1][t][0], regularized_points[i + 1][t][1], depths_of_contours[i + 1]};
                }
              // std::array<double, 3> P2 = {regularized_points[i + 1][t][0], regularized_points[i + 1][t][1], depths_of_contours[i + 1]};
              std::array<double, 3> P1P2 = {P2[0] - P1[0], P2[1] - P1[1], P2[2] - P1[2]};

              slab_segment_lengths[i][t] = std::sqrt(P1P2[0] * P1P2[0] + P1P2[1] * P1P2[1] + P1P2[2] * P1P2[2]);
              slab_segment_thickness[i][t] = slab_thickness;

              slab_segment_angles[i][t][0] = std::sin(P1P2[2] / slab_segment_lengths[i][t]);

              if (i == depths_of_contours.size() - 1)
                // Angle with the Z-axis
                slab_segment_angles[i][t][0] = slab_segment_angles[0][t][0];

              slab_segment_top_truncation[i][t] = top_truncation;
            }
        }


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
      // std::vector<double> depths_of_contours = {0, 100e3};
      std::vector<double> absolute_distances_from_planes_to_check_point;
      std::vector<double> signed_distances_from_planes_to_check_point;
      for (unsigned int i = 0; i < depths_of_contours.size() - 1; ++i)
        {
          std::vector<Point<2>> contour_i = regularized_points[i];
          std::vector<Point<2>> contour_i_plus_1 = regularized_points[i + 1];
          for (unsigned int j = 0; j < number_of_intervals - 1; ++j)
            {
              const Point<3> p1 = Point<3>(contour_i[j][0], contour_i[j][1], 500e3 - depths_of_contours[i], cartesian);
              const Point<3> p2 = Point<3>(contour_i[j + 1][0], contour_i[j + 1][1], 500e3 - depths_of_contours[i], cartesian);
              const Point<3> p3 = Point<3>(contour_i_plus_1[j][0], contour_i_plus_1[j][1], 500e3 - depths_of_contours[i + 1], cartesian);
              const Point<3> p4 = Point<3>(contour_i_plus_1[j + 1][0], contour_i_plus_1[j + 1][1], 500e3 - depths_of_contours[i + 1], cartesian);
              const double distance_1 = WorldBuilder::Utilities::calculate_distance_from_point_to_rectilinear_plane(p1, p2, p3, position_in_cartesian_coordinates);
              const double distance_2 = WorldBuilder::Utilities::calculate_distance_from_point_to_rectilinear_plane(p4, p3, p2, position_in_cartesian_coordinates);
              absolute_distances_from_planes_to_check_point.emplace_back(std::abs(distance_1));
              absolute_distances_from_planes_to_check_point.emplace_back(std::abs(distance_2));
              signed_distances_from_planes_to_check_point.emplace_back(distance_1);
              signed_distances_from_planes_to_check_point.emplace_back(distance_2);
            }
        }

      // Find the iterator to the minimum element
      auto minIt = std::min_element(absolute_distances_from_planes_to_check_point.begin(), absolute_distances_from_planes_to_check_point.end());

      // Calculate the index
      const unsigned int minIndex = std::distance(absolute_distances_from_planes_to_check_point.begin(), minIt);
      const double min_distance_to_plane = signed_distances_from_planes_to_check_point[minIndex];

      if (min_distance_to_plane >= -slab_segment_thickness[0][0] &&
          min_distance_to_plane <= 0 &&
          depth <= depths_of_contours[depths_of_contours.size() - 1])
        {
          output[entry_in_output[0]] = 10.0;
        }
    }
    /**
     * Register plugin
     */
    WB_REGISTER_FEATURE(SubductingPlateContours, subducting plate contours)
  } // namespace Features
} // namespace WorldBuilder

