#include_directories (${TEST_SOURCE_DIR}/src ${Boost_INCLUDE_DIRS})

#find all the unit test files
file(GLOB_RECURSE UNIT_TEST_SOURCES "unit_tests/*.cc" "unit_tests/*.f90")

#set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin/)

# Run through each source
foreach(test_source ${UNIT_TEST_SOURCES})
        # Extract the filename without an extension (NAME_WE)
        get_filename_component(test_name ${test_source} NAME_WE)

        # Add compile target
        add_executable(${test_name} ${test_source})

    target_link_libraries(${test_name} ${Boost_LIBRARIES} WorldBuilder)

	# Move testing binaries into a testBin directory
        set_target_properties(${test_name} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/bin/unit_tests/)

        #Finally add it to test execution -
        #Notice the WORKING_DIRECTORY and COMMAND
        add_test(NAME ${test_name}
                 WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/unit_tests/
		 COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/unit_tests/${test_name})
endforeach(test_source)

#find all the integration test files
file(GLOB_RECURSE APP_TEST_SOURCES "app/*.wb")

# Run through each sourceUforeach(test_source ${APP_TEST_SOURCES})
foreach(test_source ${APP_TEST_SOURCES})
        get_filename_component(test_name ${test_source} NAME_WE)

        # Add compile target
	#add_executable(${test_name} ${test_source})

	#target_link_libraries(${test_name} ${Boost_LIBRARIES} WorldBuilder)

        # Move testing binaries into a testBin directory
	#	set_target_properties(${test_name} PROPERTIES
	#  RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_SOURCE_DIR}/bin/app/)

        #Finally add it to test execution -
        #Notice the WORKING_DIRECTORY and COMMAND
        add_test(NAME ${test_name}
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/app/
		 COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/WorldBuilderApp ${CMAKE_SOURCE_DIR}/tests/app/${test_name}.wb ${CMAKE_SOURCE_DIR}/tests/app/${test_name}.dat)
endforeach(test_source)

