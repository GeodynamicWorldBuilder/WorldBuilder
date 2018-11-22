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
#include <world_builder/types/plugin_system.h>

#include <world_builder/assert.h>
#include <world_builder/parameters.h>

namespace WorldBuilder
{
  namespace Types
  {
    PluginSystem::PluginSystem(void ( *declare_entries)(Parameters &, const std::string &), const bool allow_multiple)
      :
      declare_entries(declare_entries),
      allow_multiple(allow_multiple),
      description("")
    {
      this->type_name = Types::type::PluginSystem;
    }

    PluginSystem::PluginSystem(const std::string &description)
      :
      description(description)
    {
      this->type_name = Types::type::PluginSystem;
    }

    PluginSystem::PluginSystem(PluginSystem &feature)
      :
      description(feature.description)
    {
      this->type_name = Types::type::PluginSystem;
    }

    PluginSystem::~PluginSystem ()
    {}

    std::unique_ptr<Interface>
    PluginSystem::clone() const
    {
      return std::unique_ptr<Interface>(new PluginSystem(description));
    }

    void
    PluginSystem::write_schema(Parameters &prm,
                               const std::string name,
                               const std::string default_value,
                               const bool required,
                               const std::string documentation) const
    {
      using namespace rapidjson;
      Document &declarations = prm.declarations;
      const std::string path = prm.get_full_json_path();

      {
        prm.enter_subsection(name);
        {
          Pointer((prm.get_full_json_path() + "/default value").c_str()).Set(prm.declarations,default_value.c_str());

          if (allow_multiple)
            {
              Pointer((prm.get_full_json_path() + "/type").c_str()).Set(prm.declarations,"array");

              prm.enter_subsection("items");
              {
                WBAssert(this->declare_entries != NULL, "No declare entries given.");
                //std::cout << path << " and name = " << name << std::endl;
                this->declare_entries(prm, name);//Features::Interface::declare_entries(prm);
              }
              prm.leave_subsection();
            }
          else
            {
              Pointer((prm.get_full_json_path() + "/type").c_str()).Set(prm.declarations,"object");
              //prm.enter_subsection("properties");
              //{
              WBAssert(this->declare_entries != NULL, "No declare entries given.");
              this->declare_entries(prm, name);//Features::Interface::declare_entries(prm);
              //}
              //prm.leave_subsection();
            }
        }
        prm.leave_subsection();
      }
      //prm.leave_subsection();
    }
  }
}

