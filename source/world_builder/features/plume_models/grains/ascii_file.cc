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

#include "world_builder/features/plume_models/grains/ascii_file.h"

#include "world_builder/nan.h"
#include "world_builder/types/array.h"
#include "world_builder/types/double.h"
#include "world_builder/types/object.h"
#include "world_builder/types/one_of.h"
#include "world_builder/types/unsigned_int.h"
#include "world_builder/types/value_at_points.h"
#include "world_builder/utilities.h"
#include "world_builder/world.h"

#include <fstream>
#include <set>
#include <sstream>

namespace WorldBuilder
{
  using namespace Utilities;

  namespace Features
  {
    namespace PlumeModels
    {
      namespace Grains
      {
        AsciiFile::AsciiFile(WorldBuilder::World *world_)
          :
          min_depth(NaN::DSNAN),
          max_depth(NaN::DSNAN),
          mineral_0_current_particle_index(0),
          mineral_1_current_particle_index(0)

        {
          this->world = world_;
          this->name = "ascii file";
        }

        AsciiFile::~AsciiFile()
          = default;

        void
        AsciiFile::declare_entries(Parameters &prm, const std::string & /*unused*/)
        {
          // Document plugin and require entries if needed.
          // Add compositions to the required parameters.
          prm.declare_entry("", Types::Object({"compositions"}),
                            "Ascii file grains model. The Euler angles and volume fractions of each "
                            "grain are read in from an ascii file.");

          // Declare entries of this plugin
          prm.declare_entry("min depth", Types::Double(0),
                            "The depth in meters from which the grains of this feature are present.");
          prm.declare_entry("max depth", Types::Double(std::numeric_limits<double>::max()),
                            "The depth in meters to which the grains of this feature are present.");

          prm.declare_entry("compositions", Types::Array(Types::UnsignedInt(),0),
                            "A list with the integer labels of the composition which are present there.");

          prm.declare_entry("file directory", Types::String(""),
                            "The directory where the ascii file is located.");

          prm.declare_entry("filename", Types::String(""),
                            "The name of the ascii file.");

        }

        void
        AsciiFile::parse_entries(Parameters &prm)
        {
          min_depth = prm.get<double>("min depth");
          max_depth = prm.get<double>("max depth");
          compositions = prm.get_vector<unsigned int>("compositions");

          directory   = prm.get<std::string>("file directory");
          filename    = prm.get<std::string>("filename");

          load_file();

        }

        void
        AsciiFile::load_file()
        {
          std::string full_path = directory + "/" + filename;
          std::ifstream in(full_path.c_str());
          WBAssertThrow(in, "Could not open ASCII file: " << full_path);

          std::string line;
          std::getline(in, line); // skip header

          std::set<unsigned int> unique_ids;

          while (std::getline(in, line))
            {
              std::istringstream iss(line);

              unsigned int id;
              if (compositions.size() == 1)
                {
                  double mineral_0_volume_fraction, mineral_0_phi, mineral_0_theta, mineral_0_z;

                  iss >> id >> mineral_0_volume_fraction >> mineral_0_phi >> mineral_0_theta >> mineral_0_z;

                  mineral_0_euler_angles_map[id].push_back({{mineral_0_phi, mineral_0_theta, mineral_0_z}});
                  mineral_0_volume_fraction_map[id].push_back(mineral_0_volume_fraction);

                  unique_ids.insert(id);
                }
              else if (compositions.size() == 2)
                {
                  double mineral_0_volume_fraction, mineral_0_phi, mineral_0_theta, mineral_0_z;
                  double mineral_1_volume_fraction, mineral_1_phi, mineral_1_theta, mineral_1_z;

                  iss >> id >> mineral_0_volume_fraction >> mineral_0_phi >> mineral_0_theta >> mineral_0_z >> mineral_1_volume_fraction >> mineral_1_phi >> mineral_1_theta >> mineral_1_z;

                  mineral_0_euler_angles_map[id].push_back({{mineral_0_phi, mineral_0_theta, mineral_0_z}});
                  mineral_0_volume_fraction_map[id].push_back(mineral_0_volume_fraction);

                  mineral_1_euler_angles_map[id].push_back({{mineral_1_phi, mineral_1_theta, mineral_1_z}});
                  mineral_1_volume_fraction_map[id].push_back(mineral_1_volume_fraction);

                  unique_ids.insert(id);
                }
              else
                {
                  WBAssertThrow(false,
                                "Currently only 1 or 2 compositions (Olivine and Enstatite) are supported for the ASCII file grains model.");
                }

            }

          in.close();

          n_particles = static_cast<unsigned int>(unique_ids.size());
          WBAssertThrow(n_particles > 0,
                        "ASCII file contains no particles.");

          mineral_0_n_grains = static_cast<unsigned int>(mineral_0_euler_angles_map.begin()->second.size());
          // --- consistency checks ---
          for (const auto &pair : mineral_0_euler_angles_map)
            {
              WBAssertThrow(pair.second.size() == mineral_0_n_grains,
                            "Particle " << pair.first
                            << " has inconsistent number of grains of Olivine.");

              WBAssertThrow(mineral_0_volume_fraction_map[pair.first].size() == mineral_0_n_grains,
                            "Mismatch between Euler angles / volume fractions for Olivine in particle "
                            << pair.first);
            }
          if (compositions.size() == 2)
            {
              mineral_1_n_grains = static_cast<unsigned int>(mineral_1_euler_angles_map.begin()->second.size());
              WBAssertThrow(mineral_0_n_grains == mineral_1_n_grains,
                            "Inconsistent grain count between Olivine and Enstatite in ASCII file.");
              for (const auto &pair : mineral_1_euler_angles_map)
                {
                  WBAssertThrow(pair.second.size() == mineral_1_n_grains,
                                "Particle " << pair.first
                                << " has inconsistent number of grains of Enstatite.");

                  WBAssertThrow(mineral_1_volume_fraction_map[pair.first].size() == mineral_1_n_grains,
                                "Mismatch between Euler angles / volume fractions for Enstatite in particle "
                                << pair.first);
                }
            }
        }

        WorldBuilder::grains
        AsciiFile::get_grains(const Point<3> & /*position_in_cartesian_coordinates*/,
                              const Objects::NaturalCoordinate & /*position_in_natural_coordinates*/,
                              const double depth,
                              const unsigned int composition_number,
                              WorldBuilder::grains grains_,
                              const double  /*feature_min_depth*/,
                              const double /*feature_max_depth*/) const
        {
          WorldBuilder::grains  grains_local = grains_;
          if (depth <= max_depth && depth >= min_depth)
            {
              for (unsigned int i =0; i < compositions.size(); ++i)
                {
                  if (compositions[i] == composition_number)
                    {
                      if (composition_number == 0)
                        {
                          const unsigned int particle_id = mineral_0_current_particle_index++;
                          auto it = mineral_0_euler_angles_map.find(particle_id);
                          WBAssertThrow(it != mineral_0_euler_angles_map.end(),
                                        "Particle id " << particle_id << " not found in ASCII file.");

                          const auto &angles = it->second;
                          const auto &fractions = mineral_0_volume_fraction_map.at(particle_id);

                          WBAssertThrow(grains_local.rotation_matrices.size() == mineral_0_n_grains,
                                        "Mismatch: ASPECT grains ("
                                        << grains_local.rotation_matrices.size()
                                        << ") vs World Builder input file grains (" << mineral_0_n_grains << ").");

                          for (size_t g = 0; g < mineral_0_n_grains; ++g)
                            {
                              grains_local.rotation_matrices[g] =
                                Utilities::euler_angles_to_rotation_matrix(
                                  angles[g][0], angles[g][1], angles[g][2]);

                              grains_local.sizes[g] = fractions[g];
                            }

                          // --- normalize ---
                          double total = 0.0;
                          for (const auto &s : grains_local.sizes)
                            total += s;

                          WBAssertThrow(total > 0,
                                        "Total grain volume fraction is zero for particle "
                                        << particle_id);

                          const double inv_total = 1.0 / total;
                          for (auto &s : grains_local.sizes)
                            s *= inv_total;

                          return grains_local;
                        }
                      else if (composition_number == 1)
                        {
                          const unsigned int particle_id = mineral_1_current_particle_index++;
                          auto it = mineral_1_euler_angles_map.find(particle_id);
                          WBAssertThrow(mineral_1_euler_angles_map.find(particle_id) != mineral_1_euler_angles_map.end(),
                                        "Particle id " << particle_id << " not found in ASCII file.");

                          const auto &angles = it->second;
                          const auto &fractions = mineral_1_volume_fraction_map.at(particle_id);

                          WBAssertThrow(grains_local.rotation_matrices.size() == mineral_1_n_grains,
                                        "Mismatch: ASPECT grains ("
                                        << grains_local.rotation_matrices.size()
                                        << ") vs World Builder input file grains (" << mineral_1_n_grains << ").");

                          for (size_t g = 0; g < mineral_1_n_grains; ++g)
                            {
                              grains_local.rotation_matrices[g] =
                                Utilities::euler_angles_to_rotation_matrix(
                                  angles[g][0], angles[g][1], angles[g][2]);

                              grains_local.sizes[g] = fractions[g];
                            }

                          double total = 0.0;
                          for (const auto &s : grains_local.sizes)
                            total += s;

                          WBAssertThrow(total > 0,
                                        "Total grain volume fraction is zero for particle "
                                        << particle_id);

                          const double inv_total = 1.0 / total;
                          for (auto &s : grains_local.sizes)
                            s *= inv_total;

                          return grains_local;
                        }
                      else
                        {
                          WBAssertThrow(false,
                                        "Currently only 2 compositions (Olivine=0 and Enstatite=1) are supported for the ASCII file grains model.");
                        }
                    }
                }
            }
          return grains_local;
        }
        WB_REGISTER_FEATURE_PLUME_GRAINS_MODEL(AsciiFile, ascii file)
      } // namespace Grains
    } // namespace PlumeModels
  } // namespace Features
} // namespace WorldBuilder

