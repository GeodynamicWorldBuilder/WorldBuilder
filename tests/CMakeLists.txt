#include_directories (${TEST_SOURCE_DIR}/src ${Boost_INCLUDE_DIRS})

#find all the test files
file(GLOB_RECURSE TEST_SOURCES "*.cc" "*.f90")

#add an executable and link it
add_executable(wb_tests ${TEST_SOURCES})
target_link_libraries (wb_tests WorldBuilder ${Boost_LIBRARIES})

#add it as a test
add_test (NAME boost_test COMMAND wb_tests)
