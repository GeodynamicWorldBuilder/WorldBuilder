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
    Segment::Segment(const double default_length,
                     const WorldBuilder::Point<2> default_thickness,
                     const WorldBuilder::Point<2> default_top_truncation,
                     const WorldBuilder::Point<2> default_angle,
                     const Types::Interface &temperature_pugin_system_,
                     const Types::Interface &composition_pugin_system_)
      :
      value_length(default_length),
      default_length(default_length),
      value_thickness(default_thickness),
      default_thickness(default_thickness),
      default_top_truncation(default_top_truncation),
      value_angle(default_angle),
      default_angle(default_angle),
      temperature_pugin_system(temperature_pugin_system_.clone()),
      composition_pugin_system(composition_pugin_system_.clone())
    {
      this->type_name = Types::type::Segment;
    }

    // todo remove function
    Segment::Segment(double default_length,
                     WorldBuilder::Point<2> default_thickness,
                     WorldBuilder::Point<2> default_angle,
                     std::string description)
      :
      value_length(default_length),
      default_length(default_length),
      value_thickness(default_thickness),
      default_top_truncation(default_thickness),
      default_thickness(default_thickness),
      value_angle(default_angle),
      default_angle(default_angle),
      description(description)
    {
      this->type_name = Types::type::Segment;
    }

    // todo update function
    Segment::Segment(const double default_length,
                     const WorldBuilder::Point<2> default_thickness,
                     const WorldBuilder::Point<2> default_top_truncation,
                     const WorldBuilder::Point<2> default_angle,
                     const std::unique_ptr<Types::Interface> &temperature_pugin_system_,
                     const std::unique_ptr<Types::Interface> &composition_pugin_system_)
      :
      value_length(default_length),
      default_length(default_length),
      value_thickness(default_thickness),
      default_thickness(default_thickness),
      default_top_truncation(default_top_truncation),
      value_angle(default_angle),
      default_angle(default_angle),
      temperature_pugin_system(temperature_pugin_system_->clone()),
      composition_pugin_system(composition_pugin_system_->clone())
    {
      this->type_name = Types::type::Segment;

    }

    Segment::~Segment ()
    {}

    // todo update function
    std::unique_ptr<Interface>
    Segment::clone() const
    {
      return std::unique_ptr<Interface>(new Segment(default_length,
                                                    default_thickness,
                                                    default_top_truncation,
                                                    default_angle,
                                                    temperature_pugin_system,
                                                    composition_pugin_system));
    }

    void
    Segment::write_schema(Parameters &prm,
                          const std::string &name,
                          const std::string &documentation) const
    {
      using namespace rapidjson;
      Document &declarations = prm.declarations;
      prm.enter_subsection(name);
      {
        std::string base = prm.get_full_json_path();

        Pointer((base + "/type").c_str()).Set(declarations,"object");
        Pointer((base + "/additionalProperties").c_str()).Set(declarations,false);
        Pointer((base + "/documentation").c_str()).Set(declarations,documentation.c_str());
        prm.enter_subsection("properties");
        {
          std::string base = prm.get_full_json_path();
          Pointer((base + "/length/type").c_str()).Set(declarations,"number");

          Pointer((base + "/thickness/type").c_str()).Set(declarations,"array");
          Pointer((base + "/thickness/minItems").c_str()).Set(declarations,1);
          Pointer((base + "/thickness/maxItems").c_str()).Set(declarations,2);
          Pointer((base + "/thickness/items/type").c_str()).Set(declarations,"number");

          Pointer((base + "/top trunctation/type").c_str()).Set(declarations,"array");
          Pointer((base + "/top trunctation/minItems").c_str()).Set(declarations,1);
          Pointer((base + "/top trunctation/maxItems").c_str()).Set(declarations,2);
          Pointer((base + "/top trunctation/items/type").c_str()).Set(declarations,"number");

          Pointer((base + "/angle/type").c_str()).Set(declarations,"array");
          Pointer((base + "/angle/minItems").c_str()).Set(declarations,1);
          Pointer((base + "/angle/maxItems").c_str()).Set(declarations,2);
          Pointer((base + "/angle/items/type").c_str()).Set(declarations,"number");

          temperature_pugin_system->write_schema(prm, "temperature models", "");
          composition_pugin_system->write_schema(prm, "composition models", "");
        }
        prm.leave_subsection();
      }
      prm.leave_subsection();

    }

  }
}

