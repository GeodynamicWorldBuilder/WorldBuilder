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

#include "world_builder/features/continental_plate_models/topography/perlin_noise.h"

#include "world_builder/nan.h"
#include "world_builder/types/double.h"
#include "world_builder/types/object.h"
#include "world_builder/types/array.h"
#include "world_builder/types/one_of.h"
#include "world_builder/types/value_at_points.h"
#include "world_builder/types/unsigned_int.h"
#include "world_builder/world.h"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>

namespace WorldBuilder
{
  using namespace Utilities;

  namespace Features
  {
    using namespace FeatureUtilities;
    namespace ContinentalPlateModels
    {
      namespace Topography
      {
        PerlinNoise::PerlinNoise(WorldBuilder::World *world_)
          :
          min_depth(NaN::DSNAN),
          max_depth(NaN::DSNAN),
          min_topography(NaN::DSNAN),
          max_topography(NaN::DSNAN),
          frequency(NaN::DSNAN),
          octaves(0),
          persistence(NaN::DSNAN),
          lacunarity(NaN::DSNAN),
          operation(Operations::ADD)
        {
          this->world = world_;
          this->name = "perlin_noise";

          std::iota(p.begin(), p.end(), 0);
        }

        PerlinNoise::~PerlinNoise() = default;

        void
        PerlinNoise::declare_entries(Parameters &prm, const std::string &)
        {
          // Document plugin. Unlike some topography plugins (e.g. DepthSurface)
          // this model does not require a single `topography` field; instead
          // it exposes `min topography` / `max topography` and Perlin settings.
          prm.declare_entry("", Types::Object(),
                            "Perlin noise topography model. Generates topography values in a range.");

          prm.declare_entry("min depth", Types::OneOf(Types::Double(0), Types::Array(Types::ValueAtPoints(0.,2)), Types::String("")),
                            "The depth in meters from which the topography of this feature is present.");

          prm.declare_entry("max depth", Types::OneOf(Types::Double(std::numeric_limits<double>::max()), Types::Array(Types::ValueAtPoints(std::numeric_limits<double>::max(),2)), Types::String("")),
                            "The depth in meters to which the topography of this feature is present.");

          prm.declare_entry("min topography", Types::Double(-1000.0), "Minimum topography in meters.");
          prm.declare_entry("max topography", Types::Double(1000.0), "Maximum topography in meters.");
          prm.declare_entry("frequency", Types::Double(1.0), "Frequency of the Perlin noise.");
          prm.declare_entry("octaves", Types::UnsignedInt(1), "Number of octaves for the Perlin noise.");
          prm.declare_entry("persistence", Types::Double(0.5), "Amplitude multiplier between octaves.");
          prm.declare_entry("lacunarity", Types::Double(2.0), "Frequency multiplier between octaves.");
          prm.declare_entry("operation", Types::String("add", std::vector<std::string>{"replace","replace defined only","add","subtract"}),
                            "How to combine the topography with existing values.");
        }

        void
        PerlinNoise::parse_entries(Parameters &prm, const std::vector<Point<2>> &coordinates)
        {
          // Parse parameter values and prepare internal state. The permutation
          // table `p` is shuffled deterministically from the world's RNG so
          // different runs produce different noise realizations when desired.
          min_depth_surface = Objects::Surface(prm.get("min depth",coordinates));
          min_depth = min_depth_surface.minimum;
          max_depth_surface = Objects::Surface(prm.get("max depth",coordinates));
          max_depth = max_depth_surface.maximum;

          min_topography = prm.get<double>("min topography");
          max_topography = prm.get<double>("max topography");

          frequency = prm.get<double>("frequency");
          octaves = prm.get<unsigned int>("octaves");
          persistence = prm.get<double>("persistence");
          lacunarity = prm.get<double>("lacunarity");

          auto deterministic_bounded_draw = [](std::mt19937 &engine, const unsigned int upper_inclusive) -> unsigned int
          {
            const unsigned long long range = static_cast<unsigned long long>(upper_inclusive) + 1ULL;
            const unsigned long long limit = (4294967296ULL / range) * range;

            unsigned long long value = 0ULL;
            do
              value = static_cast<unsigned long long>(engine());
            while (value >= limit);

            return static_cast<unsigned int>(value % range);
          };

          std::mt19937 &engine = world->get_random_number_engine();
          for (unsigned int i = 255; i > 0; --i)
            {
              const unsigned int j = deterministic_bounded_draw(engine, i);
              std::swap(p[i], p[j]);
            }

          operation = string_operations_to_enum(prm.get<std::string>("operation"));
        }

        double
        PerlinNoise::get_topography(const Point<3> & /*position_in_cartesian_coordinates*/,
                                    const Objects::NaturalCoordinate &position_in_natural_coordinates,
                                    double topography) const
        {
          // Compute topography contribution at `position` if the point lies
          // within the model's depth range. The Perlin result is scaled into
          // [min_topography, max_topography] and combined with the supplied
          // `topography` according to `operation`.
          const double depth = position_in_natural_coordinates.get_depth_coordinate();
          if (depth <= max_depth && depth >= min_depth)
            {
              const double min_depth_local = min_depth_surface.constant_value ? min_depth : min_depth_surface.local_value(position_in_natural_coordinates.get_surface_point()).interpolated_value;
              const double max_depth_local = max_depth_surface.constant_value ? max_depth : max_depth_surface.local_value(position_in_natural_coordinates.get_surface_point()).interpolated_value;
              if (depth <= max_depth_local && depth >= min_depth_local)
                {
                  // Use a 2D Perlin field for topography: sample at the surface
                  // x/y coordinates and fixed z = 0.0 so topography does not vary
                  // with depth.
                  const Point<2> surface_point = position_in_natural_coordinates.get_surface_point();
                  const double noise_val = octave_noise(surface_point[0], surface_point[1], 0.0, octaves);
                  const double new_topography = min_topography + (noise_val * 0.5 + 0.5) * (max_topography - min_topography);
                  return apply_operation(operation, topography, new_topography);
                }
            }

          return topography;
        }

        double
        PerlinNoise::fade(double t) const
        {
          return t * t * t * (t * (t * 6 - 15) + 10);
        }

        double
        PerlinNoise::lerp(double t, double a, double b) const
        {
          return a + t * (b - a);
        }

        double
        PerlinNoise::grad(int hash, double x, double y, double z) const
        {
          const int h = hash & 15;
          const double u = h < 8 ? x : y;
          const double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
          return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
        }

        double
        PerlinNoise::noise(double x, double y, double z) const
        {
          const int X = static_cast<int>(std::floor(x)) & 255;
          const int Y = static_cast<int>(std::floor(y)) & 255;
          const int Z = static_cast<int>(std::floor(z)) & 255;
          x -= std::floor(x);
          y -= std::floor(y);
          z -= std::floor(z);
          const double u = fade(x);
          const double v = fade(y);
          const double w = fade(z);

          auto perm = [this](const int idx) -> int
          {
            return p[idx & 255];
          };

          const int A = perm(X) + Y;
          const int AA = perm(A) + Z;
          const int AB = perm(A + 1) + Z;
          const int B = perm(X + 1) + Y;
          const int BA = perm(B) + Z;
          const int BB = perm(B + 1) + Z;

          return lerp(w, lerp(v, lerp(u, grad(perm(AA), x, y, z),
                    grad(perm(BA), x - 1, y, z)),
                  lerp(u, grad(perm(AB), x, y - 1, z),
                 grad(perm(BB), x - 1, y - 1, z))),
                lerp(v, lerp(u, grad(perm(AA + 1), x, y, z - 1),
                 grad(perm(BA + 1), x - 1, y, z - 1)),
               lerp(u, grad(perm(AB + 1), x, y - 1, z - 1),
              grad(perm(BB + 1), x - 1, y - 1, z - 1))));
        }

        double
        PerlinNoise::octave_noise(double x, double y, double z, unsigned int octaves_in) const
        {
          if (octaves_in == 0)
            return 0;

          double total = 0;
          double frequency_local = frequency;
          double amplitude = 1;
          double max_value_local = 0;
          for (unsigned int i = 0; i < octaves_in; ++i)
            {
              total += noise(x * frequency_local, y * frequency_local, z * frequency_local) * amplitude;
              max_value_local += amplitude;
              amplitude *= persistence;
              frequency_local *= lacunarity;
            }

          if (max_value_local <= 0.0)
            return 0;

          return total / max_value_local;
        }

        WB_REGISTER_FEATURE_CONTINENTAL_PLATE_TOPOGRAPHY_MODEL(PerlinNoise, perlin noise)
      }
    }
  }
}
