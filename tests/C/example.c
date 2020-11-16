
#include <world_builder/wrapper_c.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  // Declare the types which will be needed.
  void * ptr_world =  NULL;
  double temperature = 0.0;
  double x = 120e3;
  double y = 500e3;
  double z = 0;
  double depth = 0;
  double gravity = 10;
  unsigned int composition_number = 3;
  unsigned int random_number_seed = 1; // use a random number seed larger than zero
  double composition = 0;
  bool has_output_dir = 0; // false
  char output_dir[] = "../../doc/manual/";

   if( argc > 2 ) {
      printf("Too many arguments supplied.\n");
      return 1;
   }
   else if (argc != 2) {
      printf("One argument expected.\n");
      return 1;
   }

  // Show how to call the functions.
  printf("create world \n");
  
  create_world(&ptr_world, argv[1], &has_output_dir, output_dir, random_number_seed);

  printf("2d temperature: \n");
  temperature_2d(ptr_world,x,z,depth,gravity,&temperature);
  printf("temperature in C = %f \n", temperature);

  printf("3d temperature: \n");
  temperature_3d(ptr_world,x,y,z,depth,gravity,&temperature);
  printf("temperature in C = %f \n", temperature);

  printf("2d composition: \n");
  composition_2d(ptr_world,x,z,depth,composition_number,&composition);
  printf("composition in C = %f \n", composition);

  printf("3d composition: \n");
  composition_3d(ptr_world,x,y,z,depth,composition_number,&composition);
  printf("composition in C = %f \n", composition);

  release_world(ptr_world);

  return 0;
}
