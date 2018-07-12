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
#include <world_builder/types/list.h>
#include <world_builder/assert.h>

namespace WorldBuilder
{
  namespace Types
  {
    List::List(const Interface &type, const std::string &description)
      :
      inner_type_ptr(type.clone()),
      inner_type(type.get_type()),
      description(description)
    {
      this->type_name = type::List;
    }

    List::List(const List &type)
      :
      name(type.name),
      inner_type(type.get_type()),
      description(type.description)
    {
      this->type_name = type::List;
    }

    List::List(const std::string &name, const std::vector<unsigned int> &inner_type_index, const Types::type inner_type, const std::string &description)
      :
      name(name),
      inner_type_index(inner_type_index),
      inner_type(inner_type),
      description(description)
    {
      this->type_name = type::List;
    }


    List::~List ()
    {}

    std::unique_ptr<Interface>
    List::clone() const
    {
      return std::unique_ptr<Interface>(new List(name,inner_type_index, inner_type, description));
    }
  }
}

