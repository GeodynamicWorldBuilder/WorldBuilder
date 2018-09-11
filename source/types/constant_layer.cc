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
#include <world_builder/types/constant_layer.h>
#include <world_builder/assert.h>
#include <world_builder/utilities.h>

namespace WorldBuilder
{
  namespace Types
  {
    ConstantLayer::ConstantLayer(int default_value_composition,
                                 double default_value,
                                 double default_value_thickness,
                                 std::string description)
      :
      value_composition(default_value_composition),
      default_value_composition(default_value_composition),
      value(default_value),
      default_value(default_value),
      value_thickness(default_value_thickness),
      default_value_thickness(default_value_thickness),
      description(description)
    {
      this->type_name = type::ConstantLayer;
    }

    ConstantLayer::ConstantLayer(int value_composition,
                                 int default_value_composition,
                                 double value,
                                 double default_value,
                                 double value_thickness,
                                 double default_value_thickness,
                                 std::string description)
      :
      value_composition(value_composition),
      default_value_composition(default_value_composition),
      value(value),
      default_value(default_value),
      value_thickness(value_thickness),
      default_value_thickness(default_value_thickness),
      description(description)
    {
      this->type_name = type::ConstantLayer;

    }

    ConstantLayer::~ConstantLayer ()
    {}

    std::unique_ptr<Interface>
    ConstantLayer::clone() const
    {
      return std::unique_ptr<Interface>(new ConstantLayer(value_composition, default_value_composition,
                                                          value, default_value,
                                                          value_thickness, default_value_thickness,
                                                          description));
    }

  }
}

