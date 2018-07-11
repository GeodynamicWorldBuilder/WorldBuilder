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
#include <world_builder/types/coordinate_system.h>
#include <world_builder/assert.h>
//#include <world_builder/coordinate_systems/interface.h>

namespace WorldBuilder
{
  namespace Types
  {
    CoordinateSystem::CoordinateSystem(std::string default_value, std::string description)
      :
      default_value(default_value),
      description(description)
    {
      this->type_name = type::CoordinateSystem;
    }

    CoordinateSystem::~CoordinateSystem ()
    {}

    void
    CoordinateSystem::set_value(std::string value_)
    {
      value = CoordinateSystems::create_coordinate_system(value_);
    }

    std::shared_ptr<Interface>
    CoordinateSystem::clone() const
    {
      return std::make_shared<CoordinateSystem>(CoordinateSystem(default_value, description));
    }
  }
}

