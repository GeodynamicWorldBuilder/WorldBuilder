/*
  Copyright (C) 2018 - 2021 by the authors of the World Builder code.

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

#ifndef WORLD_BUILDER_FEATURES_SUBDUCTING_PLATE_MODELS_COMPOSITION_WATER_CONTENT_H
#define WORLD_BUILDER_FEATURES_SUBDUCTING_PLATE_MODELS_COMPOSITION_WATER_CONTENT_H


#include "world_builder/features/subducting_plate_models/composition/interface.h"
#include <mutex>


namespace WorldBuilder
{

  namespace Features
  {
    namespace SubductingPlateModels
    {
      namespace Composition
      {
        /**
         * This class represents the bound water content in a subducting plate and can implement
         * submodules for computing this bound water content. These submodules determine what
         * the returned water content will be based on the the temperature and pressure at a point
         * within the world.
         */
        class WaterContent final: public Interface
        {
          public:
            /**
             * constructor
             */
            WaterContent(WorldBuilder::World *world);

            /**
             * Destructor
             */
            ~WaterContent() override final;

            /**
             * declare and read in the world builder file into the parameters class
             */
            static
            void declare_entries(Parameters &prm, const std::string &parent_name = "");

            /**
             * declare and read in the world builder file into the parameters class
             */
            void parse_entries(Parameters &prm) override final;

            /**
             *  Calculates what the maximum bound water content is at a point given a pressure and
             * temperature. This can be done for 4 different lithologies, "sediment", "gabbro",
             * "MORB", and "peridotite"
             */
            // double calculate_water_content(double pressure,
            //                                double temperature,
            //                                unsigned int lithology_ind) const;
            /**
             * Returns a value for the bound water contend based on the given position, depth in the model,
             * gravity, and the temperature at that point.
             */
            double get_composition(const Point<3> &position,
                                   const double depth,
                                   const unsigned int composition_number,
                                   double composition,
                                   const double feature_min_depth,
                                   const double feature_max_depth,
                                   const WorldBuilder::Utilities::PointDistanceFromCurvedPlanes &distance_from_planes,
                                   // std::string lithology_str,
                                   const AdditionalParameters &additional_parameters) const override final;


          private:
            // water_content composition submodule parameters
            double min_depth;
            double max_depth;
            double density;
            std::vector<unsigned int> compositions;
            Operations operation;

            std::vector<double> max_water_contents;
            std::string lithology_str;
            unsigned int lithology_index;
        };
      } // namespace Composition
    } // namespace SubductingPlateModels
  } // namespace Features
} // namespace WorldBuilder

#endif
