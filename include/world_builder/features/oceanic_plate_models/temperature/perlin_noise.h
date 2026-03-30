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

#ifndef WORLD_BUILDER_FEATURES_OCEANIC_PLATE_MODELS_TEMPERATURE_PERLIN_NOISE_H
#define WORLD_BUILDER_FEATURES_OCEANIC_PLATE_MODELS_TEMPERATURE_PERLIN_NOISE_H

#include "world_builder/features/oceanic_plate_models/temperature/interface.h"
#include "world_builder/features/feature_utilities.h"
#include "world_builder/objects/surface.h"
#include <array>
#include <cstdint>

namespace WorldBuilder
{
  namespace Features
  {
    using namespace FeatureUtilities;
    namespace OceanicPlateModels
    {
      namespace Temperature
      {
        /**
         * This class represents a Perlin noise model submodule.
         */
        class PerlinNoise final: public Interface
        {
          public:
            /**
             * constructor
             */
            PerlinNoise(WorldBuilder::World *world);
            /**
             * Destructor
             */
            ~PerlinNoise() override final;

            /**
             * declare and read in the world builder file into the parameters class
             */
            static void declare_entries(Parameters &prm, const std::string &parent_name = "");
            /**
             * declare and read in the world builder file into the parameters class
             */
            void parse_entries(Parameters &prm, const std::vector<Point<2>> &coordinates) override final;

            /**
             * Returns a temperature based on the given position, depth in the model,
             * gravity and current temperature.
             */
            double get_temperature(const Point<3> &position,
                                   const Objects::NaturalCoordinate &position_in_natural_coordinates,
                                   const double depth,
                                   const double gravity,
                                   double temperature,
                                   const double feature_min_depth,
                                   const double feature_max_depth) const override final;

          private:
            // Perlin noise submodule parameters
            double min_depth;
            Objects::Surface min_depth_surface;
            double max_depth;
            Objects::Surface max_depth_surface;
            double min_temperature;
            double max_temperature;
            double frequency;
            unsigned int octaves;
            double persistence;
            double lacunarity;
            Operations operation;

            // Perlin noise implementation details
            using state_type = std::array<std::uint8_t, 256>;
            state_type p;

            double fade(double t) const;
            double lerp(double t, double a, double b) const;
            double grad(int hash, double x, double y, double z) const;
            double noise(double x, double y, double z) const;
            double octave_noise(double x, double y, double z, unsigned int octaves_in) const;
        };
      }
    }
  }
}

#endif


