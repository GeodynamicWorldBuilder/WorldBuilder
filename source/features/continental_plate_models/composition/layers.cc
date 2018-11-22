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
#include <world_builder/features/continental_plate_models/composition/layers.h>


namespace WorldBuilder
{
  using namespace Utilities;

  namespace Features
  {
    namespace ContinentalPlateModels
    {
      namespace Composition
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

          prm.declare_entry("layers","",true,
                            Types::PluginSystem(Features::ContinentalPlateModels::Composition::Interface::declare_entries),
                            "A list of composition models to be used as layers.");

          /*prm.declare_entry("min depth","0",false,Types::Double(0),
                            "The depth in meters from which the composition of this feature is present.");
          prm.declare_entry("max depth","0",false,Types::Double(std::numeric_limits<double>::max()),
                            "The depth in meters to which the composition of this feature is present.");
          prm.declare_entry("compositions","",true,Types::Array(Types::UnsignedInt(), true,1),
                            "A list with the labels of the composition which are present there.");
          prm.declare_entry("fractions","1",false,Types::Array(Types::Double(1.0),false,1),
                            "TA list of compositional fractions corresponding to the compositions list.");
          prm.declare_entry("operation","replace",false,Types::String(),
                            "Whether the value should replace any value previously defined at this location (replace) or "
                            "add the value to the previously define value (add).");
          */
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

          //composition_models = prm.get_unique_pointer<Features::ContinentalPlateModels::Composition::Interface>("layers");
          //composition_models->parse_entries(prm);
          /*min_depth = prm.get<double>("min depth");
          max_depth = prm.get<double>("max depth");
          compositions = prm.get_vector<unsigned int>("compositions");
          fractions = prm.get_vector<double>("fractions");

          WBAssertThrow(compositions.size() == fractions.size(),
                        "There are not the same amount of compositions and fractions.");*/
        }


        double
        Layers::get_composition(const Point<3> &position,
                                const double depth,
                                const unsigned int composition_number,
                                double composition) const
        {
          for (auto &layer: layers)
            {
              composition = layer->get_composition(position,
                                                   depth,
                                                   composition_number,
                                                   composition);
            }
          return composition;
        }

        WB_REGISTER_FEATURE_CONTINENTAL_COMPOSITION_MODEL(Layers, layers)
      }
    }
  }
}


