/*
  Copyright (C) 2018-2024 by the authors of the World Builder code.

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

#include "world_builder/features/oceanic_plate_models/temperature/ascii_data.h"


#include "world_builder/consts.h"
#include "world_builder/nan.h"
#include "world_builder/types/array.h"
#include "world_builder/types/double.h"
#include "world_builder/types/object.h"
#include "world_builder/types/one_of.h"
#include "world_builder/types/point.h"
#include "world_builder/types/value_at_points.h"
#include "world_builder/utilities.h"
#include "world_builder/world.h"
#include <cstdint>
#include <fstream>


namespace WorldBuilder
{
  using namespace Utilities;

  namespace Features
  {
    namespace OceanicPlateModels
    {
      namespace Temperature
      {
        AsciiData::AsciiData(WorldBuilder::World *world_)
          :
          min_depth(NaN::DSNAN),
          max_depth(NaN::DSNAN),
          operation(Operations::REPLACE)
        {
          this->world = world_;
          this->name = "ascii data";
        }

        AsciiData::~AsciiData()
          = default;

        void
        AsciiData::declare_entries(Parameters &prm, const std::string & /*unused*/)
        {
          // Document plugin and require entries if needed.
          // Add `ridge coordinates`, `spreading velocity`, `max depth` to the required parameters.
          prm.declare_entry("", Types::Object({"ridge coordinates", "spreading velocity", "max depth"}),
                            "Half space cooling mode");

          // Declare entries of this plugin
          prm.declare_entry("min depth", Types::OneOf(Types::Double(0),Types::Array(Types::ValueAtPoints(0., 2.))),
                            "The depth in meters from which the temperature of this feature is present.");

          prm.declare_entry("max depth", Types::OneOf(Types::Double(std::numeric_limits<double>::max()),Types::Array(Types::ValueAtPoints(std::numeric_limits<double>::max(), 2.))),
                            "The depth in meters to which the temperature of this feature is present."
                            "Because half-space reaches background temperature asymptotically, "
                            "this value should be ~2 times the nominal plate thickness of 100 km" );

          prm.declare_entry("spreading velocity", Types::OneOf(Types::Double(0.05),Types::Array(Types::ValueAtPoints(0.05, std::numeric_limits<uint64_t>::max()))),
                            "The spreading velocity of the plate in meter per year. "
                            "This is the velocity with which one side moves away from the ridge.");

          prm.declare_entry("ridge coordinates", Types::Array(Types::Array(Types::Point<2>(), 2),1),
                            "An list of ridges. Each ridge is a lists of at least 2 2d points which "
                            "define the location of the ridge. You need to define at least one ridge."
                            "So the an example with two ridges is "
                            "[[[10,20],[20,30],[10,40]],[[50,10],[60,10]]].");

          prm.declare_entry("data directory", Types::String("./"),
                            "The name of a directory that contains the model data. This path "
                            "may either be absolute (if starting with a `/') or relative to "
                            "the current directory.");

          prm.declare_entry("data file name", Types::String("temperature_data.txt"),
                            "The file name of the model data.");
        }

        void
        AsciiData::parse_entries(Parameters &prm, const std::vector<Point<2>> &coordinates)
        {
          min_depth_surface = Objects::Surface(prm.get("min depth",coordinates));
          min_depth = min_depth_surface.minimum;
          max_depth_surface = Objects::Surface(prm.get("max depth",coordinates));
          max_depth = max_depth_surface.maximum;
          operation = string_operations_to_enum(prm.get<std::string>("operation"));
          spreading_velocities = prm.get_value_at_array("spreading velocity");
          data_directory = prm.get<std::string>("data directory");
          data_file_name = prm.get<std::string>("data file name");

          mid_oceanic_ridges = prm.get_vector<std::vector<Point<2>>>("ridge coordinates");
          const double dtr = prm.coordinate_system->natural_coordinate_system() == spherical ? Consts::PI / 180.0 : 1.0;
          for (auto &ridge_coordinates : mid_oceanic_ridges)
            for (auto &ridge_coordinate : ridge_coordinates)
              {
                ridge_coordinate *= dtr;
              }

          unsigned int ridge_point_index = 0;
          for (const auto &mid_oceanic_ridge : mid_oceanic_ridges)
            {
              std::vector<double> spreading_rates_for_ridge;
              for (unsigned int index_y = 0; index_y < mid_oceanic_ridge.size(); index_y++)
                {
                  if (spreading_velocities.second.size() <= 1)
                    {
                      spreading_rates_for_ridge.push_back(spreading_velocities.second[0]);
                    }
                  else
                    {
                      spreading_rates_for_ridge.push_back(spreading_velocities.second[ridge_point_index]);
                    }
                  ridge_point_index += 1;
                }
              spreading_velocities_at_each_ridge_point.push_back(spreading_rates_for_ridge);
            }

          // Read in the data table
          if (!readDataFromFile(data_directory + data_file_name, age_depth_table))
            WBAssertThrow(false,
                          "Could not read depth-age data file.");

          // Example usage: printing the dimensions
          std::cout << "Reading " << age_depth_table.times.size()
                    << " time steps and " << age_depth_table.depths.size()
                    << " depth steps." << std::endl;
        }

        double
        AsciiData::get_temperature(const Point<3> &position,
                                   const Objects::NaturalCoordinate &position_in_natural_coordinates,
                                   const double depth,
                                   const double gravity_norm,
                                   double temperature_,
                                   const double /*feature_min_depth*/,
                                   const double /*feature_max_depth*/) const
        {
          if (depth <= max_depth && depth >= min_depth)
            {
              const double min_depth_local = min_depth_surface.constant_value ? min_depth : min_depth_surface.local_value(position_in_natural_coordinates.get_surface_point()).interpolated_value;
              const double max_depth_local = max_depth_surface.constant_value ? max_depth : max_depth_surface.local_value(position_in_natural_coordinates.get_surface_point()).interpolated_value;

              if (depth <= max_depth_local &&  depth >= min_depth_local)
                {
                  Objects::NaturalCoordinate position_in_natural_coordinates_at_min_depth = Objects::NaturalCoordinate(position,
                                                                                            *(world->parameters.coordinate_system));
                  position_in_natural_coordinates_at_min_depth.get_ref_depth_coordinate() += depth-min_depth;
                  std::vector<std::vector<double>> subducting_plate_velocities = {{0}};
                  std::vector<double> ridge_migration_times = {0.0};

                  double adiabatic_temperature =  this->world->potential_mantle_temperature *
                                                  std::exp(this->world->thermal_expansion_coefficient * gravity_norm * depth /
                                                           this->world->specific_heat);

                  std::vector<double> ridge_parameters = Utilities::calculate_ridge_distance_and_spreading(mid_oceanic_ridges,
                                                         spreading_velocities_at_each_ridge_point,
                                                         world->parameters.coordinate_system,
                                                         position_in_natural_coordinates_at_min_depth,
                                                         subducting_plate_velocities,
                                                         ridge_migration_times);

                  // Convert age to years
                  const double seconds_in_year = 60.0 * 60.0 * 24.0 * 365.25;
                  const double age = ridge_parameters[1] / (ridge_parameters[0] * seconds_in_year);

                  // Get temperature from ascii data file.
                  double temperature = interpolateTemperature (age_depth_table, age, depth);

                  // Apply adiabatic temperature gradient.
                  temperature += adiabatic_temperature - this->world->potential_mantle_temperature;

                  WBAssert(!std::isnan(temperature), "Temperature inside half-space cooling model is not a number: " << temperature
                           << ". Relevant variables: max_depth = " << max_depth
                           << ", spreading_velocity = " << ridge_parameters[0]
                           << ", age = " << age << '.');
                  WBAssert(std::isfinite(temperature), "Temperature inside half-space cooling model is not a finite: " << temperature
                           << ". Relevant variables: spreading_velocity = " << ridge_parameters[0]
                           << ", age = " << age << '.');

                  return apply_operation(operation,temperature_,temperature);

                }
            }
          return temperature_;
        }


        bool
        AsciiData::readDataFromFile(const std::string &filename,
                                    TableData &data) const
        {
          std::ifstream infile(filename);

          if (!infile.is_open())
            {
              std::cerr << "Error: Could not open file " << filename << std::endl;
              return false;
            }

          // Variables to hold Nx and Ny from the "# POINTS" line
          unsigned int Nx = 0;
          unsigned int Ny = 0;

          // Temporary variables to read the columns
          double timeVal, depthVal, tempVal;

          // For indexing as we read data
          unsigned int lineCount = 0;

          // Read the file line by line:
          std::string line;
          while (std::getline(infile, line))
            {
              // Trim or skip empty lines if necessary
              if (line.empty())
                continue;

              // Check if the line starts with "#" and is a comment
              if (line[0] == '#')
                {
                  // Look for the "# POINTS:" keyword that specifies number of lines
                  if (line.find("POINTS:") != std::string::npos)
                    {
                      // Parse Nx and Ny
                      std::istringstream iss(line);
                      std::string dummy; // to read "#", "POINTS:" etc.
                      iss >> dummy;      // read "#"
                      iss >> dummy;      // read "POINTS:"
                      iss >> Nx >> Ny;   // read Nx, Ny

                      if (Nx <= 0 || Ny <= 0)
                        {
                          std::cerr << "Error: Invalid Nx or Ny in points line." << std::endl;
                          return false;
                        }

                      // Now that we know Nx and Ny, we can allocate memory for data
                      data.times.resize(Nx);
                      data.depths.resize(Ny);
                      data.temperatures.resize(Ny, std::vector<double>(Nx, 0.0));
                    }
                  // We ignore other # lines (e.g., columns info, comments, etc.)
                  continue;
                }

              // Read in the data lines with format "time depth temperature"
              std::istringstream iss(line);
              if (!(iss >> timeVal >> depthVal >> tempVal))
                {
                  // If there's a parse error, handle it or continue
                  std::cerr << "Warning: Could not parse line:\n" << line << std::endl;
                  continue;
                }

              if (Nx == 0 || Ny == 0)
                {
                  // We have not encountered # POINTS yet but are seeing data lines
                  std::cerr << "Error: Data encountered before # POINTS line?" << std::endl;
                  return false;
                }

              if (lineCount >= Nx * Ny)
                {
                  // We already read Nx * Ny lines. Everything extra might be extraneous or error.
                  std::cerr << "Warning: More data lines than Nx*Ny. Additional lines are not read." << std::endl;
                  break;
                }

              // We assume data is in order such that time changes fastest,
              // then depth changes. i.e., the reading order is:
              //  depth=0: time=0, time=1, ..., time=Nx-1
              //  depth=1: time=0, time=1, ..., time=Nx-1
              //  ...
              const unsigned int k = lineCount; // index of the data line
              const unsigned int j = k / Nx;    // depth index
              const unsigned int i = k % Nx;    // time index

              // Store the time and depth only on the first occurrence for each row/column.
              if (j == 0)
                data.times[i] = timeVal;

              if (i == 0)
                data.depths[j] = depthVal;

              // Store temperature in 2D array
              data.temperatures[j][i] = tempVal;

              // Move to next data line
              ++lineCount;
            }

          // Check if we read the expected amount
          if (lineCount < Nx * Ny)
            {
              std::cerr << "Warning: Fewer data lines (" << lineCount
                        << ") than expected (" << Nx *Ny << ")." << std::endl;
            }

          return true; // successfully read the file
        }



        double
        AsciiData::interpolateTemperature(const TableData &table,
                                          const double age,
                                          const double depth) const
        {
          // 1) Find i such that table.times[i] <= age < table.times[i+1]
          const auto &times = table.times;
          const auto &depths = table.depths;
          const auto &temps = table.temperatures;

          if (times.empty() || depths.empty() || temps.empty())
            WBAssertThrow(false,
                          "TableData is empty. Cannot interpolate temperatures.");

          WBAssertThrow(age >= times.front() && age <= times.back(),
                        "You are asking for an age outside of the range stored "
                        "in the data table. Current age is " << age << ". "
                        "Ages allowed are from " << times.front() << " to "
                        << times.back() << ".");

          WBAssertThrow(depth >= depths.front() && depth <= depths.back(),
                        "You are asking for a depth outside of the range stored "
                        "in the data table.");

          // 2) Find the time bracket: i
          unsigned int i = 0;
          for (; i < times.size()-1; ++i)
            if (times[i] <= age && age < times[i+1])
              break;

          // 3) Find the depth bracket: j
          unsigned int j = 0;
          for (; j < depths.size()-1; ++j)
            if (depths[j] <= depth && depth < depths[j+1])
              break;

          // 4) Compute the fractional position within the bracket for both time & depth
          double t0 = times[i];
          double t1 = times[i+1];
          double d0 = depths[j];
          double d1 = depths[j+1];

          double timeFrac  = (age  - t0) / (t1 - t0);
          double depthFrac = (depth - d0) / (d1 - d0);

          // 5) Bilinear interpolation over the 4 corners:
          double T00 = temps[j  ][i  ];
          double T10 = temps[j  ][i+1];
          double T01 = temps[j+1][i  ];
          double T11 = temps[j+1][i+1];

          // Bilinear interpolation formula:
          // T = (1 - dx)*(1 - dy)*T00 + dx*(1 - dy)*T10 + (1 - dx)*dy*T01 + dx*dy*T11
          double T = (1 - timeFrac)*(1 - depthFrac) * T00
                     +      timeFrac*(1 - depthFrac) * T10
                     + (1 - timeFrac)*     depthFrac * T01
                     +      timeFrac*     depthFrac * T11;

          return T;
        }


        WB_REGISTER_FEATURE_OCEANIC_PLATE_TEMPERATURE_MODEL(AsciiData, ascii data)
      } // namespace Temperature
    } // namespace OceanicPlateModels
  } // namespace Features
} // namespace WorldBuilder

