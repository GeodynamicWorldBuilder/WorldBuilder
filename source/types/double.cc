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
#include <world_builder/types/double.h>
#include <world_builder/assert.h>
#include <world_builder/utilities.h>

namespace WorldBuilder
{
  namespace Types
  {
    Double::Double( std::string default_value, std::string description)
    :
		default_value(default_value),
		description(description)
    {
      this->type_name = type::Double;
      //TODO: Think about what should happen when the default value is empty
      if(default_value != "")
    	  value = Utilities::string_to_double(default_value);
      std::cout << "constructing a double" << std::endl;
    }

    Double::~Double ()
    {}

    std::shared_ptr<Interface>
    Double::clone() const
    {
    	std::cout << "cloning a double" << std::endl;
    	return std::unique_ptr<Interface>(new Double(std::to_string(value), description));
    }

    void
	Double::set_value(std::string value_)
    {
    	value = Utilities::string_to_double(value_);
    	std::cout << "set value of double to: " << value << std::endl;
    }
  }
}

