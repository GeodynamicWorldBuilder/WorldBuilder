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

#include <world_builder/utilities.h>
#include <world_builder/assert.h>
#include <world_builder/nan.h>
#include <world_builder/parameters.h>

#include <world_builder/types/double.h>
#include <world_builder/types/string.h>
#include <world_builder/types/object.h>
#include <world_builder/features/fault_models/composition/smooth.h>


namespace WorldBuilder
{
  using namespace Utilities;

  namespace Features
  {
    namespace FaultModels
    {
      namespace Composition
      {
        Smooth::Smooth(WorldBuilder::World *world_)
          :
          min_distance(NaN::DSNAN),
          side_distance(NaN::DSNAN),
          center_composition(NaN::DSNAN),
          side_composition(NaN::DSNAN),
          operation("")
        {
          this->world = world_;
          this->name = "smooth";
        }

        Smooth::~Smooth()
        { }

        void
        Smooth::declare_entries(Parameters &prm, const std::string &)
        {
          // Add compositions to the required parameters.
          prm.declare_entry("", Types::Object({"compositions"}), "Compositional model object");

          prm.declare_entry("min distance fault center", Types::Double(0),
                            "The distance in meters from which the composition of this feature is present.");

          prm.declare_entry("side distance fault center", Types::Double(std::numeric_limits<double>::max()),
                            "The distance over which the composition is reduced from 1 t0 0.");

          prm.declare_entry("center composition", Types::Double(1),
                            "The composition at the center of the fault.");

          prm.declare_entry("side composition", Types::Double(0),
                            "The composition at the sides of this feature.");

        }

        void
        Smooth::parse_entries(Parameters &prm)
        {
          min_distance = prm.get<double>("min distance fault center");
          side_distance = prm.get<double>("side distance fault center");
          WBAssert(side_distance >= min_distance, "distance at the side needs to be larger or equal than the min distance.");
          operation = prm.get<std::string>("operation");
          center_composition = prm.get<double>("center composition");
          side_composition = prm.get<double>("side composition");
          compositions = prm.get<double>("compositions");
        }


        double
        Smooth::get_composition( const Point<3> &,
                                 const double ,
                                 const unsigned int /*composition_number*/,
                                 double composition_,
                                 const double ,
                                 const double ,
                                 const std::map<std::string,double> &distance_from_plane) const
        {
          double composition = composition_;

          if (std::fabs(distance_from_plane.at("distanceFromPlane")) <= side_distance && std::fabs(distance_from_plane.at("distanceFromPlane")) >= min_distance)
            {
              const double min_dist_local = min_distance;
              const double side_dist_local = side_distance;
              
              // Hyperbolic tangent goes from 0 to 1 over approximately x=(0, 2) without any arguements. The function is written
              // so that the composition returned 1 to 0 over the side_distance
              const double composition = center_composition - (center_composition - side_composition) * 
                                         std::tanh( 2 *(std::fabs(distance_from_plane.at("distanceFromPlane")) - min_dist_local)/side_dist_local);
              
              return composition;
            }

          return composition;
        }

        WB_REGISTER_FEATURE_FAULT_COMPOSITION_MODEL (Smooth, smooth)
      }
    }
  }
}

