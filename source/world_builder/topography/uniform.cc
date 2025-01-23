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

#include "world_builder/topography/uniform.h"
#include "world_builder/types/double.h"
#include "world_builder/types/object.h"
#include "world_builder/world.h"


namespace WorldBuilder
{
  namespace Topography
  {
    Uniform::Uniform(WorldBuilder::World *world_)
      :
      surface_type(reference_surface)
    {
      this->world = world_;
    }

    Uniform::~Uniform()
      = default;

    void
    Uniform::declare_entries(Parameters &/*prm*/, const std::string & /*unused*/)
    {
      // Nothing to declare.
    }

    void
    Uniform::parse_entries(Parameters &/*prm*/)
    {
      // Nothing to parse as surface type is set in header.
    }

    /**
     * Returns a topography based on the position. In the uniform model,
     * we assume there is no topography.
     */
    double
    Uniform::get_topography(const Point<3> &position,
                            const Objects::NaturalCoordinate &position_in_natural_coordinates,
                            const double depth) const
    {
      return 0;
    }

    Uniform::SurfaceType
    Uniform::get_surface_type()
    {
      return surface_type;
    }

    /**
     * Register plugin
     */
    WB_REGISTER_TOPOGRAPHY(Uniform, uniform)
  } // namespace Topography
} // namespace WorldBuilder

