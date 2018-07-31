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
    distance_point_from_curved_planes(const Point<3> &point,
                                      const std::vector<Point<2> > &point_list,
                                      const std::vector<double> &global_x_list,
                                      const std::vector<Point<3> > &reference_point,
                                      const std::vector<std::vector<Point<2> > > &plane_segment_angles,
                                      const std::vector<std::vector<Point<2> > > &plane_segment_lengths,
                                      const double start_radius,
                                      const CoordinateSystems::Interface &coordinate_system,
                                      const bool only_positive)
    {
      double distance = INFINITY;
      double distance_new = NaN::DSNAN;
      double along_plane_distance = INFINITY;
      double along_plane_distance_new  = NaN::DSNAN;

      const CoordinateSystem natural_coordinate_system = coordinate_system.natural_coordinate_system();

      const Point<3> check_point(point);
      // We need this in cartesian coordinates, see explination at the creation
      // of check_point_pushed.
      const Point<3> down = natural_coordinate_system == cartesian
                            ?
                            Point<3>(0,0,-1,cartesian)
                            :
                            coordinate_system.natural_to_cartesian_coordinates(point * -1);

      double fraction = 0;
      unsigned int section = 0;
      unsigned int segment = 0;

      //const double start_radius = geometry_model->maximal_depth()-start_depth;
      // todo: the point variable contains the radius, so adding the depth to that should give the max depth

      // loop over all the planes to find out which one is closest to the point.
      for (unsigned int i_plane=0; i_plane<point_list.size()-1; ++i_plane)
        {
          /**
           * find what section of planes we are in. A section the original point distribution, and consist of planes
           * made through the above cut up/refinement. Each 'plane' can consist of several segments into the depth.
           * The global_x_list has to do with splines. Every 'original' point is an integer, all the added points are
           * in between those integers. Plane and section seem interchangeable, might merge names in the future.
           */
          double d_fraction = global_x_list[i_plane];
          const unsigned int i_section = (unsigned int)(std::floor(d_fraction));
          d_fraction = d_fraction - i_section;

          // The last section should use the previous angles
          // if there is only one segment, use that for both the current and next segment.
          const unsigned int current_section =  plane_segment_lengths.size() == 1
                                                ?
                                                0
                                                :
                                                (i_section != plane_segment_lengths.size()-1 ? i_section : i_section-1);

          const unsigned int next_section = plane_segment_lengths.size() == 1
                                            ?
                                            0
                                            :
                                            (i_section != plane_segment_lengths.size()-1 ? i_section+1 : i_section);

          // The order of a Cartesian coordinate is x,y,z and the order of a spherical coordinate it radius, long, lat (in rad).
          const Point<3> P1(natural_coordinate_system == cartesian ? point_list[i_plane][0] : start_radius,
                            natural_coordinate_system == cartesian ? point_list[i_plane][1] : point_list[i_plane][0],
                            natural_coordinate_system == cartesian ? start_radius : point_list[i_plane][1]);

          const Point<3> P2(natural_coordinate_system == cartesian ? point_list[i_plane+1][0] : start_radius,
                            natural_coordinate_system == cartesian ? point_list[i_plane+1][1] : point_list[i_plane+1][0],
                            natural_coordinate_system == cartesian ? start_radius : point_list[i_plane+1][1]);

          const Point<3> reference_point_cart(natural_coordinate_system == cartesian ? reference_point[current_section][0] : start_radius,
                                              natural_coordinate_system == cartesian ? reference_point[current_section][1] : reference_point[current_section][0],
                                              natural_coordinate_system == cartesian ? start_radius : reference_point[current_section][1]);

          // We first need to determine which way the plane is pointing to.
          // To this end, we use a reference point given by the user. The
          // plane goes down in the direction of the reference point. We
          // adjust the angle for this.


          // Turn it into x,y,z space so that meaningful distances can be
          // computed (in meters). Even in spherical coordinates, we do not
          // care about the distance along a surface since material and
          // heat can move in all directions, analogous to that for
          // example all computations in the ASPECT geodynamics FEM are
          // computed in Cartesian coordinates, even for a sphere. This means
          // that a given thickness will always stay the same no matter the
          // depth. This seems the logical choice to me since otherwise mass
          // of the fault or slab would be lost in the depth, when
          // compressiblity is ignored. If one want to make sure the thickness
          // is consistent with compressibility, that should be done by the
          // user, who also should make sure that the density is increased
          // accordingly in their application. It might be possible to in the
          // future automatically adjust the thickness based on the
          // compressiblity. One important thing is that the gravity vector in
          // spherical coordinates is constant, but it isn't when converted to
          // cartesian coordinates.
          const Point<3> check_point_pushed = coordinate_system.natural_to_cartesian_coordinates(check_point);
          Point<3> P1_pushed          = coordinate_system.natural_to_cartesian_coordinates(P1);
          Point<3> P2_pushed          = coordinate_system.natural_to_cartesian_coordinates(P2);

          // compute on what side of the line between p1 and p2 the reference
          // point is located. This is done in the natural surface coordinates.
          // This means that in spherical coordinates we check this for
          // longitude and latitude. This should be the most correct way of
          // computing this.
          const double reference_on_side_of_line =   (point_list[i_plane+1][0] - point_list[i_plane][0]) * (reference_point[current_section][1] - point_list[i_plane][1])
                                                     - (point_list[i_plane+1][1] - point_list[i_plane][1]) * (reference_point[current_section][0] - point_list[i_plane][0]);
          //const Point<3> reference_point_cart_pushed = coordinate_system.natural_to_cartesian_coordinates(reference_point_cart);

          double total_length = 0;
          for (unsigned int i_segment = 0; i_segment < plane_segment_lengths[current_section].size(); i_segment++)
            {
              // We are now computing the distance of the check point to this segment.
              // do linear interpolation for the angle and length
              const double interpolated_angle_0 = plane_segment_angles[current_section][i_segment](0) +
                                                  d_fraction * (plane_segment_angles[next_section][i_segment](0) - plane_segment_angles[current_section][i_segment](0));
              const double interpolated_angle_1 = plane_segment_angles[current_section][i_segment](1) +
                                                  d_fraction * (plane_segment_angles[next_section][i_segment](1) - plane_segment_angles[current_section][i_segment](1));
              double plane_segment_length       = plane_segment_lengths[current_section][i_segment](0) +
                                                  d_fraction*(plane_segment_lengths[next_section][i_segment](0) - plane_segment_lengths[current_section][i_segment](0));
              double angle_corrected            = 90-interpolated_angle_0;

              const Point<3> P1P2 = P2_pushed - P1_pushed;
              const Point<3> P1CP = check_point_pushed - P1_pushed;



              // For spherical P1 and P1P2 are both vectors coming from the center of the
              // Earth, and the cross product therefore gives a vector tangential to the
              // surface. For a box type model, the surface tangential we need
              Point<3> surface_tangential = Utilities::cross_product(gravity_vector, P1P2);
              WBAssert(surface_tangential.norm() != 0,
                       "surface_tangential.norm() is zero, which should not happen. Surface_tangential = "
                       << surface_tangential[0] + "," + surface_tangential[1] << "," << surface_tangential[2]);
              //const double reference_on_side_of_line = ((surface_tangential)/(surface_tangential).norm()) * (reference_point_cart_pushed - P1_pushed);

              // if reference point (vector) is not on the same (pointing to the same) side as the normal. Flip the normal
              if (reference_on_side_of_line < 0)
                {
                  angle_corrected = -(90-interpolated_angle_0);
                }
              else
                {
                  surface_tangential = dealii::cross_product_3d(P1P2,gravity_vector);//spherical_coords ?
                }

              // find the normal to the defined segment plane
              // We rotate the normal to the line P1 - P2 to get the normal of this plate
              // The code for the rotation matrix and multiplying the matrix is based on http://www.programming-techniques.com/2012/03/3d-rotation-algorithm-about-arbitrary.html
              // Find the rotation matrix.
              const double inputMatrix[4] = {surface_tangential[0], surface_tangential[1], surface_tangential[2], 1};
              double outputMatrix[4] = { };

            }
        }
    }

    template const std::array<double,2> convert_point_to_array<2>(const Point<2> &point_);
    template const std::array<double,3> convert_point_to_array<3>(const Point<3> &point_);
  }
}



