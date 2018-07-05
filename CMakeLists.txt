
cmake_minimum_required (VERSION 2.6)
project(WorldBuilder C CXX Fortran)

# load in version info and export it
FILE(STRINGS "${CMAKE_SOURCE_DIR}/VERSION" ASPECT_PACKAGE_VERSION LIMIT_COUNT 1)

# the commit's SHA1, and whether the building workspace was dirty or not
find_package(Git)
execute_process(COMMAND
  "${GIT_EXECUTABLE}" describe --match=NeVeRmAtCh --always --abbrev=40 --dirty
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE GIT_SHA1
  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(COMMAND
  "${GIT_EXECUTABLE}" rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE GIT_BRANCH
  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

# the date of the commit
execute_process(COMMAND
  "${GIT_EXECUTABLE}" log -1 --format=%ad --date=local
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE GIT_DATE
  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

# the subject of the commit
execute_process(COMMAND
  "${GIT_EXECUTABLE}" log -1 --format=%s
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE GIT_COMMIT_SUBJECT
  ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)


# generate version.cc
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/source/version.cc.in" "${PROJECT_SOURCE_DIR}/source/version.cc" @ONLY)


# Add include directory
include_directories("include/") 

# Add source directory
file(GLOB_RECURSE SOURCES "source/*.cc" "source/*.f90")


#add Boost
find_package(Boost REQUIRED COMPONENTS program_options unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# Provide "indent" target for indenting all headers and source files
ADD_CUSTOM_TARGET(indent
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMAND ./doc/indent
  COMMENT "Indenting all the World Builder header and source files..."
  )

#SET(COVERAGE OFF CACHE BOOL "Coverage")	
SET(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_DEBUG} --coverage -fprofile-arcs -ftest-coverage ")
SET(CMAKE_EXE_LINKER_FLAGS_COVERAGE "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage -fprofile-arcs -ftest-coverage ")
SET(CMAKE_SHARED_LINKER_FLAGS_COVERAGE "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} --coverage -fprofile-arcs -ftest-coverage ")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_Fortran_MODULE_DIRECTORY ${CMAKE_BINARY_DIR}/mod)

add_library(WorldBuilder ${SOURCES})
#if (COVERAGE)
#    target_compile_options(WorldBuilder PRIVATE --coverage)
#    target_link_libraries(WorldBuilder PRIVATE --coverage)
#endif()
add_executable(WorldBuilderApp "${CMAKE_CURRENT_SOURCE_DIR}/app/main.cc")
#if (COVERAGE)
#    target_compile_options(WorldBuilderApp PRIVATE --coverage)
#    target_link_libraries(WorldBuilderApp PRIVATE --coverage ${Boost_LIBRARIES} WorldBuilder)
#else()
    target_link_libraries (WorldBuilderApp WorldBuilder ${Boost_LIBRARIES})
    #endif()
# binary:
install (TARGETS WorldBuilder EXPORT WorldBuilder  DESTINATION bin)
install (DIRECTORY ${CMAKE_Fortran_MODULE_DIRECTORY}/ DESTINATION include)
install (EXPORT WorldBuilder  DESTINATION bin)
# headers:
INSTALL(DIRECTORY include/
  DESTINATION include
  COMPONENT includes
  FILES_MATCHING PATTERN "*.h")

enable_testing()
add_subdirectory(tests)
