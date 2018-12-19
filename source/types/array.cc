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
#include <world_builder/types/array.h>
#include <world_builder/assert.h>
#include <world_builder/parameters.h>

namespace WorldBuilder
{
  namespace Types
  {
    Array::Array(const Interface &type,
                 const unsigned int min_items,
                 const unsigned int max_items,
                 const bool unique_items)
      :
      inner_type(type.get_type()),
      inner_type_ptr(type.clone()),
      required(false),
      min_items(min_items),
      max_items(max_items),
      unique_items(unique_items)
    {
      this->type_name = Types::type::Array;
    }

    // deprecated
    Array::Array(const Interface &type, const std::string &description)
      :
      inner_type(type.get_type()),
      inner_type_ptr(type.clone()),
      description(description)
    {
      this->type_name = Types::type::Array;
    }

    // deprecated
    Array::Array(const Array &type)
      :
      inner_type(type.inner_type),
      inner_type_ptr(nullptr),
      inner_type_index(type.inner_type_index),
      description(type.description)
    {
      this->type_name = Types::type::Array;
    }

    // deprecated
    Array::Array(const std::vector<unsigned int> &inner_type_index, const Types::type inner_type, const std::string &description)
      :
      inner_type(inner_type),
      inner_type_ptr(nullptr),
      inner_type_index(inner_type_index),
      description(description)
    {
      this->type_name = Types::type::Array;
    }

    Array::~Array ()
    {}

    std::unique_ptr<Interface>
    Array::clone() const
    {
      WBAssertThrow(false,"probably wrong code");
      return std::unique_ptr<Interface>(new Array(inner_type_index, inner_type, description));
    }

    void
    Array::write_schema(Parameters &prm,
                        const std::string &name,
                        const std::string &documentation) const
    {
      using namespace rapidjson;
      Document &declarations = prm.declarations;
      const std::string &base = prm.get_full_json_path() + "/" + name;

      Pointer((base + "/type").c_str()).Set(declarations,"array");
      Pointer((base + "/minItems").c_str()).Set(declarations,min_items);
      Pointer((base + "/maxItems").c_str()).Set(declarations,max_items);
      Pointer((base + "/uniqueItems").c_str()).Set(declarations,unique_items);
      Pointer((base + "/documentation").c_str()).Set(declarations,documentation.c_str());

      prm.enter_subsection(name);
      {
        WBAssertThrow(this->inner_type_ptr != NULL, "Internal error, inner pointer is NULL.");
        this->inner_type_ptr->write_schema(prm, "items", "");
      }
      prm.leave_subsection();


    }
  }
}

