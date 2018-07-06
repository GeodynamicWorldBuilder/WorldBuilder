#define CATCH_CONFIG_MAIN
#include <catch2.h>
#include <world_builder/point.h>
#include <world_builder/utilities.h>

using namespace WorldBuilder;

using Catch::Matchers::Contains;

TEST_CASE("WorldBuilder Point: Initialize point to zero")
{
	const Point<2> p2;
	const Point<3> p3;

	  REQUIRE(p2.get_array() == std::array<double,2> {0,0});
	  REQUIRE(p3.get_array() == std::array<double,3> {0,0,0});
}

TEST_CASE("WorldBuilder Point: Initialize point from sources")
{
	  const Point<2> p2_array(std::array<double,2> {1,2});
	  const Point<3> p3_array(std::array<double,3> {1,2,3});

	  REQUIRE(p2_array.get_array() == std::array<double,2> {1,2});
	  REQUIRE(p3_array.get_array() == std::array<double,3> {1,2,3});

	  const Point<2> p2_point(p2_array);
	  const Point<3> p3_point(p3_array);

	  REQUIRE(p2_point.get_array() == std::array<double,2> {1,2});
	  REQUIRE(p3_point.get_array() == std::array<double,3> {1,2,3});

	  const Point<2> p2_explicit(1,2);
	  const Point<3> p3_explicit(1,2,3);

	  REQUIRE(p2_explicit.get_array() == std::array<double,2> {1,2});
	  REQUIRE(p3_explicit.get_array() == std::array<double,3> {1,2,3});

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
