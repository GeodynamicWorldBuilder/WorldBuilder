#include_directories (${TEST_SOURCE_DIR}/src ${Boost_INCLUDE_DIRS})

#find all the test files
file(GLOB_RECURSE TEST_SOURCES "*.cc" "*.f90")

# Run through each source
foreach(test_source ${TEST_SOURCES})
        # Extract the filename without an extension (NAME_WE)
        get_filename_component(test_name ${test_source} NAME_WE)

        # Add compile target
        add_executable(${test_name} ${test_source})

	# Link executable to Boost libraries and the World Builder
	target_link_libraries(${test_name} ${Boost_LIBRARIES} WorldBuilder)

	# Move testing binaries into a testBin directory
        set_target_properties(${test_name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/bin/)

        #Finally add it to test execution -
        #Notice the WORKING_DIRECTORY and COMMAND
        add_test(NAME ${test_name}
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/
                 COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/bin/${test_name} )
endforeach(test_source)

#add an executable and link it
#add_executable(wb_tests ${TEST_SOURCES})
#target_link_libraries (wb_tests WorldBuilder ${Boost_LIBRARIES})

#add it as a test
#add_test (NAME boost_test COMMAND wb_tests)
