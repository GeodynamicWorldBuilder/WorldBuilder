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
          //std::cout << i << ": (" << x[i] << ":" << y[i] << ")" << std::endl;
        }
      CubicSpline x_spline(x);
      //std::cout << "flag 1.1" << std::endl;
      CubicSpline y_spline(y);
      //std::cout << "flag 1.2" << std::endl;
      //x_spline.set_points(x);
      //y_spline.set_points(y);

      // resize all vectors
      control_points.resize(n_points-1, p[0]);
      lengths.resize(n_points-1,NaN::DSNAN);
      angles.resize(n_points,NaN::DSNAN);
      std::vector<double> angle_constrains = angle_constrains_input;
      angle_constrains.resize(n_points,NaN::DSNAN);
      if (std::isnan(angle_constrains[0]))
        {
          const double value_x = x_spline.m[0][3];
          const double value_y = y_spline.m[0][3];
          //const double derivative_x = x_spline.m[0][2];
          //const double derivative_y = y_spline.m[0][2];
          //const double value_x_q = x_spline.m[0][0]*0.25*0.25*0.25 + x_spline.m[0][1]*0.25*0.25 + x_spline.m[0][2]*0.25 + x_spline.m[0][3];
          //const double value_y_q = y_spline.m[0][0]*0.25*0.25*0.25 + y_spline.m[0][1]*0.25*0.25 + y_spline.m[0][2]*0.25 + y_spline.m[0][3];
          const double value_x_h = x_spline.m[0][0]*0.5*0.5*0.5 + x_spline.m[0][1]*0.5*0.5 + x_spline.m[0][2]*0.5 + x_spline.m[0][3];
          const double value_y_h = y_spline.m[0][0]*0.5*0.5*0.5 + y_spline.m[0][1]*0.5*0.5 + y_spline.m[0][2]*0.5 + y_spline.m[0][3];
          //const double value_x_3q = x_spline.m[0][0]*0.75*0.75*0.75 + x_spline.m[0][1]*0.75*0.75 + x_spline.m[0][2]*0.75 + x_spline.m[0][3];
          //const double value_y_3q = y_spline.m[0][0]*0.75*0.75*0.75 + y_spline.m[0][1]*0.75*0.75 + y_spline.m[0][2]*0.75 + y_spline.m[0][3];
          angles[0] = std::atan2(value_y_h-value_y,value_x_h-value_x);//std::atan2(derivative_y,derivative_x);
          //std::cout << "--> 0" << ": value_x:y = " << value_x << ":" << value_y  <<  "(" << value_x*180./M_PI  << ":" << value_y*180./M_PI  << "),  value_h x:y = " << value_x_h << ":" << value_y_h << "(" << value_x_h*180./M_PI  << ":" << value_y_h*180./M_PI  << ")," << ", angle = " << angles[0] << " (" << angles[0] *180./M_PI << ")" //<< ", derivative_x = " << derivative_x
          //          << ", x a:b:c:d = " << x_spline.m[0][0] << ":" << x_spline.m[0][1] << ":" << x_spline.m[0][2]  << ":" << x_spline.m[0][1]
          //          //<< ", derivative_y = " << derivative_y
          //          << ", y a:b:c:d = " << y_spline.m[0][0] << ":" << y_spline.m[0][1] << ":" << y_spline.m[0][2]  << ":" << y_spline.m[0][1]
          //          << std::endl;

          // compute p1 and p2 relative to p0 to compute the angles.
          //angles[0] = std::atan2(p[1][1]-p[0][1],p[1][0]-p[0][0]);
          //const double angle_p0p1 = std::atan2(p[1][1]-p[0][1],p[1][0]-p[0][0]);
          //const double angle_p0p2 = std::atan2(p[2][1]-p[0][1],p[2][0]-p[0][0]);
          //angles[0] = angle_p0p1-0.5*(angle_p0p2-angle_p0p1); //2.*angle_p0p1-angle_p0p2;//
          //std::cout << "angles[0] = " << angles[0] << " (" << angles[0] *180./M_PI << ")" << ", angle_p0p1 = " << angle_p0p1  << "(" << angle_p0p1 *180./M_PI << ")"<< ", angle_p0p2 = " << angle_p0p2  << "(" << angle_p0p2 *180./M_PI << ")"<< std::endl;
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
              const double value_x = x_spline.m[i+1][3];
              const double value_y = y_spline.m[i+1][3];
              //const double derivative_x = x_spline.m[i+1][2];
              //const double derivative_y = y_spline.m[i+1][2];
              const double value_x_h = x_spline.m[i][0]*0.5*0.5*0.5 + x_spline.m[i][1]*0.5*0.5 + x_spline.m[i][2]*0.5 + x_spline.m[i][3];
              const double value_y_h = y_spline.m[i][0]*0.5*0.5*0.5 + y_spline.m[i][1]*0.5*0.5 + y_spline.m[i][2]*0.5 + y_spline.m[i][3];
              //const double value_x = x_spline.m[i+1][0]*(i+1)*(i+1)*(i+1) + x_spline.m[i+1][1]*(i+1)*(i+1) + x_spline.m[i+1][2]*(i+1) + x_spline.m[i+1][3];
              //const double value_y = y_spline.m[i+1][0]*(i+1)*(i+1)*(i+1) + y_spline.m[i+1][1]*(i+1)*(i+1) + y_spline.m[i+1][2]*(i+1) + y_spline.m[i+1][3];
              //const double derivative_x = 3*x_spline.m[i+1][0]*(i+1)*(i+1) + 2*x_spline.m[i+1][1]*(i+1) + x_spline.m[i+1][2];
              //const double derivative_y = 3*y_spline.m[i+1][0]*(i+1)*(i+1) + 2*y_spline.m[i+1][1]*(i+1) + y_spline.m[i+1][2];
              angles[i+1] = std::atan2(value_y-value_y_h,value_x-value_x_h);
              //std::cout << "--->" << i+1 << ": value_x:y = " << value_x << ":" << value_y <<  "(" << value_x*180./M_PI  << ":" << value_y*180./M_PI  << "), "
              //<< "value_h x:y = " << value_x_h << ":" << value_y_h << "(" << value_x_h*180./M_PI  << ":" << value_y_h*180./M_PI  << "),"
              //<< "value_h-x:y = " << value_x_h-value_x << ":" << value_y_h-value_y << "(" << (value_x_h-value_x)*180./M_PI  << ":" << (value_y_h-value_y)*180./M_PI  << "),"
              //<< ", angle = " << angles[i+1] << " (" << angles[i+1] *180./M_PI << ")" //<< ", derivative_x = " << derivative_x
              //          << ", x a:b:c:d = " << x_spline.m[i+1][0] << ":" << x_spline.m[i+1][1] << ":" << x_spline.m[i+1][2]  << ":" << x_spline.m[i+1][3]
              //         // << ", derivative_y = " << derivative_y
              //          << ", y a:b:c:d = " << y_spline.m[i+1][0] << ":" << y_spline.m[i+1][1] << ":" << y_spline.m[i+1][2]  << ":" << y_spline.m[i+1][3]
              //          << std::endl;

              //const double &x = p[0][0];
              //const double derivative = 3*m[0][0]*x*x + 2*m[0][1]*x + m[0][2];
              //angles[i] = std::atan2(derivative,1.);
              // compute p1 and p2 relative to p0 to compute the angles.
              //if ( i < n_points-2)
              //  {
              //    //angles[i+1] = std::atan2(p[i+2][1]-p[i][1],p[i+2][0]-p[i][0]);
              //
              //    const double angle_p0p1 = std::atan2(p[i+2][1]-p[i+1][1],p[i+2][0]-p[i+1][0]);
              //    const double angle_p0p2 = std::atan2(p[i+3][1]-p[i+1][1],p[i+3][0]-p[i+1][0]);
              //    angles[i+1] = angle_p0p1-0.5*(angle_p0p2-angle_p0p1);//2.*angle_p0p1-angle_p0p2;
              //    std::cout << "angles[i+1] = " << angles[i+1] << " (" << angles[i+1] *180./M_PI << ")" << ", angle_p0p1 = " << angle_p0p1  << "(" << angle_p0p1 *180./M_PI << ")"<< ", angle_p0p2 = " << angle_p0p2  << "(" << angle_p0p2 *180./M_PI << ")"<< std::endl;
              //  }
              //else
              //  {
              //    //angles[i+1] = std::atan2(p[i+1][1]-p[i][1],p[i+1][0]-p[i][0]);
              //    const double angle_p0p1 = std::atan2(p[i][1]-p[i+1][1],p[i][0]-p[i+1][0]);
              //    const double angle_p0p2 = std::atan2(p[i-1][1]-p[i+1][1],p[i-1][0]-p[i+1][0]);
              //    angles[i+1] = 2.*angle_p0p1-angle_p0p2;
              //    std::cout << "angles[i+1] = " << angles[i+1] << " (" << angles[i+1] *180./M_PI << ")" << ", angle_p0p1 = " << angle_p0p1  << "(" << angle_p0p1 *180./M_PI << ")"<< ", angle_p0p2 = " << angle_p0p2  << "(" << angle_p0p2 *180./M_PI << ")"<< std::endl;
              //  }
            }
          else
            {
              // NOTE: start angle doesn't assumes slabs or faults going down, which means they should provide a negative angle to get expected behavoir
              angles[i+1] = angle_constrains[i+1];
            }

          // now find the control point: where the two lines intersect:
          // p0.x
          // if angles are the same, the control point is either on the line or at infinity. Put it at P[i] for now
          //WBAssert(std::fabs(angles[i]-angles[i+1]) > std::numeric_limits<double>::epsilon()*10., "angles are the same, so the lines are parallel. Can't find a control point. "
          //<< "angles[" << i << "] = " << angles[i] << ", angles[" << i+1 << "] = " << angles[i+1] << ", angles[i]-angles[i+1] = " << angles[i]-angles[i+1]
          //<< ", std::fabs(angles[i]-angles[i+1]) = " << std::fabs(angles[i]-angles[i+1]));
          //std::cout << std::endl << i << ": ";
          if (std::abs(fmod((fmod(angles[i]-angles[i+1],180.) + 180.), 180.)) < std::numeric_limits<double>::epsilon()*10.)
            {
              control_points[i] = p[i];
              //std::cout << ", flag 1: "<< angles[i] << " (" << angles[i] *180./M_PI << "), "<< angles[i+1] << " (" << angles[i+1] *180./M_PI << ")";
            }
          else
            {
              //std::cout << ", flag 2 ";
              const double &x0 = p[i][0];
              const double &y0 = p[i][1];
              const double &x1 = p[i+1][0];
              const double &y1 = p[i+1][1];
              if (std::abs(fmod((fmod(angles[i], 180.) + 180.), 180.) - 90.) < std::numeric_limits<double>::epsilon()*10.)
                {
                  //std::cout << ", flag 2.1 ";
                  // vertical line at x = x0
                  control_points[i][0] = x0;
                  control_points[i][1] = std::tan(angles[i+1]) * (x0-x1) + y1;
                }
              else if (std::abs(fmod((fmod(angles[i+1], 180.) + 180.), 180.) - 90.) < std::numeric_limits<double>::epsilon()*10.)
                {
                  //std::cout << ", flag 2.2 ";
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

              //std::cout << ", m0 = " << m0 << ", m1 = " << m1 << ", m0 - m1 = " << m0 - m1 << ", control point = " << control_x << ":" << m0 * (control_x - x0) + y0 << ", ";
              control_points[i][0] = control_x;
              control_points[i][1] = m0 * (control_x - x0) + y0;

              //std::cout << "control_points[" << i << "]: " << control_points[i] << "(" << control_points[i]*180./Consts::PI << ")"  << std::endl;

              double t = 0.25;
              auto pq = (1-t)*(1-t)*p[i] + 2*t*(1-t)*control_points[i] + t*t*p[i+1];
              t = 0.5;
              auto ph = (1-t)*(1-t)*p[i] + 2*t*(1-t)*control_points[i] + t*t*p[i+1];
              t = 0.75;
              auto p3q = (1-t)*(1-t)*p[i] + 2*t*(1-t)*control_points[i] + t*t*p[i+1];
              //std::cout << "length = "
              //          << sqrt((p[i][0]-pq[0])*(p[i][0]-pq[0])+(p[i][1]-pq[1])*(p[i][1]-pq[1]))
              //          + sqrt((ph[0]-pq[0])*(ph[0]-pq[0])+(ph[1]-pq[1])*(ph[1]-pq[1]))
              //          + sqrt((ph[0]-p3q[0])*(ph[0]-p3q[0])+(ph[1]-p3q[1])*(ph[1]-p3q[1]))
              //          + sqrt((p[i+1][0]-p3q[0])*(p[i+1][0]-p3q[0])+(p[i+1][1]-p3q[1])*(p[i+1][1]-p3q[1])) << std::endl;

              // compute length of segment
              lengths[i] = arc_length(p[i],control_points[i],p[i+1]);
              WBAssert(!std::isnan(lengths[i]),"");

              //std::cout << std::endl << " --> total arc length = " << arc_length(p[i],control_points[i],p[i+1])
              //          << ", t=1 arc length = " << arc_length(i, 1.)
              //          << ", t=0.5 arc length = " << arc_length(i, 0.5)
              //          << ", t=0 arc length = " << arc_length(i, 0.) << std::endl;

              //// Based on http://geomalgorithms.com/a02-_lines.html.
              //// if the control point is doesn't project on the line
              //// rotate is 180 degrees.
              //const Point<2> BSP_ESP = p[i+1] - p[i];
              //const Point<2> BSP_CP = control_points[i] - p[i];
//
              //const double c1 = BSP_ESP * BSP_CP;
              //const double c2 = BSP_ESP * BSP_ESP;
//
              //if (c1 < 0 || c2 < c1)
              //  {
              //      control_points[i][0] = 2 * x1 - control_points[i][0];
              //      control_points[i][1] = 2.*y1 - control_points[i][1];
              //  }
            }
          //std::cout << std::endl;
        }
      //std::cout << "-----" << x_spline.m.size() << " -- " << n_points-1 << std::endl;
      //if (std::isnan(angle_constrains[n_points-2]))
      //  {
      //    const double value_x = x_spline.m[n_points-2][3];
      //    const double value_y = y_spline.m[n_points-2][3];
      //    const double derivative_x = x_spline.m[n_points-2][2];
      //    const double derivative_y = y_spline.m[n_points-2][3];
      //    // have the angle be pointing the the previous halfpoint instead of the next one
      //    const double value_x_h = x_spline.m[n_points-2][0]*0.5*0.5*0.5 + x_spline.m[n_points-2][1]*0.5*0.5 + x_spline.m[n_points-2][2]*0.5 + x_spline.m[n_points-2][3];
      //    const double value_y_h = y_spline.m[n_points-2][0]*0.5*0.5*0.5 + y_spline.m[n_points-2][1]*0.5*0.5 + y_spline.m[n_points-2][2]*0.5 + y_spline.m[n_points-2][3];
      //    angles[n_points-2] = std::atan2(value_y_h-value_y,value_x_h-value_x);//std::atan2(derivative_y,derivative_x);
      //    std::cout << "---> " << n_points-2 << ": value_x:y = " << value_x << ":" << value_y << ",  value_h x:y = " << value_x_h << ":" << value_y_h << ", angle = " << angles[n_points-2] << " (" << angles[n_points-2] *180./M_PI << ")" << ", derivative_x = " << derivative_x
      //              << ", x a:b:c:d = " << x_spline.m[n_points-2][0] << ":" << x_spline.m[n_points-2][1] << ":" << x_spline.m[n_points-2][2]  << ":" << x_spline.m[n_points-2][1]
      //              << ", derivative_y = " << derivative_y
      //              << ", y a:b:c:d = " << y_spline.m[n_points-2][0] << ":" << y_spline.m[n_points-2][1] << ":" << y_spline.m[n_points-2][2]  << ":" << y_spline.m[n_points-2][1]
      //              << std::endl;
      //    // compute p1 and p2 relative to p0 to compute the angles.
      //    //angles[0] = std::atan2(p[1][1]-p[0][1],p[1][0]-p[0][0]);
      //    //const double angle_p0p1 = std::atan2(p[1][1]-p[0][1],p[1][0]-p[0][0]);
      //    //const double angle_p0p2 = std::atan2(p[2][1]-p[0][1],p[2][0]-p[0][0]);
      //    //angles[0] = angle_p0p1-0.5*(angle_p0p2-angle_p0p1); //2.*angle_p0p1-angle_p0p2;//
      //    //std::cout << "angles[0] = " << angles[0] << " (" << angles[0] *180./M_PI << ")" << ", angle_p0p1 = " << angle_p0p1  << "(" << angle_p0p1 *180./M_PI << ")"<< ", angle_p0p2 = " << angle_p0p2  << "(" << angle_p0p2 *180./M_PI << ")"<< std::endl;
      //  }
      //else
      //  {
      //    // NOTE: start angle assumes slabs or faults going down, which means they should provide a negative angle to get expected behavoir
      //    angles[n_points-2] = angle_constrains[n_points-2];
      //  }

      if (std::isnan(angle_constrains[n_points-1]))
        {
          const double value_x = x_spline.m[n_points-2][0] + x_spline.m[n_points-2][1] + x_spline.m[n_points-2][2] + x_spline.m[n_points-2][3];
          const double value_y = y_spline.m[n_points-2][0] + y_spline.m[n_points-2][1] + y_spline.m[n_points-2][2] + y_spline.m[n_points-2][3];
          const double derivative_x = x_spline.m[n_points-2][0] + x_spline.m[n_points-2][1] + x_spline.m[n_points-2][2];
          const double derivative_y = y_spline.m[n_points-2][0] + y_spline.m[n_points-2][1] + y_spline.m[n_points-2][3];
          // have the angle be pointing the the previous halfpoint instead of the next one
          const double value_x_h = x_spline.m[n_points-2][0]*0.5*0.5*0.5 + x_spline.m[n_points-2][1]*0.5*0.5 + x_spline.m[n_points-2][2]*0.5 + x_spline.m[n_points-2][3];
          const double value_y_h = y_spline.m[n_points-2][0]*0.5*0.5*0.5 + y_spline.m[n_points-2][1]*0.5*0.5 + y_spline.m[n_points-2][2]*0.5 + y_spline.m[n_points-2][3];
          angles[n_points-1] = std::atan2(value_y_h-value_y,value_x_h-value_x);//std::atan2(derivative_y,derivative_x);
          //std::cout <<  "----->" << n_points-1 << ": value_x:y = " << value_x << ":" << value_y << ",  value_h x:y = " << value_x_h << ":" << value_y_h << ", angle = " << angles[n_points-1] << " (" << angles[n_points-1] *180./M_PI << ")" << ", derivative_x = " << derivative_x
          //          << ", x a:b:c:d = " << x_spline.m[n_points-2][0] << ":" << x_spline.m[n_points-2][1] << ":" << x_spline.m[n_points-2][2]  << ":" << x_spline.m[n_points-2][1]
          //          << ", derivative_y = " << derivative_y
          //          << ", y a:b:c:d = " << y_spline.m[n_points-2][0] << ":" << y_spline.m[n_points-2][1] << ":" << y_spline.m[n_points-2][2]  << ":" << y_spline.m[n_points-2][1]
          //          << std::endl;
          // compute p1 and p2 relative to p0 to compute the angles.
          //angles[0] = std::atan2(p[1][1]-p[0][1],p[1][0]-p[0][0]);
          //const double angle_p0p1 = std::atan2(p[1][1]-p[0][1],p[1][0]-p[0][0]);
          //const double angle_p0p2 = std::atan2(p[2][1]-p[0][1],p[2][0]-p[0][0]);
          //angles[0] = angle_p0p1-0.5*(angle_p0p2-angle_p0p1); //2.*angle_p0p1-angle_p0p2;//
          //std::cout << "angles[0] = " << angles[0] << " (" << angles[0] *180./M_PI << ")" << ", angle_p0p1 = " << angle_p0p1  << "(" << angle_p0p1 *180./M_PI << ")"<< ", angle_p0p2 = " << angle_p0p2  << "(" << angle_p0p2 *180./M_PI << ")"<< std::endl;
        }
      else
        {
          // NOTE: start angle assumes slabs or faults going down, which means they should provide a negative angle to get expected behavoir
          angles[n_points-1] = angle_constrains[n_points-1];
        }
      size_t i = n_points-2;
      //std::cout << i << ": ";
      if (std::abs(fmod((fmod(angles[i]-angles[i+1],180.) + 180.), 180.)) < std::numeric_limits<double>::epsilon()*10.)
        {
          control_points[i] = p[i];
          //std::cout << ", flag 1: "<< angles[i] << " (" << angles[i] *180./M_PI << "), "<< angles[i+1] << " (" << angles[i+1] *180./M_PI << ")";
        }
      else
        {
          //std::cout << ", flag 2";
          const double &x0 = p[i][0];
          const double &y0 = p[i][1];
          const double &x1 = p[i+1][0];
          const double &y1 = p[i+1][1];
          if (std::abs(fmod((fmod(angles[i], 180.) + 180.), 180.) - 90.) < std::numeric_limits<double>::epsilon()*10.)
            {
              //std::cout << ", flag 2.1";
              // vertical line at x = x0
              control_points[i][0] = x0;
              control_points[i][1] = std::tan(angles[i+1]) * (x0-x1) + y1;
            }
          else if (std::abs(fmod((fmod(angles[i+1], 180.) + 180.), 180.) - 90.) < std::numeric_limits<double>::epsilon()*10.)
            {
              //std::cout << ", flag 2.2";
              // vertical line at x = x0
              control_points[i][0] = x1;
              control_points[i][1] = std::tan(angles[i]) * (x1-x0) + y0;
            }
          //std::cout << ", flag 2.3: angles i: " << angles[i] << " (" << angles[i]*180./M_PI  << "), angles i+1: " << angles[i+1] << " (" << angles[i+1]*180./M_PI  << ")";
          const double m0 = std::tan(angles[i]); // Line 0: y = m0 (x - x0) + y0
          const double m1 = std::tan(angles[i+1]); // Line 1: y = m1 (x - x1) + y1
          const double control_x = ((m0 * x0 - m1 * x1) - (y0 - y1)) / (m0 - m1);
          //std::cout << ", m0 = " << m0 << ", m1 = " << m1 << ", m0 - m1 = " << m0 - m1 << ", control point = " << control_x << ":" << m0 * (control_x - x0) + y0;
          control_points[i][0] = control_x;
          control_points[i][1] = m0 * (control_x - x0) + y0;

          //std::cout << "control_points[" << i << "] = " << control_points[i] << "(" << control_points[i]*180./Consts::PI << ")" << std::endl;

          //// Based on http://geomalgorithms.com/a02-_lines.html.
          //// if the control point is doesn't project on the line
          //// rotate is 180 degrees.
          //const Point<2> BSP_ESP = p[i+1] - p[i];
          //const Point<2> BSP_CP = control_points[i] - p[i];
          //const double c1 = BSP_ESP * BSP_CP;
          //const double c2 = BSP_ESP * BSP_ESP;
          //if (c1 < 0 || c2 < c1)
          //  {
          //      control_points[i][0] = 2 * x1 - control_points[i][0];
          //      control_points[i][1] = 2.*y1 - control_points[i][1];
          //  }
        }
      //std::cout << std::endl;

      // compute length of segment
      lengths[n_points-2] = arc_length(p[n_points-2],control_points[n_points-2],p[n_points-1]);

      //std::cout << "points:" << std::endl;
      //for (size_t j = 0; j < points.size(); ++j)
      //  std::cout << points[j]*180./Consts::PI << ", ";
      //std::cout << std::endl << ", control points: ";
      //for (size_t j = 0; j < control_points.size(); ++j)
      //  std::cout << control_points[j]*180./Consts::PI << ", ";
      //std::cout << std::endl;

      //std::cout << " lengths.size() = " << lengths.size() << ", n_points = " << n_points << ", n_points-2:" << n_points-2 << std::endl;
      //for (size_t j = 0; j < lengths.size(); ++j)
      //  std::cout << lengths[j] << std::endl;

      WBAssert(!std::isnan(lengths[n_points-2]), "lengths[n_points-2] = " << lengths[n_points-2] << ", n_points:" << n_points << ", p[n_points-2]: " << p[n_points-2] << ", control_points[n_points-2]: "<< control_points[n_points-2]<< ", p[n_points-1]: " << p[n_points-1]);
      //std::cout << p[n_points-2] << " - " << control_points[n_points-2] << " - " << p[n_points-1] << std::endl;

      //std::cout << "=====================================================" << std::endl << std::endl;

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
      //std::cout << ", uu = " << uu << std::endl;

      // check if all points are in a line
      //std::cout << " fabs((a[1] - b[1]) * (a[0] - c[0]) - (a[1] - c[1]) * (c[0] - b[0])) = " << fabs((a[1] - b[1]) * (a[0] - c[0]) - (a[1] - c[1]) * (c[0] - b[0])) << std::endl;
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
      //std::cout << "P0: " << P0 << ", Pc: " << Pc << ", P1: " << P1 << ", dt: " << dt << ", ddt: " << ddt << std::endl;
      const double a = 4*(dt[0]*dt[0]+dt[1]*dt[1]);//dt*dt;//
      const double c = ddt[0]*ddt[0]+ddt[1]*ddt[1];//ddt*ddt;//

      //std::cout << "a: " << a << ", c: " << c << ", t: " << t << std::endl;
      if (a < 5e-4 * c || points[index] == control_points[index] || control_points[index] == points[index+1])//(c < 0.0000001)
        {
          // all points are on a line
          // todo: check that this is actually correct
          //WBAssert(false,"todo: length = " << sqrt(((points[index+1]-points[index])*t)*((points[index+1]-points[index])*t))
          //<< ", t=" << t << ", index=" << index << ", points[index]=" << points[index] << ", points[index+1]=" << points[index+1]
          // << ", control_points[index]=" << control_points[index] << ", control_points[index+1]=" << control_points[index+1]);
          //std::cout << "everyting in a line!" << std::endl;
          return sqrt(((points[index+1]-points[index])*t)*((points[index+1]-points[index])*t));//std::sqrt((points[index][0] + dx1*t)*(points[index][0] + dx1*t) + (points[index][1] + dy1*t)*(points[index][1] + dy1*t));
        }

      const double b = 4*(dt[0]*ddt[0]+dt[1]*ddt[1]);//2*ddt*dt;////at*bt;

      const double u = (b*b)/(4*a);
      const double k = (4*a*c-b*b)/(4*a);
      double x = t*sqrt(a)+sqrt(u);

      // todo: optimize
      const double integral = ((b+2.*a*t)*sqrt(c+t*(b+a*t)))/(4*a) - ((b*b-4.*a*c)*log(b+2.*a*t+2.*sqrt(a)*sqrt(c+t*(b+a*t))))/(8.*pow(a,(3./2.)));
      const double constant = (b*sqrt(c))/(4*a) - ((b*b-4.*a*c)*log(b+2.*sqrt(a)*sqrt(c)))/(8.*pow(a,(3./2.)));
      //std::cout << "integral = " << integral << ", constant = " << constant << ", min = " << integral-constant << std::endl;

      return integral-constant;
    }





    ClosestPointOnCurve
    BezierCurve::closest_point_on_curve_segment(const Point<2> &check_point) const
    {
      // go through each section and find all roots in domain 0 to 1 and choose the smallest
      ClosestPointOnCurve closest_point_on_curve;
      //if(check_point[0] > 0.127991-1e-6 && check_point[0] < 0.127991+1e-6
      //&& check_point[1] > -0.0184307-1e-6 && check_point[1] < -0.0184307+1e-6)
      //{
      //  std::cout << "===>>>start computing the curve" << std::endl;
      //}
      for ( size_t i = 0; i < control_points.size(); ++i)
        {

          //if(check_point[0] > 0.127991-1e-6 && check_point[0] < 0.127991+1e-6
          //&& check_point[1] > -0.0184307-1e-6 && check_point[1] < -0.0184307+1e-6)
          //{
          //  std::cout << "i = " << i << ",";
          //}
          std::vector<double> real_roots;
          if (points[i] == control_points[i] || control_points[i] == points[i+1])
            {
              //if(check_point[0] > 0.127991-1e-6 && check_point[0] < 0.127991+1e-6
              //&& check_point[1] > -0.0184307-1e-6 && check_point[1] < -0.0184307+1e-6)
              //{
              //    std::cout << "closest_point_on_curve LINEAR!!: " << std::endl;//<< -d/c << ", c = " << c << ", d = " << d << std::endl;
              //}
              // y = ax-a*x1-y1 = ax-b; x = -y1/a + x1
              const Point<2> point_a = control_points[i]-points[i];
              if (std::abs(points[i][1]) < std::numeric_limits<double>::epsilon()*10 )
                {
                  // point i is on zero, so set the root to the x of point i
                  real_roots.emplace_back(points[i][0]);
                  //std::cout << "flag 1: root = " << points[i][0]<< std::endl;
                }
              else if (std::abs(point_a[0]) < std::numeric_limits<double>::epsilon()*10)
                {
                  // vertical line going through x.
                  real_roots.emplace_back(points[i][0]);
                  //std::cout << "flag 2: root = " << points[i][0]<< std::endl;
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
          //std::cout << "real roots (" << real_roots.size() << "): ";
          for (size_t root_i = 0; root_i < real_roots.size(); ++root_i)
            {
              //if(check_point[0] > 0.127991-1e-6 && check_point[0] < 0.127991+1e-6
              //&& check_point[1] > -0.0184307-1e-6 && check_point[1] < -0.0184307+1e-6)
              //{
              //    std::cout << root_i << "=" << real_roots[root_i] << "; ";
              //}
              if (real_roots[root_i] >= 0. && real_roots[root_i] <= 1.)
                {
                  //std::cout << "(*this) = " << this << std::endl;
                  const Point<2> point_on_curve = (*this)(i,real_roots[root_i]);
                  //std::cout << root_i << " = " << real_roots[root_i] << ", poc=" << point_on_curve << ", d = " << point_on_curve.distance(check_point) << "; ";
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
                      //std::cout << "derivative point = " <<  derivative_point  << ", velocity: " << derivative_point.norm() << ", second_derivative_point = " << second_derivative_point
                      //          << ", tangent point? = " << tangent_point << " -- " << tangent_point+point_on_curve << ", sign = " << sign << ", dot product = " << dot_product << ",  distance before: " << point_on_curve.distance(check_point) << std::endl;
                      closest_point_on_curve.distance = sign*point_on_curve.distance(check_point);
                      closest_point_on_curve.parametric_fraction = real_roots[root_i];
                      //std::cout << "i: " << i << ", real_roots[root_i] = " << real_roots[root_i]  << ", real roots: " << real_roots[0] << ":" << real_roots[1] << ":" << real_roots[2] << std::endl;
                      closest_point_on_curve.interpolation_fraction = arc_length(i,real_roots[root_i])/lengths[i];
                      //std::cout << "closest_point_on_curve.interpolation_fraction = " << closest_point_on_curve.interpolation_fraction << ", lengths[i] = " << lengths[i]  << ", arc_length(i,real_roots[root_i]): " << arc_length(i,real_roots[root_i]) << ", index = " << i << std::endl;
                      closest_point_on_curve.index = i;
                      closest_point_on_curve.point = point_on_curve;
                    }
                }
              else
                {
                  //std::cout << root_i << " = " << real_roots[root_i] << "; ";
                }
            }
          //std::cout << "index = " << closest_point_on_curve.index << std::endl ;
        }
      //        if(check_point[0] > 0.127991-1e-6 && check_point[0] < 0.127991+1e-6
      //&& check_point[1] > -0.0184307-1e-6 && check_point[1] < -0.0184307+1e-6)
      //{
      //std::cout << std::endl;
      //}
      //std::cout << "final index = " << closest_point_on_curve.index << std::endl ;
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
      //std::cout << "a_original = " << a_original << ", b_original: " << b_original << ", c_original:" << c_original << ", d_original: " << d_original << std::endl;
      if (std::abs(a_original) <= std::numeric_limits<double>::epsilon()*10.)
        {
          if (std::abs(b_original) <= std::numeric_limits<double>::epsilon()*10.)
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
          if (std::abs(discriminant) <= std::numeric_limits<double>::epsilon()*10.)
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
              const double theta = std::abs(q) <= std::numeric_limits<double>::epsilon()*10. ? 0 : acos(r/std::pow(-q,3./2.));
              const double phi_1 = theta/3.;
              const double phi_2 = phi_1 - 2.*Consts::PI/3.;
              const double phi_3 = phi_1 + 2.*Consts::PI/3.;
              const double sqrt_q_3 = 2*sqrt(-q);
              const double value_1 = sqrt_q_3 *cos(phi_1)-b/3.;
              const double value_2 = sqrt_q_3 *cos(phi_2)-b/3.;
              const double value_3 = sqrt_q_3 *cos(phi_3)-b/3.;

              real_roots.emplace_back(value_1);
              if (std::abs(value_1 - value_2) > std::numeric_limits<double>::epsilon()*10.)
                {
                  real_roots.emplace_back(value_2);
                }
              // we don't have to check value 1 and 3 because z3 <= z2 <= z1
              // so if z2 and z3 are not equal, z1 and z3 are not either.
              if (std::abs(value_2 - value_3) > std::numeric_limits<double>::epsilon()*10.)
                {
                  real_roots.emplace_back(value_3);
                }
            }
        }
      return real_roots;
    }

  }
}