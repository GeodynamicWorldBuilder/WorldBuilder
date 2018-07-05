#include <world_builder/world.h>
#include <iostream>

extern "C" {
  /**
   * This function creates an object of the world builder and returns a pointer
   * to it. This pointer can then be used to call the temperature and composition
   * functions. When done call the release world function to destroy the object.
   */
  void create_world(void **ptr_ptr_world, char *world_builder_file)
  {
    WorldBuilder::World *a = new WorldBuilder::World(std::string(world_builder_file));
    *ptr_ptr_world = reinterpret_cast<void *>(a);
  }

  /**
   * This function return the temperature at a specific location given x, z, depth and
   * gravity.
   */
  void temperature_2d(void *ptr_ptr_world, double x, double z, double depth, double gravity, double *temperature)
  {
    WorldBuilder::World *a = reinterpret_cast<WorldBuilder::World *>(ptr_ptr_world);
    std::array<double,2> position = {x,z};
    *temperature = a->temperature(position,depth,gravity);
  }

  /**
   * This function return the temperature at a specific location given x, y, z, depth and
   * gravity.
   */
  void temperature_3d(void *ptr_ptr_world, double x, double y, double z, double depth, double gravity, double *temperature)
  {
    WorldBuilder::World *a = reinterpret_cast<WorldBuilder::World *>(ptr_ptr_world);
    std::array<double,3> position = {x,y,z};
    *temperature = a->temperature(position,depth,gravity);
  }



  /**
   * This function return the composition at a specific location given x, z, depth and
   * composition number.
   */
  void composition_2d(void *ptr_ptr_world, double x, double z, double depth, unsigned int composition_number, bool *composition)
  {
    WorldBuilder::World *a = reinterpret_cast<WorldBuilder::World *>(ptr_ptr_world);
    std::array<double,2> position = {x,z};
    *composition = a->composition(position,depth,composition_number);
  }

  /**
   * This function return the composition at a specific location given x, y, z, depth and
   * composition number.
   */
  void composition_3d(void *ptr_ptr_world, double x, double y, double z, double depth, unsigned int composition_number, bool *composition)
  {
    WorldBuilder::World *a = reinterpret_cast<WorldBuilder::World *>(ptr_ptr_world);
    std::array<double,3> position = {x,y,z};
    *composition = a->composition(position,depth,composition_number);
  }

  /**
   * The destructor for the world builder class. Call this function when done with the
   * world builder.
   */
  void release_world(void *ptr_ptr_world)
  {
    WorldBuilder::World *a = reinterpret_cast<WorldBuilder::World *>(ptr_ptr_world);
    delete a;
  }
}
