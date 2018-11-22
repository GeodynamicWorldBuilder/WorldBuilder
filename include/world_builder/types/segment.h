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

#ifndef _world_feature_types_segment_h
#define _world_feature_types_segment_h

#include <world_builder/types/interface.h>
#include <world_builder/point.h>


namespace WorldBuilder
{
  namespace Types
  {

    /**
     * This class represents a double value with documentation
     */
    class Segment : public Interface
    {
      public:
        /**
         * A constructor for the load_entry function
         */
        Segment(double default_value_length,
                WorldBuilder::Point<2> default_value_thickness,
                WorldBuilder::Point<2> default_value_angle,
                std::string description);

        /**
         * A constructor for the clone and set_entry function
         */
        Segment(double   value_length,    double   default_value_length,
                WorldBuilder::Point<2> value_thickness, WorldBuilder::Point<2> default_value_thickness,
                WorldBuilder::Point<2> value_angle,     WorldBuilder::Point<2> default_value_angle,
                std::string description);

        /**
         * Destructor
         */
        ~Segment();

        /**
         * Clone. The caller of clone is responsible for the lifetime of it,
         * so return a unique pionter.
         */
        virtual
        std::unique_ptr<Interface> clone() const;

        /**
         * Todo
         */
        virtual
        void write_schema(Parameters &prm,
                          const std::string &name,
                          const std::string &documentation) const;

        double value_length;
        double default_value_length;
        WorldBuilder::Point<2> value_thickness;
        WorldBuilder::Point<2> default_value_thickness;
        WorldBuilder::Point<2> value_angle;
        WorldBuilder::Point<2> default_value_angle;
        std::string description;

      private:

    };
  }
}

#endif
