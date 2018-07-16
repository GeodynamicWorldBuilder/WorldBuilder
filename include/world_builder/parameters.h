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
#include <world_builder/types/unsigned_int.h>
#include <world_builder/types/double.h>
#include <world_builder/types/string.h>
#include <world_builder/types/point.h>
#include <world_builder/types/array.h>
#include <world_builder/types/list.h>



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

  /**
   * A class to hold all the parameters needed by the world builder. Internally
   * it holds all values in the form of vectors of class Types. Values can be
   * entered in two ways into this class. The first way is through the
   * load_entry function which load the value from the provided world builder
   * file. The second way is through the set_entry function, through which
   * values can be directly entered into the parameter class. Values can be
   * retrieved through the get functions which take the name of the value
   * with which it was set. It is also required for bot loading, setting and
   * getting values to do it in the correct subsection. Subsections can be
   * entered with the enter_subsection function and left with the
   * leave_subsection function. The current path can be retrieved through the
   * function get_current_path() and get_current_path_without_arrays().
   */
  class Parameters
  {
    public:
      /**
       * Constructor
       * \param filename A string with the path to the world builder file
       * \param world A reference to the World class
       */
      Parameters(std::string &filename, World &);

      /**
       * Destructor
       */
      ~Parameters();

      /**
       * Loads an entry into the parameter class from the world builder file,
       * where the name is the key in the file relative to the nesting in the
       * world builder file. Note that classes receive the subtree which is
       * relevant for them, so the they only have to account for their nesting.
       *
       * Note that for the array and list, default values are not implemented.
       *
       * \param name The name of the parameter which should be loaded
       * \param required Determines whether a value is required to be present.
       * When it isn't present, and the value is set to true, an assert is
       * thrown. If it is set to false and the value is not found, the default
       * value is used.
       * \param type A value of class Type. Note that these may be nested.
       *
       * \return Returns whether the value was found.
       *
       * @see enter_subsection()
       * @see leave_subsection()
       * @see Types::Interface
       */
      bool load_entry(const std::string &name, const bool required, const Types::Interface &type);

      /**
       * Sets an entry into the parameter class, where the name is the key in
       * the file relative to the nesting defined by the enter_subsection and
       * leave_subsection functions. Note that classes receive the subtree which
       * is relevant for them, so the they only have to account for their nesting.
       *
       * Note that for the array and list, default values are not implemented.
       *
       * @see enter_subsection()
       * @see leave_subsection()
       * @see Types::Interface
       */
      void set_entry(const std::string &name, const Types::Interface &type);

      /**
       * This function is used to enter a subsection. It appends to the path
       * variable. This action is revesed by the leave subsection function.
       * \param name The name of the subsection to be entered.
       * @see path
       * @see leave_subsection()
       */
      void enter_subsection(const std::string name);

      /**
       * This function is used to leave a subsection by removing the last
       * element of the path variable. It reverses the action of the enter
       * subsection function.
       * @see path
       * @see enter_subsection()
       */
      void leave_subsection();

      /**
       * This function retrieves an unsigned int from the parameter class with
       * the key in variable name, based on the path. Note the path rules for
       * called classes explained in the parameter class explanation.
       * \param name The key where the variable is stored.
       */
      unsigned int get_unsigned_int(const std::string &name) const;

      /**
       * This function retrieves a double from the parameter class with
       * the key in variable name, based on the path. Note the path rules for
       * called classes explained in the parameter class explanation.
       * \param name The key where the variable is stored.
       */
      double get_double(const std::string &name) const;

      /**
       * This function retrieves astring from the parameter class with
       * the key in variable name, based on the path. Note the path rules for
       * called classes explained in the parameter class explanation.
       * \param name The key where the variable is stored.
       */
      std::string get_string(const std::string &name) const;

      // get_array(const std::string& name);

      /**
       * This function retrieves a point from the parameter class with
       * the key in variable name, based on the path. Note the path rules for
       * called classes explained in the parameter class explanation.
       * \param name The key where the variable is stored.
       * \return Point
       */
      template<int dim>
      Point<dim> get_point(const std::string &name) const;

      // TODO:
      //template<class T>
      //T get(const std::string &name);


      /**
       * This function retrieves a Type::Array from the parameter class with
       * the key in variable name, based on the path. Note the path rules for
       * called classes explained in the parameter class explanation.
       * \param name The key where the variable is stored. The array contains
       * the underlying type and the indeces where the values are stored.
       * \return Types::Array
       */
      const Types::Array &get_array(const std::string &name) const;

      /**
       * Returns the array with pointers to the requested type. The reason a
       * raw pointer is returned is that caller is not responsible for the
       * classes pointed at in the return vector or has ownership on them.
       * \param name The key where the variable is stored. The array contains
       * \return std::vector<T*>
       */
      template<class T>
      const std::vector<T *> get_array(const std::string &name) const;

      /**
       * A reference to the World class. This is needed to create the features.
       */
      World &world;

      /**
       * This variable stores what path separtor is used in the property tree
       * and in this class.
       */
      const std::string path_seperator = ".";

      /**
       * This variable stores the path in a vector of strings.
       * @see enter_subsection()
       * @see leave_subsection()
       */
      std::vector<std::string> path;


      /**
       * The boost property tree, used to read in the data from a json file.
       */
      ptree tree;

      /**
       * This variable ontains a pointer to the a part of the tree. This
       * variable is not repsonsible for the tree, so using a raw pointer.
       * @see tree
       */
      ptree *local_tree;

      /**
       * An unordered map used to store and retrieve the indices where the
       * variables in this class are stored based on the path key.
       * @see path
       * @see get_current_path()
       */
      std::unordered_map<std::string,unsigned int> string_to_type_map;

      /**
       * A vector which stores all the UnsignedInt types. These can be
       * retrieved with the help of the string_to_type_map and the
       * get_unsigned_int() function.
       * @see string_to_type_map
       * @see get_unsigned_int()
       * @see Types::UnsignedInt
       */
      std::vector<Types::UnsignedInt> vector_unsigned_int;

      /**
       * A vector which stores all the Double types. These can be
       * retrieved with the help of the string_to_type_map and the
       * get_double() function.
       * @see string_to_type_map
       * @see get_double()
       * @see Types::Double
       */
      std::vector<Types::Double> vector_double;

      /**
       * A vector which stores all the String types. These can be
       * retrieved with the help of the string_to_type_map and the
       * get_string() function.
       * @see string_to_type_map
       * @see get_unsigned_int()
       * @see Types::UnsignedInt
       */
      std::vector<Types::String> vector_string;

      /**
       * A vector which stores all the Array types. These can be
       * retrieved with the help of the string_to_type_map and the
       * get_array() function.
       * @see string_to_type_map
       * @see get_array()
       * @see Types::Array
       */
      std::vector<Types::Array> vector_array;

      /**
       * A vector which stores all the List types. These can be
       * retrieved with the help of the string_to_type_map.
       * @see string_to_type_map
       * @see Types::List
       */
      std::vector<Types::List> vector_list;

      /**
       * A vector which stores all the Point<2> types. These can be
       * retrieved with the help of the string_to_type_map and the
       * get_Point<2>() function.
       * @see string_to_type_map
       * @see get_point<2>()
       * @see Types::Point
       */
      std::vector<Types::Point<2> > vector_point_2d;

      /**
       * A vector which stores all the Point<3> types. These can be
       * retrieved with the help of the string_to_type_map and the
       * get_Point<3>() function.
       * @see string_to_type_map
       * @see get_point<3>()
       * @see Types::Point
       */
      std::vector<Types::Point<3> > vector_point_3d;


      /**
       * A vector containing all the pointers to the features. This vector is
       * responsible for the features and has ownership over them. Therefore
       * unique pointers are used.
       * @see Features
       */
      std::vector<std::unique_ptr<WorldBuilder::Features::Interface> > features;

      /**
       * A pointers to the corodinate system. This variable is responsible for
       * the coordinate system and has ownership over it. Therefore a unique
       * pointer are used.
       * @see CoordinateSystem
       */
      std::unique_ptr<WorldBuilder::CoordinateSystems::Interface> coordinate_system;

      /**
       * This function return the current path as stored in the path variable
       * as a string.
       * \return std::string
       */
      std::string get_full_path() const;

      /**
       * This function return the current path as stored in the path variable
       * as a string, but the arrays are striped. This is useful for working
       * with the boost property tree.
       * \return std::string
       */
      std::string get_full_path_without_arrays() const;

    private:
      /**
       * A helper function which performs the actual work for load_entry().
       * This used to keep the input for the public load_entry function simple.
       * @see load_entry()
       */
      bool load_entry(const std::string &name, const bool required, const Types::Interface &type, unsigned int &location);

      /**
       * A helper function which performs the actual work for set_entry(). This
       * used to keep the input for the public load_entry function simple.
       * @see load_entry()
       */
      void set_entry(const std::string &name, const Types::Interface &type, unsigned int &location);


      /**
       * This is used for the get relative path functions. It stores how many
       * top entries of the path should be ignored.
       */
      unsigned int path_level;

      /**
       * A function which returns the relative path, which is the full path
       * minus the path_level top entries..
       * @see get_current_path()
       */
      std::string get_relative_path() const;

      /**
       * A function which returns the relative path, which is the full path
       * minus the path_level top entries., without names for the arrays.
       * @see get_current_path_without_arrays()
       */
      std::string get_relative_path_without_arrays() const;
  };
}
#endif
