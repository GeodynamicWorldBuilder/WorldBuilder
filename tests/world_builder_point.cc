#define CATCH_CONFIG_MAIN
#include <catch2.h>
#include <world_builder/point.h>
#include <world_builder/utilities.h>
#include <world_builder/coordinate_systems/interface.h>
#include <world_builder/coordinate_systems/cartesian.h>

using namespace WorldBuilder;

using Catch::Matchers::Contains;

/**
 * Compare the given two std::vector<double> entries with an epsilon (using Catch::Approx)
 */
inline void compare_vectors_approx(
  const std::vector<double> &computed,
  const std::vector<double> &expected)
{
  REQUIRE(computed.size() == expected.size());
  for (unsigned int i=0; i< computed.size(); ++i)
    {
      INFO("vector index i=" << i << ": ");
      REQUIRE(computed[i] == Approx(expected[i]));
    }
}


TEST_CASE("WorldBuilder Point: Testing initialize and operators")
{
	// Test initialization of the Point class
	Point<2> p2;
	Point<3> p3;

	REQUIRE(p2.get_array() == std::array<double,2> {0,0});
	REQUIRE(p3.get_array() == std::array<double,3> {0,0,0});

	const Point<2> p2_array(std::array<double,2> {1,2});
	const Point<3> p3_array(std::array<double,3> {1,2,3});

	REQUIRE(p2_array.get_array() == std::array<double,2> {1,2});
	REQUIRE(p3_array.get_array() == std::array<double,3> {1,2,3});

	const Point<2> p2_point(p2_array);
	const Point<3> p3_point(p3_array);

	REQUIRE(p2_point.get_array() == std::array<double,2> {1,2});
	REQUIRE(p3_point.get_array() == std::array<double,3> {1,2,3});

	const Point<2> p2_explicit(3,4);
	const Point<3> p3_explicit(4,5,6);

	REQUIRE(p2_explicit.get_array() == std::array<double,2> {3,4});
	REQUIRE(p3_explicit.get_array() == std::array<double,3> {4,5,6});

	/**
	 * Test Point operators
	 */
	// Test assign operator
	p2 = p2_array;
	p3 = p3_array;

	REQUIRE(p2.get_array() == std::array<double,2> {1,2});
	REQUIRE(p3.get_array() == std::array<double,3> {1,2,3});

	// Test multiply operator
	p2 = 2 * p2 * 1.0;
	p3 = 2 * p3 * 1.0;

	REQUIRE(p2.get_array() == std::array<double,2> {2,4});
	REQUIRE(p3.get_array() == std::array<double,3> {2,4,6});

	p2 *= 2;
	p3 *= 2;

	REQUIRE(p2.get_array() == std::array<double,2> {4,8});
	REQUIRE(p3.get_array() == std::array<double,3> {4,8,12});

	// Test dot operator
	REQUIRE(p2_array * p2_explicit == 11);
	REQUIRE(p3_array * p3_explicit == 32);

	// Test add operator
	p2 = p2 + p2;
	p3 = p3 + p3;

	REQUIRE(p2.get_array() == std::array<double,2> {8,16});
	REQUIRE(p3.get_array() == std::array<double,3> {8,16,24});

	p2 += p2;
	p3 += p3;

	REQUIRE(p2.get_array() == std::array<double,2> {16,32});
	REQUIRE(p3.get_array() == std::array<double,3> {16,32,48});

	// Test subtract operator
	p2 = p2 - (0.5 * p2);
	p3 = p3 - (0.5 * p3);

	REQUIRE(p2.get_array() == std::array<double,2> {8,16});
	REQUIRE(p3.get_array() == std::array<double,3> {8,16,24});

	p2 -=  (0.5 * p2);
	p3 -=  (0.5 * p3);

	REQUIRE(p2.get_array() == std::array<double,2> {4,8});
	REQUIRE(p3.get_array() == std::array<double,3> {4,8,12});

	// Test coordinate system
	REQUIRE(p2.get_coordinate_system() == CoordinateSystem::cartesian);
	REQUIRE(p3.get_coordinate_system() == CoordinateSystem::cartesian);

	// Test norm and norm_square
	REQUIRE(p2.norm_square() == 80);
	REQUIRE(p3.norm_square() == 224);

	REQUIRE(p2.norm() == std::sqrt(80));
	REQUIRE(p3.norm() == std::sqrt(224));

	// Test Point utility classes
	std::array<double,2> an2 = Utilities::convert_point_to_array(p2_point);
	std::array<double,3> an3 = Utilities::convert_point_to_array(p3_point);

	REQUIRE(an2 == std::array<double,2> {1,2});
	REQUIRE(an3 == std::array<double,3> {1,2,3});

	REQUIRE_THROWS_WITH(Point<2>(1,2,3),Contains("Can't use the 3d constructor in 2d."));
	REQUIRE_THROWS_WITH(Point<3>(1,2),Contains("Can't use the 2d constructor in 3d."));



}

TEST_CASE("WorldBuilder Utilities: string to conversions")
{
	// Test string to number conversion
	REQUIRE(Utilities::string_to_double("1") == 1.0);
	REQUIRE(Utilities::string_to_double(" 1 ") == 1.0);
	REQUIRE(Utilities::string_to_double(" 1.01 ") == 1.01);

	REQUIRE_THROWS_WITH(Utilities::string_to_double("1a"),
			            Contains("Conversion of \"1a\" to double failed (bad cast): "));
	REQUIRE_THROWS_WITH(Utilities::string_to_double("a1"),
			            Contains("Conversion of \"a1\" to double failed (bad cast): "));
	REQUIRE_THROWS_WITH(Utilities::string_to_double("a"),
			            Contains("Conversion of \"a\" to double failed (bad cast): "));

	REQUIRE(Utilities::string_to_int("2") == 2);
	REQUIRE(Utilities::string_to_int(" 2 ") == 2);

	REQUIRE_THROWS_WITH(Utilities::string_to_int(" 2.02 "),
			            Contains("Conversion of \" 2.02 \" to int failed (bad cast): "));
	REQUIRE_THROWS_WITH(Utilities::string_to_int("2b"),
			            Contains("Conversion of \"2b\" to int failed (bad cast): "));
	REQUIRE_THROWS_WITH(Utilities::string_to_int("b2"),
			            Contains("Conversion of \"b2\" to int failed (bad cast): "));
	REQUIRE_THROWS_WITH(Utilities::string_to_int("b"),
			            Contains("Conversion of \"b\" to int failed (bad cast): "));

	REQUIRE(Utilities::string_to_unsigned_int("3") == 3);
	REQUIRE(Utilities::string_to_unsigned_int(" 3 ") == 3);

	REQUIRE_THROWS_WITH(Utilities::string_to_unsigned_int(" 3.03 "),
				        Contains("Conversion of \" 3.03 \" to unsigned int failed (bad cast): "));
	REQUIRE_THROWS_WITH(Utilities::string_to_unsigned_int("3c"),
			            Contains("Conversion of \"3c\" to unsigned int failed (bad cast): "));
	REQUIRE_THROWS_WITH(Utilities::string_to_unsigned_int("c3"),
			            Contains("Conversion of \"c3\" to unsigned int failed (bad cast): "));
	REQUIRE_THROWS_WITH(Utilities::string_to_unsigned_int("c"),
			            Contains("Conversion of \"c\" to unsigned int failed (bad cast): "));

	// Test point to array conversion
	const Point<2> p2(1,2);
	const Point<3> p3(1,2,3);

	REQUIRE(Utilities::convert_point_to_array(p2) == std::array<double,2>{1,2});
	REQUIRE(Utilities::convert_point_to_array(p3) == std::array<double,3>{1,2,3});

	// Test coordinate system
	REQUIRE(Utilities::string_to_coordinate_system("cartesian") == CoordinateSystem::cartesian);
	REQUIRE(Utilities::string_to_coordinate_system("spherical") == CoordinateSystem::spherical);
	REQUIRE_THROWS_WITH(Utilities::string_to_coordinate_system("other"), Contains("Coordinate system not implemented."));
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

	for(unsigned int i = 0; i < check_points.size(); ++i)
	{
		INFO("checking point " << i << " = (" << check_points[i][0] << ":" << check_points[i][1] << ")");
		REQUIRE(Utilities::polygon_contains_point(point_list_4_elements,check_points[i]) == awnsers[i][0]);
		REQUIRE(Utilities::polygon_contains_point(point_list_3_elements,check_points[i]) == awnsers[i][1]);
		REQUIRE(Utilities::signed_distance_to_polygon(point_list_4_elements,check_points[i]) == Approx(awnsers_signed_distance[i][0]));
		REQUIRE(Utilities::signed_distance_to_polygon(point_list_3_elements,check_points[i]) == Approx(awnsers_signed_distance[i][1]));
	}

	std::vector<Point<2> > point_list_2_elements(2);
	REQUIRE_THROWS_WITH(Utilities::signed_distance_to_polygon(point_list_2_elements,check_points[0]),
			                                                  Contains("Not enough polygon points were specified."));

	std::vector<Point<2> > point_list_1_elements(1);
	REQUIRE_THROWS_WITH(Utilities::signed_distance_to_polygon(point_list_1_elements,check_points[0]),
			                                                  Contains("Not enough polygon points were specified."));

	std::vector<Point<2> > point_list_0_elements(0);
	REQUIRE_THROWS_WITH(Utilities::signed_distance_to_polygon(point_list_0_elements,check_points[0]),
			                                                  Contains("Not enough polygon points were specified."));
}


TEST_CASE("WorldBuilder Utilities: Natural Coordinate")
{
	CoordinateSystems::Interface* cartesian = new CoordinateSystems::Cartesian;

	Utilities::NaturalCoordinate nca1(std::array<double,3>{1,2,3},*cartesian);
	REQUIRE(nca1.get_coordinates() == std::array<double,3>{1,2,3});
	REQUIRE(nca1.get_surface_coordinates() == std::array<double,2>{1,2});
	REQUIRE(nca1.get_depth_coordinate() == 3);

	Utilities::NaturalCoordinate ncp1(Point<3>(1,2,3),*cartesian);
	REQUIRE(ncp1.get_coordinates() == std::array<double,3>{1,2,3});
	REQUIRE(ncp1.get_surface_coordinates() == std::array<double,2>{1,2});
	REQUIRE(ncp1.get_depth_coordinate() == 3);
}

TEST_CASE("WorldBuilder Utilities: Coordinate systems transformations")
{
	// Test coordinate system transformation
	Point<3> cartesian(3,4,5);

	Point<3> spherical(Utilities::cartesian_to_spherical_coordinates(cartesian.get_array()), CoordinateSystem::spherical);

	compare_vectors_approx(std::vector<double>(std::begin(spherical.get_array()), std::end(spherical.get_array())),
			               std::vector<double>{std::sqrt(3*3+4*4+5*5),0.927295218001613,0.7853982});

	Point<3> cartesian_back(Utilities::spherical_to_cartesian_coordinates(spherical.get_array()), CoordinateSystem::cartesian);

	compare_vectors_approx(std::vector<double>(std::begin(cartesian_back.get_array()), std::end(cartesian_back.get_array())),
			               std::vector<double>{3,4,5});


}

TEST_CASE("WorldBuilder Utilities: ptree function")
{
	ptree tree;
	tree.put("value", 3.14159);
	REQUIRE(Utilities::string_to_double(Utilities::get_from_ptree(tree, "pi", "value", ".")) == Approx(3.14159));
	REQUIRE_THROWS_WITH(Utilities::get_from_ptree(tree, "pi", "value_pi", "."),
			            Contains("Entry undeclared: pi.value_pi"));
}
