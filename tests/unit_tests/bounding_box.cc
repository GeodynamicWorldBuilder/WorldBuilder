/*
  Copyright (C) 2021 by the authors of the World Builder code.

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

#define CATCH_CONFIG_MAIN


#include "catch2.h"

#include "world_builder/bounding_box.h"

using namespace WorldBuilder;


TEST_CASE("bounding box")
{
  Point<3> p1 ({1,1,1}, CoordinateSystem::cartesian);
  Point<3> p2 ({2,3,4}, CoordinateSystem::cartesian);

  BoundingBox<3> bb ({p1, p2});

  // Check the center function
  CHECK(bb.center().distance(Point<3>(1.5,2,2.5,CoordinateSystem::cartesian)) < 1e-12);

  // Check that the function point_inside works as expected
  CHECK(bb.point_inside(Point<3>(1.5,2,2.5,CoordinateSystem::cartesian)) == true);
  CHECK(bb.point_inside(Point<3>(-1.5,2,2.5,CoordinateSystem::cartesian)) == false);
  CHECK(bb.point_inside(Point<3>(1.5,-2,2.5,CoordinateSystem::cartesian)) == false);
  CHECK(bb.point_inside(Point<3>(1.5,2,-2.5,CoordinateSystem::cartesian)) == false);
}
