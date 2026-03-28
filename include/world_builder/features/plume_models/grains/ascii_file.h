/*
  Copyright (C) 2018-2026 by the authors of the World Builder code.

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

#ifndef WORLD_BUILDER_FEATURES_PLUME_MODELS_GRAINS_ASCII_FILE_H
#define WORLD_BUILDER_FEATURES_PLUME_MODELS_GRAINS_ASCII_FILE_H


#include "world_builder/features/plume_models/grains/interface.h"
#include "world_builder/objects/surface.h"

namespace WorldBuilder
{

  namespace Features
  {
    namespace PlumeModels
    {
      namespace Grains
      {
        /**
         * This class represents a plume and can implement
         * submodules for temperature and grains. These submodules determine
         * what the returned temperature or grains of the temperature and grains
         * functions of this class will be.
         */
        class AsciiFile final: public Interface
        {
          public:
            /**
             * constructor
             */
            AsciiFile(WorldBuilder::World *world);

            /**
             * Destructor
             */
            ~AsciiFile() override final;

            /**
             * declare and read in the world builder file into the parameters
             * class
             */
            static void declare_entries(Parameters &prm,
                                        const std::string &parent_name = "");

            /**
             * declare and read in the world builder file into the parameters
             * class
             */
            static void declare_grain_size_model_entries(
              Parameters &prm, const std::string &parent_name,
              const std::vector<std::string> &required_entries);

            /**
             * declare and read in the world builder file into the parameters
             * class
             */
            static void
            declare_fixed_size_model_entries(Parameters &prm,
                                             const std::string &parent_name = "");

            /**
             * declare and read in the world builder file into the parameters
             * class
             */
            void parse_entries(Parameters &prm) override final;

            /**
             * Returns a grains based on the given position, composition (e.g.
             * olivine and/or enstatite)depth in the model, gravity and current grains.
             */
            WorldBuilder::grains
            get_grains(const Point<3> &position,
                       const Objects::NaturalCoordinate &position_in_natural_coordinates,
                       const double depth,
                       const unsigned int composition_number,
                       WorldBuilder::grains grains,
                       const double feature_min_depth,
                       const double feature_max_depth) const override final;

          private:


            // particle_id -> list of Euler angles
            std::map<unsigned int, std::vector<std::array<double,3>>> mineral_0_euler_angles_map;
            std::map<unsigned int, std::vector<std::array<double,3>>> mineral_1_euler_angles_map;
            std::map<unsigned int, std::vector<double>> mineral_0_volume_fraction_map;
            std::map<unsigned int, std::vector<double>> mineral_1_volume_fraction_map;

            double min_depth;
            Objects::Surface min_depth_surface;
            double max_depth;
            Objects::Surface max_depth_surface;
            std::vector<unsigned int> compositions;

            mutable unsigned int mineral_0_current_particle_index;
            mutable unsigned int mineral_1_current_particle_index;

            void load_file();
            std::string directory;
            std::string filename;

            unsigned int n_particles;
            unsigned int mineral_0_n_grains;
            unsigned int mineral_1_n_grains;
        };
      } // namespace Grains
    }   // namespace PlumeModels
  }     // namespace Features
} // namespace WorldBuilder

#endif