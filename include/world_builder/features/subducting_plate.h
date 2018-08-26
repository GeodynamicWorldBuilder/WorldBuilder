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

#ifndef _world_feature_features_subducting_plate_h
#define _world_feature_features_subducting_plate_h

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
    class SubductingPlate : public Interface
    {
      public:
        /**
         * constructor
         */
    	SubductingPlate(WorldBuilder::World *world);

        /**
         * Destructor
         */
        ~SubductingPlate();

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
        bool composition(const Point<3> &position,
                         const double depth,
                         const unsigned int composition_number,
                         bool temperature) const;



      private:
        //
        double starting_depth;
        double maximum_depth;
        Point<2> reference_point;

        std::vector<std::vector<double> > slab_segment_lengths;
        std::vector<std::vector<Point<2> > > slab_segment_thickness;
        std::vector<std::vector<Point<2> > > slab_segment_angles;
        std::vector<double> total_slab_length;
        double maximum_total_slab_length;
        double maximum_slab_thickness;

        // constant temperature submodule parameters
        double temperature_submodule_constant_depth;
        double temperature_submodule_constant_temperature;

        // linear submodule parameters
        double temperature_submodule_linear_depth;
        double temperature_submodule_linear_top_temperature;
        double temperature_submodule_linear_bottom_temperature;

        // plate model submodule parameters
        double temperature_submodule_plate_model_depth;
        double temperature_submodule_plate_model_top_temperature;
        double temperature_submodule_plate_model_bottom_temperature;
        std::vector<Point<2> > temperature_submodule_plate_model_ridge_points;
        double temperature_submodule_plate_model_spreading_velocity;

        // constant composition submodule parameters
        double composition_submodule_constant_depth;
        unsigned int composition_submodule_constant_composition;
    };
  }
}

#endif
