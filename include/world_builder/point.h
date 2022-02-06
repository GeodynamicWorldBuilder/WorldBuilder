/*
  Copyright (C) 2018-2021 by the authors of the World Builder code.

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

#ifndef WORLD_BUILDER_POINT_H
#define WORLD_BUILDER_POINT_H
#define _USE_MATH_DEFINES
#include <array>
#include <cmath>

#include "world_builder/assert.h"
#include "world_builder/coordinate_system.h"

namespace WorldBuilder
{
  /**
   * A class which stores 2d and 3d arrays of doubles (depending on the dimension),
   * and the coordinate system which the coordinates can be used for. It also
   * implements several operations such as the computation of the l2 norm and the
   * dot product.
   */
  template<int dim>
  class Point
  {
    public:
      /**
       * Constructor. Constructs a Point at (0,0) in 2d or (0,0,0) in 3d
       * with a Cartesian coordinate system.
       */
      inline
      Point(CoordinateSystem coordinate_system_)
        :
        point(std::array<double,dim>()),
        coordinate_system(coordinate_system_)
      {}

      /**
       * Constructor. Constructs a Point from a std::array<double,dim> and
       * a coordinate system.
       */
      inline
      Point(const std::array<double,dim> &location, CoordinateSystem coordinate_system_)
        :
        point(location),
        coordinate_system(coordinate_system_)
      {}

      /**
       * Constructor. Constructs a Point from an other Point and
       * a coordinate system.
       */
      inline
      Point(const Point<dim> &location, CoordinateSystem coordinate_system_)
        :
        point(location.get_array()),
        coordinate_system(coordinate_system_)
      {}

      /**
       * Constructor. Constructs a Point from an other Point.
       */
      inline
      Point(const Point<dim> &location)
        :
        point(location.get_array()),
        coordinate_system(location.get_coordinate_system())
      {}

      /**
       * Constructor. Constructs a 2d Point from two doubles and
       * a coordinate system.
       */
      Point(const double x, const double y, CoordinateSystem coordinate_system);

      /**
       * Constructor. Constructs a 3d Point from three doubles and
       * a coordinate system.
       */
      Point(const double x, const double y, const double z, CoordinateSystem coordinate_system);

      /**
       * Destructor
       */
      inline
      ~Point() = default;

      inline
      Point<dim> &operator=(const Point<dim> &point_right)
      {
        point = point_right.point;
        coordinate_system = point_right.coordinate_system;
        return *this;
      }

      /**
       * dot product
       */
      inline
      double operator*(const Point<dim> &point_right) const
      {
        WBAssert(coordinate_system == point_right.get_coordinate_system(),
                 "Cannot take the dot product of two points which represent different coordinate systems.");
        const std::array<double,dim> &array = point_right.get_array();
        double dot_product = 0;
        for (unsigned int i = 0; i < dim; ++i)
          dot_product += point[i] * array[i];
        return dot_product;
      }


      /**
       * Multiply the vector with a scalar
       */
      inline
      Point<dim> operator*(const double scalar) const
      {
        // initialize the array to zero.
        std::array<double,dim> array;
        for (unsigned int i = 0; i < dim; ++i)
          array[i] = point[i] * scalar;
        return Point<dim>(array,coordinate_system);
      }

      /**
       * Divide the vector through a scalar
       */
      inline
      Point<dim> operator/(const double scalar) const
      {
        // initialize the array to zero.
        std::array<double,dim> array;
        const double one_over_scalar = 1/scalar;
        for (unsigned int i = 0; i < dim; ++i)
          array[i] = point[i] * one_over_scalar;
        return Point<dim>(array,coordinate_system);
      }

      /**
       * add two points
       */
      inline
      Point<dim> operator+(const Point<dim> &point_right) const
      {
        WBAssert(coordinate_system == point_right.get_coordinate_system(),
                 "Cannot add two points which represent different coordinate systems.");
        Point<dim> point_tmp(point,coordinate_system);
        for (unsigned int i = 0; i < dim; ++i)
          point_tmp[i] += point_right[i];

        return point_tmp;
      }


      /**
       * Substract two points
       */
      inline
      Point<dim> operator-(const Point<dim> &point_right) const
      {
        WBAssert(coordinate_system == point_right.get_coordinate_system(),
                 "Cannot substract two points which represent different coordinate systems. Internal has type " << static_cast<int>(coordinate_system)
                 << ", other point has type " << static_cast<int>(point_right.get_coordinate_system()));
        Point<dim> point_tmp(point,coordinate_system);
        for (unsigned int i = 0; i < dim; ++i)
          point_tmp[i] -= point_right[i];

        return point_tmp;
      }


      /**
       * Multiply the vector with a scalar
       */
      inline
      Point<dim> &operator*=(const double scalar)
      {
        for (unsigned int i = 0; i < dim; ++i)
          point[i] *= scalar;
        return *this;
      }
      /**
       * Divide the vector through a scalar
       */
      inline
      Point<dim> &operator/=(const double scalar)
      {
        for (unsigned int i = 0; i < dim; ++i)
          point[i] /= scalar;
        return *this;
      }

      /**
       * add two points
       */
      inline
      Point<dim> &operator+=(const Point<dim> &point_right)
      {
        WBAssert(coordinate_system == point_right.get_coordinate_system(),
                 "Cannot add two points which represent different coordinate systems.");
        for (unsigned int i = 0; i < dim; ++i)
          point[i] += point_right[i];
        return *this;
      }

      /**
       * substract two points
       */
      inline
      Point<dim> &operator-=(const Point<dim> &point_right)
      {
        WBAssert(coordinate_system == point_right.get_coordinate_system(),
                 "Cannot subtract two points which represent different coordinate systems.");
        for (unsigned int i = 0; i < dim; ++i)
          point[i] -= point_right[i];
        return *this;
      }

      /**
       * access index (const)
       */
      inline
      const double &operator[](const size_t index) const
      {
        WBAssert(index <= dim, "Can't ask for element " << index << " in an point with dimension " << dim << '.');
        return point[index];
      }


      /**
       * access index
       */
      inline double &operator[](const size_t index)
      {
        WBAssert(index <= dim, "Can't ask for element " << index << " in an point with dimension " << dim << '.');
        return point[index];
      }

      /**
       * Computes the distance between this and a given point.
       * In spherical coordinates it returns the central angle in radians.
       */
      double
      distance(const Point<dim> &two) const;

      /**
       * Computes the cheapest relative distance between this and a given point.
       * The return value itself is only guartenteed to have the property that a
       * larger value is further away.
       * In the current implementation that means for the cartasian case the squared
       * value is returned and for the spherical value the result of the havearsine
       * function without asin and sqrt is returned.
       */
      double
      cheap_relative_distance(const Point<dim> &two) const;

      /**
       * return the internal array which stores the point data.
       */
      inline
      const std::array<double,dim> &get_array() const
      {
        return point;
      }


      /**
       * returns the coordinate system associated with the data.
       */
      inline
      CoordinateSystem get_coordinate_system() const
      {
        return coordinate_system;
      }


      /**
      * Computes the L2 norm: sqrt(x_i * x_i + y_i * y_i + z_i * z_i) in 3d.
      */
      inline
      double norm() const
      {
        return std::sqrt(this->norm_square());
      }


      /**
      * Computes the square of the norm, which is the sum of the absolute squares
      * x_i * x_i + y_i * y_i + z_i * z_i in 3d.
      */
      inline
      double norm_square() const
      {
        WBAssertThrow(false,"This function is only available in 2d or 3d.");
        return 0;
      }

      /**
       * Outputs the values of the point to std cout separated by spaces. This does not
       * output the coordinate system.
       */
      friend std::ostream &operator<<( std::ostream &output, const Point<dim> &stream_point )
      {
        for (size_t i = 0; i < dim-1; i++)
          {
            output <<  stream_point[i] << ' ';
          }
        output << stream_point[dim-1];

        return output;
      }


    private:
      std::array<double,dim> point;
      CoordinateSystem coordinate_system;

  };

  template <>
  inline
  double Point<2>::norm_square() const
  {
    return (point[0] * point[0]) + (point[1] * point[1]);
  }

  template <>
  inline
  double Point<3>::norm_square() const
  {
    return (point[0] * point[0]) + (point[1] * point[1]) + (point[2] * point[2]);
  }

  /**
   * This namespace contains some faster but less accurate version of the
   * trigonomic functions and a faster version of the fmod function.
   */
  namespace FT
  {
    constexpr double const_pi = 3.141592653589793238462643383279502884;

    /**
     * Fast version of the fmod function.
     */
    inline double fmod(const double x, const double y)
    {
      const double x_div_y = x/y;
      return (x_div_y-static_cast<int>(x_div_y))*y;
    }

    /**
     * Fast sin function, accurate for values between 0 and pi. The implemenation is
     * based on discussion at https://stackoverflow.com/a/6104692.
     *
     * The accuracy seem good enough for most purposes. The unit test tests in steps
     * of 0.01 from -4 pi to 4 pi and compares against the std sin function and the difference
     * is always smaller than 1.2e-5. If the test is run with intervals of 0.001 then there
     * are 12 entries which are (very slightly) above that (<3e-8) at angles of about
     * -174, -6, 6  and 174.
     *
     */
    inline double fast_sin_d(const double angle)
    {
      constexpr double A = 4.0/(const_pi *const_pi);
      constexpr double oneminPmin = 1.-0.1952403377008734-0.01915214119105392;

      const double y = A* angle * ( const_pi - angle );
      return y*( oneminPmin + y*( 0.1952403377008734 + y * 0.01915214119105392 ) ) ;
    }

    /**
     * Fast but less accurate sin function for any angle.
     * Implemented by calling fast_sin_d with a mirrored x if needed to
     * forfill the constrained of fast_sin_d to only have values between
     * zero and pi.
     */
    inline double sin(const double raw_angle)
    {
      const double angle = (raw_angle > -const_pi && raw_angle < const_pi)
                           ?
                           raw_angle
                           :
                           FT::fmod(raw_angle + std::copysign(const_pi,raw_angle), const_pi * 2.0) - std::copysign(const_pi,raw_angle);

      if (angle >= 0)
        return fast_sin_d(angle);
      return -fast_sin_d(-angle);
    }

    /**
     * Fast but less accurate cos function for any angle.
     */
    inline double cos(const double angle)
    {
      return FT::sin((const_pi*0.5)-angle);
    }
  } // namespace FT


  template<int dim>
  inline
  Point<dim> operator*(const double scalar, const Point<dim> &point)
  {
    return point*scalar;
  }
} // namespace WorldBuilder
#endif
