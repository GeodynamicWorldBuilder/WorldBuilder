/*
  Copyright (C) 2018-2025 by the authors of the World Builder code.

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

#ifndef WORLD_BUILDER_TOPOGRAPHY_MINIMUM_DEPTH_H
#define WORLD_BUILDER_TOPOGRAPHY_MINIMUM_DEPTH_H

#include "world_builder/topography/interface.h"

#include "world_builder/utilities.h"
#include "world_builder/objects/natural_coordinate.h"


namespace WorldBuilder
{

  namespace Topography
  {
    /**
     * This implements a minimum depth topography. The minimum depth topography
     * finds the minimum depth of a feature and uses that for the topography.
     */
    class Uniform final : public Interface
    {
      public:
        /**
         * Constructor
         */
        Uniform(WorldBuilder::World *world);

        /**
         * Destructor
         */
        ~ Uniform() override final;

        /**
         * declare and read in the world builder file into the topography class
         */
        static
        void declare_entries(Parameters &prm, const std::string &parent_name = "");

        /**
         * declare and read in the world builder file into the topography class
         */
        void parse_entries(Parameters &prm) override final;

        /**
         * declare and read in the world builder file into the topography class
         */
        SurfaceType
        get_surface_type() override final;

        /**
         * Returns a temperature based on the given position, depth in the model,
         * gravity and current temperature.
         */
        double get_topography(const Point<3> &position,
                              const Objects::NaturalCoordinate &position_in_natural_coordinates,
                              const double depth) const override final;

      private:

        /**
         * Whether to consider a reference or deformed surface.
         */
        SurfaceType surface_type;

    };
  } // namespace Topography
} // namespace WorldBuilder

#endif
