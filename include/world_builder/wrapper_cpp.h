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

#ifndef _world_builder_wrapper_cpp_h
#define _world_builder_wrapper_cpp_h

#include <string>
#include <vector>

namespace wrapper_cpp
{
  /**
   * This class is to be used by SWIG. To make it easy for SWIG we do not use any
   * other world builder header file in this header file. This means that the class
   * stores a void pointer internally. The cpp implementation can use the world builder
   * header files.
   */
  class WorldBuilderWrapper
  {
    public:
      /**
       * constructor
       */
      WorldBuilderWrapper(std::string filename, bool has_output_dir = false, std::string output_dir = "");

      /**
       * destructor
       */
      ~WorldBuilderWrapper();

      /**
       * This function return the temperature at a specific location given x, z, depth and
       * gravity.
       */
      double temperature_2d(double x, double z, double depth, double gravity);

      /**
       * This function return the temperature at a specific location given x, y, z, depth and
       * gravity.
       */
      double temperature_3d(double x, double y, double z, double depth, double gravity);

      /**
       * This function return the composition at a specific location given x, z, depth and
       * composition number.
       */
      double composition_2d(double x, double z, double depth, unsigned int composition_number);

      /**
       * This function return the composition at a specific location given x, y, z, depth and
       * composition number.
       */
      double composition_3d(double x, double y, double z, double depth, unsigned int composition_number);

      /**
       * This function return the temperature at a specific location given x, z, depth and
       * gravity.
       */
      std::vector<double> temperature_2d(std::vector<double> x, std::vector<double> z, std::vector<double> depth, std::vector<double> gravity);

      /**
       * test
       */
      void temperature_2d(double *p_output, unsigned int n_output, double *p_x, unsigned int n_x, double *p_z, unsigned int n_z, double *p_depth, unsigned int n_depth, double *p_gravity, unsigned int n_gravity);//, double* p_z, unsigned int n_z, double* p_depth, unsigned int n_depth, double* gp_ravity, unsigned int n_gravity);

      /**
       * This function return the temperature at a specific location given x, z, depth and
       * gravity.
       */
      std::vector<double> temperature_3d(std::vector<double> x, std::vector<double> y, std::vector<double> z, std::vector<double> depth, std::vector<double> gravity);

      /**
       * test
       */
      void temperature_3d(double *p_output, unsigned int n_output,
                          double *p_x, unsigned int n_x,
                          double *p_y, unsigned int n_y,
                          double *p_z, unsigned int n_z,
                          double *p_depth, unsigned int n_depth,
                          double *p_gravity, unsigned int n_gravity);//, double* p_z, unsigned int n_z, double* p_depth, unsigned int n_depth, double* gp_ravity, unsigned int n_gravity);


      /**
       * This function return the composition at a specific location given x, z, depth.
       */
      std::vector<double> composition_2d(std::vector<double> x, std::vector<double> z, std::vector<double> depth, unsigned int composition);

      /**
       * test
       */
      void composition_2d(double *p_output, unsigned int n_output, double *p_x, unsigned int n_x, double *p_z, unsigned int n_z, double *p_depth, unsigned int n_depth, double *p_compositions,unsigned int n_compositions);//, double* p_z, unsigned int n_z, double* p_depth, unsigned int n_depth, double* gp_ravity, unsigned int n_gravity);

    private:
      void *ptr_ptr_world;


  };
}
#endif