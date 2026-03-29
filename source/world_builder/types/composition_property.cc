/*
  Copyright (C) 2018-2026 by the authors of the World Builder code.

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
#include "world_builder/types/composition_property.h"
#include "world_builder/parameters.h"

namespace WorldBuilder
{
  namespace Types
  {
    CompositionProperty::CompositionProperty()
    // constructor
    // default values for the schema
      :
      required({"index"}),
             default_reference_density(3300.0)
    {
      this->type_name = Types::type::Object;
    }

    void
    CompositionProperty::write_schema(Parameters &prm,
                                      const std::string &name,
                                      const std::string &documentation) const
    {
      using namespace rapidjson;
      Document &declarations = prm.declarations;
      const std::string strict_base = prm.get_full_json_path() + "/" + name;

      Pointer((strict_base + "/type").c_str()).Set(declarations,"object");
      Pointer((strict_base + "/description").c_str()).Set(declarations,documentation.c_str());
      Pointer((strict_base + "/additionalProperties").c_str()).Set(declarations,false);

      if (!required.empty())
        {
          for (unsigned int i = 0; i < required.size(); ++i)
            {
              if (i == 0 && Pointer((strict_base + "/required").c_str()).Get(declarations) == nullptr)
                {
                  Pointer((strict_base + "/required/0").c_str()).Create(declarations);
                  Pointer((strict_base + "/required/0").c_str()).Set(declarations, required[i].c_str());
                }
              else
                {
                  Pointer((strict_base + "/required/-").c_str()).Set(declarations, required[i].c_str());
                }
            }
        }

      // named properties and their defaults
      Pointer((strict_base + "/properties/index/type").c_str()).Set(declarations,"integer");
      Pointer((strict_base + "/properties/index/minimum").c_str()).Set(declarations,0);
      Pointer((strict_base + "/properties/index/description").c_str()).Set(declarations,
                                                                           "The composition index used in composition lookups.");

      Pointer((strict_base + "/properties/name/type").c_str()).Set(declarations,"string");
      Pointer((strict_base + "/properties/name/default value").c_str()).Set(declarations, "");
      Pointer((strict_base + "/properties/name/description").c_str()).Set(declarations,
                                                                          "Optional composition name. Defaults to <index>.");

      Pointer((strict_base + "/properties/reference density/type").c_str()).Set(declarations,"number");
      Pointer((strict_base + "/properties/reference density/default value").c_str()).Set(declarations, CompositionProperty::get_default_reference_density());
      Pointer((strict_base + "/properties/reference density/description").c_str()).Set(declarations,
          "Optional reference density in kg/m^3. Defaults to 3300.");
    }
  } // namespace Types
} // namespace WorldBuilder

