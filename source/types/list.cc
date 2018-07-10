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
  List::List(const Interface& type, std::string description)
  :
  		inner_type(type.clone()),
			description(description)
  {
    this->type_name = type::List;
    std::cout << "Constructing a list" << std::endl;
  }


  List::~List ()
    {}

    std::shared_ptr<Interface>
    List::clone() const
    {
    	std::cout << "cloning an list" << std::endl;
    	return std::unique_ptr<Interface>(new List(*inner_type, description));
    }
  }
}

