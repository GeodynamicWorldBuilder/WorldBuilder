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

#ifndef _world_builder_world_h
#define _world_builder_world_h


#include <boost/property_tree/ptree.hpp>

#include <world_builder/parameters.h>

using boost::property_tree::ptree;


namespace WorldBuilder
{
  namespace Features
  {
    class Interface;
  }

  class World
  {
    public:
      /**
       * Constructor. This constructor requires a atring with the location of
       * the world builder file to initialize the world..
       */
      World(std::string filename);

      /**
       * Destructor
       */
      ~World();

      /**
       * read in the world builder file
       */
      void declare_and_parse(Parameters &parameters);

      double temperature(const std::array<double, 2> &point, const double depth, const double gravity_norm) const;

      double temperature(const std::array<double, 3> &point, const double depth, const double gravity_norm) const;


      double composition(const std::array<double, 2> &point, const double depth, const unsigned int composition_number) const;

      double composition(const std::array<double, 3> &point, const double depth, const unsigned int composition_number) const;

      /**
       * Stores the path separtor used for the property tree.
       */
      static const char path_seperator = '.';


      Parameters parameters;

    private:
      unsigned int dim;






  };
}

#endif
