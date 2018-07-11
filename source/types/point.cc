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
    Point<dim>::Point(const WorldBuilder::Point<dim>& default_value, const std::string& description)
    :
		default_value(default_value),
		description(description)
    {
      this->type_name = type::Point;
      std::cout << "constructing a double" << std::endl;
    }

  template <int dim>
    Point<dim>::Point(const WorldBuilder::Point<dim>& value, const WorldBuilder::Point<dim>& default_value, const std::string& description)
    :
	value(value),
		default_value(default_value),
		description(description)
    {
      this->type_name = type::Point;
      std::cout << "constructing a Point" << std::endl;
    }

  template <int dim>
      Point<dim>::~Point ()
    {}

  template <int dim>
    std::shared_ptr<Interface>
        Point<dim>::clone() const
    {
    	std::cout << "cloning a Point" << std::endl;
    	return std::unique_ptr<Interface>(new Point(value, default_value, description));
    }

  template <int dim>
    void
	Point<dim>::set_value(WorldBuilder::Point<dim> value_)
    {
    	value = value_;
    	//std::cout << "set value of double to: " << value << std::endl;
    }

  template class Point<2>;
  template class Point<3>;
  }
}

