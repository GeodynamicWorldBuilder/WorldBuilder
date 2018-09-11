/*
  Copyright (C) 2018 by the authors of the World Builder code.

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

#ifndef _world_feature_features_continental_plate_h
#define _world_feature_features_continental_plate_h

#include <world_builder/features/interface.h>
#include <world_builder/world.h>


namespace WorldBuilder
{
  namespace Features
  {

    /**
     * This class represents a continental plate and can implement submodules
     * for temperature and composition. These submodules determine what
     * the returned temperature or composition of the temperature and composition
     * functions of this class will be.
     */
    class ContinentalPlate : public Interface
    {
      public:
        /**
         * constructor
         */
        ContinentalPlate(WorldBuilder::World *world);

        /**
         * Destructor
         */
        ~ContinentalPlate();

        /**
         * declare and read in the world builder file into the parameters class
         */
        virtual
        void decare_entries();


        /**
         * Returns a temperature based on the given position
         */
        virtual
        double temperature(const Point<3> &position,
                           const double depth,
                           const double gravity,
                           double temperature) const;

        /**
         * Returns a value for the requests composition (0 is not present,
         * 1 is present) based on the given position and
         */
        virtual
        double composition(const Point<3> &position,
                           const double depth,
                           const unsigned int composition_number,
                           double value) const;



      private:
        // constant temperature submodule parameters
        double temperature_submodule_constant_depth;
        double temperature_submodule_constant_temperature;

        // linear submodule parameters
        double temperature_submodule_linear_depth;
        double temperature_submodule_linear_top_temperature;
        double temperature_submodule_linear_bottom_temperature;

        // constant composition submodule parameters
        double composition_submodule_constant_depth;
        unsigned int composition_submodule_constant_composition;
        double composition_submodule_constant_value;

        // constant layers composition submodule parameters
        std::vector<int> composition_submodule_constant_layers_compositions;
        std::vector<int> composition_submodule_constant_layers_composition_value;
        std::vector<double> composition_submodule_constant_layers_thicknesses;
    };
  }
}

#endif
