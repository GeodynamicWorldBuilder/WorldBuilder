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

#ifndef _world_builder_wrapper_c_h
#define _world_builder_wrapper_c_h

extern "C" {
  void create_world(void **ptr_ptr_world, const char *world_builder_file);

  void temperature_2d(void *ptr_ptr_world, double x, double z, double depth, double gravity, double *temperature);

  void temperature_3d(void *ptr_ptr_world, double x, double y, double z, double depth, double gravity, double *temperature);

  void composition_2d(void *ptr_ptr_world, double x, double z, double depth, unsigned int composition_number, double *composition);

  void composition_3d(void *ptr_ptr_world, double x, double y, double z, double depth, unsigned int composition_number, double *composition);

  void release_world(void *ptr_ptr_world);
}

#endif
