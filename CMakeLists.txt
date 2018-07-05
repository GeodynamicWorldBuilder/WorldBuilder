
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
find_package(Boost REQUIRED COMPONENTS program_options)
include_directories(${Boost_INCLUDE_DIR})


add_library(WorldBuilder ${SOURCES})
add_executable(WorldBuilderApp "${CMAKE_CURRENT_SOURCE_DIR}/app/main.cc")
target_link_libraries (WorldBuilderApp WorldBuilder ${Boost_LIBRARIES})
