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
#include <world_builder/parameters.h>
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
      this->type_name = Types::type::CoordinateSystem;
    }

    CoordinateSystem::~CoordinateSystem ()
    {}

    std::unique_ptr<Interface>
    CoordinateSystem::clone() const
    {
      return std::unique_ptr<CoordinateSystem>(new CoordinateSystem(default_value, description));
    }

    void
    CoordinateSystem::write_schema(Parameters &prm,
                                   const std::string name,
                                   const std::string default_value,
                                   const bool required,
                                   const std::string documentation) const
    {
      using namespace rapidjson;
      Document &declarations = prm.declarations;
      const std::string path = prm.get_full_json_path();
      const std::string type_name = "double";
      Pointer((path + "/type").c_str()).Set(declarations,"object");
      const std::string base = path + "/properties/" + name;
      std::cout << "base name = " << base << std::endl;
      WBAssertThrow(false,"Not implemented.");
      /*Pointer((base + "/default").c_str()).Set(declarations,default_value.c_str());
      Pointer((base + "/required").c_str()).Set(declarations,required);
      Pointer((base + "/type").c_str()).Set(declarations,type_name.c_str());
      Pointer((base + "/documentation").c_str()).Set(declarations,documentation.c_str());
      if(required)
      {
        if(Pointer((path + "/required").c_str()).Get(declarations) == NULL)
        {
          // The required array doesn't exist yet, so we create it and fill it.
          Pointer((path + "/required/0").c_str()).Create(declarations);
          Pointer((path + "/required/0").c_str()).Set(declarations, name.c_str());
        }
        else
        {
          // The required array already exist yet, so we add an element to the end.
          Pointer((path + "/required/-").c_str()).Set(declarations, name.c_str());
        }
      }*/
    }
  }
}

