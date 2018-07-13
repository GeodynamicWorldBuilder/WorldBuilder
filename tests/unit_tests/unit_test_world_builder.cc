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

#define CATCH_CONFIG_MAIN

#include <iostream>
#include <memory>

#include <boost/property_tree/json_parser.hpp>

#include <catch2.h>

#include <world_builder/coordinate_systems/interface.h>
#include <world_builder/coordinate_systems/cartesian.h>
#include <world_builder/features/interface.h>
#include <world_builder/features/continental_plate.h>
#include <world_builder/point.h>
#include <world_builder/types/array.h>
#include <world_builder/types/coordinate_system.h>
#include <world_builder/types/double.h>
#include <world_builder/types/feature.h>
#include <world_builder/types/list.h>
#include <world_builder/types/string.h>
#include <world_builder/types/unsigned_int.h>
#include <world_builder/utilities.h>
#include <world_builder/wrapper_c.h>
#include <config.h>

using namespace WorldBuilder;

using Catch::Matchers::Contains;

/**
 * Compare the given two std::vector<double> entries with an epsilon (using Catch::Approx)
 */
inline void compare_vectors_approx(
  const std::vector<double> &computed,
  const std::vector<double> &expected)
{
  CHECK(computed.size() == expected.size());
  for (unsigned int i=0; i< computed.size(); ++i)
    {
      INFO("vector index i=" << i << ": ");
      CHECK(computed[i] == Approx(expected[i]));
    }
}


TEST_CASE("WorldBuilder Point: Testing initialize and operators")
{
  // Test initialization of the Point class
  Point<2> p2;
  Point<3> p3;

  CHECK(p2.get_array() == std::array<double,2> {0,0});
  CHECK(p3.get_array() == std::array<double,3> {0,0,0});

  const Point<2> p2_array(std::array<double,2> {1,2});
  const Point<3> p3_array(std::array<double,3> {1,2,3});

  CHECK(p2_array.get_array() == std::array<double,2> {1,2});
  CHECK(p3_array.get_array() == std::array<double,3> {1,2,3});

  const Point<2> p2_point(p2_array);
  const Point<3> p3_point(p3_array);

  CHECK(p2_point.get_array() == std::array<double,2> {1,2});
  CHECK(p3_point.get_array() == std::array<double,3> {1,2,3});

  const Point<2> p2_explicit(3,4);
  const Point<3> p3_explicit(4,5,6);

  CHECK(p2_explicit.get_array() == std::array<double,2> {3,4});
  CHECK(p3_explicit.get_array() == std::array<double,3> {4,5,6});

  /**
   * Test Point operators
   */
  // Test assign operator
  p2 = p2_array;
  p3 = p3_array;

  CHECK(p2.get_array() == std::array<double,2> {1,2});
  CHECK(p3.get_array() == std::array<double,3> {1,2,3});

  // Test multiply operator
  p2 = 2 * p2 * 1.0;
  p3 = 2 * p3 * 1.0;

  CHECK(p2.get_array() == std::array<double,2> {2,4});
  CHECK(p3.get_array() == std::array<double,3> {2,4,6});

  p2 *= 2;
  p3 *= 2;

  CHECK(p2.get_array() == std::array<double,2> {4,8});
  CHECK(p3.get_array() == std::array<double,3> {4,8,12});

  // Test dot operator
  CHECK(p2_array * p2_explicit == 11);
  CHECK(p3_array * p3_explicit == 32);

  // Test add operator
  p2 = p2 + p2;
  p3 = p3 + p3;

  CHECK(p2.get_array() == std::array<double,2> {8,16});
  CHECK(p3.get_array() == std::array<double,3> {8,16,24});

  p2 += p2;
  p3 += p3;

  CHECK(p2.get_array() == std::array<double,2> {16,32});
  CHECK(p3.get_array() == std::array<double,3> {16,32,48});

  // Test subtract operator
  p2 = p2 - (0.5 * p2);
  p3 = p3 - (0.5 * p3);

  CHECK(p2.get_array() == std::array<double,2> {8,16});
  CHECK(p3.get_array() == std::array<double,3> {8,16,24});

  p2 -=  (0.5 * p2);
  p3 -=  (0.5 * p3);

  CHECK(p2.get_array() == std::array<double,2> {4,8});
  CHECK(p3.get_array() == std::array<double,3> {4,8,12});

  // Test coordinate system
  CHECK(p2.get_coordinate_system() == CoordinateSystem::cartesian);
  CHECK(p3.get_coordinate_system() == CoordinateSystem::cartesian);

  // Test norm and norm_square
  CHECK(p2.norm_square() == 80);
  CHECK(p3.norm_square() == 224);

  CHECK(p2.norm() == std::sqrt(80));
  CHECK(p3.norm() == std::sqrt(224));

  // Test Point utility classes
  std::array<double,2> an2 = Utilities::convert_point_to_array(p2_point);
  std::array<double,3> an3 = Utilities::convert_point_to_array(p3_point);

  CHECK(an2 == std::array<double,2> {1,2});
  CHECK(an3 == std::array<double,3> {1,2,3});

  CHECK_THROWS_WITH(Point<2>(1,2,3),Contains("Can't use the 3d constructor in 2d."));
  CHECK_THROWS_WITH(Point<3>(1,2),Contains("Can't use the 2d constructor in 3d."));



}

TEST_CASE("WorldBuilder Utilities: string to conversions")
{
  // Test string to number conversion
  CHECK(Utilities::string_to_double("1") == 1.0);
  CHECK(Utilities::string_to_double(" 1 ") == 1.0);
  CHECK(Utilities::string_to_double(" 1.01 ") == 1.01);

  CHECK_THROWS_WITH(Utilities::string_to_double("1a"),
                      Contains("Conversion of \"1a\" to double failed (bad cast): "));
  CHECK_THROWS_WITH(Utilities::string_to_double("a1"),
                      Contains("Conversion of \"a1\" to double failed (bad cast): "));
  CHECK_THROWS_WITH(Utilities::string_to_double("a"),
                      Contains("Conversion of \"a\" to double failed (bad cast): "));

  CHECK(Utilities::string_to_int("2") == 2);
  CHECK(Utilities::string_to_int(" 2 ") == 2);

  CHECK_THROWS_WITH(Utilities::string_to_int(" 2.02 "),
                      Contains("Conversion of \" 2.02 \" to int failed (bad cast): "));
  CHECK_THROWS_WITH(Utilities::string_to_int("2b"),
                      Contains("Conversion of \"2b\" to int failed (bad cast): "));
  CHECK_THROWS_WITH(Utilities::string_to_int("b2"),
                      Contains("Conversion of \"b2\" to int failed (bad cast): "));
  CHECK_THROWS_WITH(Utilities::string_to_int("b"),
                      Contains("Conversion of \"b\" to int failed (bad cast): "));

  CHECK(Utilities::string_to_unsigned_int("3") == 3);
  CHECK(Utilities::string_to_unsigned_int(" 3 ") == 3);

  CHECK_THROWS_WITH(Utilities::string_to_unsigned_int(" 3.03 "),
                      Contains("Conversion of \" 3.03 \" to unsigned int failed (bad cast): "));
  CHECK_THROWS_WITH(Utilities::string_to_unsigned_int("3c"),
                      Contains("Conversion of \"3c\" to unsigned int failed (bad cast): "));
  CHECK_THROWS_WITH(Utilities::string_to_unsigned_int("c3"),
                      Contains("Conversion of \"c3\" to unsigned int failed (bad cast): "));
  CHECK_THROWS_WITH(Utilities::string_to_unsigned_int("c"),
                      Contains("Conversion of \"c\" to unsigned int failed (bad cast): "));

  // Test point to array conversion
  const Point<2> p2(1,2);
  const Point<3> p3(1,2,3);

  CHECK(Utilities::convert_point_to_array(p2) == std::array<double,2> {1,2});
  CHECK(Utilities::convert_point_to_array(p3) == std::array<double,3> {1,2,3});

  // Test coordinate system
  CHECK(Utilities::string_to_coordinate_system("cartesian") == CoordinateSystem::cartesian);
  CHECK(Utilities::string_to_coordinate_system("spherical") == CoordinateSystem::spherical);
  CHECK_THROWS_WITH(Utilities::string_to_coordinate_system("other"), Contains("Coordinate system not implemented."));
}

TEST_CASE("WorldBuilder Utilities: Point in polygon")
{
  std::vector<Point<2> > point_list_4_elements(4);
  point_list_4_elements[0] = Point<2>(0,0);
  point_list_4_elements[1] = Point<2>(5,0);
  point_list_4_elements[2] = Point<2>(5,5);
  point_list_4_elements[3] = Point<2>(0,5);

  std::vector<Point<2> > point_list_3_elements(3);
  point_list_3_elements[0] = Point<2>(10,10);
  point_list_3_elements[1] = Point<2>(10,15);
  point_list_3_elements[2] = Point<2>(15,15);

  std::vector<Point<2> > check_points(9);
  check_points[0] = Point<2>(-1,-1);
  check_points[1] = Point<2>(0,0);
  check_points[2] = Point<2>(0,5);
  check_points[3] = Point<2>(5,0);
  check_points[4] = Point<2>(5,5);
  check_points[5] = Point<2>(5,5.01);
  check_points[6] = Point<2>(1,1);
  check_points[7] = Point<2>(12.5,12);
  check_points[8] = Point<2>(11.5,12);

  std::vector<std::array<bool,2> > awnsers(9);
  awnsers[0] = {false,false};
  awnsers[1] = {true,false};
  awnsers[2] = {true,false};
  awnsers[3] = {true,false};
  awnsers[4] = {true,false};
  awnsers[5] = {false,false};
  awnsers[6] = {true,false};
  awnsers[7] = {false,false};
  awnsers[8] = {false,true};

  std::vector<std::array<double,2> > awnsers_signed_distance(9);
  awnsers_signed_distance[0] = {-std::sqrt(2), -std::sqrt(11 * 11 + 11 * 11)};
  awnsers_signed_distance[1] = {0,-std::sqrt(10 * 10 + 10 * 10)};
  awnsers_signed_distance[2] = {0,-std::sqrt(125)};
  awnsers_signed_distance[3] = {0,-std::sqrt(125)};
  awnsers_signed_distance[4] = {0,-std::sqrt(50)};
  awnsers_signed_distance[5] = {-std::sqrt(0.01 * 0.01),-std::sqrt(5 * 5 + 4.99 * 4.99)};
  awnsers_signed_distance[6] = {1,-std::sqrt(9 * 9 + 9 * 9)};
  awnsers_signed_distance[7] = {-10.2591422643,-0.3535533906};
  awnsers_signed_distance[8] = {-9.5524865873,0.3535533906};

  for (unsigned int i = 0; i < check_points.size(); ++i)
    {
      INFO("checking point " << i << " = (" << check_points[i][0] << ":" << check_points[i][1] << ")");
      CHECK(Utilities::polygon_contains_point(point_list_4_elements,check_points[i]) == awnsers[i][0]);
      CHECK(Utilities::polygon_contains_point(point_list_3_elements,check_points[i]) == awnsers[i][1]);
      CHECK(Utilities::signed_distance_to_polygon(point_list_4_elements,check_points[i]) == Approx(awnsers_signed_distance[i][0]));
      CHECK(Utilities::signed_distance_to_polygon(point_list_3_elements,check_points[i]) == Approx(awnsers_signed_distance[i][1]));
    }

  std::vector<Point<2> > point_list_2_elements(2);
  CHECK_THROWS_WITH(Utilities::signed_distance_to_polygon(point_list_2_elements,check_points[0]),
                      Contains("Not enough polygon points were specified."));

  std::vector<Point<2> > point_list_1_elements(1);
  CHECK_THROWS_WITH(Utilities::signed_distance_to_polygon(point_list_1_elements,check_points[0]),
                      Contains("Not enough polygon points were specified."));

  std::vector<Point<2> > point_list_0_elements(0);
  CHECK_THROWS_WITH(Utilities::signed_distance_to_polygon(point_list_0_elements,check_points[0]),
                      Contains("Not enough polygon points were specified."));
}


TEST_CASE("WorldBuilder Utilities: Natural Coordinate")
{
  CoordinateSystems::Interface *cartesian = new CoordinateSystems::Cartesian;

  // Test the natural coordinate system
  Utilities::NaturalCoordinate nca1(std::array<double,3> {1,2,3},*cartesian);
  CHECK(nca1.get_coordinates() == std::array<double,3> {1,2,3});
  CHECK(nca1.get_surface_coordinates() == std::array<double,2> {1,2});
  CHECK(nca1.get_depth_coordinate() == 3);

  Utilities::NaturalCoordinate ncp1(Point<3>(1,2,3),*cartesian);
  CHECK(ncp1.get_coordinates() == std::array<double,3> {1,2,3});
  CHECK(ncp1.get_surface_coordinates() == std::array<double,2> {1,2});
  CHECK(ncp1.get_depth_coordinate() == 3);

  delete cartesian;
}

TEST_CASE("WorldBuilder Utilities: Coordinate systems transformations")
{
  // Test coordinate system transformation
  {
    Point<3> cartesian(3,4,5);

    Point<3> spherical(Utilities::cartesian_to_spherical_coordinates(cartesian.get_array()), CoordinateSystem::spherical);

    compare_vectors_approx(std::vector<double>(std::begin(spherical.get_array()), std::end(spherical.get_array())),
                           std::vector<double> {std::sqrt(3*3+4*4+5*5),0.927295218001613,0.7853982});

    Point<3> cartesian_back(Utilities::spherical_to_cartesian_coordinates(spherical.get_array()), CoordinateSystem::cartesian);

    compare_vectors_approx(std::vector<double>(std::begin(cartesian_back.get_array()), std::end(cartesian_back.get_array())),
                           std::vector<double> {3,4,5});
  }

  {
    Point<3> cartesian(-2,-1,6);

    Point<3> spherical(Utilities::cartesian_to_spherical_coordinates(cartesian.get_array()), CoordinateSystem::spherical);

    compare_vectors_approx(std::vector<double>(std::begin(spherical.get_array()), std::end(spherical.get_array())),
                           std::vector<double> {std::sqrt(2*2+1*1+6*6),3.60524026718,0.356733389});

    Point<3> cartesian_back(Utilities::spherical_to_cartesian_coordinates(spherical.get_array()), CoordinateSystem::cartesian);

    compare_vectors_approx(std::vector<double>(std::begin(cartesian_back.get_array()), std::end(cartesian_back.get_array())),
                           std::vector<double> {-2,-1,6});
  }


}

TEST_CASE("WorldBuilder Utilities: ptree function")
{
  ptree tree;
  tree.put("value", 3.14159);
  CHECK(Utilities::string_to_double(Utilities::get_from_ptree(tree, "pi", "value", true, ".").get()) == Approx(3.14159));
  CHECK_THROWS_WITH(Utilities::get_from_ptree(tree, "pi", "value_pi", true, "."),
                      Contains("Entry undeclared: pi.value_pi"));
}

TEST_CASE("WorldBuilder C wrapper")
{
  // First test a world builder file with a cross section defined
  std::string file = WorldBuilder::Data::WORLD_BUILDER_SOURCE_DIR + "/tests/data/simple_wb1.json";
  void *ptr_world = NULL;
  void **ptr_ptr_world = &ptr_world;
  const char *world_builder_file = file.c_str();

  create_world(ptr_ptr_world, world_builder_file);

  double temperature = 0;

  temperature_2d(*ptr_ptr_world, 1, 2, 0, 10, &temperature);
  CHECK(temperature == Approx(1600));
  temperature_3d(*ptr_ptr_world, 1, 2, 3, 0, 10, &temperature);
  CHECK(temperature == Approx(1600));
  temperature_3d(*ptr_ptr_world, 120e3, 500e3, 0, 0, 10, &temperature);
  CHECK(temperature == Approx(150));

  // TODO: figure out why it isn't 150
  //temperature_2d(*ptr_ptr_world, 1800e3, 0, 0, 10, &temperature);
  //CHECK(temperature == Approx(150));

  // Test the compositions
  bool composition = false;

  composition_2d(*ptr_ptr_world, 1, 2, 0, 2, &composition);
  CHECK(composition == false);
  composition_3d(*ptr_ptr_world, 1, 2, 3, 0, 2, &composition);
  CHECK(composition == false);
  composition_3d(*ptr_ptr_world, 120e3, 500e3, 0, 0, 3, &composition);
  CHECK(composition == true);

  // TODO: figure out why it isn't true
  //composition_2d(*ptr_ptr_world, 1800e3, 0, 0, 3, &composition);
  //CHECK(composition == true);

  release_world(*ptr_ptr_world);

  // Now test a world builder file without a cross section defined
  file = WorldBuilder::Data::WORLD_BUILDER_SOURCE_DIR + "/tests/data/simple_wb2.json";
  ptr_world = NULL;
  ptr_ptr_world = &ptr_world;
  const char *world_builder_file2 = file.c_str();

  create_world(ptr_ptr_world, world_builder_file2);


  CHECK_THROWS_WITH(temperature_2d(*ptr_ptr_world, 1, 2, 0, 10, &temperature),
                      Contains("This function can only be called when the cross section "
                               "variable in the world builder file has been set. Dim is 3."));
  temperature_3d(*ptr_ptr_world, 1, 2, 3, 0, 10, &temperature);
  CHECK(temperature == Approx(1600));
  temperature_3d(*ptr_ptr_world, 120e3, 500e3, 0, 0, 10, &temperature);
  CHECK(temperature == Approx(150));

  // TODO: figure out why it isn't 150
  //temperature_2d(*ptr_ptr_world, 1800e3, 0, 0, 10, &temperature);
  //CHECK(temperature == Approx(150));

  // Test the compositions
  CHECK_THROWS_WITH(composition_2d(*ptr_ptr_world, 1, 2, 0, 2, &composition),
                      Contains("This function can only be called when the cross section "
                               "variable in the world builder file has been set. Dim is 3."));

  composition_3d(*ptr_ptr_world, 1, 2, 3, 0, 2, &composition);
  CHECK(composition == false);
  composition_3d(*ptr_ptr_world, 120e3, 500e3, 0, 0, 3, &composition);
  CHECK(composition == true);

  // TODO: figure out why it isn't true
  //composition_2d(*ptr_ptr_world, 1800e3, 0, 0, 3, &composition);
  //CHECK(composition == true);

  release_world(*ptr_ptr_world);
}


TEST_CASE("WorldBuilder Coordinate Systems: Interface")
{
  CHECK_THROWS_WITH(CoordinateSystems::create_coordinate_system("!not_implemented_coordinate_system!"),
                      Contains("Coordinate system not implemented."));

  CoordinateSystems::Interface *interface = new CoordinateSystems::Cartesian;

  std::string path = "";
  interface->decare_entries(path);

  CHECK(interface->cartesian_to_natural_coordinates(std::array<double,3> {1,2,3}) == std::array<double,3> {1,2,3});
  CHECK(interface->natural_to_cartesian_coordinates(std::array<double,3> {1,2,3}) == std::array<double,3> {1,2,3});

  CHECK(interface->natural_coordinate_system() == CoordinateSystem::cartesian);

  delete interface;
}

TEST_CASE("WorldBuilder Coordinate Systems: Cartesian")
{
  CoordinateSystems::Cartesian *cartesian = new CoordinateSystems::Cartesian;

  std::string path = "";
  cartesian->decare_entries(path);

  CHECK(cartesian->cartesian_to_natural_coordinates(std::array<double,3> {1,2,3}) == std::array<double,3> {1,2,3});
  CHECK(cartesian->natural_to_cartesian_coordinates(std::array<double,3> {1,2,3}) == std::array<double,3> {1,2,3});

  CHECK(cartesian->natural_coordinate_system() == CoordinateSystem::cartesian);

  delete cartesian;
}


TEST_CASE("WorldBuilder Features: Interface")
{
  std::string file_name = WorldBuilder::Data::WORLD_BUILDER_SOURCE_DIR + "/tests/data/simple_wb1.json";
  WorldBuilder::World world(file_name);
  CHECK_THROWS_WITH(Features::create_feature("!not_implemented_feature!", &world),
                      Contains("Feature !not_implemented_feature! not implemented."));

  Features::Interface *interface = new Features::ContinentalPlate(&world);

  delete interface;
}

TEST_CASE("WorldBuilder Features: Continental Plate")
{
  std::string file_name = WorldBuilder::Data::WORLD_BUILDER_SOURCE_DIR + "/tests/data/simple_wb1.json";
  WorldBuilder::World world1(file_name);
  Features::ContinentalPlate *continental_plate = new Features::ContinentalPlate(&world1);

  delete continental_plate;
}

TEST_CASE("WorldBuilder Types: Double")
{
#define TYPE Double
	Types::TYPE type(1,"test");
   CHECK(type.value == 1);
   CHECK(type.default_value == 1);
   CHECK(type.description == "test");
   CHECK(type.get_type() == Types::type::TYPE);

   Types::TYPE type_copy(type);
   CHECK(type_copy.value == 1);
   CHECK(type_copy.default_value == 1);
   CHECK(type_copy.description == "test");
   CHECK(type_copy.get_type() == Types::type::TYPE);

   Types::TYPE type_explicit(2, 3, "test explicit");
   CHECK(type_explicit.value == 2);
   CHECK(type_explicit.default_value == 3);
   CHECK(type_explicit.description == "test explicit");
   CHECK(type_explicit.get_type() == Types::type::TYPE);

   std::unique_ptr<Types::Interface> type_clone = type_explicit.clone();
   Types::TYPE* type_clone_natural = dynamic_cast<Types::TYPE*>(type_clone.get());
   CHECK(type_clone_natural->value == 2);
   CHECK(type_clone_natural->default_value == 3);
   CHECK(type_clone_natural->description == "test explicit");
   CHECK(type_clone_natural->get_type() == Types::type::TYPE);
#undef TYPE
}

TEST_CASE("WorldBuilder Types: UnsignedInt")
{
#define TYPE UnsignedInt
	Types::TYPE type(1,"test");
   CHECK(type.value == 1);
   CHECK(type.default_value == 1);
   CHECK(type.description == "test");
   CHECK(type.get_type() == Types::type::TYPE);

   Types::TYPE type_copy(type);
   CHECK(type_copy.value == 1);
   CHECK(type_copy.default_value == 1);
   CHECK(type_copy.description == "test");
   CHECK(type_copy.get_type() == Types::type::TYPE);

   Types::TYPE type_explicit(2, 3, "test explicit");
   CHECK(type_explicit.value == 2);
   CHECK(type_explicit.default_value == 3);
   CHECK(type_explicit.description == "test explicit");
   CHECK(type_explicit.get_type() == Types::type::TYPE);

   std::unique_ptr<Types::Interface> type_clone = type_explicit.clone();
   Types::TYPE* type_clone_natural = dynamic_cast<Types::TYPE*>(type_clone.get());
   CHECK(type_clone_natural->value == 2);
   CHECK(type_clone_natural->default_value == 3);
   CHECK(type_clone_natural->description == "test explicit");
   CHECK(type_clone_natural->get_type() == Types::type::TYPE);
#undef TYPE
}

TEST_CASE("WorldBuilder Types: String")
{
#define TYPE String
	Types::TYPE type("1","test");
   CHECK(type.value == "1");
   CHECK(type.default_value == "1");
   CHECK(type.description == "test");
   CHECK(type.get_type() == Types::type::TYPE);

   Types::TYPE type_copy(type);
   CHECK(type_copy.value == "1");
   CHECK(type_copy.default_value == "1");
   CHECK(type_copy.description == "test");
   CHECK(type_copy.get_type() == Types::type::TYPE);

   Types::TYPE type_explicit("2", "3", "test explicit");
   CHECK(type_explicit.value == "2");
   CHECK(type_explicit.default_value == "3");
   CHECK(type_explicit.description == "test explicit");
   CHECK(type_explicit.get_type() == Types::type::TYPE);

   std::unique_ptr<Types::Interface> type_clone = type_explicit.clone();
   Types::TYPE* type_clone_natural = dynamic_cast<Types::TYPE*>(type_clone.get());
   CHECK(type_clone_natural->value == "2");
   CHECK(type_clone_natural->default_value == "3");
   CHECK(type_clone_natural->description == "test explicit");
   CHECK(type_clone_natural->get_type() == Types::type::TYPE);
#undef TYPE
}

TEST_CASE("WorldBuilder Types: Point 2d")
{
#define TYPE Point<2>
	Types::TYPE type(TYPE(1,2),"test");
	   CHECK(type.value[0] == TYPE(1,2)[0]);
	   CHECK(type.value[1] == TYPE(1,2)[1]);
	   CHECK(type.default_value[0] == TYPE(1,2)[0]);
	   CHECK(type.default_value[1] == TYPE(1,2)[1]);
   CHECK(type.description == "test");
   CHECK(type.get_type() == Types::type::Point2D);

   Types::TYPE type_copy(type);
   CHECK(type_copy.value[0] == TYPE(1,2)[0]);
   CHECK(type_copy.value[1] == TYPE(1,2)[1]);
   CHECK(type.default_value[0] == TYPE(1,2)[0]);
   CHECK(type.default_value[1] == TYPE(1,2)[1]);
   CHECK(type_copy.description == "test");
   CHECK(type_copy.get_type() == Types::type::Point2D);

   Types::TYPE type_explicit(TYPE(3,4), TYPE(5,6), "test explicit");
   CHECK(type_explicit.value[0] == TYPE(3,4)[0]);
   CHECK(type_explicit.value[1] == TYPE(3,4)[1]);
   CHECK(type_explicit.default_value[0] == TYPE(5,6)[0]);
   CHECK(type_explicit.default_value[1] == TYPE(5,6)[1]);
   CHECK(type_explicit.description == "test explicit");
   CHECK(type_explicit.get_type() == Types::type::Point2D);

   std::unique_ptr<Types::Interface> type_clone = type_explicit.clone();
   Types::TYPE* type_clone_natural = dynamic_cast<Types::TYPE*>(type_clone.get());
   CHECK(type_clone_natural->value[0] == TYPE(3,4)[0]);
   CHECK(type_clone_natural->value[1] == TYPE(3,4)[1]);
   CHECK(type_clone_natural->default_value[0] == TYPE(5,6)[0]);
   CHECK(type_clone_natural->default_value[1] == TYPE(5,6)[1]);
   CHECK(type_clone_natural->description == "test explicit");
   CHECK(type_clone_natural->get_type() == Types::type::Point2D);


   /**
     * Test Point operators
     */
   const TYPE point_array(std::array<double,2> {1,2});
   const TYPE point_explicit(3,4);

   Types::TYPE type_point_array(point_array, point_array, "test array");
   Types::TYPE type_point_explicit(point_explicit, point_explicit, "test array");

   CHECK(type_point_array.value.get_array() == std::array<double,2> {1,2});
   CHECK(type_point_explicit.value.get_array() == std::array<double,2> {3,4});

    // Test multiply operator
    TYPE point = 2 * type_point_array * 1.0;

    CHECK(point.get_array() == std::array<double,2> {2,4});

    // Test dot operator
    CHECK(type_point_array * type_point_explicit == 11);

    // Test add operator
    point = type_point_array + type_point_explicit;

    CHECK(point.get_array() == std::array<double,2> {4,6});

    // Test subtract operator
    point = type_point_explicit - type_point_array;

    CHECK(point.get_array() == std::array<double,2> {2,2});

    // test the access operator
    CHECK(type_point_array[0] == 1);

    type_point_array[0] = 2;
    CHECK(type_point_array[0] == 2);
#undef TYPE
}

TEST_CASE("WorldBuilder Types: Point 3d")
{
#define TYPE Point<3>
	Types::TYPE type(TYPE(1,2,3),"test");
	   CHECK(type.value[0] == 1);
	   CHECK(type.value[1] == 2);
	   CHECK(type.value[2] == 3);
	   CHECK(type.default_value[0] == 1);
	   CHECK(type.default_value[1] == 2);
	   CHECK(type.default_value[2] == 3);
   CHECK(type.description == "test");
   CHECK(type.get_type() == Types::type::Point3D);

   Types::TYPE type_copy(type);
   CHECK(type_copy.value[0] == 1);
   CHECK(type_copy.value[1] == 2);
   CHECK(type_copy.value[2] == 3);
   CHECK(type_copy.default_value[0] == 1);
   CHECK(type_copy.default_value[1] == 2);
   CHECK(type_copy.default_value[2] == 3);
   CHECK(type_copy.description == "test");
   CHECK(type_copy.get_type() == Types::type::Point3D);

   Types::TYPE type_explicit(TYPE(4,5,6), TYPE(7,8,9), "test explicit");
   CHECK(type_explicit.value[0] == 4);
   CHECK(type_explicit.value[1] == 5);
   CHECK(type_explicit.value[2] == 6);
   CHECK(type_explicit.default_value[0] == 7);
   CHECK(type_explicit.default_value[1] == 8);
   CHECK(type_explicit.default_value[2] == 9);
   CHECK(type_explicit.description == "test explicit");
   CHECK(type_explicit.get_type() == Types::type::Point3D);

   std::unique_ptr<Types::Interface> type_clone = type_explicit.clone();
   Types::TYPE* type_clone_natural = dynamic_cast<Types::TYPE*>(type_clone.get());
   CHECK(type_clone_natural->value[0] == 4);
   CHECK(type_clone_natural->value[1] == 5);
   CHECK(type_clone_natural->value[2] == 6);
   CHECK(type_clone_natural->default_value[0] == 7);
   CHECK(type_clone_natural->default_value[1] == 8);
   CHECK(type_clone_natural->default_value[2] == 9);
   CHECK(type_clone_natural->description == "test explicit");
   CHECK(type_clone_natural->get_type() == Types::type::Point3D);

   /**
     * Test Point operators
     */
   const TYPE point_array(std::array<double,3> {1,2,3});
   const TYPE point_explicit(4,5,6);

   Types::TYPE type_point_array(point_array, point_array, "test array");
   Types::TYPE type_point_explicit(point_explicit, point_explicit, "test array");

   CHECK(type_point_array.value.get_array() == std::array<double,3> {1,2,3});
   CHECK(type_point_explicit.value.get_array() == std::array<double,3> {4,5,6});

    // Test multiply operator
    TYPE point = 2 * type_point_array * 1.0;

    CHECK(point.get_array() == std::array<double,3> {2,4,6});

    // Test dot operator
    CHECK(type_point_array * type_point_explicit == 32);

    // Test add operator
    point = type_point_array + type_point_explicit;

    CHECK(point.get_array() == std::array<double,3> {5,7,9});

    // Test subtract operator
    point = type_point_explicit - type_point_array;

    CHECK(point.get_array() == std::array<double,3> {3,3,3});

    // test the access operator
    CHECK(type_point_array[0] == 1);

    type_point_array[0] = 2;
    CHECK(type_point_array[0] == 2);

#undef TYPE
}

TEST_CASE("WorldBuilder Types: Coordinate System")
{
#define TYPE CoordinateSystem
	Types::TYPE type("1","test");
   CHECK(type.value == nullptr);
   CHECK(type.default_value == "1");
   CHECK(type.description == "test");
   CHECK(type.get_type() == Types::type::TYPE);

   std::unique_ptr<Types::Interface> type_clone = type.clone();
   Types::TYPE* type_clone_natural = dynamic_cast<Types::TYPE*>(type_clone.get());
   CHECK(type_clone_natural->value == nullptr);
   CHECK(type_clone_natural->default_value == "1");
   CHECK(type_clone_natural->description == "test");
   CHECK(type_clone_natural->get_type() == Types::type::TYPE);

   /**
    * todo: test the set value function.
    */
#undef TYPE
}

TEST_CASE("WorldBuilder Types: Feature")
{
#define TYPE Feature
	Types::TYPE type("test");
   CHECK(type.description == "test");
   CHECK(type.get_type() == Types::type::TYPE);

   Types::TYPE type_copy(type);
   CHECK(type_copy.description == "test");
   CHECK(type_copy.get_type() == Types::type::TYPE);

   std::unique_ptr<Types::Interface> type_clone = type_copy.clone();
   Types::TYPE* type_clone_natural = dynamic_cast<Types::TYPE*>(type_clone.get());
   CHECK(type_clone_natural->description == "test");
   CHECK(type_clone_natural->get_type() == Types::type::TYPE);

#undef TYPE
}

TEST_CASE("WorldBuilder Types: Array")
{
#define TYPE Array
	Types::TYPE type(Types::Double(0,"double test"),"array test");
   CHECK(type.inner_type == Types::type::Double);
   CHECK(type.inner_type_ptr.get() != nullptr);
   CHECK(type.inner_type_index.size() == 0);
   CHECK(type.description == "array test");
   CHECK(type.get_type() == Types::type::TYPE);

   Types::TYPE type_copy(type);
   CHECK(type_copy.inner_type == Types::type::Double);
   CHECK(type_copy.inner_type_ptr.get() == nullptr);
   CHECK(type_copy.inner_type_index.size() == 0);
   CHECK(type_copy.description == "array test");
   CHECK(type_copy.get_type() == Types::type::TYPE);

   Types::TYPE type_explicit(std::vector<unsigned int> {1,2}, Types::type::Double, "array test explicit");
   CHECK(type_explicit.inner_type == Types::type::Double);
   CHECK(type_explicit.inner_type_ptr.get() == nullptr);
   CHECK(type_explicit.inner_type_index.size() == 2);
   CHECK(type_explicit.description == "array test explicit");
   CHECK(type_explicit.get_type() == Types::type::TYPE);

   std::unique_ptr<Types::Interface> type_clone = type_explicit.clone();
   Types::TYPE* type_clone_natural = dynamic_cast<Types::TYPE*>(type_clone.get());
   CHECK(type_clone_natural->inner_type == Types::type::Double);
   CHECK(type_clone_natural->inner_type_ptr.get() == nullptr);
   CHECK(type_clone_natural->inner_type_index.size() == 2);
   CHECK(type_clone_natural->description == "array test explicit");
   CHECK(type_clone_natural->get_type() == Types::type::TYPE);

   Types::TYPE type_copy2(*type_clone_natural);
   CHECK(type_copy2.inner_type == Types::type::Double);
   CHECK(type_copy2.inner_type_ptr.get() == nullptr);
   CHECK(type_copy2.inner_type_index.size() == 2);
   CHECK(type_copy2.description == "array test explicit");
   CHECK(type_copy2.get_type() == Types::type::TYPE);

#undef TYPE
}

TEST_CASE("WorldBuilder Types: List")
{
#define TYPE List
	Types::TYPE type(Types::Double(0,"double test"),"list test");
   CHECK(type.inner_type == Types::type::Double);
   CHECK(type.inner_type_ptr.get() != nullptr);
   CHECK(type.inner_type_index.size() == 0);
   CHECK(type.description == "list test");
   CHECK(type.get_type() == Types::type::TYPE);

   Types::TYPE type_copy(type);
   CHECK(type_copy.inner_type == Types::type::Double);
   CHECK(type_copy.inner_type_ptr.get() == nullptr);
   CHECK(type_copy.inner_type_index.size() == 0);
   CHECK(type_copy.description == "list test");
   CHECK(type_copy.get_type() == Types::type::TYPE);

   Types::TYPE type_explicit("name",std::vector<unsigned int> {1,2}, Types::type::Double, "list test explicit");
   CHECK(type_explicit.name == "name");
   CHECK(type_explicit.inner_type == Types::type::Double);
   CHECK(type_explicit.inner_type_ptr.get() == nullptr);
   CHECK(type_explicit.inner_type_index.size() == 2);
   CHECK(type_explicit.description == "list test explicit");
   CHECK(type_explicit.get_type() == Types::type::TYPE);

   std::unique_ptr<Types::Interface> type_clone = type_explicit.clone();
   Types::TYPE* type_clone_natural = dynamic_cast<Types::TYPE*>(type_clone.get());
   CHECK(type_clone_natural->name == "name");
   CHECK(type_clone_natural->inner_type == Types::type::Double);
   CHECK(type_clone_natural->inner_type_ptr.get() == nullptr);
   CHECK(type_clone_natural->inner_type_index.size() == 2);
   CHECK(type_clone_natural->description == "list test explicit");
   CHECK(type_clone_natural->get_type() == Types::type::TYPE);

   Types::TYPE type_copy2(*type_clone_natural);
   CHECK(type_copy2.name == "name");
   CHECK(type_copy2.inner_type == Types::type::Double);
   CHECK(type_copy2.inner_type_ptr.get() == nullptr);
   CHECK(type_copy2.inner_type_index.size() == 2);
   CHECK(type_copy2.description == "list test explicit");
   CHECK(type_copy2.get_type() == Types::type::TYPE);
#undef TYPE
}


TEST_CASE("WorldBuilder Types: print_tree")
{
	std::string file_name = WorldBuilder::Data::WORLD_BUILDER_SOURCE_DIR + "/tests/data/simple_wb1.json";
	boost::property_tree::ptree tree;
    std::ifstream json_input_stream(file_name.c_str());
    boost::property_tree::json_parser::read_json (json_input_stream, tree);
    std::stringstream output;
    output <<
    "{\n" <<
    "  \"Cross section\": \n" <<
    "  {\n" <<
    "    \"\": \n" <<
    "    {\n" <<
    "      \"\": \"100e3\",\n" <<
    "      \"\": \"100e3\"\n" <<
    "     },\n" <<
    "    \"\": \n" <<
    "    {\n" <<
    "      \"\": \"400e3\",\n" <<
    "      \"\": \"500e3\"\n" <<
    "     }\n" <<
    "   },\n" <<
    "  \"Coordinate system\": \n" <<
    "  {\n" <<
    "    \"cartesian\": \"\"\n" <<
    "   },\n" <<
    "  \"Surface rotation point\": \n" <<
    "  {\n" <<
    "    \"\": \"165e3\",\n" <<
    "    \"\": \"166e3\"\n" <<
    "   },\n" <<
    "  \"Surface rotation angle\": \"0\",\n" <<
    "  \"Minimum parts per distance unit\": \"5\",\n" <<
    "  \"Minimum distance points\": \"1e-5\",\n" <<
    "  \"Surface objects\": \n" <<
    "  {\n" <<
    "    \"Continental plate\": \n" <<
    "    {\n" <<
    "      \"name\": \"Carribean\",\n" <<
    "      \"coordinates\": \n" <<
    "      {\n" <<
    "        \"\": \n" <<
    "        {\n" <<
    "          \"\": \"-1e3\",\n" <<
    "          \"\": \"500e3\"\n" <<
    "         },\n" <<
    "        \"\": \n" <<
    "        {\n" <<
    "          \"\": \"500e3\",\n" <<
    "          \"\": \"500e3\"\n" <<
    "         },\n" <<
    "        \"\": \n" <<
    "        {\n" <<
    "          \"\": \"500e3\",\n" <<
    "          \"\": \"1000e3\"\n" <<
    "         },\n" <<
    "        \"\": \n" <<
    "        {\n" <<
    "          \"\": \"-1e3\",\n" <<
    "          \"\": \"1000e3\"\n" <<
    "         }\n" <<
    "       },\n" <<
    "      \"temperature submodule\": \n" <<
    "      {\n" <<
    "        \"name\": \"constant\",\n" <<
    "        \"depth\": \"250e3\",\n" <<
    "        \"temperature\": \"150\"\n" <<
    "       },\n" <<
    "      \"composition submodule\": \n" <<
    "      {\n" <<
    "        \"name\": \"none\"\n" <<
    "       }\n" <<
    "     },\n" <<
    "    \"Continental Plate\": \n" <<
    "    {\n" <<
    "      \"name\": \"Rest\",\n" <<
    "      \"coordinates\": \n" <<
    "      {\n" <<
    "        \"\": \n" <<
    "        {\n" <<
    "          \"\": \"2000e3\",\n" <<
    "          \"\": \"2000e3\"\n" <<
    "         },\n" <<
    "        \"\": \n" <<
    "        {\n" <<
    "          \"\": \"1000e3\",\n" <<
    "          \"\": \"2000e3\"\n" <<
    "         },\n" <<
    "        \"\": \n" <<
    "        {\n" <<
    "          \"\": \"1000e3\",\n" <<
    "          \"\": \"1000e3\"\n" <<
    "         },\n" <<
    "        \"\": \n" <<
    "        {\n" <<
    "          \"\": \"2000e3\",\n" <<
    "          \"\": \"1000e3\"\n" <<
    "         }\n" <<
    "       },\n" <<
    "      \"temperature submodule\": \n" <<
    "      {\n" <<
    "        \"name\": \"constant\",\n" <<
    "        \"depth\": \"250e3\",\n" <<
    "        \"temperature\": \"20\"\n" <<
    "       },\n" <<
    "      \"composition submodule\": \n" <<
    "      {\n" <<
    "        \"name\": \"constant\",\n" <<
    "        \"depth\": \"250e3\",\n" <<
    "        \"composition\": \"2\"\n" <<
    "       }\n" <<
    "     },\n" <<
    "    \"Continental plate\": \n" <<
    "    {\n" <<
    "      \"name\": \"Carribean2\",\n" <<
    "      \"coordinates\": \n" <<
    "      {\n" <<
    "        \"\": \n" <<
    "        {\n" <<
    "          \"\": \"-1e3\",\n" <<
    "          \"\": \"500e3\"\n" <<
    "         },\n" <<
    "        \"\": \n" <<
    "        {\n" <<
    "          \"\": \"500e3\",\n" <<
    "          \"\": \"500e3\"\n" <<
    "         },\n" <<
    "        \"\": \n" <<
    "        {\n" <<
    "          \"\": \"500e3\",\n" <<
    "          \"\": \"1000e3\"\n" <<
    "         },\n" <<
    "        \"\": \n" <<
    "        {\n" <<
    "          \"\": \"-1e3\",\n" <<
    "          \"\": \"1000e3\"\n" <<
    "         }\n" <<
    "       },\n" <<
    "      \"temperature submodule\": \n" <<
    "      {\n" <<
    "        \"name\": \"none\",\n" <<
    "        \"depth\": \"250e3\",\n" <<
    "        \"temperature\": \"150\"\n" <<
    "       },\n" <<
    "      \"composition submodule\": \n" <<
    "      {\n" <<
    "        \"name\": \"constant\",\n" <<
    "        \"depth\": \"250e3\",\n" <<
    "        \"composition\": \"3\"\n" <<
    "       }\n" <<
    "     }\n" <<
    "   }\n" <<
    " }";
    CHECK(Utilities::print_tree(tree, 0).str() == output.str());
}

TEST_CASE("WorldBuilder Parameters")
{
  // First test a world builder file with a cross section defined
  std::string file = WorldBuilder::Data::WORLD_BUILDER_SOURCE_DIR + "/tests/data/simple_wb1.json";
  World* null_world = NULL;

  Parameters prm(file, *null_world);


  // Test the UnsignedInt functions
  CHECK_THROWS_WITH(prm.load_entry("non existent unsigned int", true, Types::UnsignedInt(1,"description")),
		            Contains("Entry undeclared: non existent unsigned int"));

  CHECK_THROWS_WITH(prm.get_unsigned_int("non existent unsigned int"),
		  Contains("Could not find entry 'non existent unsigned int' not found. Make sure it is loaded or set"));

  CHECK(prm.load_entry("non existent unsigned int", false, Types::UnsignedInt(1,"description")) == false);
  CHECK(prm.get_unsigned_int("non existent unsigned int") == 1);

  prm.set_entry("new unsigned int", Types::UnsignedInt(2,"description"));
  CHECK(prm.get_unsigned_int("new unsigned int") == 2);

  // Test the Double functions
  CHECK_THROWS_WITH(prm.load_entry("non existent double", true, Types::Double(1,"description")),
		            Contains("Entry undeclared: non existent"));

  CHECK_THROWS_WITH(prm.get_double("non existent double"),
		  Contains("Could not find entry 'non existent double' not found. Make sure it is loaded or set"));

  CHECK(prm.load_entry("non existent double", false, Types::Double(1,"description")) == false);
  CHECK(prm.get_double("non existent double") == 1);

  prm.set_entry("new double", Types::Double(2,"description"));
  CHECK(prm.get_double("new double") == 2);


  // Test the String functions
  CHECK_THROWS_WITH(prm.load_entry("non existent string", true, Types::String("1","description")),
		            Contains("Entry undeclared: non existent string"));

  CHECK_THROWS_WITH(prm.get_string("non existent string"),
		  Contains("Could not find entry 'non existent string' not found. Make sure it is loaded or set"));

  CHECK(prm.load_entry("non exitent string", false, Types::String("1","description")) == false);
  CHECK(prm.get_string("non exitent string") == "1");

  prm.set_entry("new string", Types::String("2","description"));
  CHECK(prm.get_string("new string") == "2");

  // Test the Point functions
  CHECK_THROWS_WITH(prm.load_entry("non existent 2d Point", true, Types::Point<2>(Point<2>(1,2),"description")),
		            Contains("Could not find .non existent 2d Point, while it is set as required."));
  CHECK_THROWS_WITH(prm.load_entry("non existent 3d Point", true, Types::Point<3>(Point<3>(1,2,3),"description")),
		            Contains("Could not find .non existent 3d Point, while it is set as required."));

  CHECK_THROWS_WITH(prm.get_point<2>("non existent 2d Point"),
		  Contains("Could not find entry 'non existent 2d Point' not found. Make sure it is loaded or set"));
  CHECK_THROWS_WITH(prm.get_point<3>("non existent 3d Point"),
		  Contains("Could not find entry 'non existent 3d Point' not found. Make sure it is loaded or set"));

  CHECK(prm.load_entry("non existent 2d Point", false, Types::Point<2>(Point<2>(1,2),"description")) == false);
  CHECK(prm.load_entry("non existent 3d Point", false, Types::Point<3>(Point<3>(1,2,3),"description")) == false);

  CHECK(prm.get_point<2>("non existent 2d Point").get_array() == std::array<double,2>{1,2});
  CHECK(prm.get_point<3>("non existent 3d Point").get_array() == std::array<double,3>{1,2,3});

  prm.set_entry("new Point 2d", Types::Point<2>(Point<2>(3,4),"description"));
  prm.set_entry("new Point 3d", Types::Point<3>(Point<3>(5,6,7),"description"));

  CHECK(prm.get_point<2>("new Point 2d").get_array() == std::array<double,2>{3,4});
  CHECK(prm.get_point<3>("new Point 3d").get_array() == std::array<double,3>{5,6,7});

/**
 * Todo: add tests for list, array, feature and coordinate system.
 */



}
