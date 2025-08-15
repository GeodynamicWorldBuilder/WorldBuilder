/*
  Copyright (C) 2018-2024 by the authors of the World Builder code.

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

#ifndef WORLD_BUILDER_OBJECTS_CONTOUR_H
#define WORLD_BUILDER_OBJECTS_CONTOUR_H


#include "world_builder/types/plugin_system.h"


namespace WorldBuilder
{
  class Parameters;

  namespace Objects
  {

    /**
      * This class represents an actual segment
      */
    template <class A, class B, class C, class D>
    class Contour
    {
      public:

        /**
         * A constructor for the clone and set_entry function
         */
        Contour(const double default_depth,
                const WorldBuilder::Point<2> &default_thickness,
                const WorldBuilder::Point<2> &default_top_truncation,
                std::vector<std::shared_ptr<A> > temperature_systems,
                std::vector<std::shared_ptr<B> > composition_systems,
                std::vector<std::shared_ptr<C> > grains_systems,
                std::vector<std::shared_ptr<D> > velocity_systems);

        /**
         * Copy constructor
         */
        Contour(Contour const &other);

        /**
         * Destructor
         */
        ~Contour();

        double value_depth;
        WorldBuilder::Point<2> value_thickness;
        WorldBuilder::Point<2> value_top_truncation;
        std::vector<std::shared_ptr<A> > temperature_systems;
        std::vector<std::shared_ptr<B> > composition_systems;
        std::vector<std::shared_ptr<C> > grains_systems;
        std::vector<std::shared_ptr<D> > velocity_systems;

      protected:
      private:

    };
  } // namespace Objects
} // namespace WorldBuilder

#endif
