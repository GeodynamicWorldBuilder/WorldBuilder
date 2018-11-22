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
#include <world_builder/types/segment.h>
#include <world_builder/assert.h>
#include <world_builder/utilities.h>
#include <world_builder/parameters.h>

namespace WorldBuilder
{
  namespace Types
  {
    Segment::Segment(double default_value_length,
                     WorldBuilder::Point<2> default_value_thickness,
                     WorldBuilder::Point<2> default_value_angle,
                     std::string description)
      :
      value_length(default_value_length),
      default_value_length(default_value_length),
      value_thickness(default_value_thickness),
      default_value_thickness(default_value_thickness),
      value_angle(default_value_angle),
      default_value_angle(default_value_angle),
      description(description)
    {
      this->type_name = Types::type::Segment;
    }

    Segment::Segment(double   value_length,
                     double   default_value_length,
                     WorldBuilder::Point<2> value_thickness,
                     WorldBuilder::Point<2> default_value_thickness,
                     WorldBuilder::Point<2> value_angle,
                     WorldBuilder::Point<2> default_value_angle,
                     std::string description)
      :
      value_length(value_length),
      default_value_length(default_value_length),
      value_thickness(value_thickness),
      default_value_thickness(default_value_thickness),
      value_angle(value_angle),
      default_value_angle(default_value_angle),
      description(description)
    {
      this->type_name = Types::type::Segment;

    }

    Segment::~Segment ()
    {}

    std::unique_ptr<Interface>
    Segment::clone() const
    {
      return std::unique_ptr<Interface>(new Segment(value_length, default_value_length,
                                                    value_thickness, default_value_thickness,
                                                    value_angle, default_value_angle,
                                                    description));
    }

    void
    Segment::write_schema(Parameters &prm,
                          const std::string &name,
                          const std::string &documentation) const
    {
      using namespace rapidjson;
      Document &declarations = prm.declarations;
      const std::string path = prm.get_full_json_path();
      Pointer((path + "/type").c_str()).Set(declarations,"object");
      const std::string base = path + "/properties/" + name;
      //std::cout << "base name = " << base << std::endl;
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

