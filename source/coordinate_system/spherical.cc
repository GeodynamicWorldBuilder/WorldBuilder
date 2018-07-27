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

#include <world_builder/assert.h>
#include <world_builder/coordinate_systems/spherical.h>

namespace WorldBuilder
{
  namespace CoordinateSystems
  {
    Spherical::Spherical()
    {}

    Spherical::~Spherical()
    {}

    void
    Spherical::decare_entries()
    {}


    CoordinateSystem
    Spherical::natural_coordinate_system() const
    {
      return CoordinateSystem::spherical;
    }


    std::array<double,3>
    Spherical::cartesian_to_natural_coordinates(const std::array<double,3> &position) const
    {
      return Utilities::cartesian_to_spherical_coordinates(Point<3>(position,cartesian));
    }


    std::array<double,3>
    Spherical::natural_to_cartesian_coordinates(const std::array<double,3> &position) const
    {
      return Utilities::spherical_to_cartesian_coordinates(position).get_array();
    }

    double
    Spherical::distance_between_points_at_same_depth(const Point<3> &point_1, const Point<3> &point_2) const
    {

      WBAssert(point_1.get_coordinate_system() == spherical,
               "Can not convert non spherical points through the spherical coordinate system.");
      WBAssert(point_2.get_coordinate_system() == spherical,
               "Can not convert non spherical points through the spherical coordinate system.");
      const double radius = point_1[0];
      WBAssert((radius - point_2[0]) < std::numeric_limits<double>::epsilon() * std::max(1.0,radius), "The radius of point 1 is not the same as the radius of point 2.");
      const double lat_1 = point_1[1];
      const double lat_2 = point_2[1];
      const double long_1 = point_1[2];
      const double long_2 = point_2[2];
      const double long_diff = std::fabs(long_1 - long_2);
      const double sin_lat_1 = std::sin(lat_1);
      const double sin_lat_2 = std::sin(lat_2);
      const double cos_lat_1 = std::cos(lat_1);
      const double cos_lat_2 = std::cos(lat_2);
      const double sin_long_diff = std::sin(long_diff);
      const double cos_long_diff = std::cos(long_diff);

      const double top = std::sqrt((cos_lat_2 * sin_long_diff) * (cos_lat_2 * sin_long_diff) +
                                   (cos_lat_1 * sin_lat_2 - sin_lat_1 * cos_lat_2 * cos_long_diff) * (cos_lat_1 * sin_lat_2 - sin_lat_1 * cos_lat_2 * cos_long_diff));
      const double bottom = sin_lat_1 * sin_lat_2 + cos_lat_1 * cos_lat_2 * cos_long_diff;

      return radius * std::atan2(top, bottom);
    }
  }
}

