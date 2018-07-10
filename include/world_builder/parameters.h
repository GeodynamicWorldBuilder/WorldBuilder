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

#ifndef _world_builder_parameters_h
#define _world_builder_parameters_h

#include <string>
#include <vector>
#include <unordered_map>

#include <boost/property_tree/ptree.hpp>

#include <world_builder/types/interface.h>
#include <world_builder/types/double.h>
#include <world_builder/types/string.h>
#include <world_builder/types/array.h>
#include <world_builder/types/list.h>


//#include <world_builder/coordinate_systems/interface.h>
//#include <world_builder/features/interface.h>


using boost::property_tree::ptree;

namespace WorldBuilder
{

namespace Features
{
class Interface;
}

namespace CoordinateSystems
{
class Interface;
}

class World;

  class Parameters
  {
    public:
      /**
       * Constructor
       */
      Parameters(std::string& filename, World&);

      /**
       * Destructor
       */
      ~Parameters();

      void declare_entry(const std::string& name, const bool required, const Types::Interface& type);

      void enter_subsection(const std::string name);

      void leave_subsection();

      World& world;

      const std::string path_seperator = ".";
      std::vector<std::string> path;
      ptree tree;

      std::unordered_map<std::string, std::pair<Types::Interface*, unsigned int> > string_to_type_map;
      std::vector<Types::Double> vector_double;
      std::vector<Types::String> vector_string;
      std::vector<Types::Array> vector_array;
      std::vector<Types::List> vector_list;


      /**
       * contains all the plugins.
       */
      std::vector<std::shared_ptr<WorldBuilder::Features::Interface> > features;

      // coordinate system
      std::shared_ptr<WorldBuilder::CoordinateSystems::Interface> coordinate_system;

    private:
      void declare_entry(const ptree& tree, std::vector<std::string> path, const std::string& name, const bool required, const Types::Interface& type);

      std::string get_current_path();

      std::string get_current_path_without_arrays();

      /**
       * No reference, but copy to be able to change it locally
       */
      std::string get_current_path(std::vector<std::string>& path);

      /**
       * No reference, but copy to be able to change it locally
       */
      std::string get_current_path_without_arrays(std::vector<std::string>& path);
  };
}
#endif
