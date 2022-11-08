/*
Copyright (C) 2018 - 2022 by the authors of the World Builder code.

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

#ifndef WORLD_BUILDER_OBJECTS_CLOSEST_POINT_ON_CURVE_H
#define WORLD_BUILDER_OBJECTS_CLOSEST_POINT_ON_CURVE_H


#include "world_builder/point.h"

using namespace WorldBuilder;

namespace WorldBuilder
{
  namespace Objects
  {


    struct ClosestPointOnCurve
    {
      ClosestPointOnCurve()
        :
        distance(std::numeric_limits<double>::infinity()),
        parametric_fraction(std::numeric_limits<double>::signaling_NaN()),
        interpolation_fraction(std::numeric_limits<double>::signaling_NaN()),
        index(0),
        point(Point<2>(std::numeric_limits<double>::signaling_NaN(),std::numeric_limits<double>::signaling_NaN(),CoordinateSystem::invalid))
      {}

      double distance;
      double parametric_fraction;
      double interpolation_fraction;
      size_t index;
      Point<2> point;
    };
  }
}
#endif