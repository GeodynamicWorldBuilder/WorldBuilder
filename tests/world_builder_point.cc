#define BOOST_TEST_MAIN
#if !defined( WIN32 )
    #define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MODULE "BaseClassModule"
#include <boost/test/unit_test.hpp>
#include <world_builder/point.h>
#include <world_builder/utilities.h>

using namespace WorldBuilder;

BOOST_AUTO_TEST_CASE( la ) {
	const Point<2> p2;
	const Point<3> p3;
	const std::array<double,2> a2 = {0,0};
	const std::array<double,3> a3 = {0,0,0};

	BOOST_CHECK_EQUAL_COLLECTIONS(p2.get_array().begin(), p2.get_array().end(), a2.begin(), a2.end());
	BOOST_CHECK_EQUAL_COLLECTIONS(p3.get_array().begin(), p3.get_array().end(), a3.begin(), a3.end());
}

BOOST_AUTO_TEST_CASE( WorldBuilderPointInit )
{
  const std::array<double,2> a2 = {1,2};
  const std::array<double,3> a3 = {1,2,3};
  const Point<2> p2_array(a2);
  const Point<3> p3_array(a3);
  
  BOOST_CHECK_EQUAL_COLLECTIONS(p2_array.get_array().begin(), p2_array.get_array().end(), a2.begin(), a2.end());
  BOOST_CHECK_EQUAL_COLLECTIONS(p3_array.get_array().begin(), p3_array.get_array().end(), a3.begin(), a3.end());

  const Point<2> p2_point(p2_array);
  const Point<3> p3_point(p3_array);

  BOOST_CHECK_EQUAL_COLLECTIONS(p2_point.get_array().begin(), p2_point.get_array().end(), a2.begin(), a2.end());
  BOOST_CHECK_EQUAL_COLLECTIONS(p3_point.get_array().begin(), p3_point.get_array().end(), a3.begin(), a3.end());

  const Point<2> p2_explicit(1,2);
  const Point<3> p3_explicit(1,2,3);

  BOOST_CHECK_EQUAL_COLLECTIONS(p2_explicit.get_array().begin(), p2_explicit.get_array().end(), a2.begin(), a2.end());
  BOOST_CHECK_EQUAL_COLLECTIONS(p3_explicit.get_array().begin(), p3_explicit.get_array().end(), a3.begin(), a3.end());

  std::array<double,2> an2 = Utilities::convert_point_to_array(p2_point);
  std::array<double,3> an3 = Utilities::convert_point_to_array(p3_point);

  BOOST_CHECK_EQUAL_COLLECTIONS(p2_point.get_array().begin(), p2_point.get_array().end(), an2.begin(), an2.end());
  BOOST_CHECK_EQUAL_COLLECTIONS(p3_point.get_array().begin(), p3_point.get_array().end(), an3.begin(), an3.end());

//  REQUIRE_THROWS_WITH(Point<2>(1,2,3),Contains("Can't use the 3d constructor in 2d."));
//  REQUIRE_THROWS_WITH(Point<3>(1,2),Contains("Can't use the 2d constructor in 3d."));

}
BOOST_AUTO_TEST_CASE( lb ) {
    BOOST_CHECK_EQUAL(Utilities::string_to_double("1"), 1.0);
    BOOST_CHECK_EQUAL(Utilities::string_to_int("2"), 2);
    BOOST_CHECK_EQUAL(Utilities::string_to_unsigned_int("3"), 3);
}
