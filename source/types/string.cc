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
#include <world_builder/types/string.h>
#include <world_builder/assert.h>
#include <world_builder/parameters.h>

namespace WorldBuilder
{
  namespace Types
  {

    String::String()
      :
      value(""),
      default_value(""),
      description(""),
      restricted_values({})
    {
      this->type_name = Types::type::String;
    }

    String::String(const std::string restricted_value)
      :
      value(""),
      default_value(""),
      description(""),
      restricted_values({restricted_value})
    {
      this->type_name = Types::type::String;
    }


    String::String(const std::vector<std::string> &restricted_values)
      :
      value(""),
      default_value(""),
      description(""),
      restricted_values(restricted_values)
    {
      this->type_name = Types::type::String;
    }

    String::String(std::string default_value, std::string description)
      :
      value(default_value),
      default_value(default_value),
      description(description)
    {
      this->type_name = Types::type::String;
    }

    String::String(std::string value, std::string default_value, std::string description)
      :
      value(value),
      default_value(default_value),
      description(description)
    {
      this->type_name = Types::type::String;
    }

    String::~String ()
    {}

    void
    String::write_schema(Parameters &prm,
                         const std::string name,
                         const std::string default_value,
                         const bool required,
                         const std::string documentation) const
    {
      using namespace rapidjson;
      Document &declarations = prm.declarations;
      const std::string path = prm.get_full_json_path();
      //Pointer((path + "/type").c_str()).Set(declarations,"object");
      const std::string base = path + "/" + name;
      //std::cout << "base name = " << base << std::endl;
      Pointer((base + "/default value").c_str()).Set(declarations,default_value.c_str());
      Pointer((base + "/required").c_str()).Set(declarations,required);
      Pointer((base + "/type").c_str()).Set(declarations,"string");
      Pointer((base + "/documentation").c_str()).Set(declarations,documentation.c_str());
      for (unsigned int i = 0; i < restricted_values.size(); ++i)
        {
          if (restricted_values[i] != "")
            {
              //std::cout << "enum = " << Pointer((base + "/enum").c_str()).Get(declarations) << std::endl;
              if (i == 0 && Pointer((base + "/enum").c_str()).Get(declarations) == NULL)
                {
                  //std::cout << base  + "/enum" << ", new enum! " << restricted_values[i] << std::endl;
                  // The enum array doesn't exist yet, so we create it and fill it.
                  Pointer((base + "/enum/0").c_str()).Create(declarations);
                  Pointer((base + "/enum/0").c_str()).Set(declarations, restricted_values[i].c_str());
                }
              else
                {
                  //std::cout << "fill enum! " << restricted_values[i] << std::endl;
                  // The enum array already exist yet, so we add an element to the end.
                  Pointer((base + "/enum/-").c_str()).Set(declarations, restricted_values[i].c_str());
                }
            }
        }
      /*if (required)
        {
          //std::cout << "required = " << Pointer((base + "/required").c_str()).Get(declarations) << std::endl;
          if (Pointer((path + "/required").c_str()).Get(declarations) == NULL)
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

    void
    String::set_value(std::string value_)
    {
      value = value_;
    }

    std::unique_ptr<Interface>
    String::clone() const
    {
      return std::unique_ptr<Interface>(new String(value, default_value, description));
    }
  }
}

