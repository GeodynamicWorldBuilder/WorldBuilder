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
#include <world_builder/types/feature.h>
#include <world_builder/assert.h>

namespace WorldBuilder
{
  namespace Types
  {
    Feature::Feature(const std::string &description)
      :
      description(description)
    {
      this->type_name = Types::type::Feature;
    }

    Feature::Feature(Feature &feature)
      :
      description(feature.description)
    {
      this->type_name = Types::type::Feature;
    }

    Feature::~Feature ()
    {}

    std::unique_ptr<Interface>
    Feature::clone() const
    {
      return std::unique_ptr<Interface>(new Feature(description));
    }
  }
}

