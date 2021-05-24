/*
  Copyright (C) 2018 - 2020 by the authors of the World Builder code.

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

#include <iostream>
#include <iomanip>
#include <algorithm>

#include <world_builder/assert.h>
#include <world_builder/coordinate_systems/interface.h>
#include <world_builder/nan.h>
#include <world_builder/utilities.h>


namespace WorldBuilder
{
  namespace Utilities
  {
    bool
    polygon_contains_point(const std::vector<Point<2> > &point_list,
                           const Point<2> &point)
    {
      if (point.get_coordinate_system() == CoordinateSystem::spherical)
        {
          Point<2> other_point = point;
          other_point[0] += point[0] < 0 ? 2.0 * const_pi : -2.0 * const_pi;

          return (polygon_contains_point_implementation(point_list, point) ||
                  polygon_contains_point_implementation(point_list, other_point));
        }
      else
        {
          return polygon_contains_point_implementation(point_list, point);
        }
    }

    bool
    polygon_contains_point_implementation(const std::vector<Point<2> > &point_list,
                                          const Point<2> &point)
    {
      /**
       * This code has been based on http://geomalgorithms.com/a03-_inclusion.html,
       * and therefore requires the following copyright notice:
       *
       * Copyright 2000 softSurfer, 2012 Dan Sunday
       * This code may be freely used and modified for any purpose
       * providing that this copyright notice is included with it.
       * SoftSurfer makes no warranty for this code, and cannot be held
       * liable for any real or imagined damage resulting from its use.
       * Users of this code must verify correctness for their application.
       *
       * The main functional difference between the original code and this
       * code is that all the boundaries are considered to be inside the
       * polygon. One should of course realize that with floating point
       * arithmetic no guarantees can be made for the borders, but for
       * exact arithmetic this algorithm would work (also see polygon
       * in point test).
       */
      size_t pointNo = point_list.size();
      size_t    wn = 0;    // the  winding number counter
      size_t   j=pointNo-1;

      // loop through all edges of the polygon
      for (size_t i=0; i<pointNo; i++)
        {
          // edge from V[i] to  V[i+1]
          if (point_list[j][1] <= point[1])
            {
              // start y <= P.y
              if (point_list[i][1] >= point[1])      // an upward crossing
                {
                  const double is_left = (point_list[i][0] - point_list[j][0]) * (point[1] - point_list[j][1])
                                         - (point[0] -  point_list[j][0]) * (point_list[i][1] - point_list[j][1]);

                  if ( is_left > 0 && point_list[i][1] > point[1])
                    {
                      // P left of  edge
                      ++wn;            // have  a valid up intersect
                    }
                  else if ( std::abs(is_left) < std::numeric_limits<double>::epsilon())
                    {
                      // The point is exactly on the infinite line.
                      // determine if it is on the segment
                      const double dot_product = (point - point_list[j])*(point_list[i] - point_list[j]);

                      if (dot_product >= 0)
                        {
                          const double squaredlength = (point_list[i] - point_list[j]).norm_square();

                          if (dot_product <= squaredlength)
                            {
                              return true;
                            }
                        }
                    }
                }
            }
          else
            {
              // start y > P.y (no test needed)
              if (point_list[i][1]  <= point[1])     // a downward crossing
                {
                  const double is_left = (point_list[i][0] - point_list[j][0]) * (point[1] - point_list[j][1])
                                         - (point[0] -  point_list[j][0]) * (point_list[i][1] - point_list[j][1]);

                  if ( is_left < 0)
                    {
                      // P right of  edge
                      --wn;            // have  a valid down intersect
                    }
                  else if (std::abs(is_left) < std::numeric_limits<double>::epsilon())
                    {
                      // This code is to make sure that the boundaries are included in the polygon.
                      // The point is exactly on the infinite line.
                      // determine if it is on the segment
                      const double dot_product = (point - point_list[j])*(point_list[i] - point_list[j]);

                      if (dot_product >= 0)
                        {
                          const double squaredlength = (point_list[i] - point_list[j]).norm_square();

                          if (dot_product <= squaredlength)
                            {
                              return true;
                            }
                        }
                    }
                }
            }
          j=i;
        }

      return (wn != 0);
    }

    double
    signed_distance_to_polygon(const std::vector<Point<2> > &point_list,
                               const Point<2> &point)
    {
      // If the point lies outside polygon, we give it a negative sign,
      // inside a positive sign.
      const double sign = polygon_contains_point(point_list, point) ? 1.0 : -1.0;

      /**
       * This code is based on http://geomalgorithms.com/a02-_lines.html#Distance-to-Infinite-Line,
       * and therefore requires the following copyright notice:
       *
       * Copyright 2000 softSurfer, 2012 Dan Sunday
       * This code may be freely used and modified for any purpose
       * providing that this copyright notice is included with it.
       * SoftSurfer makes no warranty for this code, and cannot be held
       * liable for any real or imagined damage resulting from its use.
       * Users of this code must verify correctness for their application.
       *
       */

      const size_t n_poly_points = point_list.size();
      WBAssertThrow(n_poly_points >= 3, "Not enough polygon points were specified.");

      // Initialize a vector of distances for each point of the polygon with a very large distance
      std::vector<double> distances(n_poly_points, 1e23);

      // Create another polygon but with all points shifted 1 position to the right
      std::vector<Point<2> > shifted_point_list(n_poly_points, Point<2>(point.get_coordinate_system()));
      shifted_point_list[0] = point_list[n_poly_points-1];

      for (size_t i = 0; i < n_poly_points-1; ++i)
        shifted_point_list[i+1] = point_list[i];

      for (size_t i = 0; i < n_poly_points; ++i)
        {
          // Create vector along the polygon line segment
          Point<2> vector_segment = shifted_point_list[i] - point_list[i];
          // Create vector from point to the second segment point
          Point<2> vector_point_segment = point - point_list[i];

          // Compute dot products to get angles
          const double c1 = vector_point_segment * vector_segment;
          const double c2 = vector_segment * vector_segment;

          // point lies closer to not-shifted polygon point, but perpendicular base line lies outside segment
          if (c1 <= 0.0)
            distances[i] = (Point<2> (point_list[i] - point)).norm();
          // point lies closer to shifted polygon point, but perpendicular base line lies outside segment
          else if (c2 <= c1)
            distances[i] = (Point<2> (shifted_point_list[i] - point)).norm();
          // perpendicular base line lies on segment
          else
            {
              const Point<2> point_on_segment = point_list[i] + (c1/c2) * vector_segment;
              distances[i] = (Point<2> (point - point_on_segment)).norm();
            }
        }

      // Return the minimum of the distances of the point to all polygon segments
      return *std::min_element(distances.begin(),distances.end()) * sign;
    }


    NaturalCoordinate::NaturalCoordinate(const std::array<double,3> &position,
                                         const CoordinateSystems::Interface &coordinate_system_)
    {
      coordinate_system = coordinate_system_.natural_coordinate_system();
      coordinates = coordinate_system_.cartesian_to_natural_coordinates(position);
    }

    // todo, should be possible to make this without the interface, since the Point knows the coord system.
    NaturalCoordinate::NaturalCoordinate(const Point<3> &position,
                                         const CoordinateSystems::Interface &coordinate_system_)
    {
      coordinate_system = coordinate_system_.natural_coordinate_system();
      coordinates = coordinate_system_.cartesian_to_natural_coordinates(position.get_array());
    }

    const std::array<double,3> &NaturalCoordinate::get_coordinates()
    {
      return coordinates;
    }


    /*std::array<double,1> NaturalCoordinate::get_surface_coordinates() const
    {
      std::array<double,1> coordinate;

      switch (coordinate_system)
        {
          case Coordinates::CoordinateSystem::cartesian:
            coordinate[0] = coordinates[0];
            break;

          case Coordinates::CoordinateSystem::spherical:
            coordinate[0] = coordinates[1];
            break;

          default:
            coordinate[0] = 0;
            WBAssert (false, ExcNotImplemented());
            break;
        }

      return coordinate;
    }*/


    const std::array<double,2> NaturalCoordinate::get_surface_coordinates() const
    {
      std::array<double,2> coordinate;

      switch (coordinate_system)
        {
          case CoordinateSystem::cartesian:
            coordinate[0] = coordinates[0];
            coordinate[1] = coordinates[1];
            break;

          case CoordinateSystem::spherical:
            coordinate[0] = coordinates[1];
            coordinate[1] = coordinates[2];
            break;

          default:
            WBAssert (false, "Coordinate system not implemented.");
        }

      return coordinate;
    }


    CoordinateSystem
    NaturalCoordinate::get_coordinate_system() const
    {
      return coordinate_system;
    }


    double NaturalCoordinate::get_depth_coordinate() const
    {
      switch (coordinate_system)
        {
          case CoordinateSystem::cartesian:
            return coordinates[2];

          case CoordinateSystem::spherical:
            return coordinates[0];

          default:
            WBAssert (false, "Coordinate system not implemented.");
        }

      return 0;
    }


    std::array<double,3>
    cartesian_to_spherical_coordinates(const Point<3> &position)
    {
      std::array<double,3> scoord;

      scoord[0] = position.norm(); // R
      scoord[1] = std::atan2(position[1],position[0]); // Phi
      //if (scoord[1] < 0.0)
      //scoord[1] += 2.0*const_pi; // correct phi to [0,2*pi]

      if (scoord[0] > std::numeric_limits<double>::min())
        scoord[2] = 0.5 * const_pi - std::acos(position[2]/scoord[0]);
      else
        scoord[2] = 0.0;

      return scoord;
    }

    Point<3>
    spherical_to_cartesian_coordinates(const std::array<double,3> &scoord)
    {
      Point<3> ccoord(cartesian);

      ccoord[0] = scoord[0] * std::sin(0.5 * const_pi - scoord[2]) * std::cos(scoord[1]); // X
      ccoord[1] = scoord[0] * std::sin(0.5 * const_pi - scoord[2]) * std::sin(scoord[1]); // Y
      ccoord[2] = scoord[0] * std::cos(0.5 * const_pi - scoord[2]); // Z


      return ccoord;
    }



    CoordinateSystem
    string_to_coordinate_system(const std::string &coordinate_system)
    {
      if (coordinate_system == "cartesian")
        return CoordinateSystem::cartesian;
      else if (coordinate_system == "spherical")
        return CoordinateSystem::spherical;
      else
        WBAssertThrow(false, "Coordinate system not implemented.");

      return invalid;
    }


    template<int dim>
    const std::array<double,dim>
    convert_point_to_array(const Point<dim> &point_)
    {
      std::array<double,dim> array;
      for (size_t i = 0; i < dim; ++i)
        array[i] = point_[i];
      return array;
    }

    double
    string_to_double(const std::string &string)
    {
      // trim whitespace on either side of the text if necessary
      std::string s = string;
      while ((s.size() > 0) && (s[0] == ' '))
        s.erase(s.begin());
      while ((s.size() > 0) && (s[s.size() - 1] == ' '))
        s.erase(s.end() - 1);

      std::istringstream i(s);
      double d;
      char c;
      if (!(i >> d) || i.get(c))
        WBAssertThrow(false, "Could not convert \"" + s + "\" to a double.");

      return d;
    }

    int
    string_to_int(const std::string &string)
    {
      // trim whitespace on either side of the text if necessary
      std::string s = string;
      while ((s.size() > 0) && (s[0] == ' '))
        s.erase(s.begin());
      while ((s.size() > 0) && (s[s.size() - 1] == ' '))
        s.erase(s.end() - 1);

      std::istringstream i(s);
      int d;
      char c;
      if (!(i >> d) || i.get(c))
        WBAssertThrow(false, "Could not convert \"" + s + "\" to an int.");

      return d;
    }


    unsigned int
    string_to_unsigned_int(const std::string &string)
    {
      // trim whitespace on either side of the text if necessary
      std::string s = string;
      while ((s.size() > 0) && (s[0] == ' '))
        s.erase(s.begin());
      while ((s.size() > 0) && (s[s.size() - 1] == ' '))
        s.erase(s.end() - 1);


      std::istringstream i(s);
      unsigned int d;
      char c;
      if (!(i >> d) || i.get(c))
        WBAssertThrow(false, "Could not convert \"" + s + "\" to an unsigned int.");

      return d;
    }


    Point<3>
    cross_product(const Point<3> &a, const Point<3> &b)
    {
      WBAssert(a.get_coordinate_system() == b.get_coordinate_system(), "Trying to do a cross product of points of a different coordinate system.");
      const double x = a[1] * b[2] - b[1] * a[2];
      const double y = a[2] * b[0] - b[2] * a[0];
      const double z = a[0] * b[1] - b[0] * a[1];
      return Point<3>(x,y,z,a.get_coordinate_system());
    }

    std::map<std::string,double>
    distance_point_from_curved_planes(const Point<3> &check_point, // cartesian point in spherical system
                                      const Point<2> &reference_point, // in (rad) spherical coordinates in spherical system
                                      const std::vector<Point<2> > &point_list, // in  (rad) spherical coordinates in spherical system
                                      const std::vector<std::vector<double> > &plane_segment_lengths,
                                      const std::vector<std::vector<Point<2> > > &plane_segment_angles,
                                      const double start_radius,
                                      const std::unique_ptr<CoordinateSystems::Interface> &coordinate_system,
                                      const bool only_positive,
                                      const InterpolationType interpolation_type,
                                      const interpolation x_spline,
                                      const interpolation y_spline,
                                      std::vector<double> global_x_list)
    {
      // TODO: Assert that point_list, plane_segment_angles and plane_segment_lenghts have the same size.
      /*WBAssert(point_list.size() == plane_segment_lengths.size(),
               "Internal error: The size of point_list (" << point_list.size()
               << ") and plane_segment_lengths (" << plane_segment_lengths.size() << ") are different.");
      WBAssert(point_list.size() == plane_segment_angles.size(),
               "Internal error: The size of point_list (" << point_list.size()
               << ") and plane_segment_angles (" << plane_segment_angles.size() << ") are different.");
      WBAssert(point_list.size() == plane_segment_angles.size(),
               "Internal error: The size of point_list (" << point_list.size()
               << ") and global_x_list (" << global_x_list.size() << ") are different.");*/

      if (global_x_list.size() == 0)
        {
          // fill it
          global_x_list.resize(point_list.size());
          for (size_t i = 0; i < point_list.size(); ++i)
            global_x_list[i] = i;
        }
      WBAssertThrow(global_x_list.size() == point_list.size(), "The given global_x_list doesn't have "
                    "the same size as the point list. This is required.");

      double distance = INFINITY;
      double new_distance = INFINITY;
      double along_plane_distance = INFINITY;
      double new_along_plane_distance  = INFINITY;

      const CoordinateSystem natural_coordinate_system = coordinate_system->natural_coordinate_system();
      const bool bool_cartesian = natural_coordinate_system == cartesian;

      const Point<3> check_point_natural(coordinate_system->cartesian_to_natural_coordinates(check_point.get_array()),natural_coordinate_system);
      const Point<3> check_point_surface(bool_cartesian ? check_point_natural[0] : start_radius,
                                         check_point_natural[1],
                                         bool_cartesian ? start_radius           : check_point_natural[2],
                                         natural_coordinate_system);
      const Point<2> check_point_surface_2d(bool_cartesian ? check_point_natural[0] : check_point_natural[1],
                                            bool_cartesian ? check_point_natural[1] : check_point_natural[2],
                                            natural_coordinate_system);

      // The section which is checked.
      size_t section = 0;

      // The 'horizontal' fraction between the points at the surface.
      double section_fraction = 0.0;

      // What segment the point on the line is in.
      size_t segment = 0;

      // The 'vertical' fraction, indicates how far in the current segment the
      // point on the line is.
      double segment_fraction = 0.0;
      double total_average_angle = 0.0;

      const DepthMethod depth_method = coordinate_system->depth_method();
      WBAssertThrow(depth_method == DepthMethod::none
                    || depth_method == DepthMethod::angle_at_starting_point_with_surface
                    || depth_method == DepthMethod::angle_at_begin_segment_with_surface,
                    "Only the depth methods none, angle_at_starting_point_with_surface and "
                    "angle_at_begin_segment_with_surface are implemented");

      double min_distance_check_point_surface_2d_line = INFINITY;
      size_t i_section_min_distance = 0;
      Point<2> closest_point_on_line_2d(0,0,natural_coordinate_system);
      Point<2> closest_point_on_line_2d_temp(0,0,natural_coordinate_system);
      double fraction_CPL_P1P2_strict =  INFINITY; // or NAN?

      if (interpolation_type != InterpolationType::ContinuousMonotoneSpline)
        {
          // loop over all the planes to find out which one is closest to the point.

          for (size_t i_section=0; i_section < point_list.size()-1; ++i_section)
            {
              const size_t current_section = i_section;
              const size_t next_section = i_section+1;

              const Point<2> P1(point_list[current_section]);
              const Point<2> P2(point_list[next_section]);

              const Point<2> P1P2 = P2 - P1;
              const Point<2> P1PC = check_point_surface_2d - P1;

              // Compute the closest point on the line P1 to P2 from the check
              // point at the surface. We do this in natural coordinates on
              // purpose, because in spherical coordinates it is more accurate.
              closest_point_on_line_2d_temp = P1 + ((P1PC * P1P2) / (P1P2 * P1P2)) * P1P2;

              // compute what fraction of the distance between P1 and P2 the
              // closest point lies.
              Point<2> P1CPL = closest_point_on_line_2d_temp - P1;

              // This determines where the check point is between the coordinates
              // in the coordinate list.
              double fraction_CPL_P1P2_strict_temp = (P1CPL * P1P2 <= 0 ? -1.0 : 1.0)
                                                     * (1 - (P1P2.norm() - P1CPL.norm()) / P1P2.norm());

              Point<2> CPLCPS2 = closest_point_on_line_2d_temp - check_point_surface_2d;

              // If fraction_CPL_P1P2_strict_temp is between 0 and 1 it means that the point can be projected perpendicual to the line segment. For the non-contiuous case we only conder points which are
              // perpendicular to a line segment.
              // There can be mutliple lines segment to which a point is perpundicual. Choose the point which is closed in 2D (x-y).


              if (fraction_CPL_P1P2_strict_temp >= 0. && fraction_CPL_P1P2_strict_temp <= 1. && CPLCPS2.norm() < min_distance_check_point_surface_2d_line)
                {
                  min_distance_check_point_surface_2d_line = CPLCPS2.norm();
                  i_section_min_distance = i_section;
                  closest_point_on_line_2d = closest_point_on_line_2d_temp;
                  fraction_CPL_P1P2_strict = fraction_CPL_P1P2_strict_temp;
                }
            }


          // If the point on the line does not lay between point P1 and P2
          // then ignore it. Otherwise continue.
          if (fabs(fraction_CPL_P1P2_strict) < INFINITY && fraction_CPL_P1P2_strict >= 0. && fraction_CPL_P1P2_strict <= 1.)
            {
              // now compute the relevant x and y axis
              double fraction_CPL_P1P2 = std::numeric_limits<double>::signaling_NaN();
              Point<3> x_axis(std::numeric_limits<double>::signaling_NaN(),std::numeric_limits<double>::signaling_NaN(),std::numeric_limits<double>::signaling_NaN(),cartesian);
              Point<3> y_axis(std::numeric_limits<double>::signaling_NaN(),std::numeric_limits<double>::signaling_NaN(),std::numeric_limits<double>::signaling_NaN(),cartesian);
              Point<3> closest_point_on_line_cartesian(std::numeric_limits<double>::signaling_NaN(),std::numeric_limits<double>::signaling_NaN(),std::numeric_limits<double>::signaling_NaN(),cartesian);
              Point<3> closest_point_on_line_bottom_cartesian(std::numeric_limits<double>::signaling_NaN(),std::numeric_limits<double>::signaling_NaN(),std::numeric_limits<double>::signaling_NaN(),cartesian);
              size_t current_section = i_section_min_distance;
              size_t next_section = i_section_min_distance+1;
              // translate to orignal coordinates current and next section
              size_t original_current_section = static_cast<size_t>(std::floor(global_x_list[i_section_min_distance]));
              size_t original_next_section = original_current_section + 1;

              // now figure out where the point is in relation with the user
              // get P1 and P2 back

              const Point<2> P1(point_list[current_section]);

              const Point<2> P2(point_list[next_section]);

              const Point<2> P1P2 = P2 - P1;

              // defined coordinates
              fraction_CPL_P1P2 = global_x_list[i_section_min_distance] - static_cast<int>(global_x_list[i_section_min_distance])
                                  + (global_x_list[i_section_min_distance+1]-global_x_list[i_section_min_distance]) * fraction_CPL_P1P2_strict;


              const Point<2> unit_normal_to_plane_spherical = P1P2 / P1P2.norm();
              const Point<2> closest_point_on_line_plus_normal_to_plane_spherical = closest_point_on_line_2d + 1e-8 * (closest_point_on_line_2d.norm() > 1.0 ? closest_point_on_line_2d.norm() : 1.0) * unit_normal_to_plane_spherical;

              WBAssert(std::fabs(closest_point_on_line_plus_normal_to_plane_spherical.norm()) > std::numeric_limits<double>::epsilon(),
                       "Internal error: The norm of variable 'closest_point_on_line_plus_normal_to_plane_spherical' "
                       "is  zero, while this may not happen.");

              // We now need 3d points from this point on, so make them.
              // The order of a Cartesian coordinate is x,y,z and the order of
              // a spherical coordinate it radius, long, lat (in rad).
              const Point<3> closest_point_on_line_surface(bool_cartesian ? closest_point_on_line_2d[0] : start_radius,
                                                           bool_cartesian ? closest_point_on_line_2d[1] : closest_point_on_line_2d[0],
                                                           bool_cartesian ? start_radius : closest_point_on_line_2d[1],
                                                           natural_coordinate_system);

              Point<3> closest_point_on_line_bottom = closest_point_on_line_surface;
              closest_point_on_line_bottom[bool_cartesian ? 2 : 0] = 0;

              WBAssert(!std::isnan(closest_point_on_line_bottom[0]),
                       "Internal error: The y_axis variable is not a number: " << closest_point_on_line_bottom[0]);
              WBAssert(!std::isnan(closest_point_on_line_bottom[1]),
                       "Internal error: The y_axis variable is not a number: " << closest_point_on_line_bottom[1]);
              WBAssert(!std::isnan(closest_point_on_line_bottom[2]),
                       "Internal error: The y_axis variable is not a number: " << closest_point_on_line_bottom[2]);

              const Point<3> closest_point_on_line_plus_normal_to_plane_surface_spherical(bool_cartesian ? closest_point_on_line_plus_normal_to_plane_spherical[0] : start_radius,
                                                                                          bool_cartesian ? closest_point_on_line_plus_normal_to_plane_spherical[1] : closest_point_on_line_plus_normal_to_plane_spherical[0],
                                                                                          bool_cartesian ? start_radius : closest_point_on_line_plus_normal_to_plane_spherical[1],
                                                                                          natural_coordinate_system);

              // Now that we have both the check point and the
              // closest_point_on_line, we need to push them to cartesian.
              const Point<3> check_point_surface_cartesian(coordinate_system->natural_to_cartesian_coordinates(check_point_surface.get_array()),cartesian);
              closest_point_on_line_cartesian = Point<3>(coordinate_system->natural_to_cartesian_coordinates(closest_point_on_line_surface.get_array()),cartesian);
              closest_point_on_line_bottom_cartesian = Point<3>(coordinate_system->natural_to_cartesian_coordinates(closest_point_on_line_bottom.get_array()),cartesian);
              const Point<3> closest_point_on_line_plus_normal_to_plane_cartesian(coordinate_system->natural_to_cartesian_coordinates(closest_point_on_line_plus_normal_to_plane_surface_spherical.get_array()),cartesian);


              WBAssert(!std::isnan(closest_point_on_line_bottom_cartesian[0]),
                       "Internal error: The y_axis variable is not a number: " << closest_point_on_line_bottom_cartesian[0]);
              WBAssert(!std::isnan(closest_point_on_line_bottom_cartesian[1]),
                       "Internal error: The y_axis variable is not a number: " << closest_point_on_line_bottom_cartesian[1]);
              WBAssert(!std::isnan(closest_point_on_line_bottom_cartesian[2]),
                       "Internal error: The y_axis variable is not a number: " << closest_point_on_line_bottom_cartesian[2]);

              // If the point to check is on the line, we don't need to search any further, because we know the distance is zero.
              if (std::fabs((check_point - closest_point_on_line_cartesian).norm()) > 2e-14)
                {

                  Point<3> normal_to_plane = closest_point_on_line_plus_normal_to_plane_cartesian - closest_point_on_line_cartesian;
                  normal_to_plane = normal_to_plane / normal_to_plane.norm();

                  // The y-axis is from the bottom/center to the closest_point_on_line,
                  // the x-axis is 90 degrees rotated from that, so we rotate around
                  // the line P1P2.
                  // Todo: Assert that the norm of the axis are not equal to zero.
                  y_axis = closest_point_on_line_cartesian - closest_point_on_line_bottom_cartesian;

                  WBAssert(std::abs(y_axis.norm()) > std::numeric_limits<double>::epsilon(),
                           "World Builder error: Cannot detemine the up direction in the model. This is most likely due to the provided start radius being zero."
                           << " Techical details: The y_axis.norm() is zero. Y_axis is " << y_axis[0] << ":" << y_axis[1] << ":" << y_axis[2]
                           << ". closest_point_on_line_cartesian = " << closest_point_on_line_cartesian[0] << ":" << closest_point_on_line_cartesian[1] << ":" << closest_point_on_line_cartesian[2]
                           << ", closest_point_on_line_bottom_cartesian = " << closest_point_on_line_bottom_cartesian[0] << ":" << closest_point_on_line_bottom_cartesian[1] << ":" << closest_point_on_line_bottom_cartesian[2]);

                  WBAssert(!std::isnan(y_axis[0]),
                           "Internal error: The y_axis variable is not a number: " << y_axis[0]);
                  WBAssert(!std::isnan(y_axis[1]),
                           "Internal error: The y_axis variable is not a number: " << y_axis[1]);
                  WBAssert(!std::isnan(y_axis[2]),
                           "Internal error: The y_axis variable is not a number: " << y_axis[2]);


                  y_axis = y_axis / y_axis.norm();


                  WBAssert(!std::isnan(y_axis[0]),
                           "Internal error: The y_axis variable is not a number: " << y_axis[0]);
                  WBAssert(!std::isnan(y_axis[1]),
                           "Internal error: The y_axis variable is not a number: " << y_axis[1]);
                  WBAssert(!std::isnan(y_axis[2]),
                           "Internal error: The y_axis variable is not a number: " << y_axis[2]);


                  // shorthand notation for computing the x_axis
                  double vx = y_axis[0];
                  double vy = y_axis[1];
                  double vz = y_axis[2];
                  double ux = normal_to_plane[0];
                  double uy = normal_to_plane[1];
                  double uz = normal_to_plane[2];

                  x_axis = Point<3>(ux*ux*vx + ux*uy*vy - uz*vy + uy*uz*vz + uy*vz,
                                    uy*ux*vx + uz*vx + uy*uy*vy + uy*uz*vz - ux*vz,
                                    uz*ux*vx - uy*vx + uz*uy*vy + ux*vy + uz*uz*vz,
                                    cartesian);

                  // see on what side the line P1P2 reference point is. This is based on the determinant
                  const double reference_on_side_of_line = (point_list[next_section][0] - point_list[current_section][0])
                                                           * (reference_point[1] - point_list[current_section][1])
                                                           - (point_list[next_section][1] - point_list[current_section][1])
                                                           * (reference_point[0] - point_list[current_section][0])
                                                           < 0 ? 1 : -1;

                  WBAssert(!std::isnan(x_axis[0]),
                           "Internal error: The x_axis variable is not a number: " << x_axis[0]);
                  WBAssert(!std::isnan(x_axis[1]),
                           "Internal error: The x_axis variable is not a number: " << x_axis[1]);
                  WBAssert(!std::isnan(x_axis[2]),
                           "Internal error: The x_axis variable is not a number: " << x_axis[2]);

                  x_axis = x_axis *(reference_on_side_of_line / x_axis.norm());


                  WBAssert(!std::isnan(x_axis[0]),
                           "Internal error: The x_axis variable is not a number: " << x_axis[0]);
                  WBAssert(!std::isnan(x_axis[1]),
                           "Internal error: The x_axis variable is not a number: " << x_axis[1]);
                  WBAssert(!std::isnan(x_axis[2]),
                           "Internal error: The x_axis variable is not a number: " << x_axis[2]);
                }
              else
                {
                  total_average_angle = plane_segment_angles[original_current_section][0][0]
                                        + fraction_CPL_P1P2 * (plane_segment_angles[original_next_section][0][0]
                                                               - plane_segment_angles[original_current_section][0][0]);

                  std::map<std::string, double> return_values;
                  return_values["distanceFromPlane"] = 0.0;
                  return_values["distanceAlongPlane"] = 0.0;
                  return_values["sectionFraction"] = fraction_CPL_P1P2;
                  return_values["segmentFraction"] = 0.0;
                  return_values["section"] = static_cast<double>(current_section);
                  return_values["segment"] = 0;
                  return_values["averageAngle"] = total_average_angle;
                  return return_values;

                }


              WBAssert(!std::isnan(x_axis[0]),
                       "Internal error: The x_axis[0] variable is not a number: " << x_axis[0] << ". Relevant values:  check_point = " << check_point[0] << ":" <<  check_point[1] << ":" <<  check_point[2] << ".");
              WBAssert(!std::isnan(x_axis[1]),
                       "Internal error: The x_axis[1] variable is not a number: " << x_axis[1]);
              WBAssert(!std::isnan(x_axis[2]),
                       "Internal error: The x_axis[2] variable is not a number: " << x_axis[2]);


              // now that we have the x and y axes computed, convert the 3d check point into a 2d one.
              Point<2> check_point_2d(x_axis * (check_point - closest_point_on_line_bottom_cartesian),
                                      y_axis * (check_point - closest_point_on_line_bottom_cartesian),
                                      cartesian);



              Point<2> begin_segment(x_axis * (closest_point_on_line_cartesian - closest_point_on_line_bottom_cartesian),
                                     y_axis * (closest_point_on_line_cartesian - closest_point_on_line_bottom_cartesian),
                                     cartesian);


              WBAssert(!std::isnan(check_point_2d[0]),
                       "Internal error: The check_point_2d variable is not a number: " << check_point_2d[0]);
              WBAssert(!std::isnan(check_point_2d[1]),
                       "Internal error: The check_point_2d variable is not a number: " << check_point_2d[1]);


              WBAssert(!std::isnan(begin_segment[0]),
                       "Internal error: The begin_segment variable is not a number: " << begin_segment[0]);
              WBAssert(!std::isnan(begin_segment[1]),
                       "Internal error: The begin_segment variable is not a number: " << begin_segment[1]);

              Point<2> end_segment = begin_segment;


              double total_length = 0.0;
              double add_angle = 0.0;
              double average_angle = 0.0;
              for (size_t i_segment = 0; i_segment < plane_segment_lengths[original_current_section].size(); i_segment++)
                {
                  const size_t current_segment = i_segment;

                  // compute the angle between the the previous begin and end if
                  // the depth method is angle_at_begin_segment_with_surface.
                  if (i_segment != 0 && depth_method == DepthMethod::angle_at_begin_segment_with_surface)
                    {
                      const double add_angle_inner = (begin_segment * end_segment) / (begin_segment.norm() * end_segment.norm());

                      WBAssert(!std::isnan(add_angle_inner),
                               "Internal error: The add_angle_inner variable is not a number: " << add_angle_inner
                               << ". Variables: begin_segment = " << begin_segment[0] << ":" << begin_segment[1]
                               << ", end_segment = " << end_segment[0] << ":" << end_segment[1]
                               << ", begin_segment * end_segment / (begin_segment.norm() * end_segment.norm()) = "
                               << std::setprecision(32) << begin_segment * end_segment / (begin_segment.norm() * end_segment.norm())
                               << ".");

                      // there could be round of error problems here is the inner part is close to one
                      WBAssert(add_angle_inner >= 0 && add_angle_inner <= 1,
                               "Internal error: The variable add_angle_inner is smaller than zero or larger then one,"
                               "which causes the std::acos to return nan. If it is only a little bit larger then one, "
                               "this is probably caused by that begin and end segment are the same and round off error. "
                               "The value of add_angle_inner = " << add_angle_inner);

                      add_angle += std::acos(add_angle_inner);

                      WBAssert(!std::isnan(add_angle),
                               "Internal error: The add_angle variable is not a number: " << add_angle
                               << ". Variables: begin_segment = " << begin_segment[0] << ":" << begin_segment[1]
                               << ", end_segment = " << end_segment[0] << ":" << end_segment[1]
                               << ", begin_segment * end_segment / (begin_segment.norm() * end_segment.norm()) = "
                               << std::setprecision(32) << begin_segment * end_segment / (begin_segment.norm() * end_segment.norm())
                               << ", std::acos(begin_segment * end_segment / (begin_segment.norm() * end_segment.norm())) = "
                               << std::acos(begin_segment * end_segment / (begin_segment.norm() * end_segment.norm())));
                    }




                  begin_segment = end_segment;

                  WBAssert(!std::isnan(begin_segment[0]),
                           "Internal error: The begin_segment variable is not a number: " << begin_segment[0]);
                  WBAssert(!std::isnan(begin_segment[1]),
                           "Internal error: The begin_segment variable is not a number: " << begin_segment[1]);


                  // This interpolates different properties between P1 and P2 (the
                  // points of the plane at the surface)
                  const double degree_90_to_rad = 0.5 * const_pi;

                  WBAssert(plane_segment_angles.size() > original_next_section,
                           "Error: original_next_section = " << original_next_section
                           << ", and plane_segment_angles.size() = " << plane_segment_angles.size());


                  WBAssert(plane_segment_angles[original_next_section].size() > current_segment,
                           "Error: current_segment = "  << current_segment
                           << ", and current_segment.size() = " << plane_segment_angles[original_next_section].size());

                  const double interpolated_angle_top    = plane_segment_angles[original_current_section][current_segment][0]
                                                           + fraction_CPL_P1P2 * (plane_segment_angles[original_next_section][current_segment][0]
                                                                                  - plane_segment_angles[original_current_section][current_segment][0])
                                                           + add_angle;

                  const double interpolated_angle_bottom = plane_segment_angles[original_current_section][current_segment][1]
                                                           + fraction_CPL_P1P2 * (plane_segment_angles[original_next_section][current_segment][1]
                                                                                  - plane_segment_angles[original_current_section][current_segment][1])
                                                           + add_angle;


                  double interpolated_segment_length     = plane_segment_lengths[original_current_section][current_segment]
                                                           + fraction_CPL_P1P2 * (plane_segment_lengths[original_next_section][current_segment]
                                                                                  - plane_segment_lengths[original_current_section][current_segment]);
                  WBAssert(!std::isnan(interpolated_angle_top),
                           "Internal error: The interpolated_angle_top variable is not a number: " << interpolated_angle_top);

                  // We want to know where the end point of this segment is (and
                  // the start of the next segment). There are two cases which we
                  // will deal with separately. The first one is if the angle is
                  // constant. The second one is if the angle changes.
                  const double difference_in_angle_along_segment = interpolated_angle_top - interpolated_angle_bottom;

                  if (std::fabs(difference_in_angle_along_segment) < 1e-8)
                    {
                      // The angle is constant. It is easy find find the end of
                      // this segment and the distance.
                      if (std::fabs(interpolated_segment_length) > std::numeric_limits<double>::epsilon())
                        {
                          end_segment[0] += interpolated_segment_length * std::sin(degree_90_to_rad - interpolated_angle_top);
                          end_segment[1] -= interpolated_segment_length * std::cos(degree_90_to_rad - interpolated_angle_top);

                          Point<2> begin_end_segment = end_segment - begin_segment;
                          Point<2> normal_2d_plane(-begin_end_segment[0],begin_end_segment[1], cartesian);
                          WBAssert(std::fabs(normal_2d_plane.norm()) > std::numeric_limits<double>::epsilon(), "Internal Error: normal_2d_plane.norm() is zero, which should not happen. "
                                   << "Extra info: begin_end_segment[0] = " << begin_end_segment[0]
                                   << ", begin_end_segment[1] = " << begin_end_segment[1]
                                   << ", end_segment: [" << end_segment[0] << "," << end_segment[1] << "]"
                                   << ", begin_segment: [" << begin_segment[0] << "," << begin_segment[1] << "]"
                                  );
                          normal_2d_plane /= normal_2d_plane.norm();

                          // Now find the distance of a point to this line.
                          // Based on http://geomalgorithms.com/a02-_lines.html.
                          const Point<2> BSP_ESP = end_segment - begin_segment;
                          const Point<2> BSP_CP = check_point_2d - begin_segment;

                          const double c1 = BSP_ESP * BSP_CP;
                          const double c2 = BSP_ESP * BSP_ESP;

                          if (c1 < 0 || c2 < c1)
                            {
                              new_distance = INFINITY;
                              new_along_plane_distance = INFINITY;
                            }
                          else
                            {
                              const Point<2> Pb = begin_segment + (c1/c2) * BSP_ESP;
                              const double side_of_line =  (begin_segment[0] - end_segment[0]) * (check_point_2d[1] - begin_segment[1])
                                                           - (begin_segment[1] - end_segment[1]) * (check_point_2d[0] - begin_segment[0])
                                                           < 0 ? -1.0 : 1.0;

                              new_distance = side_of_line * (check_point_2d - Pb).norm();
                              new_along_plane_distance = (begin_segment - Pb).norm();
                            }

                        }
                    }
                  else
                    {
                      // The angle is not constant. This means that we need to
                      // define a circle. First find the center of the circle.
                      const double radius_angle_circle = std::fabs(interpolated_segment_length/difference_in_angle_along_segment);

                      WBAssert(!std::isnan(radius_angle_circle),
                               "Internal error: The radius_angle_circle variable is not a number: " << radius_angle_circle
                               << ". interpolated_segment_length = " << interpolated_segment_length
                               << ", difference_in_angle_along_segment = " << difference_in_angle_along_segment);

                      const double cos_angle_top = std::cos(interpolated_angle_top);

                      WBAssert(!std::isnan(cos_angle_top),
                               "Internal error: The radius_angle_circle variable is not a number: " << cos_angle_top
                               << ". interpolated_angle_top = " << interpolated_angle_top);

                      Point<2> center_circle(cartesian);
                      if (std::fabs(interpolated_angle_top - 0.5 * const_pi) < 1e-8)
                        {
                          // if interpolated_angle_top is 90 degrees, the tan function
                          // is undefined (1/0). What we really want in this case is
                          // set the center to the correct location which is x = the x
                          //begin point + radius and y = the y begin point.
                          center_circle[0] = difference_in_angle_along_segment > 0 ? begin_segment[0] + radius_angle_circle : begin_segment[0] - radius_angle_circle;
                          center_circle[1] = begin_segment[1];
                        }
                      else if (std::fabs(interpolated_angle_top - 1.5 * const_pi) < 1e-8)
                        {
                          // if interpolated_angle_top is 270 degrees, the tan function
                          // is undefined (-1/0). What we really want in this case is
                          // set the center to the correct location which is x = the x
                          //begin point - radius and y = the y begin point.
                          center_circle[0] = difference_in_angle_along_segment > 0 ? begin_segment[0] - radius_angle_circle : begin_segment[0] + radius_angle_circle;
                          center_circle[1] = begin_segment[1];
                        }
                      else
                        {
                          double tan_angle_top = std::tan(interpolated_angle_top);

                          WBAssert(!std::isnan(tan_angle_top),
                                   "Internal error: The tan_angle_top variable is not a number: " << tan_angle_top);
                          const double center_circle_y = difference_in_angle_along_segment < 0 ?
                                                         begin_segment[1] - radius_angle_circle * cos_angle_top
                                                         : begin_segment[1] + radius_angle_circle * cos_angle_top;

                          WBAssert(!std::isnan(center_circle_y),
                                   "Internal error: The center_circle_y variable is not a number: " << center_circle_y
                                   << ". begin_segment[1] = " << begin_segment[1]
                                   << ", radius_angle_circle = " << radius_angle_circle
                                   << ", cos_angle_top = " << cos_angle_top);

                          // to prevent round off errors becomming dominant, we check
                          // whether center_circle_y - begin_segment[1] should be zero.
                          // TODO: improve this to some kind of relative difference.
                          const double CCYBS = center_circle_y - begin_segment[1];

                          WBAssert(!std::isnan(CCYBS),
                                   "Internal error: The CCYBS variable is not a number: " << CCYBS);



                          center_circle[0] = begin_segment[0] + tan_angle_top * (CCYBS);
                          center_circle[1] = center_circle_y;
                        }

                      WBAssert(!std::isnan(center_circle[0]) || !std::isnan(center_circle[1]),
                               "Internal error: The center variable contains not a number: " << center_circle[0] << ":" << center_circle[0]);
                      WBAssert(std::fabs((begin_segment-center_circle).norm() - std::fabs(radius_angle_circle))
                               < 1e-8 * std::fabs((begin_segment-center_circle).norm() + std::fabs(radius_angle_circle)),
                               "Internal error: The center of the circle is not a radius away from the begin point. " << std::endl
                               << "The center is located at " << center_circle[0] << ":" << center_circle[1] << std::endl
                               << "The begin point is located at " << begin_segment[0] << ":" << begin_segment[1] << std::endl
                               << "The computed radius is " << std::fabs((begin_segment-center_circle).norm())
                               << ", and it should be " << radius_angle_circle << ".");


                      // Now compute the location of the end of the segment by
                      // rotating P1 around the center_circle
                      Point<2> BSPC = begin_segment - center_circle;
                      const double sin_angle_diff = sin(difference_in_angle_along_segment);
                      const double cos_angle_diff = cos(difference_in_angle_along_segment);
                      end_segment[0] = cos_angle_diff * BSPC[0] - sin_angle_diff * BSPC[1] + center_circle[0];
                      end_segment[1] = sin_angle_diff * BSPC[0] + cos_angle_diff * BSPC[1] + center_circle[1];



                      WBAssert(std::fabs((end_segment-center_circle).norm() - std::fabs(radius_angle_circle))
                               < 1e-8 * std::fabs((end_segment-center_circle).norm() + std::fabs(radius_angle_circle)) ,
                               "Internal error: The center of the circle is not a radius away from the end point. " << std::endl
                               << "The center is located at " << center_circle[0] << ":" << center_circle[1] << std::endl
                               << "The end point is located at " << end_segment[0] << ":" << end_segment[1] << std::endl
                               << "The computed radius is " << std::fabs((end_segment-center_circle).norm())
                               << ", and it should be " << radius_angle_circle << ".");

                      // Now check if the angle of the check point in this circle
                      // is larger then the angle of P1 and smaller then P1 + angle
                      // difference. If that is the case then the distance from the
                      // plane is radius - (center - check_point).norm(). Otherwise
                      // it is infinity.
                      // The angle of the check point is computed with the help of
                      // dot product. But before that we need to adjust the check
                      // point 2d.
                      const Point<2> CPCR = check_point_2d - center_circle;
                      const double CPCR_norm = CPCR.norm();

                      const double dot_product = CPCR * Point<2>(0, radius_angle_circle, cartesian);
                      // If the x of the check point is larger then the x of center
                      // the circle, the angle is more than 180 degree, but the dot
                      // product will decrease instead of increase from 180 degrees.
                      // To fix this we make a special case for this.
                      // Furthermore, when the check point is at the same location as
                      // the center of the circle, we count that point as belonging
                      // to the top of the top segment (0 degree).
                      double check_point_angle = std::fabs(CPCR_norm) < std::numeric_limits<double>::epsilon() ? 2.0 * const_pi : (check_point_2d[0] <= center_circle[0]
                                                 ? std::acos(dot_product/(CPCR_norm * radius_angle_circle))
                                                 : 2.0 * const_pi - std::acos(dot_product/(CPCR_norm * radius_angle_circle)));
                      check_point_angle = difference_in_angle_along_segment >= 0 ? const_pi - check_point_angle : 2.0 * const_pi - check_point_angle;

                      // In the case that it is exactly 2 * pi, bring it back to zero
                      check_point_angle = (std::fabs(check_point_angle - 2 * const_pi) < 1e-14 ? 0 : check_point_angle);

                      if ((difference_in_angle_along_segment > 0 && (check_point_angle <= interpolated_angle_top || std::fabs(check_point_angle - interpolated_angle_top) < 1e-12)
                           && (check_point_angle >= interpolated_angle_bottom || std::fabs(check_point_angle - interpolated_angle_bottom) < 1e-12))
                          || (difference_in_angle_along_segment < 0 && (check_point_angle >= interpolated_angle_top || std::fabs(check_point_angle - interpolated_angle_top) < 1e-12)
                              && (check_point_angle <= interpolated_angle_bottom || std::fabs(check_point_angle - interpolated_angle_bottom) < 1e-12)))
                        {
                          new_distance = (radius_angle_circle - CPCR_norm) * (difference_in_angle_along_segment < 0 ? 1 : -1);
                          new_along_plane_distance = (radius_angle_circle * check_point_angle - radius_angle_circle * interpolated_angle_top) * (difference_in_angle_along_segment < 0 ? 1 : -1);
                        }

                    }

                  // Now we need to see whether we need to update the information
                  // based on whether this segment is the closest one to the point
                  // up to now. To do this we first look whether the point falls
                  // within the bound of the segment and if it is actually closer.
                  // TODO: find out whether the fabs() are needed.
                  if (new_along_plane_distance >= -1e-10 &&
                      new_along_plane_distance <= std::fabs(interpolated_segment_length) &&
                      std::fabs(new_distance) < std::fabs(distance))
                    {
                      // There are two specific cases we are concerned with. The
                      // first case is that we want to have both the positive and
                      // negative distances (above and below the line). The second
                      // case is that we only want positive distances.
                      distance = only_positive ? std::fabs(new_distance) : new_distance;
                      along_plane_distance = new_along_plane_distance + total_length;
                      section = current_section;
                      section_fraction = fraction_CPL_P1P2;
                      segment = i_segment;
                      segment_fraction = new_along_plane_distance / interpolated_segment_length;
                      total_average_angle = (average_angle * total_length
                                             + 0.5 * (interpolated_angle_top + interpolated_angle_bottom  - 2 * add_angle) * new_along_plane_distance);
                      total_average_angle = (std::fabs(total_average_angle) < std::numeric_limits<double>::epsilon() ? 0 : total_average_angle /
                                             (total_length + new_along_plane_distance));
                    }

                  // increase average angle
                  average_angle = (average_angle * total_length +
                                   0.5 * (interpolated_angle_top + interpolated_angle_bottom  - 2 * add_angle) * interpolated_segment_length);
                  average_angle = (std::fabs(average_angle) < std::numeric_limits<double>::epsilon() ? 0 : average_angle /
                                   (total_length + interpolated_segment_length));
                  // increase the total length for the next segment.
                  total_length += interpolated_segment_length;
                }
            }

        }
      else
        {

          bool continue_computation = false;
          // we now have an initial guess for our Newton method to find out what the
          // actual closest point is to the spline.

          double solution = fabs(global_x_list[i_section_min_distance]) <= 10.*nextafter(0.0,1.0) ? fraction_CPL_P1P2_strict : global_x_list[i_section_min_distance];//+fraction_CPL_P1P2_strict;//i_section_min_distance + fraction_CPL_P1P2_strict;// - global_x_list[i_section_min_distance];//global_x_list[i_section_min_distance] + fraction_CPL_P1P2_strict;// - global_x_list[point_list.size()-1];

          WBAssert(!isnan(solution), "Solution is not a number. Relevant variables: = " << solution << ",  check_point = " << check_point[0] << ":" <<  check_point[1] << ":" <<  check_point[2] << ".");

          // compute initial residual
          double residual_x = x_spline.residual_closest_point(solution,check_point_surface_2d[0]);
          double residual_y = y_spline.residual_closest_point(solution,check_point_surface_2d[1]);
          double inital_residual = fabs(residual_x)+fabs(residual_y);
          double residual = inital_residual;

          // get a better estimate for the closest point between P1 and P2.
          double min_estimate_solution = -1./25.;
          solution = min_estimate_solution;
          double min_estimate_solution_temp = min_estimate_solution;
          double x_minimum_estimate_residual = x_spline.residual_closest_point(min_estimate_solution,check_point_surface_2d[0]);
          double y_minimum_estimate_residual = y_spline.residual_closest_point(min_estimate_solution,check_point_surface_2d[1]);
          double minimum_estimate_residual = fabs(x_minimum_estimate_residual)+fabs(y_minimum_estimate_residual);
          double x_distance_to_reference_point = x_spline(min_estimate_solution)-check_point_surface_2d[0];
          double y_distance_to_reference_point = y_spline(min_estimate_solution)-check_point_surface_2d[1];
          double minimum_distance_to_reference_point = sqrt(x_distance_to_reference_point*x_distance_to_reference_point + y_distance_to_reference_point*y_distance_to_reference_point);
          for (size_t i_estimate = 1; i_estimate < 25.*(global_x_list[point_list.size()-1]+ 2.0/25); i_estimate++)
            {
              min_estimate_solution_temp = min_estimate_solution_temp + 1.0/25.;


              double x_minimum_estimate_residual_temp = x_spline.residual_closest_point(min_estimate_solution_temp,check_point_surface_2d[0]);
              double y_minimum_estimate_residual_temp = y_spline.residual_closest_point(min_estimate_solution_temp,check_point_surface_2d[1]);
              double minimum_estimate_residual_temp = fabs(x_minimum_estimate_residual_temp)+fabs(y_minimum_estimate_residual_temp);


              double x_distance_to_reference_point_temp = x_spline(min_estimate_solution_temp)-check_point_surface_2d[0];
              double y_distance_to_reference_point_temp = y_spline(min_estimate_solution_temp)-check_point_surface_2d[1];
              double minimum_distance_to_reference_point_temp = sqrt(x_distance_to_reference_point_temp*x_distance_to_reference_point_temp + y_distance_to_reference_point_temp*y_distance_to_reference_point_temp);

              if (fabs(minimum_distance_to_reference_point_temp) < fabs(minimum_distance_to_reference_point))
                {
                  minimum_estimate_residual = minimum_estimate_residual_temp;
                  minimum_distance_to_reference_point = minimum_distance_to_reference_point_temp;
                  min_estimate_solution = min_estimate_solution_temp;
                }

            }

          solution = min_estimate_solution;

          double old_newton_update = INFINITY;
          double old_old_newton_update = INFINITY;
          double old_old_old_newton_update = INFINITY;
          double aternating_factor = 1.0;
          // copmute a newton update
          for (size_t i_newton =0; i_newton < 10; i_newton++)
            {
              std::pair<double,double> x_spline_update = x_spline.newton_update(solution,check_point_surface_2d[0]);
              std::pair<double,double> y_spline_update = y_spline.newton_update(solution,check_point_surface_2d[1]);

              // todo, if x_spline_update.first and y_spline_update.first are zero, I think we have found the solution.
              if (fabs(x_spline_update.first) <= 10.*nextafter(0.0,1.0) && fabs(y_spline_update.first) <= 10.*nextafter(0.0,1.0))
                break;

              double temp_solution = solution;
              double newton_update = (x_spline_update.first + y_spline_update.first)/(x_spline_update.second + y_spline_update.second);

              WBAssert(isfinite(newton_update), "The newton update is not finite. Relevant variables: newton_update = " << newton_update << ", temp_solution = " << temp_solution
                       << ", solution = " << solution << ", x_spline_update.first = " << x_spline_update.first << ", y_spline_update.first = " << y_spline_update.first << ",  x_spline_update.second = "
                       << x_spline_update.second  << ",  y_spline_update.second = " << y_spline_update.second << ",  check_point = " << check_point[0] << ":" <<  check_point[1] << ":" <<  check_point[2] << ".");
              WBAssert(!isnan(newton_update), "newton_update is not a number or not finite. Relevant variables: newton_update = " << newton_update << ", temp_solution = " << temp_solution
                       << ", solution = " << solution << ", x_spline_update.first = " << x_spline_update.first << ", y_spline_update.first = " << y_spline_update.first << ",  x_spline_update.second = "
                       << x_spline_update.second  << ",  y_spline_update.second = " << y_spline_update.second << ",  check_point = " << check_point[0] << ":" <<  check_point[1] << ":" <<  check_point[2] << ".");

              temp_solution = solution - newton_update;

              WBAssert(!isnan(temp_solution), "Solution is not a number. Relevant variables: temp_solution = " << temp_solution << ", solution = " << solution << ", newton_update = " << newton_update << ",  check_point = " << check_point[0] << ":" <<  check_point[1] << ":" <<  check_point[2] << ".");

              // compute residual
              residual_x = x_spline.residual_closest_point(temp_solution,check_point_surface_2d[0]);
              residual_y = y_spline.residual_closest_point(temp_solution,check_point_surface_2d[1]);

              double temp_residual = fabs(residual_x)+fabs(residual_y);
              double relative_residual = temp_residual/inital_residual;

              double factor = 1;
              if (newton_update > 0.15 && newton_update > 0.0)
                {
                  factor = 0.15/newton_update;
                  temp_solution = solution -factor* newton_update;
                  residual_x = x_spline.residual_closest_point(temp_solution,check_point_surface_2d[0]);
                  residual_y = y_spline.residual_closest_point(temp_solution,check_point_surface_2d[1]);
                  temp_residual = fabs(residual_x)+fabs(residual_y);
                  relative_residual = temp_residual/inital_residual;
                }

              factor *= aternating_factor;


              for (size_t i_line_search = 0; i_line_search < 25; i_line_search++)
                {

                  if (temp_residual > 0.9*residual)
                    {
                      factor *= 2./3.;
                      temp_solution = solution -factor* newton_update;
                      residual_x = x_spline.residual_closest_point(temp_solution,check_point_surface_2d[0]);
                      residual_y = y_spline.residual_closest_point(temp_solution,check_point_surface_2d[1]);
                      temp_residual = fabs(residual_x)+fabs(residual_y);
                      relative_residual = temp_residual/inital_residual;
                    }
                  else
                    {
                      break;
                    }

                }

              if (
                (fabs(old_old_old_newton_update-old_newton_update) < 1e-7 && fabs(old_old_newton_update-newton_update) < 1e-7 && ((old_old_old_newton_update<0) != (newton_update<0)))
              )
                {
                  aternating_factor *= 0.5;
                }

              solution = temp_solution;
              residual = temp_residual;

              WBAssert(!isnan(solution), "Solution is not a number. Relevant variables: = " << solution << ",  check_point = " << check_point[0] << ":" <<  check_point[1] << ":" <<  check_point[2] << ".");

              // Besides making sure that the relative residual is small enough, also check wheter the update is sufficiently smaller than the solution
              // to prevent ossilations between posive and negative values from dominationg the solution.
              if ((residual/inital_residual <=1e-52 && newton_update < 0.1*solution)
                  || (fabs(old_old_old_newton_update-old_old_newton_update) < 1e-30 && fabs(old_old_newton_update-old_newton_update) < 1e-30 && fabs(old_newton_update-newton_update) < 1e-30) // in the last four iterations the newton update has been the same. There is no point in iterating further and we can probably safely abort
                  //||(fabs(old_old_old_newton_update-old_newton_update) < 1e-7 && fabs(old_old_newton_update-newton_update) < 1e-7 && ((old_old_old_newton_update<0) != (newton_update<0)))
                 )
                break;

              old_old_old_newton_update = old_old_newton_update;
              old_old_newton_update = old_newton_update;
              old_newton_update = newton_update;
            }
          WBAssert(!isnan(solution), "Solution is not a number. Relevant variables: = " << solution << ",  check_point = " << check_point[0] << ":" <<  check_point[1] << ":" <<  check_point[2] << ".");


          if (fabs(solution-global_x_list[point_list.size()-1]) < 5.0*nextafter(0.0,1.0))
            {
              solution = global_x_list[point_list.size()-1] - std::numeric_limits<double>::epsilon();
            }

          double dx = x_spline.derivative(solution);
          double dy = y_spline.derivative(solution);

          WBAssert(!isnan(dx), "dx is not a number: " << dx << ", Relevant variables: solution = " << solution << ",  check_point = " << check_point[0] << ":" <<  check_point[1] << ":" <<  check_point[2]);
          WBAssert(!isnan(dy), "dy is not a number: " << dy << ", Relevant variables: solution = " << solution << ",  check_point = " << check_point[0] << ":" <<  check_point[1] << ":" <<  check_point[2]);

          Point<2> closest_point_on_spline_2d(x_spline(solution),y_spline(solution),natural_coordinate_system);



          double fraction_CPL_P1P2 = std::numeric_limits<double>::signaling_NaN();
          Point<3> x_axis(std::numeric_limits<double>::signaling_NaN(),std::numeric_limits<double>::signaling_NaN(),std::numeric_limits<double>::signaling_NaN(),cartesian);
          Point<3> y_axis(std::numeric_limits<double>::signaling_NaN(),std::numeric_limits<double>::signaling_NaN(),std::numeric_limits<double>::signaling_NaN(),cartesian);
          Point<3> closest_point_on_line_cartesian(std::numeric_limits<double>::signaling_NaN(),std::numeric_limits<double>::signaling_NaN(),std::numeric_limits<double>::signaling_NaN(),cartesian);
          Point<3> closest_point_on_line_bottom_cartesian(std::numeric_limits<double>::signaling_NaN(),std::numeric_limits<double>::signaling_NaN(),std::numeric_limits<double>::signaling_NaN(),cartesian);
          size_t current_section = i_section_min_distance;
          size_t next_section = i_section_min_distance+1;
          // translate to orignal coordinates current and next section
          size_t original_current_section = static_cast<size_t>(std::floor(global_x_list[i_section_min_distance]));
          size_t original_next_section = original_current_section + 1;

          if (solution > 0 && floor(solution) <= global_x_list[point_list.size()-2] && floor(solution)  >= 0)
            {
              continue_computation = true;
              // we need to compute the section which we are in. This might have changed in the Newton iteration.
              //
              const double diff = solution - global_x_list[i_section_min_distance];
              current_section = (size_t) floor(solution);
              WBAssert((solution >= 0 && current_section <= point_list.size()-2), "current_section wrong: " << current_section << ", because of solution, which is " << solution
                       << ", i_section_min_distance = " << i_section_min_distance << ", or diff = ");

              next_section = current_section+1;
              // translate to orignal coordinates current and next section
              original_current_section = floor(solution);
              original_next_section = original_current_section + 1;

              const Point<2> P1(point_list[current_section]);

              const Point<2> P2(point_list[next_section]);

              const Point<2> P1P2 = P2 - P1;
              const Point<2> P1PC = check_point_surface_2d - P1;


              // compute what fraction of the distance between P1 and P2 the
              // closest point lies.
              Point<2> P1CPL = closest_point_on_spline_2d - P1;

              fraction_CPL_P1P2_strict = solution-floor(solution);


              // now figure out where the point is in relation with the user
              // defined coordinates
              fraction_CPL_P1P2 = fraction_CPL_P1P2_strict;
              Point<2> direction_variable = Point<2>(-dy,dx,natural_coordinate_system);
              const Point<2> unit_normal_to_plane_spherical = direction_variable.norm() >= 10.*nextafter(0.0,1.0) ?direction_variable/direction_variable.norm() : P1P2 / P1P2.norm();
              const Point<2> closest_point_on_line_plus_normal_to_plane_spherical = closest_point_on_spline_2d + 1e-8 * (closest_point_on_spline_2d.norm() > 1.0 ? closest_point_on_spline_2d.norm() : 1.0) * unit_normal_to_plane_spherical;

              WBAssert(closest_point_on_line_plus_normal_to_plane_spherical.norm() >=  10.*nextafter(0.0,1.0),
                       "Internal error: The norm of variable 'closest_point_on_line_plus_normal_to_plane_spherical' "
                       "is  zero, while this may not happen.");

              // We now need 3d points from this point on, so make them.
              // The order of a Cartesian coordinate is x,y,z and the order of
              // a spherical coordinate it radius, long, lat (in rad).
              const Point<3> closest_point_on_line_surface(bool_cartesian ? closest_point_on_spline_2d[0] : start_radius,
                                                           bool_cartesian ? closest_point_on_spline_2d[1] : closest_point_on_spline_2d[0],
                                                           bool_cartesian ? start_radius : closest_point_on_spline_2d[1],
                                                           natural_coordinate_system);

              Point<3> closest_point_on_line_bottom = closest_point_on_line_surface;
              closest_point_on_line_bottom[bool_cartesian ? 2 : 0] = 0;

              const Point<3> closest_point_on_line_plus_normal_to_plane_surface_spherical(bool_cartesian ? closest_point_on_line_plus_normal_to_plane_spherical[0] : start_radius,
                                                                                          bool_cartesian ? closest_point_on_line_plus_normal_to_plane_spherical[1] : closest_point_on_line_plus_normal_to_plane_spherical[0],
                                                                                          bool_cartesian ? start_radius : closest_point_on_line_plus_normal_to_plane_spherical[1],
                                                                                          natural_coordinate_system);


              WBAssert(!std::isnan(closest_point_on_line_plus_normal_to_plane_surface_spherical[0]),
                       "Internal error: The closest_point_on_line_plus_normal_to_plane_surface_spherical[0] variable is not a number: " << closest_point_on_line_plus_normal_to_plane_surface_spherical[0]
                       << ". Relevant values: bool_cartesian = " << bool_cartesian << ", closest_point_on_line_plus_normal_to_plane_spherical[0] = " << closest_point_on_line_plus_normal_to_plane_spherical[0] << ", starting_radius = " << start_radius << ".");
              WBAssert(!std::isnan(closest_point_on_line_plus_normal_to_plane_surface_spherical[1]),
                       "Internal error: The closest_point_on_line_plus_normal_to_plane_surface_spherical[0] variable is not a number: " << closest_point_on_line_plus_normal_to_plane_surface_spherical[1]);
              WBAssert(!std::isnan(closest_point_on_line_plus_normal_to_plane_surface_spherical[2]),
                       "Internal error: The closest_point_on_line_plus_normal_to_plane_surface_spherical[0] variable is not a number: " << closest_point_on_line_plus_normal_to_plane_surface_spherical[2]);

              // Now that we have both the check point and the
              // closest_point_on_line, we need to push them to cartesian.
              Point<3> check_point_cartesian = Point<3>(check_point);
              Point<3> check_point_surface_cartesian =  Point<3>(coordinate_system->natural_to_cartesian_coordinates(check_point_surface.get_array()),cartesian);
              closest_point_on_line_cartesian =  Point<3>(coordinate_system->natural_to_cartesian_coordinates(closest_point_on_line_surface.get_array()),cartesian);
              closest_point_on_line_bottom_cartesian= Point<3>(coordinate_system->natural_to_cartesian_coordinates(closest_point_on_line_bottom.get_array()),cartesian);
              Point<3> closest_point_on_line_plus_normal_to_plane_cartesian =  Point<3>(coordinate_system->natural_to_cartesian_coordinates(closest_point_on_line_plus_normal_to_plane_surface_spherical.get_array()),cartesian);


              // if the two points are the same, we don't need to search any further
              if (std::fabs((check_point_cartesian - closest_point_on_line_cartesian).norm()) < 2e-14)
                {
                  continue_computation = false;
                  distance = 0.0;
                  along_plane_distance = 0.0;
                  section = current_section;
                  section_fraction = fraction_CPL_P1P2;
                  segment = 0;
                  segment_fraction = 0.0;
                  total_average_angle = plane_segment_angles[original_current_section][0][0]
                                        + fraction_CPL_P1P2 * (plane_segment_angles[original_next_section][0][0]
                                                               - plane_segment_angles[original_current_section][0][0]);
                }
              else
                {

                  Point<3> normal_to_plane = closest_point_on_line_plus_normal_to_plane_cartesian - closest_point_on_line_cartesian;
                  normal_to_plane = normal_to_plane / normal_to_plane.norm();

                  // The y-axis is from the bottom/center to the closest_point_on_line,
                  // the x-axis is 90 degrees rotated from that, so we rotate around
                  // the line P1P2.
                  // Todo: Assert that the norm of the axis are not equal to zero.
                  y_axis = closest_point_on_line_cartesian - closest_point_on_line_bottom_cartesian;

                  WBAssert(y_axis.norm() >=  10.*nextafter(0.0,1.0),
                           "Internal error: The y_axis.norm() is zero. Y_axis is " << y_axis[0] << ":" << y_axis[1] << ":" << y_axis[2]
                           << ". closest_point_on_line_cartesian = " << closest_point_on_line_cartesian[0] << ":" << closest_point_on_line_cartesian[1] << ":" << closest_point_on_line_cartesian[2]
                           << ", closest_point_on_line_bottom_cartesian = " << closest_point_on_line_bottom_cartesian[0] << ":" << closest_point_on_line_bottom_cartesian[1] << ":" << closest_point_on_line_bottom_cartesian[2]);

                  WBAssert(!std::isnan(y_axis[0]),
                           "Internal error: The y_axis variable is not a number: " << y_axis[0]);
                  WBAssert(!std::isnan(y_axis[1]),
                           "Internal error: The y_axis variable is not a number: " << y_axis[1]);
                  WBAssert(!std::isnan(y_axis[2]),
                           "Internal error: The y_axis variable is not a number: " << y_axis[2]);


                  y_axis = y_axis / y_axis.norm();


                  WBAssert(!std::isnan(y_axis[0]),
                           "Internal error: The y_axis variable is not a number: " << y_axis[0]);
                  WBAssert(!std::isnan(y_axis[1]),
                           "Internal error: The y_axis variable is not a number: " << y_axis[1]);
                  WBAssert(!std::isnan(y_axis[2]),
                           "Internal error: The y_axis variable is not a number: " << y_axis[2]);

                  x_axis = Point<3>(-dy,
                                    dx,
                                    0,
                                    cartesian);


                  WBAssert(!std::isnan(x_axis[0]),
                           "Internal error: The x_axis[0] variable is not a number: " << x_axis[0]);
                  WBAssert(!std::isnan(x_axis[1]),
                           "Internal error: The x_axis[1] variable is not a number: " << x_axis[1]);
                  WBAssert(!std::isnan(x_axis[2]),
                           "Internal error: The x_axis[2] variable is not a number: " << x_axis[2]);

                  // If we can't find a good axis through the newton method, we might want to fall back on how the axis is computed in the
                  // non-contious method, but I can't find a case where I trigger this issue.
                  WBAssert(x_axis.norm() >  10.*nextafter(0.0,1.0), "The norm of the x_axis is smaller than zero. Please report this to the developers "
                           << "with the world builder file and the following relvant variables: "
                           << "check_point =  " << std::setprecision(12) << check_point[0] << " " << check_point[1] << " " << check_point[2] << " " << start_radius
                           <<  "x_axis: " << x_axis[0] << ":" <<  x_axis[1] << ":" <<  x_axis[2] << ".");

                  // see on what side the line P1P2 reference point is. This is based on the determinant
                  const double reference_on_side_of_line = (point_list[next_section][0] - point_list[current_section][0])
                                                           * (reference_point[1] - point_list[current_section][1])
                                                           - (point_list[next_section][1] - point_list[current_section][1])
                                                           * (reference_point[0] - point_list[current_section][0])
                                                           < 0 ? -1 : 1;
                  WBAssert(x_axis.norm() > 0, "norm of the x-axis is zero. Relevant values: dx =" << dx << ", dy = " << dy );
                  x_axis = x_axis *(reference_on_side_of_line / x_axis.norm());


                  WBAssert(!std::isnan(x_axis[0]),
                           "Internal error: The x_axis[0] variable is not a number: " << x_axis[0] << ". Relevant values: reference_on_side_of_line = " << reference_on_side_of_line << ",  x_axis.norm() = " <<  x_axis.norm() << ".");
                  WBAssert(!std::isnan(x_axis[1]),
                           "Internal error: The x_axis[1] variable is not a number: " << x_axis[1] << ". Relevant values: reference_on_side_of_line = " << reference_on_side_of_line << ",  x_axis.norm() = " <<  x_axis.norm() << ".");
                  WBAssert(!std::isnan(x_axis[2]),
                           "Internal error: The x_axis[2] variable is not a number: " << x_axis[2] << ". Relevant values: reference_on_side_of_line = " << reference_on_side_of_line << ",  x_axis.norm() = " <<  x_axis.norm() << ".");

                }

            }



          if (continue_computation)
            {

              WBAssert(!std::isnan(x_axis[0]),
                       "Internal error: The x_axis[0] variable is not a number: " << x_axis[0] << ". Relevant values:  check_point = " << check_point[0] << ":" <<  check_point[1] << ":" <<  check_point[2] << ".");
              WBAssert(!std::isnan(x_axis[1]),
                       "Internal error: The x_axis[1] variable is not a number: " << x_axis[1]);
              WBAssert(!std::isnan(x_axis[2]),
                       "Internal error: The x_axis[2] variable is not a number: " << x_axis[2]);


              // now that we have the x and y axes computed, convert the 3d check point into a 2d one.
              Point<2> check_point_2d(x_axis * (check_point - closest_point_on_line_bottom_cartesian),
                                      y_axis * (check_point - closest_point_on_line_bottom_cartesian),
                                      cartesian);



              Point<2> begin_segment(x_axis * (closest_point_on_line_cartesian - closest_point_on_line_bottom_cartesian),
                                     y_axis * (closest_point_on_line_cartesian - closest_point_on_line_bottom_cartesian),
                                     cartesian);


              WBAssert(!std::isnan(check_point_2d[0]),
                       "Internal error: The check_point_2d variable is not a number: " << check_point_2d[0]);
              WBAssert(!std::isnan(check_point_2d[1]),
                       "Internal error: The check_point_2d variable is not a number: " << check_point_2d[1]);


              WBAssert(!std::isnan(begin_segment[0]),
                       "Internal error: The begin_segment variable is not a number: " << begin_segment[0]);
              WBAssert(!std::isnan(begin_segment[1]),
                       "Internal error: The begin_segment variable is not a number: " << begin_segment[1]);

              Point<2> end_segment = begin_segment;


              double total_length = 0.0;
              double add_angle = 0.0;
              double average_angle = 0.0;
              for (size_t i_segment = 0; i_segment < plane_segment_lengths[original_current_section].size(); i_segment++)
                {
                  const size_t current_segment = i_segment;

                  // compute the angle between the the previous begin and end if
                  // the depth method is angle_at_begin_segment_with_surface.
                  if (i_segment != 0 && depth_method == DepthMethod::angle_at_begin_segment_with_surface)
                    {
                      const double add_angle_inner = (begin_segment * end_segment) / (begin_segment.norm() * end_segment.norm());

                      WBAssert(!std::isnan(add_angle_inner),
                               "Internal error: The add_angle_inner variable is not a number: " << add_angle_inner
                               << ". Variables: begin_segment = " << begin_segment[0] << ":" << begin_segment[1]
                               << ", end_segment = " << end_segment[0] << ":" << end_segment[1]
                               << ", begin_segment * end_segment / (begin_segment.norm() * end_segment.norm()) = "
                               << std::setprecision(32) << begin_segment * end_segment / (begin_segment.norm() * end_segment.norm())
                               << ".");

                      // there could be round of error problems here is the inner part is close to one
                      WBAssert(add_angle_inner >= 0 && add_angle_inner <= 1,
                               "Internal error: The variable add_angle_inner is smaller than zero or larger then one,"
                               "which causes the std::acos to return nan. If it is only a little bit larger then one, "
                               "this is probably caused by that begin and end segment are the same and round off error. "
                               "The value of add_angle_inner = " << add_angle_inner);

                      add_angle += std::acos(add_angle_inner);

                      WBAssert(!std::isnan(add_angle),
                               "Internal error: The add_angle variable is not a number: " << add_angle
                               << ". Variables: begin_segment = " << begin_segment[0] << ":" << begin_segment[1]
                               << ", end_segment = " << end_segment[0] << ":" << end_segment[1]
                               << ", begin_segment * end_segment / (begin_segment.norm() * end_segment.norm()) = "
                               << std::setprecision(32) << begin_segment * end_segment / (begin_segment.norm() * end_segment.norm())
                               << ", std::acos(begin_segment * end_segment / (begin_segment.norm() * end_segment.norm())) = "
                               << std::acos(begin_segment * end_segment / (begin_segment.norm() * end_segment.norm())));
                    }




                  begin_segment = end_segment;

                  WBAssert(!std::isnan(begin_segment[0]),
                           "Internal error: The begin_segment variable is not a number: " << begin_segment[0]);
                  WBAssert(!std::isnan(begin_segment[1]),
                           "Internal error: The begin_segment variable is not a number: " << begin_segment[1]);


                  // This interpolates different properties between P1 and P2 (the
                  // points of the plane at the surface)
                  const double degree_90_to_rad = 0.5 * const_pi;

                  WBAssert(plane_segment_angles.size() > original_next_section,
                           "Error: original_next_section = " << original_next_section
                           << ", and plane_segment_angles.size() = " << plane_segment_angles.size());


                  WBAssert(plane_segment_angles[original_next_section].size() > current_segment,
                           "Error: current_segment = "  << current_segment
                           << ", and current_segment.size() = " << plane_segment_angles[original_next_section].size());

                  const double interpolated_angle_top    = plane_segment_angles[original_current_section][current_segment][0]
                                                           + fraction_CPL_P1P2 * (plane_segment_angles[original_next_section][current_segment][0]
                                                                                  - plane_segment_angles[original_current_section][current_segment][0])
                                                           + add_angle;

                  const double interpolated_angle_bottom = plane_segment_angles[original_current_section][current_segment][1]
                                                           + fraction_CPL_P1P2 * (plane_segment_angles[original_next_section][current_segment][1]
                                                                                  - plane_segment_angles[original_current_section][current_segment][1])
                                                           + add_angle;


                  double interpolated_segment_length     = plane_segment_lengths[original_current_section][current_segment]
                                                           + fraction_CPL_P1P2 * (plane_segment_lengths[original_next_section][current_segment]
                                                                                  - plane_segment_lengths[original_current_section][current_segment]);
                  WBAssert(!std::isnan(interpolated_angle_top),
                           "Internal error: The interpolated_angle_top variable is not a number: " << interpolated_angle_top);

                  // We want to know where the end point of this segment is (and
                  // the start of the next segment). There are two cases which we
                  // will deal with separately. The first one is if the angle is
                  // constant. The second one is if the angle changes.
                  const double difference_in_angle_along_segment = interpolated_angle_top - interpolated_angle_bottom;

                  if (std::fabs(difference_in_angle_along_segment) < 1e-8)
                    {
                      // The angle is constant. It is easy find find the end of
                      // this segment and the distance.
                      if (std::fabs(interpolated_segment_length) > std::numeric_limits<double>::epsilon())
                        {
                          end_segment[0] += interpolated_segment_length * std::sin(degree_90_to_rad - interpolated_angle_top);
                          end_segment[1] -= interpolated_segment_length * std::cos(degree_90_to_rad - interpolated_angle_top);

                          Point<2> begin_end_segment = end_segment - begin_segment;
                          Point<2> normal_2d_plane(-begin_end_segment[0],begin_end_segment[1], cartesian);
                          WBAssert(std::fabs(normal_2d_plane.norm()) > std::numeric_limits<double>::epsilon(), "Internal Error: normal_2d_plane.norm() is zero, which should not happen. "
                                   << "Extra info: begin_end_segment[0] = " << begin_end_segment[0]
                                   << ", begin_end_segment[1] = " << begin_end_segment[1]
                                   << ", end_segment: [" << end_segment[0] << "," << end_segment[1] << "]"
                                   << ", begin_segment: [" << begin_segment[0] << "," << begin_segment[1] << "]"
                                  );
                          normal_2d_plane /= normal_2d_plane.norm();

                          // Now find the distance of a point to this line.
                          // Based on http://geomalgorithms.com/a02-_lines.html.
                          const Point<2> BSP_ESP = end_segment - begin_segment;
                          const Point<2> BSP_CP = check_point_2d - begin_segment;

                          const double c1 = BSP_ESP * BSP_CP;
                          const double c2 = BSP_ESP * BSP_ESP;

                          if (c1 < 0 || c2 < c1)
                            {
                              new_distance = INFINITY;
                              new_along_plane_distance = INFINITY;
                            }
                          else
                            {
                              const Point<2> Pb = begin_segment + (c1/c2) * BSP_ESP;
                              const double side_of_line =  (begin_segment[0] - end_segment[0]) * (check_point_2d[1] - begin_segment[1])
                                                           - (begin_segment[1] - end_segment[1]) * (check_point_2d[0] - begin_segment[0])
                                                           < 0 ? -1.0 : 1.0;

                              new_distance = side_of_line * (check_point_2d - Pb).norm();
                              new_along_plane_distance = (begin_segment - Pb).norm();
                            }

                        }
                    }
                  else
                    {
                      // The angle is not constant. This means that we need to
                      // define a circle. First find the center of the circle.
                      const double radius_angle_circle = std::fabs(interpolated_segment_length/difference_in_angle_along_segment);

                      WBAssert(!std::isnan(radius_angle_circle),
                               "Internal error: The radius_angle_circle variable is not a number: " << radius_angle_circle
                               << ". interpolated_segment_length = " << interpolated_segment_length
                               << ", difference_in_angle_along_segment = " << difference_in_angle_along_segment);

                      const double cos_angle_top = std::cos(interpolated_angle_top);

                      WBAssert(!std::isnan(cos_angle_top),
                               "Internal error: The radius_angle_circle variable is not a number: " << cos_angle_top
                               << ". interpolated_angle_top = " << interpolated_angle_top);

                      Point<2> center_circle(cartesian);
                      if (std::fabs(interpolated_angle_top - 0.5 * const_pi) < 1e-8)
                        {
                          // if interpolated_angle_top is 90 degrees, the tan function
                          // is undefined (1/0). What we really want in this case is
                          // set the center to the correct location which is x = the x
                          //begin point + radius and y = the y begin point.
                          center_circle[0] = difference_in_angle_along_segment > 0 ? begin_segment[0] + radius_angle_circle : begin_segment[0] - radius_angle_circle;
                          center_circle[1] = begin_segment[1];
                        }
                      else if (std::fabs(interpolated_angle_top - 1.5 * const_pi) < 1e-8)
                        {
                          // if interpolated_angle_top is 270 degrees, the tan function
                          // is undefined (-1/0). What we really want in this case is
                          // set the center to the correct location which is x = the x
                          //begin point - radius and y = the y begin point.
                          center_circle[0] = difference_in_angle_along_segment > 0 ? begin_segment[0] - radius_angle_circle : begin_segment[0] + radius_angle_circle;
                          center_circle[1] = begin_segment[1];
                        }
                      else
                        {
                          double tan_angle_top = std::tan(interpolated_angle_top);

                          WBAssert(!std::isnan(tan_angle_top),
                                   "Internal error: The tan_angle_top variable is not a number: " << tan_angle_top);
                          const double center_circle_y = difference_in_angle_along_segment < 0 ?
                                                         begin_segment[1] - radius_angle_circle * cos_angle_top
                                                         : begin_segment[1] + radius_angle_circle * cos_angle_top;

                          WBAssert(!std::isnan(center_circle_y),
                                   "Internal error: The center_circle_y variable is not a number: " << center_circle_y
                                   << ". begin_segment[1] = " << begin_segment[1]
                                   << ", radius_angle_circle = " << radius_angle_circle
                                   << ", cos_angle_top = " << cos_angle_top);

                          // to prevent round off errors becomming dominant, we check
                          // whether center_circle_y - begin_segment[1] should be zero.
                          // TODO: improve this to some kind of relative difference.
                          const double CCYBS = center_circle_y - begin_segment[1];

                          WBAssert(!std::isnan(CCYBS),
                                   "Internal error: The CCYBS variable is not a number: " << CCYBS);



                          center_circle[0] = begin_segment[0] + tan_angle_top * (CCYBS);
                          center_circle[1] = center_circle_y;
                        }

                      WBAssert(!std::isnan(center_circle[0]) || !std::isnan(center_circle[1]),
                               "Internal error: The center variable contains not a number: " << center_circle[0] << ":" << center_circle[0]);
                      WBAssert(std::fabs((begin_segment-center_circle).norm() - std::fabs(radius_angle_circle))
                               < 1e-8 * std::fabs((begin_segment-center_circle).norm() + std::fabs(radius_angle_circle)),
                               "Internal error: The center of the circle is not a radius away from the begin point. " << std::endl
                               << "The center is located at " << center_circle[0] << ":" << center_circle[1] << std::endl
                               << "The begin point is located at " << begin_segment[0] << ":" << begin_segment[1] << std::endl
                               << "The computed radius is " << std::fabs((begin_segment-center_circle).norm())
                               << ", and it should be " << radius_angle_circle << ".");


                      // Now compute the location of the end of the segment by
                      // rotating P1 around the center_circle
                      Point<2> BSPC = begin_segment - center_circle;
                      const double sin_angle_diff = sin(difference_in_angle_along_segment);
                      const double cos_angle_diff = cos(difference_in_angle_along_segment);
                      end_segment[0] = cos_angle_diff * BSPC[0] - sin_angle_diff * BSPC[1] + center_circle[0];
                      end_segment[1] = sin_angle_diff * BSPC[0] + cos_angle_diff * BSPC[1] + center_circle[1];



                      WBAssert(std::fabs((end_segment-center_circle).norm() - std::fabs(radius_angle_circle))
                               < 1e-8 * std::fabs((end_segment-center_circle).norm() + std::fabs(radius_angle_circle)) ,
                               "Internal error: The center of the circle is not a radius away from the end point. " << std::endl
                               << "The center is located at " << center_circle[0] << ":" << center_circle[1] << std::endl
                               << "The end point is located at " << end_segment[0] << ":" << end_segment[1] << std::endl
                               << "The computed radius is " << std::fabs((end_segment-center_circle).norm())
                               << ", and it should be " << radius_angle_circle << ".");

                      // Now check if the angle of the check point in this circle
                      // is larger then the angle of P1 and smaller then P1 + angle
                      // difference. If that is the case then the distance from the
                      // plane is radius - (center - check_point).norm(). Otherwise
                      // it is infinity.
                      // The angle of the check point is computed with the help of
                      // dot product. But before that we need to adjust the check
                      // point 2d.
                      const Point<2> CPCR = check_point_2d - center_circle;
                      const double CPCR_norm = CPCR.norm();

                      const double dot_product = CPCR * Point<2>(0, radius_angle_circle, cartesian);
                      // If the x of the check point is larger then the x of center
                      // the circle, the angle is more than 180 degree, but the dot
                      // product will decrease instead of increase from 180 degrees.
                      // To fix this we make a special case for this.
                      // Furthermore, when the check point is at the same location as
                      // the center of the circle, we count that point as belonging
                      // to the top of the top segment (0 degree).
                      double check_point_angle = std::fabs(CPCR_norm) < std::numeric_limits<double>::epsilon() ? 2.0 * const_pi : (check_point_2d[0] <= center_circle[0]
                                                 ? std::acos(dot_product/(CPCR_norm * radius_angle_circle))
                                                 : 2.0 * const_pi - std::acos(dot_product/(CPCR_norm * radius_angle_circle)));
                      check_point_angle = difference_in_angle_along_segment >= 0 ? const_pi - check_point_angle : 2.0 * const_pi - check_point_angle;

                      // In the case that it is exactly 2 * pi, bring it back to zero
                      check_point_angle = (std::fabs(check_point_angle - 2 * const_pi) < 1e-14 ? 0 : check_point_angle);

                      if ((difference_in_angle_along_segment > 0 && (check_point_angle <= interpolated_angle_top || std::fabs(check_point_angle - interpolated_angle_top) < 1e-12)
                           && (check_point_angle >= interpolated_angle_bottom || std::fabs(check_point_angle - interpolated_angle_bottom) < 1e-12))
                          || (difference_in_angle_along_segment < 0 && (check_point_angle >= interpolated_angle_top || std::fabs(check_point_angle - interpolated_angle_top) < 1e-12)
                              && (check_point_angle <= interpolated_angle_bottom || std::fabs(check_point_angle - interpolated_angle_bottom) < 1e-12)))
                        {
                          new_distance = (radius_angle_circle - CPCR_norm) * (difference_in_angle_along_segment < 0 ? 1 : -1);
                          new_along_plane_distance = (radius_angle_circle * check_point_angle - radius_angle_circle * interpolated_angle_top) * (difference_in_angle_along_segment < 0 ? 1 : -1);
                        }

                    }

                  // Now we need to see whether we need to update the information
                  // based on whether this segment is the closest one to the point
                  // up to now. To do this we first look whether the point falls
                  // within the bound of the segment and if it is actually closer.
                  // TODO: find out whether the fabs() are needed.
                  if (new_along_plane_distance >= -1e-10 &&
                      new_along_plane_distance <= std::fabs(interpolated_segment_length) &&
                      std::fabs(new_distance) < std::fabs(distance))
                    {
                      // There are two specific cases we are concerned with. The
                      // first case is that we want to have both the positive and
                      // negative distances (above and below the line). The second
                      // case is that we only want positive distances.
                      distance = only_positive ? std::fabs(new_distance) : new_distance;
                      along_plane_distance = new_along_plane_distance + total_length;
                      section = current_section;
                      section_fraction = fraction_CPL_P1P2;
                      segment = i_segment;
                      segment_fraction = new_along_plane_distance / interpolated_segment_length;
                      total_average_angle = (average_angle * total_length
                                             + 0.5 * (interpolated_angle_top + interpolated_angle_bottom  - 2 * add_angle) * new_along_plane_distance);
                      total_average_angle = (std::fabs(total_average_angle) < std::numeric_limits<double>::epsilon() ? 0 : total_average_angle /
                                             (total_length + new_along_plane_distance));
                    }

                  // increase average angle
                  average_angle = (average_angle * total_length +
                                   0.5 * (interpolated_angle_top + interpolated_angle_bottom  - 2 * add_angle) * interpolated_segment_length);
                  average_angle = (std::fabs(average_angle) < std::numeric_limits<double>::epsilon() ? 0 : average_angle /
                                   (total_length + interpolated_segment_length));
                  // increase the total length for the next segment.
                  total_length += interpolated_segment_length;
                }
            }



        }

      std::map<std::string, double> return_values;
      return_values["distanceFromPlane"] = distance;
      return_values["distanceAlongPlane"] = along_plane_distance;
      return_values["sectionFraction"] = section_fraction;
      return_values["segmentFraction"] = segment_fraction;
      return_values["section"] = static_cast<double>(section);
      return_values["segment"] = segment;
      return_values["averageAngle"] = total_average_angle;
      return return_values;
    }

    void interpolation::set_points(const std::vector<double> &x,
                                   const std::vector<double> &y,
                                   bool monotone_spline)
    {
      WBAssert(x.size() != 0, "Internal error: The x in the set points function is zero.");
      assert(x.size() == y.size());
      m_x = x;
      m_y = y;
      const size_t n = x.size();
      for (size_t i = 0; i < n-1; i++)
        {
          assert(m_x[i] < m_x[i+1]);
        }

      if (monotone_spline == true)
        {
          /**
           * This monotone spline algorithm is based on the javascript version
           * at https://en.wikipedia.org/wiki/Monotone_cubic_interpolation. The
           * parameters from this algorithm prevent overshooting in the
           * interpolation spline.
           */
          std::vector<double> dys(n-1), dxs(n-1), ms(n-1);
          for (size_t i=0; i < n-1; i++)
            {
              dxs[i] = x[i+1]-x[i];
              dys[i] = y[i+1]-y[i];
              ms[i] = dys[i]/dxs[i];
            }

          // get m_a parameter
          m_c.resize(n);
          m_c[0] = 0;

          for (size_t i = 0; i < n-2; i++)
            {
              const double m0 = ms[i];
              const double m1 = ms[i+1];

              if (m0 * m1 <= 0)
                {
                  m_c[i+1] = 0;
                }
              else
                {
                  const double dx0 = dxs[i];
                  const double dx1 = dxs[i+1];
                  const double common = dx0 + dx1;
                  m_c[i+1] = 3*common/((common + dx0)/m0 + (common + dx1)/m1);
                }
            }
          m_c[n-1] = ms[n-2];

          // Get b and c coefficients
          m_a.resize(n);
          m_b.resize(n);
          for (size_t i = 0; i < m_c.size()-1; i++)
            {
              const double c1 = m_c[i];
              const double m0 = ms[i];

              const double invDx = 1/dxs[i];
              const double common0 = c1 + m_c[i+1] - m0 - m0;
              m_b[i] = (m0 - c1 - common0) * invDx;
              m_a[i] = common0 * invDx * invDx;
            }
        }
      else     // linear interpolation
        {
          m_a.resize(n);
          m_b.resize(n);
          m_c.resize(n);
          for (size_t i = 0; i<n-1; i++)
            {
              m_a[i] = 0.0;
              m_b[i] = 0.0;
              m_c[i] = (m_y[i+1]-m_y[i])/(m_x[i+1]-m_x[i]);
            }
        }

      // for the right boundary we define
      // f_{n-1}(x) = b*(x-x_{n-1})^2 + c*(x-x_{n-1}) + y_{n-1}
      double h = x[n-1]-x[n-2];
      // m_b[n-1] is determined by the boundary condition
      if (!monotone_spline)
        {
          m_a[n-1] = 0.0;
          m_c[n-1] = 3.0*m_a[n-2]*h*h+2.0*m_b[n-2]*h+m_c[n-2];   // = f'_{n-2}(x_{n-1})
        }
    }

    double interpolation::operator() (double x) const
    {
      size_t n = m_x.size();
      // find the closest point m_x[idx] < x, idx=0 even if x<m_x[0]
      std::vector<double>::const_iterator it;
      it = std::lower_bound(m_x.begin(),m_x.end(),x);
      size_t idx = static_cast<size_t>(std::max( static_cast<int>(it-m_x.begin())-1, 0));

      double h = x-m_x[idx];
      double interpol;
      if (x<m_x[0])
        {
          // extrapolation to the left
          interpol = ((m_b[0])*h + m_c[0])*h + m_y[0];
        }
      else if (x>m_x[n-1])
        {
          // extrapolation to the right
          interpol = ((m_b[n-1])*h + m_c[n-1])*h + m_y[n-1];
        }
      else
        {
          // interpolation
          interpol = ((m_a[idx]*h + m_b[idx])*h + m_c[idx])*h + m_y[idx];
        }
      return interpol;
    }

    double interpolation::derivative (double x) const
    {
      size_t n = m_x.size();
      // find the closest point m_x[idx] < x, idx=0 even if x<m_x[0]
      std::vector<double>::const_iterator it;
      it = std::lower_bound(m_x.begin(),m_x.end(),x);
      int idx = std::max( int(it-m_x.begin())-1, 0);

      double h = x-m_x[idx];
      double interpol;
      if (x<m_x[0])
        {
          // extrapolation to the left
          interpol = 2.0*m_b[0]*h + m_c[0];
        }
      else if (x>m_x[n-1])
        {
          // extrapolation to the right
          interpol = 2.0*m_b[n-1]*h + m_c[n-1];
        }
      else
        {
          // interpolation
          interpol = 3.0*m_a[idx]*h*h + 2.0*m_b[idx]*h + m_c[idx];
        }
      //if(interpol == 0.0)
      //  std::cout << "interpol zero, m_a[idx] = " << m_a[idx] << ", h = " << h << ", m_b[idx] = " << m_b[idx] << ", m_c[idx] = " << m_c[idx] << ", m_x[idx] = " << m_x[idx] << ", x = " << x << std::endl;
      return interpol;
    }

    std::pair<double,double> interpolation::newton_update(double x, double p) const
    {
      size_t n = m_x.size();
      // find the closest point m_x[idx] < x, idx=0 even if x<m_x[0]
      std::vector<double>::const_iterator it;
      it = std::lower_bound(m_x.begin(),m_x.end(),x);
      int idx = std::max( int(it-m_x.begin())-1, 0);

      double f = 0;
      double df = 0;
      if (x<m_x[0])
        {
          // extrapolation to the left
          //interpol = ((m_b[0])*h + m_c[0])*h + m_y[0];
          double h = x-m_x[0];
          double a = m_a[0];
          double b = m_b[0];
          double c = m_c[0];
          double d = m_y[0];
          f = (p-(b*h*h*h+c*h+d))*(2*b*h+c);
          df = -20*a*b*h*h*h  -(6*b*b)*h*h  +6*(-b*c)*h   + (2*p*b-c*c-2*b*d);
        }
      else if (x>m_x[n-1])
        {
          // extrapolation to the right
          //interpol = ((m_b[n-1])*h + m_c[n-1])*h + m_y[n-1];
          double h = x-m_x[n-1];
          double a = m_a[n-1];
          double b = m_b[n-1];
          double c = m_c[n-1];
          double d = m_y[n-1];
          f = (p-(b*h*h*h+c*h+d))*(2*b*h+c);
          df = -20*a*b*h*h*h  -(6*b*b)*h*h  +6*(-b*c)*h   + (2*p*b-c*c-2*b*d);
        }
      else
        {
          // interpolation
          //interpol = ((m_a[idx]*h + m_b[idx])*h + m_c[idx])*h + m_y[idx];
          double h = x-m_x[idx];
          double a = m_a[idx];
          double b = m_b[idx];
          double c = m_c[idx];
          double d = m_y[idx];
          f = (p-(a*h*h*h+b*h*h*h+c*h+d))*(3*a*h*h+2*b*h+c);
          df = -15*a*a*h*h*h*h -20*a*b*h*h*h  -(12*a*c+6*b*b)*h*h  +6*(p*a-b*c-a*d)*h   + (2*p*b-c*c-2*b*d);
        }


      return std::pair<double,double>(f,df);
    }

    double interpolation::residual_closest_point(double x, double p) const
    {
      WBAssert(m_x.size() != 0, "Internal error: The size of m_x is zero.");
      size_t n = m_x.size();
      std::vector<double>::const_iterator it;
      it = std::lower_bound(m_x.begin(),m_x.end(),x);
      int idx = std::max( int(it-m_x.begin())-1, 0);
      if (x<m_x[0])
        {
          // extrapolation to the left
          //interpol = ((m_b[0])*h + m_c[0])*h + m_y[0];
          double h = x-m_x[0];
          double a = m_a[0];
          double b = m_b[0];
          double c = m_c[0];
          double d = m_y[0];
          return (p-(b*h*h*h+c*h+d))*(2*b*h+c);
        }
      else if (x>m_x[n-1])
        {
          // extrapolation to the right
          //interpol = ((m_b[n-1])*h + m_c[n-1])*h + m_y[n-1];
          double h = x-m_x[n-1];
          double a = m_a[n-1];
          double b = m_b[n-1];
          double c = m_c[n-1];
          double d = m_y[n-1];
          return (p-(b*h*h*h+c*h+d))*(2*b*h+c);
        }
      else
        {
          // interpolation
          //interpol = ((m_a[idx]*h + m_b[idx])*h + m_c[idx])*h + m_y[idx];
          double h = x-m_x[idx];
          double a = m_a[idx];
          double b = m_b[idx];
          double c = m_c[idx];
          double d = m_y[idx];
          return (p-(a*h*h*h+b*h*h*h+c*h+d))*(3*a*h*h+2*b*h+c);
        }
    }


    double wrap_angle(const double angle)
    {
      return angle - 360.0*std::floor(angle/360.0);
    }

    std::array<double,3>
    euler_angles_from_rotation_matrix(const std::array<std::array<double,3>,3> &rotation_matrix)
    {
      const double rad_to_degree = 180.0/const_pi;
      std::array<double,3> euler_angles;
      //const double s2 = std::sqrt(rotation_matrix[2][1] * rotation_matrix[2][1] + rotation_matrix[2][0] * rotation_matrix[2][0]);
      std::ostringstream os;
      for (size_t i = 0; i < 3; i++)
        for (size_t j = 0; j < 3; j++)
          WBAssert(std::fabs(rotation_matrix[i][j]) <= 1.0,
                   "rotation_matrix[" + std::to_string(i) + "][" + std::to_string(j) +
                   "] is larger than one: " + std::to_string(rotation_matrix[i][j]) + ". rotation_matrix = \n"
                   + std::to_string(rotation_matrix[0][0]) + " " + std::to_string(rotation_matrix[0][1]) + " " + std::to_string(rotation_matrix[0][2]) + "\n"
                   + std::to_string(rotation_matrix[1][0]) + " " + std::to_string(rotation_matrix[1][1]) + " " + std::to_string(rotation_matrix[1][2]) + "\n"
                   + std::to_string(rotation_matrix[2][0]) + " " + std::to_string(rotation_matrix[2][1]) + " " + std::to_string(rotation_matrix[2][2]));


      const double theta = std::acos(rotation_matrix[2][2]);
      const double phi1  = std::atan2(rotation_matrix[2][0]/-sin(theta),rotation_matrix[2][1]/-sin(theta));
      const double phi2  = std::atan2(rotation_matrix[0][2]/-sin(theta),rotation_matrix[1][2]/sin(theta));

      euler_angles[0] = wrap_angle(phi1 * rad_to_degree);
      euler_angles[1] = wrap_angle(theta * rad_to_degree);
      euler_angles[2] = wrap_angle(phi2 * rad_to_degree);

      return euler_angles;
    }

    std::array<std::array<double,3>,3>
    euler_angles_to_rotation_matrix(double phi1_d, double theta_d, double phi2_d)
    {

      const double degree_to_rad = const_pi/180.0;
      const double phi1 = phi1_d * degree_to_rad;
      const double theta = theta_d * degree_to_rad;
      const double phi2 = phi2_d * degree_to_rad;
      std::array<std::array<double,3>,3> rot_matrix;


      rot_matrix[0][0] = cos(phi2)*cos(phi1) - cos(theta)*sin(phi1)*sin(phi2);
      rot_matrix[0][1] = -cos(phi2)*sin(phi1) - cos(theta)*cos(phi1)*sin(phi2);
      rot_matrix[0][2] = -sin(phi2)*sin(theta);

      rot_matrix[1][0] = sin(phi2)*cos(phi1) + cos(theta)*sin(phi1)*cos(phi2);
      rot_matrix[1][1] = -sin(phi2)*sin(phi1) + cos(theta)*cos(phi1)*cos(phi2);
      rot_matrix[1][2] = cos(phi2)*sin(theta);

      rot_matrix[2][0] = -sin(theta)*sin(phi1);
      rot_matrix[2][1] = -sin(theta)*cos(phi1);
      rot_matrix[2][2] = cos(theta);
      return rot_matrix;
    }

    template const std::array<double,2> convert_point_to_array<2>(const Point<2> &point_);
    template const std::array<double,3> convert_point_to_array<3>(const Point<3> &point_);
  }
}



