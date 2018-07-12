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

#ifndef _world_feature_types_list_h
#define _world_feature_types_list_h

#include <world_builder/types/interface.h>


namespace WorldBuilder
{
  namespace Types
  {

    /**
     * This class represents a continental plate and can implement submodules
     * for temperature and composition. These submodules determine what
     * the returned temperature or composition of the temperature and composition
     * functions of this class will be.
     */
    class List : public Interface
    {
      public:
        /**
         * Constructor for load
         */
        List(const Interface &type, const std::string &description);


        /**
         * Constructor
         */
        List(const List &type);

        /**
         * Constructor for set
         */
        List(const std::string &name, const std::vector<unsigned int> &inner_type_index, const Types::type inner_type, const std::string &description);


        /**
         * Destructor
         */
        ~List();


        /**
         * clone
         */
        virtual
        std::unique_ptr<Interface> clone() const;

        std::string name;


        std::unique_ptr<Interface> inner_type_ptr;
        Types::type inner_type;
        std::vector<unsigned int> inner_type_index;
        std::string description;

    };
  }
}

#endif
