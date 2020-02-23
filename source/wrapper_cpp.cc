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

#include "world_builder/wrapper_cpp.h"
#include "world_builder/world.h"
#include "world_builder/assert.h"
#include "iostream"

using namespace WorldBuilder;
namespace wrapper_cpp
{
  WorldBuilderWrapper::WorldBuilderWrapper(std::string filename, bool has_output_dir, std::string output_dir)
    : ptr_ptr_world(NULL)
  {
    WorldBuilder::World *a = new WorldBuilder::World(filename, has_output_dir, output_dir);
    ptr_ptr_world = reinterpret_cast<void *>(a);
  }


  WorldBuilderWrapper::~WorldBuilderWrapper()
  {
    WorldBuilder::World *a = reinterpret_cast<WorldBuilder::World *>(ptr_ptr_world);
    delete a;
  }


  double
  WorldBuilderWrapper::temperature_2d(double x, double z, double depth, double gravity)
  {
    std::array<double,2> position = {{x,z}};
    return reinterpret_cast<WorldBuilder::World *>(ptr_ptr_world)->temperature(position,depth,gravity);
  }

  double
  WorldBuilderWrapper::temperature_3d(double x, double y, double z, double depth, double gravity)
  {
    std::array<double,3> position = {{x,y,z}};
    return reinterpret_cast<WorldBuilder::World *>(ptr_ptr_world)->temperature(position,depth,gravity);
  }

  double
  WorldBuilderWrapper::composition_2d(double x, double z, double depth, unsigned int composition_number)
  {
    std::array<double,2> position = {{x,z}};
    return reinterpret_cast<WorldBuilder::World *>(ptr_ptr_world)->composition(position,depth,composition_number);
  }

  double
  WorldBuilderWrapper::composition_3d(double x, double y, double z, double depth, unsigned int composition_number)
  {
    std::array<double,3> position = {{x,y,z}};
    return reinterpret_cast<WorldBuilder::World *>(ptr_ptr_world)->composition(position,depth,composition_number);
  }

  std::vector<double>
  WorldBuilderWrapper::temperature_2d(std::vector<double> x, std::vector<double> z, std::vector<double> depth, std::vector<double> gravity)
  {
    WBAssertThrow(x.size() == z.size(),
                  "Fatal error: In the vector temperature 2d function the size of the x and z vectors are not the same. "
                  "The sizeof x is " << x.size() << "and z is " << z.size());

    WBAssertThrow(x.size() == depth.size(),
                  "Fatal error: In the vector temperature 2d function the size of the x and depth vectors are not the same. "
                  "The sizeof x is " << x.size() << "and depth is " << z.size());

    WBAssertThrow(x.size() == gravity.size(),
                  "Fatal error: In the vector temperature 2d function the size of the x and gravity vectors are not the same. "
                  "The sizeof x is " << x.size() << "and gravity is " << gravity.size());

    std::vector<double> temperatures(x.size());
    for (size_t i = 0; i < x.size(); i++)
      {
        std::array<double,2> position = {{x[i],z[i]}};
        temperatures[i] = reinterpret_cast<WorldBuilder::World *>(ptr_ptr_world)->temperature(position,depth[i],gravity[i]);
      }
    return temperatures;
  }


  void
  WorldBuilderWrapper::temperature_2d(double *p_output, unsigned int n_output, double *p_x, unsigned int n_x, double *p_z, unsigned int n_z, double *p_depth, unsigned int n_depth, double *p_gravity, unsigned int n_gravity)
  {
    WBAssertThrow(n_output == n_x,
                  "Fatal error: In the vector temperature 2d function the size of the ouput and x vectors are not the same. "
                  "The sizeof output is " << n_output << "and x is " << n_x);
    WBAssertThrow(n_output == n_z,
                  "Fatal error: In the vector temperature 2d function the size of the ouput and z vectors are not the same. "
                  "The sizeof output is " << n_output << "and z is " << n_z);
    WBAssertThrow(n_output == n_depth,
                  "Fatal error: In the vector temperature 2d function the size of the ouput and depth vectors are not the same. "
                  "The sizeof output is " << n_output << "and depth is " << n_depth);
    WBAssertThrow(n_output == n_gravity,
                  "Fatal error: In the vector temperature 2d function the size of the ouput and x vectors are not the same. "
                  "The sizeof output is " << n_output << "and gravity is " << n_gravity);

    std::vector<double> output(p_output,p_output+n_output);
    std::vector<double> x(p_x,p_x+n_x);
    std::vector<double> z(p_z,p_z+n_z);
    std::vector<double> depth(p_depth,p_depth+n_depth);
    std::vector<double> gravity(p_gravity,p_gravity+n_gravity);

    output = temperature_2d(x,z,depth,gravity);

    for (unsigned int i=0; i<n_output; i++)
      {
        p_output[i] = output[i];
      }
  }



  std::vector<double>
  WorldBuilderWrapper::temperature_3d(std::vector<double> x, std::vector<double> y, std::vector<double> z, std::vector<double> depth, std::vector<double> gravity)
  {
    WBAssertThrow(x.size() == y.size(),
                  "Fatal error: In the vector temperature 2d function the size of the x and y vectors are not the same. "
                  "The sizeof x is " << x.size() << "and y is " << y.size());

    WBAssertThrow(x.size() == z.size(),
                  "Fatal error: In the vector temperature 2d function the size of the x and z vectors are not the same. "
                  "The sizeof x is " << x.size() << "and z is " << z.size());

    WBAssertThrow(x.size() == depth.size(),
                  "Fatal error: In the vector temperature 2d function the size of the x and depth vectors are not the same. "
                  "The sizeof x is " << x.size() << "and depth is " << z.size());

    WBAssertThrow(x.size() == gravity.size(),
                  "Fatal error: In the vector temperature 2d function the size of the x and gravity vectors are not the same. "
                  "The sizeof x is " << x.size() << "and gravity is " << gravity.size());

    std::vector<double> temperatures(x.size());
    for (size_t i = 0; i < x.size(); i++)
      {
        std::array<double,3> position = {{x[i],y[i],z[i]}};
        temperatures[i] = reinterpret_cast<WorldBuilder::World *>(ptr_ptr_world)->temperature(position,depth[i],gravity[i]);
      }
    return temperatures;
  }


  void
  WorldBuilderWrapper::temperature_3d(double *p_output, unsigned int n_output,
                                      double *p_x, unsigned int n_x,
                                      double *p_y, unsigned int n_y,
                                      double *p_z, unsigned int n_z,
                                      double *p_depth, unsigned int n_depth,
                                      double *p_gravity, unsigned int n_gravity)
  {
    WBAssertThrow(n_output == n_x,
                  "Fatal error: In the vector temperature 2d function the size of the ouput and x vectors are not the same. "
                  "The sizeof output is " << n_output << "and x is " << n_x);
    WBAssertThrow(n_output == n_y,
                  "Fatal error: In the vector temperature 2d function the size of the ouput and y vectors are not the same. "
                  "The sizeof output is " << n_output << "and x is " << n_y);
    WBAssertThrow(n_output == n_z,
                  "Fatal error: In the vector temperature 2d function the size of the ouput and z vectors are not the same. "
                  "The sizeof output is " << n_output << "and z is " << n_z);
    WBAssertThrow(n_output == n_depth,
                  "Fatal error: In the vector temperature 2d function the size of the ouput and depth vectors are not the same. "
                  "The sizeof output is " << n_output << "and depth is " << n_depth);
    WBAssertThrow(n_output == n_gravity,
                  "Fatal error: In the vector temperature 2d function the size of the ouput and x vectors are not the same. "
                  "The sizeof output is " << n_output << "and gravity is " << n_gravity);

    std::vector<double> output(p_output,p_output+n_output);
    std::vector<double> x(p_x,p_x+n_x);
    std::vector<double> y(p_y,p_y+n_y);
    std::vector<double> z(p_z,p_z+n_z);
    std::vector<double> depth(p_depth,p_depth+n_depth);
    std::vector<double> gravity(p_gravity,p_gravity+n_gravity);

    output = temperature_3d(x,y,z,depth,gravity);

    for (unsigned int i=0; i<n_output; i++)
      {
        p_output[i] = output[i];
      }
  }



  std::vector<double>
  WorldBuilderWrapper::composition_2d(std::vector<double> x, std::vector<double> z, std::vector<double> depth, unsigned int composition)
  {
    WBAssertThrow(x.size() == z.size(),
                  "Fatal error: In the vector temperature 2d function the size of the x and z vectors are not the same. "
                  "The sizeof x is " << x.size() << "and z is " << z.size());

    WBAssertThrow(x.size() == depth.size(),
                  "Fatal error: In the vector temperature 2d function the size of the x and depth vectors are not the same. "
                  "The sizeof x is " << x.size() << "and depth is " << z.size());

    std::vector<double> compositions(x.size());
    for (size_t i = 0; i < x.size(); i++)
      {
        std::array<double,2> position = {{x[i],z[i]}};
        compositions[i] = reinterpret_cast<WorldBuilder::World *>(ptr_ptr_world)->composition(position,depth[i],composition);
      }
    return compositions;
  }


  void
  WorldBuilderWrapper::composition_2d(double *p_output, unsigned int n_output,
                                      double *p_x, unsigned int n_x,
                                      double *p_z, unsigned int n_z,
                                      double *p_depth, unsigned int n_depth,
                                      double *p_compositions,unsigned int n_compositions)
  {
    WBAssertThrow(n_output == n_x,
                  "Fatal error: In the vector temperature 2d function the size of the ouput and x vectors are not the same. "
                  "The sizeof output is " << n_output << "and x is " << n_x);
    WBAssertThrow(n_output == n_z,
                  "Fatal error: In the vector temperature 2d function the size of the ouput and z vectors are not the same. "
                  "The sizeof output is " << n_output << "and z is " << n_z);
    WBAssertThrow(n_output == n_depth,
                  "Fatal error: In the vector temperature 2d function the size of the ouput and depth vectors are not the same. "
                  "The sizeof output is " << n_output << "and depth is " << n_depth);

    delete p_output;
    std::vector<double> output(p_output,p_output+n_output);
    std::vector<double> x(p_x,p_x+n_x);
    std::vector<double> z(p_z,p_z+n_z);
    std::vector<double> depth(p_depth,p_depth+n_depth);
    std::vector<double> compositions(p_compositions,p_compositions+n_compositions);

    output = composition_2d(x,z,depth,compositions[0]);

    for (unsigned int i=0; i<n_output; i++)
      {
        p_output[i] = output[i];
      }
  }
}