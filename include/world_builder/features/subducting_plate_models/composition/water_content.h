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
            double calculate_water_content(double pressure,
                                           double temperature,
                                           std::string lithology) const;
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
                                   const AdditionalParameters &additional_parameters) const override final;


          private:
            // water_content composition submodule parameters
            double min_depth;
            double max_depth;
            double density; // Density used to calculate the lithostatic pressure
            std::vector<unsigned int> compositions;
            Operations operation;
            std::string lithology_str;
            double max_water_content;

            // Peridotite polynomial coefficients
            std::vector<double> LR_poly_peridotite = {-19.0609, 168.983, -630.032, 1281.84, -1543.14, 1111.88, -459.142, 95.4143, 1.97246};
            std::vector<double> c_sat_poly_peridotite = {0.00115628, 2.42179};
            std::vector<double> Td_poly_peridotite = {-15.4627, 94.9716, 636.603};

            // Gabbro polynomial coefficients
            std::vector<double> LR_poly_gabbro = {-1.81745, 7.67198, -10.8507, 5.09329, 8.14519};
            std::vector<double> c_sat_poly_gabbro = {-0.0176673, 0.0893044, 1.52732};
            std::vector<double> Td_poly_gabbro = {-1.72277, 20.5898, 637.517};

            // MORB polynomial coefficients
            std::vector<double> LR_poly_MORB = {-1.78177, 7.50871, -10.4840, 5.19725, 7.96365};
            std::vector<double> c_sat_poly_MORB = {0.0102725, -0.115390, 0.324452, 1.41588};
            std::vector<double> Td_poly_MORB = {-3.81280, 22.7809, 638.049};

            // Sediment polynomial coefficients
            std::vector<double> LR_poly_sediment = {-2.03283, 10.8186, -21.2119, 18.3351, -6.48711, 8.32459};
            std::vector<double> c_sat_poly_sediment = {-0.150662, 0.301807, 1.01867};
            std::vector<double> Td_poly_sediment = {2.83277, -24.7593, 85.9090, 524.898};

            // Maximum pressure for the lithologies (Peridotite, Gabbro, MORB, Sediment). Above these
            // pressures, the parameterized phase diagrams break down and the solubility goes to infinity.
            std::vector<double> pressure_cutoffs {10, 26, 16, 1.0};
        };
      } // namespace Composition
    } // namespace SubductingPlateModels
  } // namespace Features
} // namespace WorldBuilder

#endif
