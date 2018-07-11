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
#include <fstream>
#include <vector>
#include <tuple>

#include <boost/property_tree/json_parser.hpp>

#include <world_builder/assert.h>
#include <world_builder/parameters.h>
#include <world_builder/utilities.h>
#include <world_builder/types/feature.h>
#include <world_builder/types/coordinate_system.h>

using boost::property_tree::ptree;

namespace WorldBuilder
{
  Parameters::Parameters(std::string& filename, World& world)
  :
		  world(world)
  {
    // Get world builder file and check wether it exists
    WBAssertThrow(access( filename.c_str(), F_OK ) != -1,
                  "Could not find the world builder file at the specified location: " + filename);

std::cout << "create parameters!" << std::endl;
    // Now read in the world builder file into a file stream and
    // put it into a boost property tree.
    std::ifstream json_input_stream(filename.c_str());
    boost::property_tree::json_parser::read_json (json_input_stream, tree);
    local_tree = &tree;

	  Utilities::print_tree(tree, 0);
  }

  Parameters::~Parameters()
  {}

  bool
  Parameters::load_entry(const std::string& name, const bool required, const Types::Interface& type)
  {
	  this->load_entry(path, name, required, type);
  }

  bool
  Parameters::load_entry(std::vector<std::string> local_path, const std::string& name, const bool required, const Types::Interface& type)
  {
	  if(local_path.size() > 0)
	    local_path.erase(local_path.begin());

	  const std::string path_plus_name = (get_current_path() == "") ? name : (get_current_path() + path_seperator + name);

	if (type.get_type() == Types::type::Double)
      {
    	// First check wheter the value is in the tree. If not Assert when the value is required,
    	// otherwise return false.
    	boost::optional<std::string> value = Utilities::get_from_ptree_abs(*local_tree,
    			                         get_current_path_without_arrays(local_path),
										 (name.front() == '[' && name.back() == ']' ? "" : name), required, path_seperator);

    	WBAssertThrow((value && required == true) || required == false, "Could not find " + get_current_path(local_path) + ", while it is set as required.");
    	if(!value)
    		return false;

    	// The value is present and we have retrieved it. Now store it
    	const Types::Double& natural_type = dynamic_cast<const Types::Double&>(type);
    	vector_double.push_back(Types::Double(natural_type.default_value,natural_type.description));
    	vector_double[vector_double.size()-1].set_value(value.get());
        string_to_type_map[path_plus_name] = std::make_pair(&vector_double[vector_double.size()-1], vector_double.size()-1);
      }
    else if (type.get_type() == Types::type::String)
      {
    	// First check wheter the value is in the tree. If not Assert when the value is required,
    	// otherwise return false.
    	boost::optional<std::string> value = Utilities::get_from_ptree_abs(*local_tree,
    	    			                         get_current_path_without_arrays(local_path),
    											 (name.front() == '[' && name.back() == ']' ? "" : name), required, path_seperator);

    	WBAssertThrow((value && required == true) || required == false, "Could not find " + get_current_path(local_path) + ", while it is set as required.");
    	if(!value)
    		return false;

    	// The value is present and we have retrieved it. Now store it
    	const Types::String& natural_type = dynamic_cast<const Types::String&>(type);
    	vector_string.push_back(Types::String(natural_type.default_value,natural_type.description));

    	vector_string[vector_string.size()-1].set_value(value.get());
        string_to_type_map[path_plus_name] = std::make_pair(&vector_string[vector_string.size()-1], vector_string.size()-1);

        return true;
      }
    else if (type.get_type() == Types::type::CoordinateSystem)
      {
    	const Types::CoordinateSystem& natural_type = dynamic_cast<const Types::CoordinateSystem&>(type);
    	const std::string path_tmp = (get_current_path() == "") ? name : (get_current_path() + path_seperator + name);

        boost::optional<ptree &> child = local_tree->get_child_optional(path_tmp);
        WBAssertThrow((child && required == true) || required == false, "Could not find " + path_tmp + ", while it is set as required.");
        if (child)
          {
        	// only one entry allowed. For now we take the first one
        	// Todo: assert when there are more entries
        	std::string system = child.get().begin()->first;
        	coordinate_system = CoordinateSystems::create_coordinate_system(system);
          }
        else
        	return false;
      }

    else if (type.get_type() == Types::type::Feature)
      {
    	const Types::Feature& natural_type = dynamic_cast<const Types::Feature&>(type);
    	std::string path_tmp = (get_current_path() == "") ? name : (get_current_path() + path_seperator + name);

                features.push_back(Features::create_feature(name, &world));
                std::string path_tmp2 = "Surface objects" + World::path_seperator + name;

                features.back()->decare_entries(path_tmp);
      }
    else if (type.get_type() == Types::type::List)
      {
    	  const std::string path_plus_name_without_arrays = ((get_current_path_without_arrays() == "") ? name : (get_current_path_without_arrays() + path_seperator + name));

          boost::optional<ptree &> child = local_tree->get_child_optional(path_plus_name_without_arrays);
          WBAssertThrow((child && required == true) || required == false, "Could not find " + get_current_path() + path_seperator + name + ", while it is set as required.");
          if (child)
            {
          	const Types::List& natural_type = dynamic_cast<const Types::List&>(type);
          	vector_list.push_back(Types::List(*natural_type.inner_type,natural_type.description));
              string_to_type_map[get_current_path() + path_seperator + name] = std::make_pair(&vector_list[vector_list.size()-1],

              unsigned int current_size = 0;
              for (boost::property_tree::ptree::iterator it = child.get().begin(); it != child.get().end(); ++it)
                {
                  //TODO add a mangle for the name
              	std::cout << "processing: " << it->first << std::endl;
              	ptree* parent = local_tree;
              	local_tree = &(it->second);
              	this->load_entry(local_path,  it->first, required, *(natural_type.inner_type));
              	local_tree = parent;
                  current_size++;
                }
            }
      }
    else if (type.get_type() == Types::type::Array)
      {
  	  const std::string path_plus_name_without_arrays = ((get_current_path_without_arrays() == "") ? "" : (get_current_path_without_arrays() + path_seperator + ""))
  			                                            + (name.front() == '[' && name.back() == ']' ? "" : name);
    	std::cout << "I am an array with name " << name << ", and path: \'" << path_plus_name_without_arrays << "\'!"  << std::endl;
        boost::optional<ptree &> child = local_tree->get_child_optional(path_plus_name_without_arrays);
        WBAssertThrow((child && required == true) || required == false, "Could not find " + get_current_path() + path_seperator + name + ", while it is set as required.");
        if (child)
          {
        	const Types::Array& natural_type = dynamic_cast<const Types::Array&>(type);
        	vector_array.push_back(Types::Array(*natural_type.inner_type,natural_type.description));
            string_to_type_map[get_current_path() + path_seperator + name] = std::make_pair(&vector_array[vector_array.size()-1], vector_array.size()-1);


            unsigned int current_size = 0;
            for (boost::property_tree::ptree::iterator it = child.get().begin(); it != child.get().end(); ++it)
              {
                //TODO add a mangle for the name
            	ptree* parent = local_tree;
            	local_tree = &(it->second);
                this->load_entry(local_path, "["+ std::to_string(current_size) + "]", required, *(natural_type.inner_type));
                local_tree = parent;
                current_size++;
              }
            std::cout << "The size of this array was: " << current_size << std::endl;
          }
      }
    else
    {
    	WBAssertThrow(false,"Type not defined.");
    }
  }

  void
  Parameters::enter_subsection(const std::string name)
  {
	  path.push_back(name);
	  //TODO: WBAssert(is path valid?)
  }

  void
  Parameters::leave_subsection()
  {
	  path.pop_back();
  }

  double
  Parameters::get_double(const std::string& name)
  {
	  return vector_double[string_to_type_map[get_current_path() + path_seperator + name].second].value;
  }

  std::string
  Parameters::get_string(const std::string& name)
  {
	  return vector_string[string_to_type_map[get_current_path() + path_seperator + name].second].value;
  }

  template<int dim>
  Point<dim>
  Parameters::get_point(const std::string& name)
  {
	  if(dim == 2)
		  return vector_2dpoint[string_to_type_map[get_current_path() + path_seperator + name].second].value;
	  else if(dim == 3)
		  return vector_3dpoint[string_to_type_map[get_current_path() + path_seperator + name].second].value;
  }

  std::string
  Parameters::get_current_path()
  {
	  std::string collapse = "";
	  for(unsigned int i = 0; i < path.size(); i++)
	  {
		  collapse += path[i] + path_seperator;
	  }
	  return collapse.substr(0,collapse.size()-path_seperator.size());
  }

  std::string
  Parameters::get_current_path(std::vector<std::string>& path)
  {
	  std::string collapse = "";
	  for(unsigned int i = 0; i < path.size(); i++)
	  {
		  collapse += path[i] + path_seperator;
	  }
	  return collapse.substr(0,collapse.size()-path_seperator.size());
  }

  std::string
  Parameters::get_current_path_without_arrays()
  {
	  std::string collapse = "";
	  for(unsigned int i = 0; i < path.size(); i++)
	  {
		  collapse += (path[i].front() == '[' && path[i].back() == ']' ? "" : path[i]) + path_seperator;
	  }
	  return collapse.substr(0,collapse.size()-path_seperator.size());
  }


  std::string
  Parameters::get_current_path_without_arrays(std::vector<std::string>& path)
  {
	  std::string collapse = "";
	  for(unsigned int i = 0; i < path.size(); i++)
	  {
		  collapse += (path[i].front() == '[' && path[i].back() == ']' ? "" : path[i]) + path_seperator;
	  }
	  return collapse.substr(0,collapse.size()-path_seperator.size());
  }
}

