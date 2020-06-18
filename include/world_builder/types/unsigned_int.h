/*
  Copyright (C) 2018 - 2020 by the authors of the World Builder code.

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

#ifndef _world_feature_types_unsigned_int_h
#define _world_feature_types_unsigned_int_h

#include <world_builder/types/interface.h>


namespace WorldBuilder
{
  namespace Types
  {

    /**
     * This class represents a bool value with documentation
     */
    class UnsignedInt : public Interface
    {
      public:
        /**
         * A constructor for the load_entry function
         */
        UnsignedInt(unsigned int default_value = 0);

        /**
         * Copy constructor
         */
        UnsignedInt(UnsignedInt const &other);

        /**
         * Destructor
         */
        ~UnsignedInt();


        /**
         * Todo
         */
        void write_schema(Parameters &prm,
                          const std::string &name,
                          const std::string &documentation) const override final;

        unsigned int value;
        unsigned int default_value;

      protected:
        /**
         * This implements the actual cloneing for the clone function in the base class.
         */
        UnsignedInt *clone_impl() const override final
        {
          return new UnsignedInt(*this);
        };
      private:

    };
  }
}

#endif
