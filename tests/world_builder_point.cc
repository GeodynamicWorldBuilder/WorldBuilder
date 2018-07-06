#define CATCH_CONFIG_MAIN
#include <catch2.h>
#include <world_builder/point.h>
#include <world_builder/utilities.h>
#include <world_builder/coordinate_systems/interface.h>

using namespace WorldBuilder;

using Catch::Matchers::Contains;

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

TEST_CASE("WorldBuilder Utilities: string to number conversion")
{
	REQUIRE(Utilities::string_to_double("1") == 1.0);
	REQUIRE(Utilities::string_to_int("2") == 2);
	REQUIRE(Utilities::string_to_unsigned_int("3") == 3);
}

TEST_CASE("WorldBuilder Utilities: Point in polygon")
{
	std::vector<Point<2> > point_list1(4);
	point_list1[0] = Point<2>(0,0);
	point_list1[1] = Point<2>(5,0);
	point_list1[2] = Point<2>(5,5);
	point_list1[3] = Point<2>(0,5);

	std::vector<Point<2> > point_list2(3);
	point_list2[0] = Point<2>(10,10);
	point_list2[1] = Point<2>(10,15);
	point_list2[2] = Point<2>(15,15);

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

	for(unsigned int i = 0; i < check_points.size(); ++i)
	{
		INFO("checking point " << i << " = (" << check_points[i][0] << ":" << check_points[i][1] << ")");
		REQUIRE(Utilities::polygon_contains_point(point_list1,check_points[i]) == awnsers[i][0]);
		REQUIRE(Utilities::polygon_contains_point(point_list2,check_points[i]) == awnsers[i][1]);
	}
}

