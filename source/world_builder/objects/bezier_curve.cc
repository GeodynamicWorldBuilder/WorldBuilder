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
#include "world_builder/objects/bezier_curve.h"
#include "world_builder/objects/cubic_spline.h"
#include "world_builder/assert.h"
#include "world_builder/nan.h"

using namespace WorldBuilder;

namespace WorldBuilder
{
  namespace Objects
  {
    BezierCurve::BezierCurve(const std::vector<Point<2> > &p, const std::vector<double> &angle_constrains_input)
    {
      points = p;
      // first compute the factors for a monotome spline
      const size_t n_points = p.size();
      std::vector<double> x(n_points);
      std::vector<double> y(n_points);
      for (size_t i = 0; i < n_points; ++i)
        {
          x[i] = p[i][0];
          y[i] = p[i][1];
        }
      CubicSpline x_spline(x);
      CubicSpline y_spline(y);

      // resize all vectors
      control_points.resize(n_points-1, p[0]);
      lengths.resize(n_points-1,NaN::DSNAN);
      angles.resize(n_points,NaN::DSNAN);
      std::vector<double> angle_constrains = angle_constrains_input;
      angle_constrains.resize(n_points,NaN::DSNAN);
      if (std::isnan(angle_constrains[0]))
        {
          const double &value_x = x_spline.m[0][3];
          const double &value_y = y_spline.m[0][3];
          const double value_x_h = x_spline.m[0][0]*0.5*0.5*0.5 + x_spline.m[0][1]*0.5*0.5 + x_spline.m[0][2]*0.5 + x_spline.m[0][3];
          const double value_y_h = y_spline.m[0][0]*0.5*0.5*0.5 + y_spline.m[0][1]*0.5*0.5 + y_spline.m[0][2]*0.5 + y_spline.m[0][3];
          angles[0] = std::atan2(value_y_h-value_y,value_x_h-value_x);
        }
      else
        {
          // NOTE: start angle assumes slabs or faults going down, which means they should provide a negative angle to get expected behavoir
          angles[0] = angle_constrains[0];
        }

      for (size_t i = 0; i < n_points-2; ++i)
        {
          // first compute next angle:
          if (std::isnan(angle_constrains[i+1]))
            {
              const double &value_x = x_spline.m[i+1][3];
              const double &value_y = y_spline.m[i+1][3];
              const double value_x_h = x_spline.m[i][0]*0.5*0.5*0.5 + x_spline.m[i][1]*0.5*0.5 + x_spline.m[i][2]*0.5 + x_spline.m[i][3];
              const double value_y_h = y_spline.m[i][0]*0.5*0.5*0.5 + y_spline.m[i][1]*0.5*0.5 + y_spline.m[i][2]*0.5 + y_spline.m[i][3];
              angles[i+1] = std::atan2(value_y-value_y_h,value_x-value_x_h);
            }
          else
            {
              // NOTE: start angle doesn't assumes slabs or faults going down, which means they should provide a negative angle to get expected behavoir
              angles[i+1] = angle_constrains[i+1];
            }

          // now find the control point: where the two lines intersect:
          // p0.x
          // if angles are the same, the control point is either on the line or at infinity. Put it at P[i] for now
          if (std::abs(fmod((fmod(angles[i]-angles[i+1],180.) + 180.), 180.)) < std::numeric_limits<double>::epsilon()*10.)
            {
              control_points[i] = p[i];
            }
          else
            {
              const double &x0 = p[i][0];
              const double &y0 = p[i][1];
              const double &x1 = p[i+1][0];
              const double &y1 = p[i+1][1];
              if (std::abs(fmod((fmod(angles[i], 180.) + 180.), 180.) - 90.) < std::numeric_limits<double>::epsilon()*10.)
                {
                  // vertical line at x = x0
                  control_points[i][0] = x0;
                  control_points[i][1] = std::tan(angles[i+1]) * (x0-x1) + y1;
                }
              else if (std::abs(fmod((fmod(angles[i+1], 180.) + 180.), 180.) - 90.) < std::numeric_limits<double>::epsilon()*10.)
                {
                  // vertical line at x = x0
                  control_points[i][0] = x1;
                  control_points[i][1] = std::tan(angles[i]) * (x1-x0) + y0;
                }
              //std::cout << ", flag 2.3: angles i: " << angles[i] << " (" << angles[i]*180./M_PI  << "), angles i+1: " << angles[i+1] << " (" << angles[i+1]*180./M_PI  << ")";
              // tan(angle) = opposite/adjecent
              const double m0 = std::tan(angles[i]); // Line 0: y = m0 (x - x0) + y0
              const double m1 = std::tan(angles[i+1]); // Line 1: y = m1 (x - x1) + y1
              // m0 (x - x0) + y0 = m1 (x - x1) + y1
              // m0*x - m0*x0 + y0 = m1*x - m1*x1 + y1
              // m0*x - m1*x =  -m1*x1 + y1 + m0*x0 - y0
              // (m0*-m1)x =  -m1*x1 + y1 + m0*x0 - y0
              // x =  (-m1*x1 + y1 + m0*x0 - y0)/(m0*-m1)
              // x =  (m0*x0 - m1*x1 + y1 - y0)/(m0*-m1)
              const double control_x = ((m0 * x0 - m1 * x1) - (y0 - y1)) / (m0 - m1);

              control_points[i][0] = control_x;
              control_points[i][1] = m0 * (control_x - x0) + y0;

              // compute length of segment
              lengths[i] = arc_length(p[i],control_points[i],p[i+1]);
              WBAssert(!std::isnan(lengths[i]),"");
            }
        }

      if (std::isnan(angle_constrains[n_points-1]))
        {
          const double value_x = x_spline.m[n_points-2][0] + x_spline.m[n_points-2][1] + x_spline.m[n_points-2][2] + x_spline.m[n_points-2][3];
          const double value_y = y_spline.m[n_points-2][0] + y_spline.m[n_points-2][1] + y_spline.m[n_points-2][2] + y_spline.m[n_points-2][3];
          // have the angle be pointing the the previous halfpoint instead of the next one
          const double value_x_h = x_spline.m[n_points-2][0]*0.5*0.5*0.5 + x_spline.m[n_points-2][1]*0.5*0.5 + x_spline.m[n_points-2][2]*0.5 + x_spline.m[n_points-2][3];
          const double value_y_h = y_spline.m[n_points-2][0]*0.5*0.5*0.5 + y_spline.m[n_points-2][1]*0.5*0.5 + y_spline.m[n_points-2][2]*0.5 + y_spline.m[n_points-2][3];
          angles[n_points-1] = std::atan2(value_y_h-value_y,value_x_h-value_x);
        }
      else
        {
          // NOTE: start angle assumes slabs or faults going down, which means they should provide a negative angle to get expected behavoir
          angles[n_points-1] = angle_constrains[n_points-1];
        }
      size_t i = n_points-2;

      if (std::abs(fmod((fmod(angles[i]-angles[i+1],180.) + 180.), 180.)) < std::numeric_limits<double>::epsilon()*10.)
        {
          control_points[i] = p[i];
        }
      else
        {
          const double &x0 = p[i][0];
          const double &y0 = p[i][1];
          const double &x1 = p[i+1][0];
          const double &y1 = p[i+1][1];
          if (std::abs(fmod((fmod(angles[i], 180.) + 180.), 180.) - 90.) < std::numeric_limits<double>::epsilon()*10.)
            {
              // vertical line at x = x0
              control_points[i][0] = x0;
              control_points[i][1] = std::tan(angles[i+1]) * (x0-x1) + y1;
            }
          else if (std::abs(fmod((fmod(angles[i+1], 180.) + 180.), 180.) - 90.) < std::numeric_limits<double>::epsilon()*10.)
            {
              // vertical line at x = x0
              control_points[i][0] = x1;
              control_points[i][1] = std::tan(angles[i]) * (x1-x0) + y0;
            }

          const double m0 = std::tan(angles[i]); // Line 0: y = m0 (x - x0) + y0
          const double m1 = std::tan(angles[i+1]); // Line 1: y = m1 (x - x1) + y1
          const double control_x = ((m0 * x0 - m1 * x1) - (y0 - y1)) / (m0 - m1);

          control_points[i][0] = control_x;
          control_points[i][1] = m0 * (control_x - x0) + y0;
        }

      // compute length of segment
      lengths[n_points-2] = arc_length(p[n_points-2],control_points[n_points-2],p[n_points-1]);


      WBAssert(!std::isnan(lengths[n_points-2]),
               "lengths[n_points-2] = " << lengths[n_points-2] << ", n_points:" << n_points << ", p[n_points-2]: " << p[n_points-2]
               << ", control_points[n_points-2]: "<< control_points[n_points-2]<< ", p[n_points-1]: " << p[n_points-1]);
    }


    Point<2>
    BezierCurve::operator()(const size_t i, const double t) const
    {
      return (1-t)*(1-t)*points[i] + 2*t*(1-t)*control_points[i] + t*t*points[i+1];
    }

    double
    BezierCurve::arc_length(const Point<2> &a, const Point<2>  &b, const Point<2> &c) const
    {
      // compute the arc length of the Bezier curve
      // see https://gamedev.stackexchange.com/questions/6009/bezier-curve-arc-length
      Point<2> v = a;
      Point<2> w = b;
      v[0] = 2.*(b[0] - a[0]);
      v[1] = 2.*(b[1] - a[1]);
      w[0] = c[0] - 2.*b[0] + a[0];
      w[1] = c[1] - 2.*b[1] + a[1];

      const double uu = 4.*(w[0]*w[0] + w[1]*w[1]);

      // check if all points are in a line
      if (uu < 0.00001 || (a-b).norm() < 1e-6 || (b-c).norm() < 1e-6 || fabs(a[0] * (b[1] - c[1]) + b[0] * (c[1] - a[1]) + c[0] * (a[1] - b[1])) <= 1e-9)
        {
          return std::sqrt((c[0] - a[0])*(c[0] - a[0]) + (c[1] - a[1])*(c[1] - a[1]));
        }

      const double vv = 4.*(v[0]*w[0] + v[1]*w[1]);
      const double ww = v[0]*v[0] + v[1]*v[1];

      const double t1 = (2.*std::sqrt(uu*(uu + vv + ww)));
      const double t2 = 2.*uu+vv;
      const double t3 = vv*vv - 4.*uu*ww;
      const double t4 = (2.*std::sqrt(uu*ww));

      WBAssert(!std::isnan(((t1*t2 - t3*std::log(t2+t1) -(vv*t4 - t3*std::log(vv+t4))) / (8*std::pow(uu, 1.5)))), "result is nan: " << ((t1*t2 - t3*std::log(t2+t1) -(vv*t4 - t3*std::log(vv+t4))) / (8*std::pow(uu, 1.5)))
               << ", t1=" << t1 << ", t2=" << t2 << ", t3=" << t3 << ", t4=" << t4 << ", v = " << v << ", w=" << w << ", vv=" << vv << ", uu=" << uu
               << ", a=" << a << ", b=" << b << ", c=" << c
               << ", std::log(t2+t1)=" << std::log(t2+t1) << ", std::log(vv+t4)=" << std::log(vv+t4) << ", std::pow(uu, 1.5)=" << std::pow(uu, 1.5) << ", std::log(vv+t4)=" << std::log(vv+t4));

      return ((t1*t2 - t3*std::log(t2+t1) -(vv*t4 - t3*std::log(vv+t4))) / (8*std::pow(uu, 1.5)));
    }


    double
    BezierCurve::arc_length(const size_t index, const double t) const//const Point<2> &P0, const Point<2>  &Pc, const Point<2> &P1, const double t) const
    {
      // This method uses a similair approach as https://malczak.info/blog/quadratic-bezier-curve-length
      // but instead of the full length, we integrate the full equaion (https://www.wolframalpha.com/input?i=integrate+sqrt%28A*t%5E2%2BB*t%2Bc%29+dt)
      // leaving t in there. Then we compute the integration constant by stating that the length at t=0 should
      // be zero.
      auto dt = points[index]-2.*control_points[index]+points[index+1];
      auto ddt = 2.*control_points[index]-2.*points[index];
      const double a = 4*(dt[0]*dt[0]+dt[1]*dt[1]);
      const double c = ddt[0]*ddt[0]+ddt[1]*ddt[1];

      if (a < 5e-4 * c || points[index] == control_points[index] || control_points[index] == points[index+1])
        {
          // all points are on a line
          return sqrt(((points[index+1]-points[index])*t)*((points[index+1]-points[index])*t));//std::sqrt((points[index][0] + dx1*t)*(points[index][0] + dx1*t) + (points[index][1] + dy1*t)*(points[index][1] + dy1*t));
        }

      const double b = 4*(dt[0]*ddt[0]+dt[1]*ddt[1]);//2*ddt*dt;////at*bt;

      const double inv_4a = 1./(4*a);
      const double u = (b*b)*inv_4a;
      const double k = (4*a*c-b*b)*inv_4a;
      const double sqrt_a = sqrt(a);
      const double sqrt_c = sqrt(c);
      const double inv_8_sqrt_a_a_a = 1./(8.*sqrt(a*a*a));
      const double sqrt_c_t_b_at = sqrt(c+t*(b+a*t));
      double x = t*sqrt_a+sqrt(u);

      // todo: optimize
      const double integral = ((b+2.*a*t)*sqrt_c_t_b_at)*inv_4a - ((b*b-4.*a*c)*log(b+2.*a*t+2.*sqrt_a*sqrt_c_t_b_at))*inv_8_sqrt_a_a_a;
      const double constant = (b*sqrt_c)*inv_4a - ((b*b-4.*a*c)*log(b+2.*sqrt_a*sqrt_c))*inv_8_sqrt_a_a_a;

      return integral-constant;
    }





    ClosestPointOnCurve
    BezierCurve::closest_point_on_curve_segment(const Point<2> &check_point) const
    {
      // go through each section and find all roots in domain 0 to 1 and choose the smallest
      ClosestPointOnCurve closest_point_on_curve;

      for ( size_t i = 0; i < control_points.size(); ++i)
        {
          std::vector<double> real_roots;
          if (points[i] == control_points[i] || control_points[i] == points[i+1])
            {
              const Point<2> point_a = control_points[i]-points[i];
              if (std::abs(points[i][1]) < std::numeric_limits<double>::epsilon()*10 )
                {
                  // point i is on zero, so set the root to the x of point i
                  real_roots.emplace_back(points[i][0]);
                }
              else if (std::abs(point_a[0]) < std::numeric_limits<double>::epsilon()*10)
                {
                  // vertical line going through x.
                  real_roots.emplace_back(points[i][0]);
                }
              else if (std::abs(point_a[1]) < std::numeric_limits<double>::epsilon()*10)
                {
                  // horizontal line, so there is no root x (unless x=0, but then there
                  // are infinite roots).
                  // We can just compute what we are interested in in a different way and
                  // continue to the next curve.
                  const double fraction = check_point[0]-points[i][0];
                  if (fraction >= 0. && fraction <= 1.)
                    {
                      //The closest point on the line has a y of point[i] and a x of the check point
                      const Point<2> point_on_curve = Point<2>(check_point[0],points[i][1],check_point.get_coordinate_system());

                      if (point_on_curve.distance(check_point) < std::abs(closest_point_on_curve.distance))
                        {
                          // base the sign (like the other methods) on the direction of the points.
                          const double sign = (points[i+1][0]-points[i][0])*(check_point[1]-points[i][1]);//dot_product < 0. ? -1. : 1.;

                          closest_point_on_curve.distance = sign*point_on_curve.distance(check_point); //todo: checkpoint natural?
                          closest_point_on_curve.parametric_fraction = fraction;
                          closest_point_on_curve.interpolation_fraction = fraction;
                          closest_point_on_curve.index = i;
                          closest_point_on_curve.point = point_on_curve;
                        }
                    }
                  continue;
                }
              else
                {
                  const double a = point_a[1]/point_a[0];
                  real_roots.emplace_back((-points[i][1]/a)+points[i][0]);
                  //std::cout << "flag 3: root = " << (-points[i][1]/a)+points[i][0] << ", point_a[0] = " << point_a[0] << ", point_a[1] = "<< point_a[1] << std::endl;
                }
              //real_roots.emplace_back(points[i+1]-points[i]);
            }
          else
            {
              // compute a,b,c and d in the cubic equation describing the distance from point p to the local quadratic Bezier curve.
              // using https://blog.gludion.com/2009/08/distance-to-quadratic-bezier-curve.html
              // todo: I should also take a look at: https://iquilezles.org/articles/distfunctions2d/
              const Point<2> A = control_points[i]-points[i];
              const Point<2> B = points[i+1]-control_points[i]-A;
              const double a = B*B;
              const double b = 3*A*B;
              const double c = 2.*A*A+(points[i]-check_point)*B;
              const double d = (points[i]-check_point)*A;
              real_roots = this->solve_cubic_equation_real(a,b,c,d);
            }

          for (size_t root_i = 0; root_i < real_roots.size(); ++root_i)
            {
              if (real_roots[root_i] >= 0. && real_roots[root_i] <= 1.)
                {
                  const Point<2> point_on_curve = (*this)(i,real_roots[root_i]);

                  if (point_on_curve.distance(check_point) < std::abs(closest_point_on_curve.distance))
                    {
                      // the sign is rotating the derivative by 90 degrees.
                      // When moving in the direction of increasing t, left is positve and right is negative.
                      const double &t = real_roots[root_i];
                      const Point<2> derivative_point = ((-2+2*t)*points[i] + (2-4*t)*control_points[i] + 2*t*points[i+1]);
                      const Point<2> second_derivative_point = ((2)*points[i] + (-4)*control_points[i] + 2*points[i+1]);
                      Point<2> tangent_point = derivative_point - point_on_curve;
                      // if angle between check point and tangent point is larget than 90 degrees, return a negative distance
                      const double dot_product = (tangent_point*(check_point-point_on_curve));
                      const double sign = dot_product < 0. ? -1. : 1.;
                      tangent_point = Point<2>(-tangent_point[1],tangent_point[0],tangent_point.get_coordinate_system());

                      closest_point_on_curve.distance = sign*point_on_curve.distance(check_point);
                      closest_point_on_curve.parametric_fraction = real_roots[root_i];
                      closest_point_on_curve.interpolation_fraction = arc_length(i,real_roots[root_i])/lengths[i];
                      closest_point_on_curve.index = i;
                      closest_point_on_curve.point = point_on_curve;
                    }
                }
            }
        }
      return closest_point_on_curve;
    }


    ClosestPointOnCurve
    BezierCurve::closest_point_on_curve(const Point<2> &check_point) const
    {
      ClosestPointOnCurve closest_point_on_curve = this->closest_point_on_curve_segment(check_point);
      if (std::isnan(closest_point_on_curve.point[0]))
        {
          // The closest point is one of the edges, find which one
          const double distance_to_first_point = (points[0]-check_point).norm();
          const double distance_to_last_point = (points[points.size()-1]-check_point).norm();
          if (distance_to_first_point < distance_to_last_point)
            {
              closest_point_on_curve.distance = NaN::DSNAN; // the distance computed above is not following the curve. Could be computed if needed.
              closest_point_on_curve.index = 0;
              closest_point_on_curve.parametric_fraction = 0;
              closest_point_on_curve.interpolation_fraction = 0;
              closest_point_on_curve.point = points[0];
            }
          else
            {
              closest_point_on_curve.distance = NaN::DSNAN; // the distance computed above is not following the curve. Could be computed if needed.
              closest_point_on_curve.index = points.size()-1;
              closest_point_on_curve.parametric_fraction = 0;
              closest_point_on_curve.interpolation_fraction = 0;
              closest_point_on_curve.point = points[points.size()-1];
            }
        }
      return closest_point_on_curve;
    }


    std::vector<double>
    BezierCurve::solve_cubic_equation_real(const double a_original,const double b_original,const double c_original,const double d_original)
    {

      std::vector<double> real_roots;
      real_roots.reserve(3);
      constexpr double tolerance = 1e-10;
      if (std::abs(a_original) <= tolerance)
        {
          if (std::abs(b_original) <= tolerance)
            {
              // linear equation
              //std::cout << "LINEAR EQUATION!!" << std::endl;
              const double &a = c_original;
              const double &b = d_original;
              real_roots.emplace_back(-b/a);
              return real_roots;
            }
          // quadratic equation
          const double &a = b_original;
          const double &b = c_original;
          const double &c = d_original;
          const double discriminant = b*b -4.*a*c;
          if (std::abs(discriminant) <= tolerance)
            {
              real_roots.emplace_back((-b+sqrt(discriminant))/(2*a));
              return real_roots;
            }
          else if (discriminant > 0)
            {
              real_roots.emplace_back((-b + sqrt(discriminant))/(2.*a));
              real_roots.emplace_back((-b - sqrt(discriminant))/(2.*a));
              return real_roots;
            }
          return real_roots;
        }
      else
        {
          // based on https://quarticequations.com/Cubic.pdf
          // divide by a
          //constexpr double one_third = 1./3.;
          const double b = b_original/a_original; // a_2
          const double c = c_original/a_original; // a_1
          const double d = d_original/a_original; //a_0

          const double q = (c/3.)-(b*b/9.);
          const double r = (c*b-3*d)/6. - b*b*b/27.;
          const double discriminant = r*r+q*q*q;
          if (discriminant > 0)
            {
              // only one real solution
              const double A = std::pow(std::abs(r) + sqrt(discriminant),1./3.);
              const double t = r >= 0 ? A-q/A : q/A-A;
              real_roots.emplace_back(t-b/3.);
            }
          else
            {
              // three real solutions
              // std::pow(-q,3./2.) == sqrt(-q*q*q)
              const double phi_1 = std::abs(q) <= tolerance ? 0 : (acos(r/sqrt(-q*q*q))/3.);
              const double phi_2 = phi_1 - 2.*Consts::PI/3.;
              const double phi_3 = phi_1 + 2.*Consts::PI/3.;
              const double sqrt_q_3 = 2*sqrt(-q);
              const double b_t_one_third = b/3.;
              const double value_1 = sqrt_q_3 * cos(phi_1)-b_t_one_third;
              const double value_2 = sqrt_q_3 * cos(phi_2)-b_t_one_third;
              const double value_3 = sqrt_q_3 * cos(phi_3)-b_t_one_third;

              real_roots.emplace_back(value_1);
              if (std::abs(value_1 - value_2) > tolerance)
                {
                  real_roots.emplace_back(value_2);
                }
              // we don't have to check value 1 and 3 because z3 <= z2 <= z1
              // so if z2 and z3 are not equal, z1 and z3 are not either.
              if (std::abs(value_2 - value_3) > tolerance)
                {
                  real_roots.emplace_back(value_3);
                }
            }
        }
      return real_roots;
    }

  }
}