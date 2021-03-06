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

#ifndef WORLD_BUILDER_FEATURES_CONTINENTAL_PLATE_H
#define WORLD_BUILDER_FEATURES_CONTINENTAL_PLATE_H


#include "world_builder/features/interface.h"


namespace WorldBuilder
{
  class Parameters;
  class World;

  namespace Features
  {
    namespace ContinentalPlateModels
    {
      namespace Composition
      {
        class Interface;
      }  // namespace Composition
      namespace Grains
      {
        class Interface;
      }  // namespace Grains
      namespace Temperature
      {
        class Interface;
      }  // namespace Temperature
    }  // namespace ContinentalPlateModels

    /**
     * This class represents a continental plate and can implement submodules
     * for temperature and composition. These submodules determine what
     * the returned temperature or composition of the temperature and composition
     * functions of this class will be.
     */
    class ContinentalPlate final: public Interface
    {
      public:
        /**
         * constructor
         */
        ContinentalPlate(WorldBuilder::World *world);

        /**
         * Destructor
         */
        ~ContinentalPlate() override final;

        /**
         * declare and read in the world builder file into the parameters class
         */
        static
        void declare_entries(Parameters &prm,
                             const std::string &parent_name = "",
                             const std::vector<std::string> &required_entries = {});

        /**
         * declare and read in the world builder file into the parameters class
         */
        void parse_entries(Parameters &prm) override final;


        /**
         * Returns a temperature based on the given position, depth in the model,
         * gravity and current temperature.
         */
        double temperature(const Point<3> &position_in_cartesian_coordinates,
                           const WorldBuilder::Utilities::NaturalCoordinate &position_in_natural_coordinates,
                           const double depth,
                           const double gravity,
                           double temperature) const override final;

        /**
         * Returns a value for the requests composition (0 is not present,
         * 1 is present) based on the given position, depth in the model,
         * the composition which is being requested and the current value
         * of that composition at this location and depth.
         */
        double composition(const Point<3> &position_in_cartesian_coordinates,
                           const WorldBuilder::Utilities::NaturalCoordinate &position_in_natural_coordinates,
                           const double depth,
                           const unsigned int composition_number,
                           double composition) const override final;

        /**
         * Returns a grains (rotation matrix and grain size) based on the
         * given position, depth in the model, the composition (e.g. representing
         * olvine and/or enstatite) which is being requested and the current value
         * of that composition at this location and depth.
         */

        WorldBuilder::grains
        grains(const Point<3> &position_in_cartesian_coordinates,
               const WorldBuilder::Utilities::NaturalCoordinate &position_in_natural_coordinates,
               const double depth,
               const unsigned int composition_number,
               WorldBuilder::grains grains) const override final;



      private:
        /**
         * A vector containing all the pointers to the temperature models. This vector is
         * responsible for the features and has ownership over them. Therefore
         * unique pointers are used.
         * @see Features
         */
        std::vector<std::unique_ptr<Features::ContinentalPlateModels::Temperature::Interface> > temperature_models;

        /**
         * A vector containing all the pointers to the composition models. This vector is
         * responsible for the features and has ownership over them. Therefore
         * unique pointers are used.
         * @see Features
         */
        std::vector<std::unique_ptr<Features::ContinentalPlateModels::Composition::Interface> > composition_models;

        /**
         * A vector containing all the pointers to the grains models. This vector is
         * responsible for the features and has ownership over them. Therefore
         * unique pointers are used.
         * @see Features
         */
        std::vector<std::unique_ptr<Features::ContinentalPlateModels::Grains::Interface> > grains_models;


        double min_depth;
        double max_depth;

    };


  } // namespace Features
} // namespace WorldBuilder

#endif
