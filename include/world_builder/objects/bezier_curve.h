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

#ifndef WORLD_BUILDER_OBJECTS_BEZIER_CURVE_H
#define WORLD_BUILDER_OBJECTS_BEZIER_CURVE_H

#include "world_builder/objects/closest_point_on_curve.h"
#include "world_builder/point.h"
#include <vector>

using namespace WorldBuilder;

namespace WorldBuilder
{
  namespace Objects
  {

    /**
     * @brief Class for circle line/spline, including interpolation on it
     *
     */
    class BezierCurve
    {
      public:
        /**
         * @brief Construct a new Bezier Curve object
         *
         * @param p
         * @param angle_constrains
         */
        BezierCurve() {};

        /**
         * @brief Construct a new Bezier Curve object
         *
         * @param p
         * @param angle_constrains
         */
        BezierCurve(const std::vector<Point<2> > &p, const std::vector<double> &angle_constrains = {});

        /**
         * @brief Finds the closest point on the curve. If the the closest point
         *        doesn't fall on the segment, return a point with x and y being nan.
         *
         * @param p
         * @return ClosestPointOnCurve
         */
        ClosestPointOnCurve closest_point_on_curve_segment(const Point<2> &p) const;


        /**
         * @brief Finds the closest point on the curve.
         *
         * @param p
         * @return ClosestPointOnCurve
         */
        ClosestPointOnCurve closest_point_on_curve(const Point<2> &p) const;

        /**
         * @brief
         *
         * @param i
         * @param x
         * @return Point<2>
         */
        Point<2> operator()(const size_t i, const double x) const;


        /**
         * @brief computes the arc length of a quadratic bezier curve segment
         *
         * @param a This is a point the curve goes through.
         * @param b This is the control point.
         * @param c This is a point the curve goes through.
         * @return double
         */
        double arc_length(const Point<2> &a, const Point<2>  &b, const Point<2> &c) const;

        /**
         * @brief computes the arc length of a quadratic bezier curve segment up to t
         *
         * @param a This is a point the curve goes through.
         * @param b This is the control point.
         * @param c This is a point the curve goes through.
         * @return double
         */
        double arc_length(const size_t index, const double t) const;

        //private:
        std::vector<Point<2> > points;
        std::vector<Point<2> > control_points;
        std::vector<double> lengths;
        //SplineSet x_spline;
        //SplineSet y_spline;

        //std::vector<double> radii;
        std::vector<double> angles;
        //std::vector<double> lengths;

        //private:

        static double  *solve_cubic_equation_real(const double a_original,const double b_original,const double c_original,const double d_original, double *real_roots);


    };
  }

}


#endif