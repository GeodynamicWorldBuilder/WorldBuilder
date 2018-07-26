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

#include <boost/algorithm/string.hpp>

#include <world_builder/features/interface.h>
#include <world_builder/features/continental_plate.h>
#include <world_builder/features/oceanic_plate.h>
#include <world_builder/assert.h>


namespace WorldBuilder
{
  namespace Features
  {
    Interface::Interface()
    {}

    Interface::~Interface ()
    {}

    std::unique_ptr<Interface>
    create_feature(const std::string name, World *world)
    {
      std::string feature_name = boost::algorithm::to_lower_copy(name);
      boost::algorithm::trim(feature_name);
      if (feature_name == "continental plate")
        return std::make_unique<Features::ContinentalPlate>(world);
      else if (feature_name == "oceanic plate")
        return std::make_unique<Features::OceanicPlate>(world);
      else
        WBAssertThrow(false, "Feature " << feature_name << " not implemented.");

      return NULL;
    }
  }
}

