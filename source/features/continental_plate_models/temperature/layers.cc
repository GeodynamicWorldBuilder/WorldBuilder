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

#include <world_builder/utilities.h>
#include <world_builder/assert.h>
#include <world_builder/nan.h>
#include <world_builder/parameters.h>

#include <world_builder/types/array.h>
#include <world_builder/types/double.h>
#include <world_builder/types/string.h>
#include <world_builder/types/plugin_system.h>
#include <world_builder/types/unsigned_int.h>
#include <world_builder/features/continental_plate_models/temperature/layers.h>


namespace WorldBuilder
{
  using namespace Utilities;

  namespace Features
  {
    namespace ContinentalPlateModels
    {
      namespace Temperature
      {
        Layers::Layers(WorldBuilder::World *world_)
        {
          this->world = world_;
          this->name = "layers";
        }

        Layers::~Layers()
        { }

        void
        Layers::declare_entries(Parameters &prm, const std::string &)
        {

          prm.declare_entry("layers",
                            Types::PluginSystem("", Features::ContinentalPlateModels::Temperature::Interface::declare_entries, {"model"}),
                            "A list of temperature models to be used as layers.");
        }

        void
        Layers::parse_entries(Parameters &prm)
        {
          prm.get_unique_pointers<Interface>("layers", layers);

          prm.enter_subsection("layers");
          {
            for (unsigned int i = 0; i < layers.size(); ++i)
              {
                prm.enter_subsection(std::to_string(i));
                {
                  layers[i]->parse_entries(prm);
                }
                prm.leave_subsection();
              }
          }
          prm.leave_subsection();
        }


        double
        Layers::get_temperature(const Point<3> &position,
                                const double depth,
                                const double gravity,
                                double temperature,
                                const double feature_min_depth,
                                const double feature_max_depth) const
        {
          for (auto &layer: layers)
            {
              temperature = layer->get_temperature(position,
                                                   depth,
                                                   gravity,
                                                   temperature,
                                                   feature_min_depth,
                                                   feature_max_depth);
            }
          return temperature;
        }

        WB_REGISTER_FEATURE_CONTINENTAL_TEMPERATURE_MODEL(Layers, layers)
      }
    }
  }
}


