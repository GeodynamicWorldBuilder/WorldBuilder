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
#include <world_builder/types/segment.h>
#include <world_builder/types/constant_layer.h>
#include <world_builder/types/coordinate_system.h>
#include <config.h>

using boost::property_tree::ptree;

namespace WorldBuilder
{
  Parameters::Parameters(std::string &filename, World &world)
    :
    world(world),
    path_level(0)
  {
    // Get world builder file and check wether it exists
    WBAssertThrow(access( filename.c_str(), F_OK ) != -1,
                  "Could not find the world builder file at the specified location: " + filename);
    // Now read in the world builder file into a file stream and
    // put it into a boost property tree.
    std::ifstream json_input_stream(filename.c_str());
    boost::property_tree::json_parser::read_json (json_input_stream, tree);
    local_tree = &tree;
  }

  Parameters::~Parameters()
  {}

  bool
  Parameters::load_entry(const std::string &name, const bool required, const Types::Interface &type)
  {
    //Todo: the loading of the entry should actually be performed by the type
    unsigned int location;
    return this->load_entry(name, required, type, location);
  }

  bool
  Parameters::load_entry(const std::string &name, const bool required, const Types::Interface &type, unsigned int &location)
  {
    bool found_value = false;

    const std::string path_plus_name = (get_full_path() == "") ? name : (get_full_path() + path_seperator + name);

    if (type.get_type() == Types::type::UnsignedInt)
      {
        // First check whether the value is in the tree. If not Assert when the value is required,
        // otherwise set found_value to false.
        boost::optional<std::string> value_tree = Utilities::get_from_ptree_abs(*local_tree,
                                                                                get_relative_path_without_arrays(),
                                                                                (name.front() == '[' && name.back() == ']' ? "" : name),
                                                                                required,
                                                                                path_seperator);

        found_value = value_tree ? true : false;

        WBAssertThrow((found_value == true && required == true) || required == false,
                      "Could not find " + get_full_path() + ", while it is set as required.");

        // Store the value
        const Types::UnsignedInt &natural_type = dynamic_cast<const Types::UnsignedInt &>(type);

        const unsigned int value = found_value == true ? Utilities::string_to_unsigned_int(value_tree.get())
                                   : natural_type.default_value;

        vector_unsigned_int.push_back(Types::UnsignedInt(value,natural_type.default_value,natural_type.description));
        location = vector_unsigned_int.size()-1;
        string_to_type_map[path_plus_name] = location;

      }
    else if (type.get_type() == Types::type::Double)
      {
        // First check whether the value is in the tree. If not Assert when the value is required,
        // otherwise set found_value to false.
        boost::optional<std::string> value_tree = Utilities::get_from_ptree_abs(*local_tree,
                                                                                get_relative_path_without_arrays(),
                                                                                (name.front() == '[' && name.back() == ']' ? "" : name),
                                                                                required,
                                                                                path_seperator);
        found_value = value_tree ? true : false;

        WBAssertThrow((found_value == true && required == true) || required == false,
                      "Could not find " + get_full_path() + ", while it is set as required.");


        // The value is present and we have retrieved it. Now store it
        const Types::Double &natural_type = dynamic_cast<const Types::Double &>(type);
        const double value = found_value == true ? Utilities::string_to_double(value_tree.get()) : natural_type.default_value;
        vector_double.push_back(Types::Double(value,natural_type.default_value,natural_type.description));
        location = vector_double.size()-1;
        string_to_type_map[path_plus_name] = location;

      }
    else if (type.get_type() == Types::type::String)
      {
        // First check whether the value is in the tree. If not Assert when the value is required,
        // otherwise set found_value to false.
        boost::optional<std::string> value_tree = Utilities::get_from_ptree_abs(*local_tree,
                                                                                get_relative_path_without_arrays(),
                                                                                (name.front() == '[' && name.back() == ']' ? "" : name),
                                                                                required,
                                                                                path_seperator);


        found_value = value_tree ? true : false;

        WBAssertThrow((found_value == true && required == true) || required == false,
                      "Could not find " + get_full_path() + ", while it is set as required.");

        // The value is present and we have retrieved it. Now store it
        const Types::String &natural_type = dynamic_cast<const Types::String &>(type);
        const std::string value = value_tree ? value_tree.get() : natural_type.default_value;
        vector_string.push_back(Types::String(natural_type.default_value,natural_type.description));

        vector_string[vector_string.size()-1].set_value(value);
        location = vector_string.size()-1;
        string_to_type_map[path_plus_name] = location;

      }
    else if (type.get_type() == Types::type::Segment)
      {
        // First check whether the value is in the tree. If not Assert when the value is required,
        // otherwise set found_value to false.
        const Types::Segment &natural_type = dynamic_cast<const Types::Segment &>(type);

        // Check length value
        boost::optional<std::string> length_value_tree =
          Utilities::get_from_ptree_abs(*local_tree,
                                        get_relative_path_without_arrays(),
                                        "length",
                                        required,
                                        path_seperator);

        found_value = length_value_tree ? true : false;

        WBAssertThrow((found_value == true && required == true) || required == false,
                      "Could not find " + get_full_path() + path_seperator + "length" + ", while it is set as required.");

        // Check thickness value as a 2d point. Todo: see if it can be recursed to POINT2D
        WorldBuilder::Point<2> point_thickness(std::array<double,2> {0,0},
                                               this->coordinate_system->natural_coordinate_system());

        std::string path_plus_name_without_arrays = "thickness";//((get_relative_path_without_arrays() == "") ? "" : (get_relative_path_without_arrays() + path_seperator + ""))
        //+ (name.front() == '[' && name.back() == ']' ? "" : "thickness");

        boost::optional<ptree &> child = local_tree->get_child_optional(path_plus_name_without_arrays);

        found_value = child ? true : false;

        WBAssertThrow((found_value == true && required == true) || required == false,
                      "Could not find " + get_full_path() + path_seperator + "thickness" + ", while it is set as required.");

        unsigned int diff = path.size()-path_level;
        path_level+=diff;
        if (found_value == true)
          {
            unsigned int current_size = 0;
            for (boost::property_tree::ptree::iterator it = child.get().begin(); it != child.get().end(); ++it)
              {
                // First check whether the value is in the tree. If not Assert when the value is required,
                // otherwise return false.
                boost::optional<std::string> value_tree = Utilities::get_from_ptree_abs(it->second,
                                                                                        get_relative_path_without_arrays(),
                                                                                        "", required, path_seperator);

                found_value = value_tree ? true : false;

                WBAssertThrow((found_value == true && required == true) || required == false,
                              "Could not find " + get_full_path() + ", while it is set as required.");
                //Todo: this looks strange to me. A value should always be found, or something really weird happened...

                const double value = found_value == true ? Utilities::string_to_double(value_tree.get()) : natural_type.default_value_thickness[current_size];

                point_thickness[current_size] = value;
                current_size++;
              }

            // if only one value is filled in, set it for both.
            if (current_size == 1)
              {
                point_thickness[1] = point_thickness[0];
              }
            WBAssertThrow(current_size == 1 || current_size == 2,
                          "The entry " + get_full_path() + path_seperator + "thickness" +
                          " should contain one or two values, but the size was not 1 or 2, it was "
                          << current_size << ".");
          }
        else
          {
            point_thickness = natural_type.default_value_thickness;
          }
        path_level -= diff;


        // Check angle value as a 2d point. Todo: see if it can be recursed to POINT2D
        WorldBuilder::Point<2> point_angle(std::array<double,2> {0,0},
                                           this->coordinate_system->natural_coordinate_system());

        path_plus_name_without_arrays = "angle";//((get_relative_path_without_arrays() == "") ? "" : (get_relative_path_without_arrays() + path_seperator + ""))
        //        + (name.front() == '[' && name.back() == ']' ? "" : "angle");

        child = local_tree->get_child_optional(path_plus_name_without_arrays);

        found_value = child ? true : false;

        WBAssertThrow((found_value == true && required == true) || required == false,
                      "Could not find " + get_full_path() + path_seperator + "angle" + ", while it is set as required.");

        //unsigned int diff = path.size()-path_level;
        path_level+=diff;
        if (found_value == true)
          {
            unsigned int current_size = 0;
            for (boost::property_tree::ptree::iterator it = child.get().begin(); it != child.get().end(); ++it)
              {
                // First check whether the value is in the tree. If not Assert when the value is required,
                // otherwise return false.
                boost::optional<std::string> value_tree = Utilities::get_from_ptree_abs(it->second,
                                                                                        get_relative_path_without_arrays(),
                                                                                        "", required, path_seperator);

                found_value = value_tree ? true : false;

                WBAssertThrow((found_value == true && required == true) || required == false,
                              "Could not find " + get_full_path() + ", while it is set as required.");
                const double value = found_value == true ? Utilities::string_to_double(value_tree.get()) : natural_type.default_value_angle[current_size];

                point_angle[current_size] = value;
                current_size++;
              }

            // if only one value is filled in, set it for both.
            if (current_size == 1)
              {
                point_angle[1] = point_angle[0];
              }
            WBAssertThrow(current_size == 1 || current_size == 2,
                          "The entry " + get_full_path() + path_seperator + "angle" +
                          " should contain one or two values, but the size was not 1 or 2, it was "
                          << current_size << ".");
          }
        else
          {
            point_angle = natural_type.default_value_angle;
          }
        path_level -= diff;

        // The values are present and we have retrieved them. Now store it into a Segment type.
        const double value_length = length_value_tree ? Utilities::string_to_double(length_value_tree.get()) : natural_type.default_value_length;

        vector_segment.push_back(Types::Segment(value_length, natural_type.default_value_length,
                                                point_thickness, natural_type.default_value_thickness,
                                                point_angle, natural_type.default_value_angle,
                                                natural_type.description));

        //vector_string[vector_string.size()-1].set_value(value);
        location = vector_segment.size()-1;
        string_to_type_map[path_plus_name] = location;

      }
    else if (type.get_type() == Types::type::ConstantLayer)
      {
        // First check whether the value is in the tree. If not Assert when the value is required,
        // otherwise set found_value to false.
        const Types::ConstantLayer &natural_type = dynamic_cast<const Types::ConstantLayer &>(type);

        // Check composition value
        boost::optional<std::string> composition_value_tree =
          Utilities::get_from_ptree_abs(*local_tree,
                                        get_relative_path_without_arrays(),
                                        "composition",
                                        required,
                                        path_seperator);

        found_value = composition_value_tree ? true : false;

        WBAssertThrow((found_value == true && required == true) || required == false,
                      "Could not find " + get_full_path() + path_seperator + "composition" + ", while it is set as required.");

        // Check the value, which may be the value indicating composition or the temperature
        boost::optional<std::string> value_tree =
          Utilities::get_from_ptree_abs(*local_tree,
                                        get_relative_path_without_arrays(),
                                        "value",
                                        false,
                                        path_seperator);

        // Check thickness value
        boost::optional<std::string> thickness_value_tree =
          Utilities::get_from_ptree_abs(*local_tree,
                                        get_relative_path_without_arrays(),
                                        "thickness",
                                        required,
                                        path_seperator);


        found_value = thickness_value_tree ? true : false;

        WBAssertThrow((found_value == true && required == true) || required == false,
                      "Could not find " + get_full_path() + path_seperator + "composition" + ", while it is set as required.");



        // The values are present and we have retrieved them. Now store it into a ConstantLayer type.
        const int value_composition = composition_value_tree ? Utilities::string_to_int(composition_value_tree.get()) : natural_type.default_value_composition;
        const double value = value_tree ? Utilities::string_to_double(value_tree.get()) : natural_type.default_value;
        const double value_thickness = thickness_value_tree ? Utilities::string_to_double(thickness_value_tree.get()) : natural_type.default_value_thickness;

        vector_constant_layer.push_back(Types::ConstantLayer(value_composition, natural_type.default_value_composition,
                                                             value, natural_type.default_value,
                                                             value_thickness, natural_type.default_value_thickness,
                                                             natural_type.description));

        //vector_string[vector_string.size()-1].set_value(value);
        location = vector_constant_layer.size()-1;
        string_to_type_map[path_plus_name] = location;

      }
    else if (type.get_type() == Types::type::CoordinateSystem)
      {
        const Types::CoordinateSystem &natural_type = dynamic_cast<const Types::CoordinateSystem &>(type);
        const std::string path_tmp = (get_full_path() == "") ? name : (get_full_path() + path_seperator + name);

        boost::optional<ptree &> child = local_tree->get_child_optional(path_tmp);

        found_value = child ? true : false;

        WBAssertThrow((found_value == false && required == true) || required == false, "Could not find " + path_tmp + ", while it is set as required.");
        // only one entry allowed. For now we take the first one
        // Todo: assert when there are more entries
        std::string system = found_value == true ? child.get().begin()->first : natural_type.default_value;
        enter_subsection(name);
        {
          enter_subsection(system);
          {
            coordinate_system = CoordinateSystems::create_coordinate_system(system, &world);

            coordinate_system->decare_entries();
          }
          leave_subsection();
        }
        leave_subsection();


      }
    else if (type.get_type() == Types::type::Feature)
      {
        // Todo: redesing the path_level system.
        enter_subsection(name);
        {
          path_level++;
          features.push_back(Features::create_feature(name, &world));
          features.back()->decare_entries();
          path_level--;
        }
        leave_subsection();


        found_value = true;
      }
    else if (type.get_type() == Types::type::List)
      {
        const std::string path_plus_name_without_arrays = ((get_full_path_without_arrays() == "")
                                                           ?
                                                           name
                                                           :
                                                           (get_full_path_without_arrays() + path_seperator + name));

        boost::optional<ptree &> child = local_tree->get_child_optional(path_plus_name_without_arrays);

        found_value = child ? true : false;

        WBAssertThrow((found_value == true && required == true) || required == false,
                      "Could not find " + get_full_path() + path_seperator + name + ", while it is set as required.");
        if (child)
          {
            const Types::List &natural_type = dynamic_cast<const Types::List &>(type);
            // Todo: assert size of inner_type is one
            vector_list.push_back(Types::List(name, std::vector<unsigned int>(), natural_type.inner_type, natural_type.description));
            location = vector_list.size()-1;
            string_to_type_map[path_plus_name] = location;

            enter_subsection(name);
            {
              path_level++;
              unsigned int current_size = 0;
              for (boost::property_tree::ptree::iterator it = child.get().begin(); it != child.get().end(); ++it)
                {
                  //TODO add a mangle for the name
                  ptree *parent = local_tree;
                  local_tree = &(it->second);
                  unsigned int child_location;
                  found_value = this->load_entry(it->first, required, *natural_type.inner_type_ptr, child_location);
                  vector_list[location].inner_type_index.push_back(child_location);
                  local_tree = parent;
                  current_size++;
                }
              path_level--;
            }
            leave_subsection();
          }
        else
          {
            // TODO: set default value
          }
      }
    else if (type.get_type() == Types::type::Array)
      {

        const std::string path_plus_name_without_arrays = ((get_relative_path_without_arrays() == "") ? "" : (get_relative_path_without_arrays() + path_seperator + ""))
                                                          + (name.front() == '[' && name.back() == ']' ? path_seperator : name);

        boost::optional<ptree &> child = local_tree->get_child_optional(path_plus_name_without_arrays);

        found_value = child ? true : false;

        WBAssertThrow((found_value == true && required == true) || required == false, "Could not find " + get_full_path() + path_seperator + name + ", while it is set as required.");


        if (child)
          {
            const Types::Array &natural_type = dynamic_cast<const Types::Array &>(type);

            vector_array.push_back(Types::Array(std::vector<unsigned int>(), natural_type.inner_type,natural_type.description));

            location = vector_array.size()-1;
            string_to_type_map[path_plus_name] = location;

            unsigned int current_size = 0;
            enter_subsection(name);
            {
              //path_level++;
              for (boost::property_tree::ptree::iterator it = child.get().begin(); it != child.get().end(); ++it)
                {
                  unsigned int diff = path.size()-path_level;
                  path_level+=diff;
                  //TODO add a mangle for the name
                  ptree *parent = local_tree;
                  local_tree = &(it->second);
                  unsigned int child_location;
                  found_value = this->load_entry("["+ std::to_string(current_size) + "]", required, *natural_type.inner_type_ptr, child_location);
                  vector_array[location].inner_type_index.push_back(child_location);
                  local_tree = parent;
                  current_size++;
                  path_level -= diff;
                }
              //path_level--;
            }
            leave_subsection();
          }
        else
          {
            // TODO: set default value
          }
      }
    else if (type.get_type() == Types::type::Point2D)
      {
        WBAssertThrow(this->coordinate_system != NULL, "The coordinate system must be loaded before points are loaded.");
        const std::string path_plus_name_without_arrays = ((get_relative_path_without_arrays() == "") ? "" : (get_relative_path_without_arrays() + path_seperator + ""))
                                                          + (name.front() == '[' && name.back() == ']' ? "" : name);

        boost::optional<ptree &> child = local_tree->get_child_optional(path_plus_name_without_arrays);

        found_value = child ? true : false;

        WBAssertThrow((found_value == true && required == true) || required == false,
                      "Could not find " + get_full_path() + path_seperator + name + ", while it is set as required.");

        unsigned int diff = path.size()-path_level;
        path_level+=diff;
        const Types::Point<2> &natural_type = dynamic_cast<const Types::Point<2> &>(type);
        if (found_value == true)
          {
            WorldBuilder::Point<2> point(std::array<double,2> {0,0},
                                         this->coordinate_system->natural_coordinate_system());

            unsigned int current_size = 0;
            for (boost::property_tree::ptree::iterator it = child.get().begin(); it != child.get().end(); ++it)
              {
                // First check whether the value is in the tree. If not Assert when the value is required,
                // otherwise return false.
                boost::optional<std::string> value_tree = Utilities::get_from_ptree_abs(it->second,
                                                                                        get_relative_path_without_arrays(),
                                                                                        "", required, path_seperator);

                found_value = value_tree ? true : false;

                WBAssertThrow((found_value == true && required == true) || required == false,
                              "Could not find " + get_full_path() + ", while it is set as required.");
                const double value = found_value == true ? Utilities::string_to_double(value_tree.get()) : natural_type.default_value[current_size];

                point[current_size] = value;
                current_size++;
              }
            WBAssertThrow(current_size == 2,
                          "The entry " + get_full_path() + path_seperator + name +
                          " should represent a 2d point, but the size was not 2, it was "
                          << current_size << ".");

            vector_point_2d.push_back(Types::Point<2>(point, point,natural_type.description));
            location = vector_point_2d.size()-1;
            string_to_type_map[path_plus_name] = location;
          }
        else
          {
            vector_point_2d.push_back(natural_type);
            location = vector_point_2d.size()-1;
            string_to_type_map[path_plus_name] = location;
          }
        path_level -= diff;
      }
    else if (type.get_type() == Types::type::Point3D)
      {
        WBAssertThrow(this->coordinate_system != NULL, "The coordinate system must be loaded before points are loaded.");
        const std::string path_plus_name_without_arrays = ((get_relative_path_without_arrays() == "") ? "" : (get_relative_path_without_arrays() + path_seperator + ""))
                                                          + (name.front() == '[' && name.back() == ']' ? "" : name);

        boost::optional<ptree &> child = local_tree->get_child_optional(path_plus_name_without_arrays);

        found_value = child ? true : false;

        WBAssertThrow((found_value == true && required == true) || required == false,
                      "Could not find " + get_full_path() + path_seperator + name + ", while it is set as required.");

        unsigned int diff = path.size()-path_level;
        path_level+=diff;
        const Types::Point<3> &natural_type = dynamic_cast<const Types::Point<3> &>(type);
        if (found_value == true)
          {
            WorldBuilder::Point<3> point(std::array<double,3> {0,0,0},
                                         this->coordinate_system->natural_coordinate_system());
            unsigned int current_size = 0;
            for (boost::property_tree::ptree::iterator it = child.get().begin(); it != child.get().end(); ++it)
              {
                // First check whether the value is in the tree. If not Assert when the value is required,
                // otherwise return false.
                boost::optional<std::string> value_tree = Utilities::get_from_ptree_abs(it->second,
                                                                                        get_relative_path_without_arrays(),
                                                                                        "", required, path_seperator);

                found_value = value_tree ? true : false;

                WBAssertThrow((found_value == true && required == true) || required == false,
                              "Could not find " + get_full_path() + ", while it is set as required.");
                const double value = found_value == true ? Utilities::string_to_double(value_tree.get()) : natural_type.default_value[current_size];

                point[current_size] = value;
                current_size++;
              }
            WBAssertThrow(current_size == 3,
                          "The entry " + get_full_path() + path_seperator + name +
                          " should represent a 3d point, but the size was not 3, it was "
                          << current_size << ".");

            vector_point_3d.push_back(Types::Point<3>(point, point,natural_type.description));
            location = vector_point_3d.size()-1;
            string_to_type_map[path_plus_name] = location;
          }
        else
          {
            vector_point_3d.push_back(natural_type);
            location = vector_point_3d.size()-1;
            string_to_type_map[path_plus_name] = location;
          }
        path_level -= diff;
      }
    else
      {
        WBAssertThrow(false,"Type not defined: " << (int)type.get_type());
      }

    return found_value;
  }

  void
  Parameters::set_entry(const std::string &name, const Types::Interface &type)
  {
    unsigned int location;
    this->set_entry(name, type, location);
  }

  void
  Parameters::set_entry(const std::string &name, const Types::Interface &type, unsigned int &location)
  {

    const std::string path_plus_name = (get_full_path() == "") ? name : (get_full_path() + path_seperator + name);

    if (type.get_type() == Types::type::Double)
      {
        // The value is present and we have retrieved it. Now store it
        const Types::Double &natural_type = dynamic_cast<const Types::Double &>(type);
        vector_double.push_back(natural_type);
        location = vector_double.size()-1;
        string_to_type_map[path_plus_name] = location;
      }
    else if (type.get_type() == Types::type::UnsignedInt)
      {
        // The value is present and we have retrieved it. Now store it
        const Types::UnsignedInt &natural_type = dynamic_cast<const Types::UnsignedInt &>(type);
        vector_unsigned_int.push_back(natural_type);
        string_to_type_map[path_plus_name] = vector_unsigned_int.size()-1;
      }
    else if (type.get_type() == Types::type::String)
      {
        // The value is present and we have retrieved it. Now store it
        const Types::String &natural_type = dynamic_cast<const Types::String &>(type);
        vector_string.push_back(natural_type);
        string_to_type_map[path_plus_name] = vector_string.size()-1;
      }
    else if (type.get_type() == Types::type::CoordinateSystem)
      {

        WBAssert(false, "A Coordinate system can not be directly set, use the load_entry function.");
        /*const Types::CoordinateSystem &natural_type = dynamic_cast<const Types::CoordinateSystem &>(type);
        std::string system = natural_type.;
        coordinate_system = CoordinateSystems::create_coordinate_system(system);*/
      }

    else if (type.get_type() == Types::type::Feature)
      {
        WBAssert(false, "A Feature can not be directly set, use the load_entry function.");
      }
    /*else if (type.get_type() == Types::type::List)
      {
        const std::string path_plus_name_without_arrays = ((get_current_path_without_arrays() == "") ? name : (get_current_path_without_arrays() + path_seperator + name));


        const Types::List &natural_type = dynamic_cast<const Types::List &>(type);
        vector_list.push_back(natural_type);
        string_to_type_map[get_full_path() + path_seperator + name] = vector_list.size()-1;

        for (unsigned int i = 0; i < natural_type.inner_type.size(); ++i)
          {
            //TODO add a mangle for the name
            this->set_entry(local_path,  natural_type.name, *(natural_type.inner_type[i]));
          }

      }*/
    else if (type.get_type() == Types::type::Array)
      {
        const Types::Array &natural_type = dynamic_cast<const Types::Array &>(type);
        vector_array.push_back(Types::Array(natural_type.inner_type_index,natural_type.get_type(),natural_type.description));
        location = vector_array.size()-1;
        string_to_type_map[path_plus_name] = location;


        //unsigned int size = natural_type.inner_type.size();
        for (unsigned int i = 0; i < natural_type.inner_type_index.size(); ++i)
          {
            //TODO add a mangle for the name
            unsigned int child_location;
            this->set_entry("["+ std::to_string(i) + "]", *(natural_type.inner_type_ptr), child_location);
            vector_array[location].inner_type_index.push_back(child_location);
          }
      }
    else if (type.get_type() == Types::type::Point2D)
      {
        // 2d
        const Types::Point<2> &natural_type = dynamic_cast<const Types::Point<2> &>(type);

        vector_point_2d.push_back(natural_type);
        location = vector_point_2d.size()-1;
        string_to_type_map[path_plus_name] = location;
      }
    else if (type.get_type() == Types::type::Point3D)
      {
        const Types::Point<3> &natural_type = dynamic_cast<const Types::Point<3> &>(type);

        vector_point_3d.push_back(natural_type);
        location = vector_point_3d.size()-1;
        string_to_type_map[path_plus_name] = location;
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

  unsigned int
  Parameters::get_unsigned_int(const std::string &name) const
  {
    const std::string path_plus_name = get_full_path() == "" ? name : get_full_path() + path_seperator + name;
    WBAssert(string_to_type_map.count(path_plus_name) > 0, "Could not find entry \'" << name << "\' not found. Make sure it is loaded or set.");
    //Todo: there is a problem that when the wrong get function is used. Have to find a way to fix this.
    return vector_unsigned_int[string_to_type_map.at(path_plus_name)].value;
  }

  double
  Parameters::get_double(const std::string &name) const
  {
    const std::string path_plus_name = get_full_path() == "" ? name : get_full_path() + path_seperator + name;
    WBAssert(string_to_type_map.count(path_plus_name) > 0, "Could not find entry \'" << name << "\' not found. Make sure it is loaded or set.");
    return vector_double[string_to_type_map.at(path_plus_name)].value;
  }

  std::string
  Parameters::get_string(const std::string &name) const
  {
    const std::string path_plus_name = get_full_path() == "" ? name : get_full_path() + path_seperator + name;
    WBAssert(string_to_type_map.count(path_plus_name) > 0, "Could not find entry \'" << name << "\' not found. Make sure it is loaded or set.");
    return vector_string[string_to_type_map.at(path_plus_name)].value;
  }

  template<>
  Point<2>
  Parameters::get_point(const std::string &name) const
  {
    const std::string path_plus_name = get_full_path() == "" ? name : get_full_path() + path_seperator + name;
    WBAssert(string_to_type_map.count(path_plus_name) > 0, "Could not find entry \'" << name << "\' not found. Make sure it is loaded or set.");
    return vector_point_2d[string_to_type_map.at(path_plus_name)].value;
  }

  template<>
  Point<3>
  Parameters::get_point(const std::string &name) const
  {
    const std::string path_plus_name = get_full_path() == "" ? name : get_full_path() + path_seperator + name;
    WBAssert(string_to_type_map.count(path_plus_name) > 0, "Could not find entry \'" << name << "\' not found. Make sure it is loaded or set.");
    return vector_point_3d[string_to_type_map.at(path_plus_name)].value;
  }


  const Types::Array &
  Parameters::get_array(const std::string &name) const
  {
    //TODO: Assert that the size of the vector is larger then zero.
    const std::string path_plus_name = get_full_path() == "" ? name : get_full_path() + path_seperator + name;
    WBAssert(string_to_type_map.count(path_plus_name) > 0, "Could not find entry \'" << name << "\' not found. Make sure it is loaded or set.");

    return vector_array[string_to_type_map.at(path_plus_name)];
  }

  template<class T>
  const std::vector<T *>
  Parameters::get_array(const std::string &name) const
  {
    //TODO: Assert that the size of the vector is larger then zero.
    const std::string path_plus_name = get_full_path() == "" ? name : get_full_path() + path_seperator + name;
    WBAssert(string_to_type_map.count(path_plus_name) > 0, "Could not find entry \'" << name << "\' not found. Make sure it is loaded or set.");

    const Types::Array typed_array = vector_array[string_to_type_map.at(path_plus_name)];

    std::vector<T *> array(typed_array.inner_type_index.size());

    for (unsigned int i = 0; i < typed_array.inner_type_index.size(); ++i)
      {
        if (typed_array.inner_type == Types::type::Double)
          {
            array[i] = dynamic_cast<T *>(&vector_double[typed_array.inner_type_index[i]]);
            WBAssert(array[i] != NULL, "Could not get " << get_full_path() << (get_full_path() == "" ? "" : path_seperator) << name << ", because it is not a Double.");
          }
        else if (typed_array.inner_type == Types::type::Segment)
          {
            array[i] = dynamic_cast<T *>(&vector_segment[typed_array.inner_type_index[i]]);
            WBAssert(array[i] != NULL, "Could not get " << get_full_path() << (get_full_path() == "" ? "" : path_seperator) << name << ", because it is not a segment.");
          }
        else if (typed_array.inner_type == Types::type::ConstantLayer)
          {
            array[i] = dynamic_cast<T *>(&vector_constant_layer[typed_array.inner_type_index[i]]);
            WBAssert(array[i] != NULL, "Could not get " << get_full_path() << (get_full_path() == "" ? "" : path_seperator) << name << ", because it is not a constant layer.");
          }
        else if (typed_array.inner_type == Types::type::Point2D)
          {
            array[i] = dynamic_cast<T *>(&vector_point_2d[typed_array.inner_type_index[i]]);
            WBAssert(array[i] != NULL, "Could not get " << get_full_path() << (get_full_path() == "" ? "" : path_seperator) << name << ", because it is not a 2d Point.");
          }
        else if (typed_array.inner_type == Types::type::Point3D)
          {
            array[i] = dynamic_cast<T *>(&vector_point_3d[typed_array.inner_type_index[i]]);
            WBAssert(array[i] != NULL, "Could not get " << get_full_path() << (get_full_path() == "" ? "" : path_seperator) << name << ", because it is not a 3d Point.");
          }
        else
          {
            WBAssert(false, "type conversion not implemented for type with number " << (int)typed_array.inner_type << ".");
          }

      }

    return array;
  }

//TODO:
  /*
  template<class T>
  T Parameters::get(const std::string &name)
  {

  }*/

  std::string
  Parameters::get_full_path() const
  {
    std::string collapse = "";
    for (unsigned int i = 0; i < path.size(); i++)
      {
        collapse += path[i] + path_seperator;
      }
    return collapse.substr(0,collapse.size()-path_seperator.size());
  }

  std::string
  Parameters::get_relative_path() const
  {
    std::string collapse = "";
    for (unsigned int i = path_level; i < path.size(); i++)
      {
        collapse += path[i] + path_seperator;
      }
    return collapse.substr(0,collapse.size()-path_seperator.size());
  }

  std::string
  Parameters::get_full_path_without_arrays() const
  {
    std::string collapse = "";
    for (unsigned int i = 0; i < path.size(); i++)
      {
        collapse += (path[i].front() == '[' && path[i].back() == ']' ? "" : path[i]) + path_seperator;
      }
    return collapse.substr(0,collapse.size()-path_seperator.size());
  }


  std::string
  Parameters::get_relative_path_without_arrays() const
  {
    std::string collapse = "";
    for (unsigned int i = path_level; i < path.size(); i++)
      {
        collapse += (path[i].front() == '[' && path[i].back() == ']' ? "" : path[i]) + path_seperator;
      }
    return collapse.substr(0,collapse.size()-path_seperator.size());
  }

  /**
   * Returns a vector of pointers to the Double Type based on the provided name.
   * Note that the variable with this name has to be loaded before this function is called.
   */
  template const std::vector<const Types::Double * > Parameters::get_array<const Types::Double >(const std::string &name) const;

  /**
   * Returns a vector of pointers to the Segment Type based on the provided name.
   * Note that the variable with this name has to be loaded before this function is called.
   */
  template const std::vector<const Types::Segment * > Parameters::get_array<const Types::Segment >(const std::string &name) const;

  /**
   * Returns a vector of pointers to the ConstantLayer Type based on the provided name.
   * Note that the variable with this name has to be loaded before this function is called.
   */
  template const std::vector<const Types::ConstantLayer * > Parameters::get_array<const Types::ConstantLayer >(const std::string &name) const;

  /**
   * Returns a vector of pointers to the Point<2> Type based on the provided name.
   * Note that the variable with this name has to be loaded before this function is called.
   */
  template const std::vector<const Types::Point<2>* > Parameters::get_array<const Types::Point<2> >(const std::string &name) const;

  /**
   * Returns a vector of pointers to the Point<3> Type based on the provided name.
   * Note that the variable with this name has to be loaded before this function is called.
   */
  template const std::vector<const Types::Point<3>* > Parameters::get_array<const Types::Point<3> >(const std::string &name) const;

}

