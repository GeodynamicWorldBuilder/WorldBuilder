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

#ifndef _world_feature_types_constant_layer_h
#define _world_feature_types_constant_layer_h

#include <world_builder/types/interface.h>
#include <world_builder/point.h>


namespace WorldBuilder
{
  namespace Types
  {

    /**
     * This class represents a double value with documentation
     */
    class ConstantLayer : public Interface
    {
      public:
        /**
         * A constructor for the load_entry function
         */
        ConstantLayer(std::vector<unsigned int> default_value_composition,
                      std::vector<double> default_value,
                      double default_value_thickness,
                      std::string description);

        /**
         * A constructor for the clone and set_entry function
         */
        ConstantLayer(std::vector<unsigned int> value_composition,
                      std::vector<unsigned int> default_value_composition,
                      std::vector<double> value,
                      std::vector<double> default_value,
                      double value_thickness,
                      double default_value_thickness,
                      std::string description);

        /**
         * Destructor
         */
        ~ConstantLayer();

        /**
         * Clone. The caller of clone is responsible for the lifetime of it,
         * so return a unique pointer.
         */
        virtual
        std::unique_ptr<Interface> clone() const;

        /**
         * The composition number. An int is used so we can allow a value of
         * -1.
         */
        std::vector<unsigned int> value_composition;

        /**
         * The default composition number. An int is used so we can allow a
         * value of -1.
         */
        std::vector<unsigned int> default_value_composition;


        /**
         * The value of the composition or temperature. This value is never
         * required, even if the constant_layer required value is set to true.
         */
        std::vector<double> value;

        /**
         * The default value of the composition or temperature.
         */
        std::vector<double> default_value;

        /**
         * The thickness of the constant layer.
         */
        double value_thickness;


        /**
         * The default thickness of the constant layer.
         */
        double default_value_thickness;

        /**
         * The description what this value is used for.
         */
        std::string description;

      private:

    };
  }
}

#endif
