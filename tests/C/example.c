
#include "world_builder/wrapper_c.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  // Declare the types which will be needed.
  void * ptr_world =  NULL;
  double temperature = 0.0;
  double x = 120e3;
  double y = 500e3;
  double z = 0;
  double depth = 0;
  unsigned int composition_number = 3;
  unsigned int random_number_seed = 1; // use a random number seed larger than zero
  double composition = 0;
  bool has_output_dir = 0; // false
  char output_dir[] = "../../doc/";

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

  // simple specific interfaces
  printf("2d temperature: \n");
  temperature_2d(ptr_world,x,z,depth,&temperature);
  printf("temperature in C = %f \n", temperature);

  printf("3d temperature: \n");
  temperature_3d(ptr_world,x,y,z,depth,&temperature);
  printf("temperature in C = %f \n", temperature);

  printf("2d composition: \n");
  composition_2d(ptr_world,x,z,depth,composition_number,&composition);
  printf("composition in C = %f \n", composition);

  printf("3d composition: \n");
  composition_3d(ptr_world,x,y,z,depth,composition_number,&composition);
  printf("composition in C = %f \n", composition);

  // High performance generic interface for getting many properties at the same time
  unsigned int properties[6][3] = {
   {1,0,0}, // temmperature
  {2,0,0}, // composition 0
  {2,1,0}, // composition 1
  {3,0,10}, // grain compositon 0, 10 grains
  {4,0,0}, // tag 
  {5,0,0} // velocity (3 values)
  }; 
  double values[1];
  printf("2d temperature: \n");
  properties_2d(ptr_world,x,z,depth,properties,1,values);
  printf("temperature in C = %f \n", values[0]);

  release_world(ptr_world);

  return 0;
}
