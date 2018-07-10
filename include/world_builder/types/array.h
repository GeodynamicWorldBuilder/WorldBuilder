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

#ifndef _world_feature_types_array_h
#define _world_feature_types_array_h

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
    class Array : public Interface
    {
      public:
        /**
         * Constructor
         */
        Array(const Interface& type, std::string description);

        /**
         * Constructor
         */
        //Array(Interface* type, std::string description);

        /**
         * Destructor
         */
        ~Array();


        /**
         * clone
         */
        virtual
        std::shared_ptr<Interface> clone() const;

        std::shared_ptr<Interface> inner_type;
        std::string description;

    };
  }
}

#endif
