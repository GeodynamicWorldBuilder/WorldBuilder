/*
  Perlin noise topography model header
*/

#ifndef WORLD_BUILDER_FEATURES_CONTINENTAL_PLATE_MODELS_TOPOGRAPHY_PERLIN_NOISE_H
#define WORLD_BUILDER_FEATURES_CONTINENTAL_PLATE_MODELS_TOPOGRAPHY_PERLIN_NOISE_H

#include "world_builder/features/continental_plate_models/topography/interface.h"
#include "world_builder/features/feature_utilities.h"
#include "world_builder/objects/surface.h"
#include <array>
#include <cstdint>

namespace WorldBuilder
{

  namespace Features
  {
    using namespace FeatureUtilities;
    namespace ContinentalPlateModels
    {
      namespace Topography
      {
        /**
         * Perlin noise based topography model for continental plates.
         */
        class PerlinNoise final: public Interface
        {
          public:
            PerlinNoise(WorldBuilder::World *world);
            ~PerlinNoise() override final;

            static void declare_entries(Parameters &prm, const std::string &parent_name = "");
            void parse_entries(Parameters &prm, const std::vector<Point<2>> &coordinates) override final;

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

            double fade(double t) const;
            double lerp(double t, double a, double b) const;
            double grad(int hash, double x, double y, double z) const;
            double noise(double x, double y, double z) const;
            double octave_noise(double x, double y, double z, unsigned int octaves_in) const;
        };
      } // namespace Topography
    } // namespace ContinentalPlateModels
  } // namespace Features
} // namespace WorldBuilder

#endif
