
cmake_minimum_required (VERSION 2.6)
project(WorldBuilder C CXX Fortran)

# load in version info and export it
FILE(STRINGS "${CMAKE_SOURCE_DIR}/VERSION" WORLD_BUILDER_VERSION LIMIT_COUNT 1)

string(REPLACE "-" "." VERSION_LIST_TEMP ${WORLD_BUILDER_VERSION})
string(REPLACE "." ";" VERSION_LIST ${VERSION_LIST_TEMP})
list(LENGTH VERSION_LIST VERSION_LENGTH)
list(GET VERSION_LIST 0 WORLD_BUILDER_VERSION_MAJOR)
list(GET VERSION_LIST 1 WORLD_BUILDER_VERSION_MINOR)
list(GET VERSION_LIST 2 WORLD_BUILDER_VERSION_PATCH)
if (${VERSION_LENGTH} GREATER 3)
list(GET VERSION_LIST 3 WORLD_BUILDER_VERSION_LABEL)
endif()
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

message(STATUS "Building the Geodynamic World Builder version ${WORLD_BUILDER_VERSION} in ${CMAKE_BUILD_TYPE} mode")

set (WORLD_BUILDER_SOURCE_DIR ${PROJECT_SOURCE_DIR})

# generate version.cc
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/source/config.cc.in" "${PROJECT_SOURCE_DIR}/source/config.cc" @ONLY)


# Add include directory
include_directories("include/" "tests/") 

# Add source directory
file(GLOB_RECURSE SOURCES "source/*.cc" "source/*.f90")

# Provide "indent" target for indenting all headers and source files
ADD_CUSTOM_TARGET(indent
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMAND ./doc/indent
  COMMENT "Indenting all the World Builder header and source files..."
  )


if (NOT MSVC AND NOT APPLE)
   SET(CMAKE_CXX_FLAGS "-std=c++11 -fopenmp -pedantic -fPIC -Wall -Wextra -Wpointer-arith -Wwrite-strings -Wsynth -Wsign-compare -Wswitch -Woverloaded-virtual -Wno-placement-new -Wno-literal-suffix -Wno-parentheses -Wno-unused-local-typedefs -Wcast-qual -fstrict-aliasing")
elseif(APPLE)
   SET(CMAKE_CXX_FLAGS "-std=c++11 -Xpreprocessor -fopenmp -lomp -pedantic -fPIC -Wall -Wextra -Wpointer-arith -Wwrite-strings -Wsynth -Wsign-compare -Wswitch -Woverloaded-virtual -Wno-placement-new -Wno-literal-suffix -Wno-parentheses -Wno-unused-local-typedefs -Wcast-qual -fstrict-aliasing")
else()
    SET(CMAKE_CXX_FLAGS "/W3")
endif()


IF(NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE Debug
       CACHE STRING "Choose the type of build : None Debug Release RelWithDebInfo MinSizeRel Coverage."
       FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

IF ( CMAKE_BUILD_TYPE STREQUAL Coverage )
  SET( CMAKE_VERBOSE_MAKEFILE on )
  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ")
  SET(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_DEBUG} --coverage -fprofile-arcs -ftest-coverage ")
  SET(CMAKE_EXE_LINKER_FLAGS_COVERAGE "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage -fprofile-arcs -ftest-coverage ")
  SET(CMAKE_SHARED_LINKER_FLAGS_COVERAGE "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} --coverage -fprofile-arcs -ftest-coverage ")

MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_COVERAGE
    CMAKE_C_FLAGS_COVERAGE
    CMAKE_EXE_LINKER_FLAGS_COVERAGE
    CMAKE_SHARED_LINKER_FLAGS_COVERAGE )
ENDIF()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_Fortran_MODULE_DIRECTORY ${CMAKE_BINARY_DIR}/mod)

add_library(WorldBuilder ${SOURCES})
add_executable(WorldBuilderApp "${CMAKE_CURRENT_SOURCE_DIR}/app/main.cc")
add_executable(WorldBuilderVisualization "${CMAKE_CURRENT_SOURCE_DIR}/visualization/main.cc")
target_link_libraries (WorldBuilderApp WorldBuilder) 
target_link_libraries (WorldBuilderVisualization WorldBuilder) 

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
