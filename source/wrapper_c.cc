#include <world_builder/world.h>
#include <iostream>

extern "C" void create_world(char* world_builder_file, void** world) {
	printf("file is = %s\n",world_builder_file);

	{
		WorldBuilder::World* world_ptr = new WorldBuilder::World(std::string(world_builder_file));
		std::cout << "pointer 1= " << world << std::endl;
		*world = reinterpret_cast<void*>(world_ptr);
	}

	std::cout << "pointer 2 = " << world << std::endl;
}

extern "C" void release_world(void* world) {
	std::cout << "pointer 4 = " << world << ", deref = " << &world << std::endl;
	WorldBuilder::World* world_ptr = reinterpret_cast<WorldBuilder::World*>(world);
	         delete world_ptr;
	//delete static_cast<WorldBuilder::World*> (world);
}

extern "C" double temperature_2d(double x, double z, double depth, double gravity, double temperature, void* world)
{
	std::cout << "pointer 3 = " << world << ", deref = " << &world << std::endl;
	WorldBuilder::World* world_ptr = reinterpret_cast<WorldBuilder::World*>(world);
	std::array<double,2> position = {x,z};
	return world_ptr->temperature(position,depth,gravity);
}
