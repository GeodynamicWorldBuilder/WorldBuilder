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

#include <boost/algorithm/string.hpp>

#include <world_builder/features/interface.h>
#include <world_builder/features/continental_plate.h>
#include <world_builder/features/oceanic_plate.h>
#include <world_builder/features/subducting_plate.h>
#include <world_builder/features/mantle_layer.h>
#include <world_builder/assert.h>


namespace WorldBuilder
{
  namespace Features
  {
    std::map<std::string, ObjectFactory *> Interface::factories;

    Interface::Interface()
    {}

    Interface::~Interface ()
    {}

    void
    Interface::registerType(
      const std::string &name, ObjectFactory *factory)
    {
      factories[name] = factory;
    }

    std::unique_ptr<Interface>
    Interface::create(const std::string &name, WorldBuilder::World *world)
    {
      std::string lower_case_name;
      std::transform(name.begin(),
                     name.end(),
                     std::back_inserter(lower_case_name),
                     ::tolower);;

      // Have a nice assert message to check whether a plugin exists in the case
      // of a debug compilation.
      WBAssert(factories.find(lower_case_name) != factories.end(),
               "Internal error: Plugin with name '" << lower_case_name << "' is not found. "
               "The size of factories is " << factories.size() << ".");

      // Using at() because the [] will just insert values
      // which is undesirable in this case. An exception is
      // thrown when the name is not present.
      return factories.at(lower_case_name)->create(world);
    }

  }
}

