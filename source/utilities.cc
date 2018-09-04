/*
  Copyright (C) 2018 by the authors of the World Builder code.

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

#include <boost/lexical_cast.hpp>

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
          other_point[0] += point[0] < 0 ? 2.0 * M_PI : -2.0 * M_PI;

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
      int pointNo = point_list.size();
      int    wn = 0;    // the  winding number counter
      int   j=pointNo-1;

      // loop through all edges of the polygon
      for (int i=0; i<pointNo; i++)
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
                  else if ( is_left == 0)
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
                  else if ( is_left == 0)
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

      const unsigned int n_poly_points = point_list.size();
      WBAssertThrow(n_poly_points >= 3, "Not enough polygon points were specified.");

      // Initialize a vector of distances for each point of the polygon with a very large distance
      std::vector<double> distances(n_poly_points, 1e23);

      // Create another polygon but with all points shifted 1 position to the right
      std::vector<Point<2> > shifted_point_list(n_poly_points, Point<2>(point.get_coordinate_system()));
      shifted_point_list[0] = point_list[n_poly_points-1];

      for (unsigned int i = 0; i < n_poly_points-1; ++i)
        shifted_point_list[i+1] = point_list[i];

      for (unsigned int i = 0; i < n_poly_points; ++i)
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
      if (scoord[1] < 0.0)
        scoord[1] += 2.0*M_PI; // correct phi to [0,2*pi]

      if (scoord[0] > std::numeric_limits<double>::min())
        scoord[2] = 0.5 * M_PI - std::acos(position[2]/scoord[0]);
      else
        scoord[2] = 0.0;

      return scoord;
    }

    Point<3>
    spherical_to_cartesian_coordinates(const std::array<double,3> &scoord)
    {
      Point<3> ccoord(cartesian);

      ccoord[0] = scoord[0] * std::sin(0.5 * M_PI - scoord[2]) * std::cos(scoord[1]); // X
      ccoord[1] = scoord[0] * std::sin(0.5 * M_PI - scoord[2]) * std::sin(scoord[1]); // Y
      ccoord[2] = scoord[0] * std::cos(0.5 * M_PI - scoord[2]); // Z


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
      for (unsigned int i = 0; i < dim; ++i)
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

      double d = 0;
      try
        {
          d =  boost::lexical_cast<double>(s);
        }
      catch (const boost::bad_lexical_cast &e)
        {
          WBAssertThrow(false, "Conversion of \"" << string << "\" to double failed (bad cast): " << e.what() << std::endl);
        }

      return d;
    }

    double
    string_to_int(const std::string &string)
    {
      // trim whitespace on either side of the text if necessary
      std::string s = string;
      while ((s.size() > 0) && (s[0] == ' '))
        s.erase(s.begin());
      while ((s.size() > 0) && (s[s.size() - 1] == ' '))
        s.erase(s.end() - 1);

      double d = 0;
      try
        {
          d =  boost::lexical_cast<int>(s);
        }
      catch (const boost::bad_lexical_cast &e)
        {
          WBAssertThrow(false, "Conversion of \"" << string << "\" to int failed (bad cast): " << e.what() << std::endl);
        }

      return d;
    }


    double
    string_to_unsigned_int(const std::string &string)
    {
      // trim whitespace on either side of the text if necessary
      std::string s = string;
      while ((s.size() > 0) && (s[0] == ' '))
        s.erase(s.begin());
      while ((s.size() > 0) && (s[s.size() - 1] == ' '))
        s.erase(s.end() - 1);

      double d = 0;
      try
        {
          d =  boost::lexical_cast<unsigned int>(s);
        }
      catch (const boost::bad_lexical_cast &e)
        {
          WBAssertThrow(false, "Conversion of \"" << string << "\" to unsigned int failed (bad cast): " << e.what() << std::endl);
        }

      return d;
    }

    boost::optional<std::string>
    get_from_ptree(const ptree &tree,
                   const std::string &path,
                   const std::string &key,
                   const bool required,
                   const std::string &path_separator)
    {
      boost::optional<std::string> value  = tree.get_optional<std::string> (key);
      WBAssertThrow ((value && required == true) || required == false, "Entry undeclared: " + path + path_separator + key +
                     ". Tree: " << std::endl << print_tree(tree,0).str() << std::endl);
      return value;
    }

    boost::optional<std::string>
    get_from_ptree_abs(const ptree &tree,
                       const std::string &path,
                       const std::string &key,
                       const bool required,
                       const std::string &path_separator)
    {
      std::string use_path = path == "" ? key : path + path_separator + key;
      boost::optional<std::string> value  = tree.get_optional<std::string> (use_path);
      WBAssertThrow ((value && required == true) || required == false, "Entry undeclared: " + use_path +
                     ". Tree: " << std::endl << print_tree(tree,0).str() << std::endl);
      return value;
    }

    /*std::string
    escape_string(std::string &original)
    {
      // first escape the escape character. Lets say we start with  "abc &amp;[ ]"
      //std::replace( s.begin(), s.end(), '&', '&amp');
      // This now became "abc &ampamp[ ]". Escape the other characters:
      //std::replace( s.begin(), s.end(), ' ', '&spa');
      // This now became "abc&spa&ampamp[&spa]"
      //std::replace( s.begin(), s.end(), '[', 'lsqb');
    }*/

    std::string indent(int level)
    {
      std::string s;
      for (int i=0; i<level; i++) s += "  ";
      return s;
    }

    std::stringstream print_tree (const ptree &pt, int level)
    {
      std::stringstream ss;
      if (pt.empty())
        {
          ss << "\""<< pt.data()<< "\"";
        }

      else
        {
          if (level) ss << std::endl;

          ss << indent(level) << "{" << std::endl;

          for (ptree::const_iterator pos = pt.begin(); pos != pt.end();)
            {
              ss << indent(level+1) << "\"" << pos->first << "\": ";

              ss << print_tree(pos->second, level + 1).str();
              ++pos;
              if (pos != pt.end())
                {
                  ss << ",";
                }
              ss << std::endl;
            }
          ss << indent(level) << " }";
        }

      return ss;
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
    distance_point_from_curved_planes(const Point<3> &check_point,
                                      const Point<2> &reference_point,
                                      const std::vector<Point<2> > &point_list,
                                      const std::vector<std::vector<double> > &plane_segment_lengths,
                                      const std::vector<std::vector<Point<2> > > &plane_segment_angles,
                                      const double start_radius,
                                      const std::unique_ptr<CoordinateSystems::Interface> &coordinate_system,
                                      const bool only_positive)
    {
      // TODO: Assert that point_list, plane_segment_angles and plane_segment_lenghts have the same size.
      double distance = INFINITY;
      double new_distance = NaN::DSNAN;
      double along_plane_distance = INFINITY;
      double new_along_plane_distance  = NaN::DSNAN;

      const CoordinateSystem natural_coordinate_system = coordinate_system->natural_coordinate_system();
      const bool bool_cartesian = natural_coordinate_system == cartesian;

      //const Point<3> check_point(point);
      const double check_point_depth = natural_coordinate_system == cartesian ? start_radius - check_point[2] : start_radius - check_point[0];

      const Point<3> check_point_surface(bool_cartesian ? check_point[0]     : start_radius,
                                         bool_cartesian ? check_point[1]     : check_point[1],
                                         bool_cartesian ? start_radius : check_point[2],
                                         natural_coordinate_system);

      //std::cout << "check_point_surface = " << check_point_surface[0] << ":" << check_point_surface[1] << ":" << check_point_surface[2] << std::endl;
      // The section which is checked.
      double section = 0;

      // The 'horizontal' fraction between the points at the surface.
      double section_fraction = 0;

      // What segment the point on the line is in.
      unsigned int segment = 0;

      // The 'vertical' fraction, indicates how far in the current segment the
      // point on the line is.
      double segment_fraction = 0;

      const DepthMethod depth_method = coordinate_system->depth_method();
      WBAssertThrow(depth_method == DepthMethod::none
                    || depth_method == DepthMethod::angle_at_starting_point_with_surface,
                    "Only the depth methods none and angle_at_starting_point_with_surface are "
                    "currently implemented");

      // loop over all the planes to find out which one is closest to the point.
      for (unsigned int i_section=0; i_section < point_list.size()-1; ++i_section)
        {
          const double current_section = i_section;
          const double next_section = i_section+1;
          // see on what side the line P1P2 reference point is. This is based on the determinant
          const double reference_on_side_of_line = (point_list[next_section][0] - point_list[current_section][0])
                                                   * (reference_point[1] - point_list[current_section][1])
                                                   - (point_list[next_section][1] - point_list[current_section][1])
                                                   * (reference_point[0] - point_list[current_section][0])
                                                   < 0 ? 1 : -1;

          //std::cout << "reference_on_side_of_line = " << reference_on_side_of_line << std::endl;

          // The order of a Cartesian coordinate is x,y,z and the order of a spherical coordinate it radius, long, lat (in rad).
          const Point<3> P1(bool_cartesian ? point_list[current_section][0] : start_radius,
                            bool_cartesian ? point_list[current_section][1] : point_list[current_section][0],
                            bool_cartesian ? start_radius : point_list[current_section][1],
                            natural_coordinate_system);

          const Point<3> P2(bool_cartesian ? point_list[next_section][0] : start_radius,
                            bool_cartesian ? point_list[next_section][1] : point_list[next_section][0],
                            bool_cartesian ? start_radius : point_list[next_section][1],
                            natural_coordinate_system);

          const Point<3> reference_point_cart(bool_cartesian ? reference_point[0] : start_radius,
                                              bool_cartesian ? reference_point[1] : reference_point[0],
                                              bool_cartesian ? start_radius : reference_point[1],
                                              natural_coordinate_system);


          //std::cout << "P1 = " << P1[0] << ":" << P1[1] << ":" << P1[2] << std::endl;

          //std::cout << "P2 = " << P2[0] << ":" << P2[1] << ":" << P2[2] << std::endl;

          //std::cout << "reference_point_cart = " << reference_point_cart[0] << ":" << reference_point_cart[1] << ":" << reference_point_cart[2] << std::endl;

          const Point<3> P1P2 = P2 - P1;
          const Point<3> P1PC = check_point_surface - P1;

          //std::cout << "P1P2 = " << P1P2[0] << ":" << P1P2[1] << ":" << P1P2[2] << std::endl;
          //std::cout << "P1PC = " << P1PC[0] << ":" << P1PC[1] << ":" << P1PC[2] << std::endl;

          // Compute the closest point on the line P1 to P2 from the check
          // point at the surface. We do this in natural coordinates on
          // purpose, because in spherical coordinates it is more accurate.
          // Todo: used to be 2d result. Now testing whether 3d is fine. Check mathematically later.
          Point<3> closest_point_on_line = P1 + ((P1PC * P1P2) / (P1P2 * P1P2)) * P1P2;
          closest_point_on_line[bool_cartesian ? 2 : 0] = start_radius;
          Point<3> closest_point_on_line_bottom = closest_point_on_line;
          closest_point_on_line_bottom[bool_cartesian ? 2 : 0] = 0;


          //std::cout << "closest_point_on_line = " << closest_point_on_line[0] << ":" << closest_point_on_line[1] << ":" << closest_point_on_line[2] << std::endl;
          //std::cout << "closest_point_on_line_bottom = " << closest_point_on_line_bottom[0] << ":" << closest_point_on_line_bottom[1] << ":" << closest_point_on_line_bottom[2] << std::endl;
          //std::cout << "P1PC = " << P1PC[0] << ":" << P1PC[1] << ":" << P1PC[2] << std::endl;

          // compute what fraction of the distance between P1 and P2 the
          // closest point lies.
          const Point<3> P1CPL = closest_point_on_line - P1;
          const double fraction_CPL_P1P2 = (P1CPL * P1P2 <= 0 ? -1.0 : 1.0) * (1 - (P1P2.norm() - P1CPL.norm()) / P1P2.norm());


          //std::cout << "P1CPL = " << P1CPL[0] << ":" << P1CPL[1] << ":" << P1CPL[2] << std::endl;
          //std::cout << "fraction_CPL_P1P2 = " << fraction_CPL_P1P2 << std::endl;

          // If the point on the line does not lay between point P1 and P2
          // then ignore it. Otherwise continue.
          if (fraction_CPL_P1P2 >= 0 && fraction_CPL_P1P2 <= 1.0)
            {

              // Now that we have both the check point and the
              // closest_point_on_line, we need to push them to cartesian.
              const Point<3> check_point_cartesian(coordinate_system->natural_to_cartesian_coordinates(check_point.get_array()),cartesian);
              const Point<3> check_point_surface_cartesian(coordinate_system->natural_to_cartesian_coordinates(check_point_surface.get_array()),cartesian);
              const Point<3> closest_point_on_line_cartesian(coordinate_system->natural_to_cartesian_coordinates(closest_point_on_line.get_array()),cartesian);
              const Point<3> closest_point_on_line_bottom_cartesian(coordinate_system->natural_to_cartesian_coordinates(closest_point_on_line_bottom.get_array()),cartesian);


              //std::cout << "check_point_cartesian = " << check_point_cartesian[0] << ":" << check_point_cartesian[1] << ":" << check_point_cartesian[2] << std::endl;
              //std::cout << "check_point_surface_cartesian = " << check_point_surface_cartesian[0] << ":" << check_point_surface_cartesian[1] << ":" << check_point_surface_cartesian[2] << std::endl;
              //std::cout << "closest_point_on_line_cartesian = " << closest_point_on_line_cartesian[0] << ":" << closest_point_on_line_cartesian[1] << ":" << closest_point_on_line_cartesian[2] << std::endl;

              // The y-axis is from the bottom/center to the closest_point_on_line,
              // the x-axis is 90 degrees rotated from that, so we rotate around
              // the line P1P2.
              // Todo: Assert that the norm of the axis are not equal to zero.
              Point<3> y_axis = closest_point_on_line_cartesian - closest_point_on_line_bottom_cartesian;
              y_axis = y_axis / y_axis.norm();
              const Point<3> normal_to_plane = P1P2 / P1P2.norm();



              // shorthand notation for computing the x_axis
              double vx = y_axis[0];
              double vy = y_axis[1];
              double vz = y_axis[2];
              double ux = normal_to_plane[0];
              double uy = normal_to_plane[1];
              double uz = normal_to_plane[2];

              Point<3> x_axis(uy*uy*vx + ux*uy*vy - uz*vy + ux*uz*vz + uy*vz,
                              uy*ux*vx + uz*vx + uy*uy*vy + uy*uz*vz - ux*vz,
                              uz*ux*vx - uy*vx + uz*uy*vy + uz*vy + uz*uz*vz,
                              cartesian);

              //std::cout << "x_axis = " << x_axis[0] << ":" << x_axis[1] << ":" << x_axis[2] << std::endl;
              x_axis = x_axis *(reference_on_side_of_line / x_axis.norm());


              //std::cout << "x_axis = " << x_axis[0] << ":" << x_axis[1] << ":" << x_axis[2] << std::endl;
              //std::cout << "y_axis = " << y_axis[0] << ":" << y_axis[1] << ":" << y_axis[2] << std::endl;
              //std::cout << "z_axis = " << normal_to_plane[0] << ":" << normal_to_plane[1] << ":" << normal_to_plane[2] << std::endl;

              Point<2> check_point_2d(x_axis * (check_point - closest_point_on_line_bottom),
                                      y_axis * (check_point - closest_point_on_line_bottom),
                                      cartesian);

              //std::cout << "======================================================" << std::endl;
              //std::cout << "check_point_2d = " << check_point_2d[0] << ":" << check_point_2d[1]  << std::endl;

              // Radius in this case means height from bottom of the model.
              const double check_point_radius = start_radius - check_point_depth;


              //std::cout << "check_point_radius = " << check_point_radius << std::endl;

              Point<2> begin_segment(x_axis * (closest_point_on_line - closest_point_on_line_bottom),
                                     y_axis * (closest_point_on_line - closest_point_on_line_bottom),
                                     cartesian);


              //std::cout << "     begin_segment = " << begin_segment[0] << ":" << begin_segment[1]  << std::endl;

              Point<2> end_segment = begin_segment;
              double total_length = 0;
              for (unsigned int i_segment = 0; i_segment < plane_segment_lengths[current_section].size(); i_segment++)
                {
                  const double current_segment = i_segment;
                  //const double next_segment = i_segment+1;

                  Point<2> begin_segment = end_segment;

                  // This interpolates different properties between P1 and P2 (the
                  // points of the plane at the surface)
                  const double degree_90_to_rad = 0.5 * M_PI;
                  const double interpolated_angle_top    = plane_segment_angles[current_section][current_segment][0]
                                                           + fraction_CPL_P1P2 * (plane_segment_angles[next_section][current_segment][0]
                                                                                  - plane_segment_angles[current_section][current_segment][0]);

                  const double interpolated_angle_bottom = plane_segment_angles[current_section][current_segment][1]
                                                           + fraction_CPL_P1P2 * (plane_segment_angles[next_section][current_segment][1]
                                                                                  - plane_segment_angles[current_section][current_segment][1]);


                  double interpolated_segment_length     = plane_segment_lengths[current_section][current_segment]
                                                           + fraction_CPL_P1P2 * (plane_segment_lengths[next_section][current_segment]
                                                                                  - plane_segment_lengths[current_section][current_segment]);


                  //std::cout << "interpolated_angle_top = " << interpolated_angle_top << std::endl;
                  //std::cout << "interpolated_angle_bottom = " << interpolated_angle_bottom << std::endl;
                  //std::cout << "interpolated_segment_length = " << interpolated_segment_length << std::endl;

                  // Todo: check wheter this is still needed with the new method.
                  //double angle_corrected = 90 - interpolated_angle_top;

                  // We want to know where the end point of this segment is (and
                  // the start of the next segment). There are two cases which we
                  // will deal with separately. The first one is if the angle is
                  // constant. The second one is if the angle changes.
                  const double difference_in_angle_along_segment = interpolated_angle_top - interpolated_angle_bottom;
                  //std::cout << "flag 1, difference_in_angle_along_segment = " << difference_in_angle_along_segment << ", interpolated_angle_bottom = " << interpolated_angle_bottom << ", interpolated_angle_top = " << interpolated_angle_top << std::endl;
                  if (std::fabs(difference_in_angle_along_segment) < 1e-8)
                    {
                      //std::cout << "flag 2" << std::endl;
                      // The angle is constant. It is easy find find the end of
                      // this segment and the distance.
                      //std::cout << "end_segment before = " << end_segment[0] << ":" << end_segment[1] << ", sin = " << interpolated_segment_length * std::sin(interpolated_angle_top) << ", cos = " << interpolated_segment_length * std::cos(interpolated_angle_top) << std::endl;
                      end_segment[0] += interpolated_segment_length * std::sin(degree_90_to_rad - interpolated_angle_top);
                      end_segment[1] -= interpolated_segment_length * std::cos(degree_90_to_rad - interpolated_angle_top);

                      Point<2> begin_end_segment = end_segment - begin_segment;
                      Point<2> normal_2d_plane(-begin_end_segment[0],begin_end_segment[1], cartesian);
                      normal_2d_plane /= normal_2d_plane.norm();

                      //std::cout << "end_segment after = " << end_segment[0] << ":" << end_segment[1]  << std::endl;

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
                  else
                    {
                      // The angle is not constant. This means that we need to
                      // define a circle. First find the center of the circle.
                	  const double radius_angle_circle = std::fabs(interpolated_segment_length/difference_in_angle_along_segment);

                	  const double cos_angle_top = std::cos(interpolated_angle_top);

                	  //std::cout << "radius_angle_circle = " << radius_angle_circle << std::endl;
                	  //std::cout << "cos_angle_top = " << cos_angle_top << std::endl;
                	  //std::cout << "interpolated_angle_top = " << interpolated_angle_top << std::endl;

                	  Point<2> center_circle(cartesian);
                	  if(std::fabs(interpolated_angle_top - 0.5 * M_PI) < 1e-8)
                	  {
                		  // if interpolated_angle_top is 90 degrees, the tan function
                		  // is undefined (1/0). What we really want in this case is
                		  // set the center to the correct location which is x = the x
                		  //begin point + radius and y = the y begin point.
                		  center_circle[0] = difference_in_angle_along_segment > 0 ? begin_segment[0] + radius_angle_circle : begin_segment[0] - radius_angle_circle;
                		  center_circle[1] = begin_segment[1];
                	  }
                	  else if(std::fabs(interpolated_angle_top - 1.5 * M_PI) < 1e-8)
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

                    	  //std::cout << "interpolated_segment_length = " << interpolated_segment_length << std::endl;
                    	  //std::cout << "difference_in_angle_along_segment = " << difference_in_angle_along_segment << ", " << difference_in_angle_along_segment * 180/M_PI << std::endl;

                    	  //std::cout << "tan_angle_top = " << tan_angle_top << std::endl;
                    	  //std::cout << "cos_angle_top = " << cos_angle_top << std::endl;

                    	  const double center_circle_y = difference_in_angle_along_segment < 0 ?
                    			                         begin_segment[1] - radius_angle_circle * cos_angle_top
                    			                         : begin_segment[1] + radius_angle_circle * cos_angle_top;


                    	  // to prevent round off errors becomming dominant, we check
                    	  // whether center_circle_y - begin_segment[1] should be zero.
                    	  // TODO: improve this to some kind of relative difference.
                    	  const double CCYBS = center_circle_y - begin_segment[1];//std::fabs(center_circle_y - begin_segment[1]) > 1e-14 ? center_circle_y - begin_segment[1] : 0.0;

                    	  //std::cout << "begin_segment = " << begin_segment[0] << ", " << begin_segment[1] << std::endl;
                    	  //std::cout << "center_circle_y = " << center_circle_y  << std::endl;
                    	  //std::cout << "CCYBS = " << CCYBS  << std::endl;
                    	  //std::cout << "tan_angle_top * (CCYBS) = " << tan_angle_top * (CCYBS)  << std::endl;


                    	  center_circle[0] = begin_segment[0] + tan_angle_top * (CCYBS);//difference_in_angle_along_segment > 0 ? begin_segment[0] - tan_angle_top * (CCYBS) : begin_segment[0] + tan_angle_top * (CCYBS);
                    	  center_circle[1] = center_circle_y;
                	  }

                	  //std::cout << "     begin_segment = " << begin_segment[0] << ", " << begin_segment[1] << std::endl;

                	  WBAssert(std::fabs((begin_segment-center_circle).norm() - std::fabs(radius_angle_circle))
                	  < 1e-8 * std::fabs((begin_segment-center_circle).norm() + std::fabs(radius_angle_circle)),
                			  "Internal error: The center of the circle is not a radius away from the begin point. " << std::endl
							  << "The center is located at " << center_circle[0] << ":" << center_circle[1] << std::endl
							  << "The begin point is located at " << begin_segment[0] << ":" << begin_segment[1] << std::endl
							  << "The computed radius is " << std::fabs((begin_segment-center_circle).norm())
							  << ", and it should be " << radius_angle_circle << ".");

                	  //std::cout << "     center_circle = " << center_circle[0] << ", " << center_circle[1] << std::endl;

                	  // Now compute the location of the end of the segment by
                	  // rotating P1 around the center_circle
                	  Point<2> BSPC = begin_segment - center_circle;
                	  const double sin_angle_diff = sin(difference_in_angle_along_segment);
                	  const double cos_angle_diff = cos(difference_in_angle_along_segment);
                	  end_segment[0] = cos_angle_diff * BSPC[0] - sin_angle_diff * BSPC[1] + center_circle[0];
                	  end_segment[1] = sin_angle_diff * BSPC[0] + cos_angle_diff * BSPC[1] + center_circle[1];

                	  //std::cout << "     end_segment = " << end_segment[0] << ", " << end_segment[1] << std::endl;


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
                	  const Point<2> check_point_2d_ajusted(check_point_2d[0], check_point_2d[1], cartesian);
                	  const Point<2> CPCR = check_point_2d_ajusted - center_circle;
                	  const double CPCR_norm = CPCR.norm();
                      //std::cout << "check_point_2d_ajusted = " << check_point_2d_ajusted[0] << ":" << check_point_2d_ajusted[1]  << std::endl;
                      //std::cout << "CPCR = " << CPCR[0] << ":" << CPCR[1]  << std::endl;
                     // std::cout << "CPCR_norm = " << CPCR_norm  << std::endl;

                	  const double dot_product = CPCR * Point<2>(0, radius_angle_circle, cartesian);
                	  // If the x of the check point is larger then the x of center
                	  // the circle, the angle is more than 180 degree, but the dot
                	  // product will decrease instead of increase from 180 degrees.
                	  // To fix this we make a special case for this.
                	  // Furthermore, when the check point is at the same location as
                	  // the center of the circle, we count that point as belonging
                	  // to the top of the top segment (0 degree).
                	  //std::cout << "std::acos(dot_product/(CPCR_norm * radius_angle_circle)) = " << std::acos(dot_product/(CPCR_norm * radius_angle_circle))*180/M_PI << std::endl;
                	  //std::cout << "center_circle[0] = " << center_circle[0] << std::endl;
                	  double check_point_angle = CPCR_norm == 0 ? 2.0 * M_PI : (check_point_2d_ajusted[0] <= center_circle[0]
													   ? std::acos(dot_product/(CPCR_norm * radius_angle_circle))
                	                                   : 2.0 * M_PI - std::acos(dot_product/(CPCR.norm() * radius_angle_circle)));
                	  check_point_angle = difference_in_angle_along_segment > 0 ? M_PI - check_point_angle : 2.0 * M_PI - check_point_angle;

                	  //std::cout << "difference_in_angle_along_segment = " << difference_in_angle_along_segment << ", " << difference_in_angle_along_segment *180/M_PI << std::endl;
                	  //std::cout << "check_point_angle = " << check_point_angle << ", " << check_point_angle * 180/M_PI << std::endl;
                	  //std::cout << "interpolated_angle_top = " << interpolated_angle_top << ", " << interpolated_angle_top *180/M_PI << std::endl;
                	  //std::cout << "interpolated_angle_bottom = " << interpolated_angle_bottom << ", " << interpolated_angle_bottom *180/M_PI << std::endl;


                	  if((difference_in_angle_along_segment > 0 && check_point_angle <= interpolated_angle_top && check_point_angle >= interpolated_angle_bottom)
                		 || (difference_in_angle_along_segment < 0 && check_point_angle >= interpolated_angle_top && check_point_angle <= interpolated_angle_bottom))
                	  {
                		  new_distance = (radius_angle_circle - CPCR_norm) * (difference_in_angle_along_segment < 0 ? 1 : -1);
                    	  new_along_plane_distance = (radius_angle_circle * check_point_angle - radius_angle_circle * interpolated_angle_top) * (difference_in_angle_along_segment < 0 ? 1 : -1);
                    	  //std::cout << "new_distance = " << new_distance << std::endl;
                    	  //std::cout << "new_along_plane_distance = " << new_along_plane_distance << std::endl;
                	  }

                    }

                  // Now we need to see whether we need to update the information
                  // based on whether this segment is the closest one to the point
                  // up to now. To do this we first look whether the point falls
                  // within the bound of the segment and if it is actually closer.
                  // TODO: find out whether the fabs() are needed.
                  if (new_along_plane_distance >= 0 &&
                      new_along_plane_distance <= std::fabs(interpolated_segment_length) &&
                      std::fabs(distance) > std::fabs(new_distance))
                    {
                      // There are two specific cases we are concerned with. The
                      // first case is that we want to have both the positive and
                      // negative distances (above and below the line). The second
                      // case is that we only want positive distances.
                      if ((!only_positive) ||
                          (only_positive  && new_distance > 0))
                        {
                          distance = new_distance;
                          along_plane_distance = new_along_plane_distance + total_length;
                          section = current_section;
                          section_fraction = fraction_CPL_P1P2;
                          segment = i_segment;
                          segment_fraction = new_along_plane_distance / interpolated_segment_length;
                        }
                    }

                  // increase the total length for the next segment.
                  total_length += interpolated_segment_length;

                  //std::cout << "==========================================" << std::endl;
                }
            }
        }
      std::map<std::string, double> return_values;
      return_values["distanceFromPlane"] = distance;
      return_values["distanceAlongPlane"] = along_plane_distance;
      return_values["sectionFraction"] = section_fraction;
      return_values["segmentFraction"] = segment_fraction;
      return_values["section"] = section;
      return_values["segment"] = segment;
      return return_values;
    }

    template const std::array<double,2> convert_point_to_array<2>(const Point<2> &point_);
    template const std::array<double,3> convert_point_to_array<3>(const Point<3> &point_);
  }
}



