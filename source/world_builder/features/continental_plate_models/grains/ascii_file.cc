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

#include "world_builder/features/continental_plate_models/grains/ascii_file.h"

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
    namespace ContinentalPlateModels
    {
      namespace Grains
      {
        AsciiFile::AsciiFile(WorldBuilder::World *world_)
          :
          file_loaded(false),
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
          prm.declare_entry("min depth", Types::OneOf(Types::Double(0),Types::Array(Types::ValueAtPoints(0.,2))),
                            "The depth in meters from which the composition of this feature is present.");
          prm.declare_entry("max depth", Types::OneOf(Types::Double(std::numeric_limits<double>::max()),Types::Array(Types::ValueAtPoints(std::numeric_limits<double>::max(),2))),
                            "The depth in meters to which the composition of this feature is present.");

          prm.declare_entry("compositions", Types::Array(Types::UnsignedInt(),0),
                            "A list with the integer labels of the composition which are present there.");

          prm.declare_entry("file directory", Types::String(""),
                            "The directory where the ascii file is located.");

          prm.declare_entry("filename", Types::String(""),
                            "The name of the ascii file.");

        }

        void
        AsciiFile::parse_entries(Parameters &prm, const std::vector<Point<2>> &coordinates)
        {
          min_depth_surface = Objects::Surface(prm.get("min depth",coordinates));
          min_depth = min_depth_surface.minimum;
          max_depth_surface = Objects::Surface(prm.get("max depth",coordinates));
          max_depth = max_depth_surface.maximum;
          compositions = prm.get_vector<unsigned int>("compositions");

          directory   = prm.get<std::string>("file directory");
          filename    = prm.get<std::string>("filename");

          if (!filename.empty())
            {
              load_file();
              file_loaded = true;
            }
          else
            {
              file_loaded = false;
              WBAssertThrow(!filename.empty(),
                            "AsciiFile grains model: 'filename' is empty, cannot load file.");
            }

        }

        void
        AsciiFile::load_file()
        {
          std::string full_path = directory + "/" + filename;
          std::string file_content = Utilities::read_and_distribute_file_content(full_path);
          WBAssertThrow(!file_content.empty(),
                        "ASCII file is empty or could not be read: " << full_path);
          std::stringstream in(file_content);

          std::string line;
          std::getline(in, line); // skip header

          std::set<unsigned int> unique_ids;

          while (std::getline(in, line))
            {
              std::istringstream iss(line);

              unsigned int id;
              iss >> id;
              WBAssertThrow(iss, "Malformed line in ASCII file: " << line);

              // Helper lambda to read one mineral's data
              auto read_mineral = [&](std::map<unsigned int, std::vector<std::array<double,3>>> &angles_map,
                                      std::map<unsigned int, std::vector<double>> &fraction_map)
              {
                double volume_fraction, phi, theta, z;
                iss >> volume_fraction >> phi >> theta >> z;
                WBAssertThrow(iss, "Malformed mineral data in ASCII file: " << line);

                angles_map[id].push_back(std::array<double,3> {{phi, theta, z}});
                fraction_map[id].push_back(volume_fraction);
              };

              // We expect only 1 composition (Olivine) or 2 compositions (Olivine and Enstatite) in the file, and we read them accordingly.
              // If there are more than 2 compositions, we throw an error.
              if (compositions.size() == 1)
                {
                  read_mineral(mineral_0_euler_angles_map, mineral_0_volume_fraction_map);
                }
              else if (compositions.size() == 2)
                {
                  read_mineral(mineral_0_euler_angles_map, mineral_0_volume_fraction_map);
                  read_mineral(mineral_1_euler_angles_map, mineral_1_volume_fraction_map);
                }
              else
                {
                  WBAssertThrow(false,
                                "Currently only 1 or 2 compositions (Olivine and Enstatite) are supported for the ASCII file grains model.");
                }

              // Record the particle id
              unique_ids.insert(id);

            }

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
                              const Objects::NaturalCoordinate &position_in_natural_coordinates,
                              const double depth,
                              const unsigned int composition_number,
                              WorldBuilder::grains grains_,
                              const double  /*feature_min_depth*/,
                              const double /*feature_max_depth*/) const
        {
          WBAssertThrow(file_loaded,
                        "AsciiFile grains model: get_grains() called but no file was loaded. "
                        "Check that 'filename' is set correctly in the input file.");

          WorldBuilder::grains  grains_local = grains_;
          if (depth <= max_depth && depth >= min_depth)
            {
              const double min_depth_local = min_depth_surface.constant_value ? min_depth : min_depth_surface.local_value(position_in_natural_coordinates.get_surface_point()).interpolated_value;
              const double max_depth_local = max_depth_surface.constant_value ? max_depth : max_depth_surface.local_value(position_in_natural_coordinates.get_surface_point()).interpolated_value;
              if (depth <= max_depth_local &&  depth >= min_depth_local)
                {
                  for (unsigned int i =0; i < compositions.size(); ++i)
                    {
                      if (compositions[i] == composition_number)
                        {
                          auto fill_grains = [&](unsigned int &current_particle_index,
                                                 const std::map<unsigned int, std::vector<std::array<double,3>>> &euler_map,
                                                 const std::map<unsigned int, std::vector<double>> &volume_map,
                                                 const unsigned int n_grains)
                          {
                            const unsigned int particle_id = current_particle_index++;
                            auto it = euler_map.find(particle_id);
                            WBAssertThrow(it != euler_map.end(),
                                          "Particle id " << particle_id << " not found in ASCII file.");

                            const auto &angles = it->second;
                            const auto &fractions = volume_map.at(particle_id);

                            WBAssertThrow(grains_local.rotation_matrices.size() == n_grains,
                                          "Mismatch: ASPECT grains ("
                                          << grains_local.rotation_matrices.size()
                                          << ") vs World Builder input file grains (" << n_grains << ").");

                            for (size_t g = 0; g < n_grains; ++g)
                              {
                                grains_local.rotation_matrices[g] =
                                  Utilities::euler_angles_to_rotation_matrix(
                                    angles[g][0], angles[g][1], angles[g][2]);
                                grains_local.sizes[g] = fractions[g];
                              }

                            // normalize
                            double total = 0.0;
                            for (const auto &s : grains_local.sizes)
                              total += s;

                            WBAssertThrow(total > 0,
                                          "Total grain volume fraction is zero for particle " << particle_id);

                            const double inv_total = 1.0 / total;
                            for (auto &s : grains_local.sizes)
                              s *= inv_total;

                            return grains_local;
                          };

                          // If the composition_number is 0 or 1, we fill the grains with Olivine and Enstatite respectively.
                          // If not, we throw an error.
                          if (composition_number == 0)
                            return fill_grains(mineral_0_current_particle_index, mineral_0_euler_angles_map,
                                               mineral_0_volume_fraction_map, mineral_0_n_grains);
                          else if (composition_number == 1)
                            return fill_grains(mineral_1_current_particle_index, mineral_1_euler_angles_map,
                                               mineral_1_volume_fraction_map, mineral_1_n_grains);
                          else
                            WBAssertThrow(false,
                                          "Currently only 2 compositions (Olivine=0 and Enstatite=1) are supported for the ASCII file grains model.");
                        }
                    }
                }
            }
          return grains_local;
        }
        WB_REGISTER_FEATURE_CONTINENTAL_PLATE_GRAINS_MODEL(AsciiFile, ascii file)
      } // namespace Grains
    } // namespace ContinentalPlateModels
  } // namespace Features
} // namespace WorldBuilder

