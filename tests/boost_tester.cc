#define BOOST_TEST_MAIN
#if !defined( WIN32 )
    #define BOOST_TEST_DYN_LINK
#endif
#define BOOST_TEST_MODULE "BaseClassModule"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( la ) {
    BOOST_CHECK_EQUAL(1, 1);
}
BOOST_AUTO_TEST_CASE( lb ) {
    BOOST_CHECK_EQUAL(1, 1);
    BOOST_CHECK_EQUAL(2, 2);
}
