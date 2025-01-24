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

#ifndef WORLD_BUILDER_FEATURES_OCEANIC_PLATE_MODELS_ASCII_DATA_H
#define WORLD_BUILDER_FEATURES_OCEANIC_PLATE_MODELS_ASCII_DATA_H


#include "world_builder/features/oceanic_plate_models/temperature/interface.h"
#include "world_builder/features/feature_utilities.h"
#include "world_builder/objects/surface.h"


namespace WorldBuilder
{
  class Parameters;
  class World;

  namespace Features
  {
    using namespace FeatureUtilities;
    namespace OceanicPlateModels
    {
      namespace Temperature
      {
        /**
         * This class represents a oceanic plate and can implement submodules
         * for temperature and composition. These submodules determine what
         * the returned temperature or composition of the temperature and composition
         * functions of this class will be.
         */
        class AsciiData final: public Interface
        {
          public:
            /**
             * constructor
             */
            AsciiData(WorldBuilder::World *world);

            /**
             * Destructor
             */
            ~AsciiData() override final;

            /**
             * declare and read in the world builder file into the parameters class
             */
            static
            void declare_entries(Parameters &prm, const std::string &parent_name = "");

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
            // plate model temperature submodule parameters
            double min_depth;
            Objects::Surface min_depth_surface;
            double max_depth;
            Objects::Surface max_depth_surface;
            std::string data_file_name;
            std::string data_directory;
            std::pair<std::vector<double>,std::vector<double>> spreading_velocities;
            std::vector<std::vector<Point<2> > > mid_oceanic_ridges;
            std::vector<std::vector<double>> spreading_velocities_at_each_ridge_point;
            Operations operation;

            struct TableData
            {
              std::vector<double> times;                       // size Nx
              std::vector<double> depths;                      // size Ny
              std::vector<std::vector<double>> temperatures;   // size Ny x Nx
            };

            TableData age_depth_table;

            /**
             * A function that reads in the ascii data file that provides
             * the temperature distribution in dependence of age and depth
             * and returns it as TableData.
             */
            bool readDataFromFile(const std::string &filename,
                                  TableData &data) const;

            /**
             * A function that extracts the temperature from the table structure
             * by linearly interpolating between the closest entries for depth
             * and age.
             */
            double interpolateTemperature(const TableData &table,
                                          const double age,
                                          const double depth) const;


        };
      } // namespace Temperature
    } // namespace OceanicPlateModels
  } // namespace Features
} // namespace WorldBuilder

#endif
