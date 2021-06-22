/*
  Copyright (C) 2018 - 2021 by the authors of the World Builder code.

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
#include <world_builder/types/bool.h>
#include <world_builder/assert.h>
#include <world_builder/utilities.h>
#include <world_builder/parameters.h>

namespace WorldBuilder
{
  namespace Types
  {
    Bool::Bool(const bool default_value_)
      :
      default_value(default_value_)
    {
      this->type_name = Types::type::Bool;
    }


    Bool::Bool(Bool const &other)
      :
      default_value(other.default_value)
    {
      this->type_name = Types::type::Bool;
    }


    Bool::~Bool ()
      = default;


    void
    Bool::write_schema(Parameters &prm,
                       const std::string &name,
                       const std::string &documentation) const
    {
      using namespace rapidjson;
      Document &declarations = prm.declarations;
      const std::string base = prm.get_full_json_path() + "/" + name;

      Pointer((base + "/default value").c_str()).Set(declarations,default_value);
      Pointer((base + "/type").c_str()).Set(declarations,"boolean");
      Pointer((base + "/documentation").c_str()).Set(declarations,documentation.c_str());
    }
  }
}

