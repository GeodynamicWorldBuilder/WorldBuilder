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

#ifndef WORLD_BUILDER_FEATURES_OCEANIC_PLATE_MODELS_TOPOGRAPHY_PERLIN_NOISE_H
#define WORLD_BUILDER_FEATURES_OCEANIC_PLATE_MODELS_TOPOGRAPHY_PERLIN_NOISE_H

#include "world_builder/features/oceanic_plate_models/topography/interface.h"
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
      namespace Topography
      {
        /**
         * Perlin noise based topography model for oceanic plates.
         *
         * This model generates topography using layered Perlin noise (octaves).
         * Parameters exposed to input files are similar to the continental
         * implementation: min/max depth, min/max topography, frequency,
         * octaves, persistence, lacunarity and operation.
         */
        class PerlinNoise final: public Interface
        {
          public:
            /**
             * Constructor
             * @param world pointer to the containing World instance
             */
            PerlinNoise(WorldBuilder::World *world);

            /**
             * Destructor
             */
            ~PerlinNoise() override final;

            /**
             * Declare parameter entries used by this model to the Parameters system.
             */
            static void declare_entries(Parameters &prm, const std::string &parent_name = "");

            /**
             * Parse parameter values from the Parameters instance and prepare
             * any internal state (e.g. permutation table shuffling).
             */
            void parse_entries(Parameters &prm, const std::vector<Point<2>> &coordinates) override final;

            /**
             * Compute the topography at the given position. Respects depth
             * ranges and combines the generated value with the input topography
             * according to `operation`.
             */
            double get_topography(const Point<3> &position,
                                  const Objects::NaturalCoordinate &position_in_natural_coordinates,
                                  double topography) const override final;

          private:
            // parameters
            double min_depth;
            Objects::Surface min_depth_surface;
            double max_depth;
            Objects::Surface max_depth_surface;
            double min_topography;
            double max_topography;
            double frequency;
            unsigned int octaves;
            double persistence;
            double lacunarity;
            Operations operation;

            // Perlin noise implementation details
            using state_type = std::array<std::uint8_t, 256>;
            state_type p;

            /**
             * Smoothstep function used by Perlin interpolation.
             */
            double fade(double t) const;

            /**
             * Linear interpolation helper.
             */
            double lerp(double t, double a, double b) const;

            /**
             * Gradient function for Perlin noise.
             */
            double grad(int hash, double x, double y, double z) const;

            /**
             * Base Perlin noise function returning value in [-1,1].
             */
            double noise(double x, double y, double z) const;

            /**
             * Layered (octave) Perlin noise normalized to [-1,1].
             */
            double octave_noise(double x, double y, double z, unsigned int octaves_in) const;
        };
      } // namespace Topography
    } // namespace OceanicPlateModels
  } // namespace Features
} // namespace WorldBuilder

#endif
