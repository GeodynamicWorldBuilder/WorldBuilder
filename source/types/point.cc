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
#include <world_builder/types/point.h>
#include <world_builder/assert.h>
#include <world_builder/utilities.h>

namespace WorldBuilder
{
  namespace Types
  {
    template <int dim>
    Point<dim>::Point(const WorldBuilder::Point<dim> &default_value, const std::string &description)
      :
      value(default_value),
      default_value(default_value),
      description(description)
    {
      this->type_name = dim == 2 ? type::Point2D : type::Point3D;
    }

    template <int dim>
    Point<dim>::Point(const WorldBuilder::Point<dim> &value, const WorldBuilder::Point<dim> &default_value, const std::string &description)
      :
      value(value),
      default_value(default_value),
      description(description)
    {
      this->type_name = dim == 2 ? type::Point2D : type::Point3D;
    }

    template <int dim>
    Point<dim>::~Point ()
    {}

    template <int dim>
    std::unique_ptr<Interface>
    Point<dim>::clone() const
    {
      return std::unique_ptr<Interface>(new Point(value, default_value, description));
    }


    template<int dim>
    double Point<dim>::operator*(const Point<dim> &point_) const
    {
      const std::array<double,dim> array = point_.value.get_array();
      double dot_product = 0;
      for (unsigned int i = 0; i < dim; ++i)
        dot_product += value[i] * array[i];
      return dot_product;
    }


    template<int dim>
    WorldBuilder::Point<dim>
    Point<dim>::operator*(const double scalar) const
    {
      // initialize the array to zero.
      std::array<double,dim> array = WorldBuilder::Point<dim>(value.get_coordinate_system()).get_array();
      for (unsigned int i = 0; i < dim; ++i)
        array[i] += value[i] * scalar;
      return WorldBuilder::Point<dim>(array,value.get_coordinate_system());
    }

    template<int dim>
    WorldBuilder::Point<dim>
    Point<dim>::operator+(const Point<dim> &point_) const
    {
      WorldBuilder::Point<dim> point_tmp(value);
      point_tmp += point_.value;

      return point_tmp;
    }

    template<int dim>
    WorldBuilder::Point<dim>
    Point<dim>::operator-(const Point<dim> &point_) const
    {
      WorldBuilder::Point<dim> point_tmp(value);
      point_tmp -= point_.value;

      return point_tmp;
    }

    /**
     * access index
     */
    template<int dim>
    const double &
    Point<dim>::operator[](const unsigned int index) const
    {
      return value[index];
    }


    /**
     * access index
     */
    template<int dim>
    double &
    Point<dim>::operator[](const unsigned int index)
    {
      return value[index];
    }

    /**
     * Multiplies a Types::Point<dim> with a scalr and returns a
     * WorldBuilder::Point<dim>.
     */
    template<int dim>
    WorldBuilder::Point<dim>
    operator*(const double scalar, const Point<dim> &point)
    {
      return point.value*scalar;
    }

    template class Point<2>;
    template class Point<3>;

    /**
     * Multiplies a Types::Point<2> with a scalr and returns a
     * WorldBuilder::Point<2>.
     */
    template WorldBuilder::Point<2> operator*(const double scalar, const Point<2> &point);


    /**
     * Multiplies a Types::Point<3> with a scalr and returns a
     * WorldBuilder::Point<3>.
     */
    template WorldBuilder::Point<3> operator*(const double scalar, const Point<3> &point);
  }
}

