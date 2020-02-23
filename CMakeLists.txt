cmake_minimum_required (VERSION 2.6)
project(WorldBuilder C CXX)

# SWIG: use standard target name.
if(POLICY CMP0078)
  cmake_policy(SET CMP0078 NEW)
endif()
## SWIG: use SWIG_MODULE_NAME property.
if(POLICY CMP0086)
  cmake_policy(SET CMP0086 NEW)
endif()

# load in version info and export it
SET(WORLD_BUILDER_SOURCE_DIR ${CMAKE_SOURCE_DIR})
INCLUDE("${CMAKE_SOURCE_DIR}/cmake/version.cmake")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "Building the Geodynamic World Builder version ${WORLD_BUILDER_VERSION} in ${CMAKE_BUILD_TYPE} mode")

set (WORLD_BUILDER_SOURCE_DIR ${PROJECT_SOURCE_DIR})

# generate version.cc
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/source/config.cc.in" "${CMAKE_BINARY_DIR}/source/config.cc" @ONLY)

# finding support for different languages
# finding a fortran compiler
if(${CMAKE_VERSION} VERSION_LESS "2.8.8") 
  message(STATUS "Fortran not supported with cmake version lower than  2.8.8. Please upgrade your camke version if you need it. Disabling Fortran wrapper and tests.")
else()
  include(CheckLanguage)
  check_language(Fortran)
  if(CMAKE_Fortran_COMPILER)
    enable_language(Fortran)
    message(STATUS "Found Fortran support. Enabling Fortran wrapper and tests.")
  else()
    message(STATUS "Did not find Fortran support. Disabling Fortran wrapper and tests.")
  endif()
endif()

#finding support for makeing a python wrapper
SET(MAKE_PYTHON_WRAPPER FALSE)
#find swig
FIND_PACKAGE(SWIG)
if(SWIG_FOUND)
  INCLUDE(${SWIG_USE_FILE})
SET(CMAKE_SWIG_FLAGS "")
else()
  message(STATUS "SWIG was not found, disableling python wrapper compilation.")
endif()

#find python libs
if(${CMAKE_VERSION} VERSION_LESS "3.12.0") 
  FIND_PACKAGE(PythonLibs 3.0)
  FIND_PACKAGE(PythonInterp 3.0)

    if(PYTHONLIBS_FOUND AND PYTHONINTERP_FOUND)
      SET(Python3_FOUND TRUE)
      SET(Python3_EXECUTABLE ${PYTHON_EXECUTABLE})
      SET(Python3_INCLUDE_DIRS ${PYTHON_INCLUDE_DIRS})
      SET(Python3_LIBRARIES ${PYTHON_LIBRARIES})
      SET(Python3_LIBRARIES_RELEASE ${PYTHON_LIBRARIES_RELEASE})
      include_directories(${PYTHON_INCLUDE_DIRS})
    else()
      SET(Python_FOUND FALSE)
      message(STATUS "Python was not found. Disableling python wrapper compilation.")
    endif()

else()
  if(${CMAKE_VERSION} VERSION_LESS "3.12.0") 
    FIND_PACKAGE(Python3 COMPONENTS Interpreter Development)
  else()
    FIND_PACKAGE(Python3 COMPONENTS Interpreter Development NumPy)
    if(NOT Python3_FOUND)
      FIND_PACKAGE(Python3 COMPONENTS Interpreter Development)
    endif()
  endif()
  if(Python3_FOUND)
    include_directories(${Python3_INCLUDE_DIRS})
  else()
    message(STATUS "Python 3 was not found, disableling python wrapper compilation.")
  endif()
  if(Python3_NumPy_FOUND)
    message(STATUS "Python 3 NUMPY was found.")
    include_directories(${Python3_NumPy_INCLUDE_DIRS})
  else()
    message(STATUS "Python 3 NUMPY was not found, this may be because it was not installed or because you have a cmake version lower than 3.14.")
  endif()
endif()


if(SWIG_FOUND AND Python3_FOUND)
  SET(MAKE_PYTHON_WRAPPER TRUE)
    message(STATUS "Both SWIG and Python 3 are found, enableling python wrapper compilation.")
endif()

# Add include directory
include_directories("include/" "tests/") 

# Add source directory
file(GLOB_RECURSE SOURCES_CXX "source/*.cc" "${CMAKE_BINARY_DIR}/source/*.cc")

if(CMAKE_Fortran_COMPILER)
  file(GLOB_RECURSE SOURCES_FORTAN "source/*.f90")
endif()

set(SOURCES ${SOURCES_CXX} ${SOURCES_FORTAN})


# Provide "indent" target for indenting all headers and source files
ADD_CUSTOM_TARGET(indent
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMAND ./doc/indent
  COMMENT "Indenting all the World Builder header and source files..."
  )

if (NOT MSVC AND NOT APPLE)
   #TODO: if other compiles need to be threaded, see https://computing.llnl.gov/tutorials/pthreads/#Compiling to add the correct cases.
   if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER MATCHES "[Cc]lang")
       SET(CMAKE_CXX_FLAGS "-std=c++11 -pthread -pedantic -fPIC -Wall -Wextra -Wpointer-arith -Wwrite-strings -Wsynth -Wsign-compare -Wswitch -Woverloaded-virtual -Wno-literal-range -Wno-parentheses -Wno-unused-local-typedefs -Wcast-qual -fstrict-aliasing -Wuninitialized -Werror=uninitialized")
   elseif(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
       SET(CMAKE_CXX_FLAGS "-std=c++11 -pthread -pedantic -fPIC -Wall -Wextra -Wpointer-arith -Wwrite-strings -Wsign-compare -Woverloaded-virtual -Wno-parentheses -Wcast-qual -fstrict-aliasing -Wmaybe-uninitialized -Werror=maybe-uninitialized")
   else()
       SET(CMAKE_CXX_FLAGS "-std=c++11 -pthread -pedantic -fPIC -Wall -Wextra -Wpointer-arith -Wwrite-strings -Wsynth -Wsign-compare -Woverloaded-virtual -Wno-placement-new -Wno-literal-suffix -Wno-parentheses -Wno-unused-local-typedefs -Wcast-qual -fstrict-aliasing -Wmaybe-uninitialized -Werror=maybe-uninitialized")
   endif()
   SET(CMAKE_EXE_LINKER_FLAGS "-lstdc++")
elseif(APPLE)
   SET(CMAKE_CXX_FLAGS "-std=c++11 -pedantic -fPIC -Wall -Wextra -Wpointer-arith -Wwrite-strings -Wsynth -Wsign-compare -Woverloaded-virtual -Wno-literal-range -Wno-parentheses -Wno-unused-local-typedefs -Wcast-qual -fstrict-aliasing -stdlib=libc++ -Wuninitialized -Werror=uninitialized")
   SET(CMAKE_EXE_LINKER_FLAGS "-lc++") #-stdlib=libc++")
else()
    SET(CMAKE_CXX_FLAGS "/W3 /EHsc -std=c++11")
     #-stdlib=libc++")
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
  SET(CMAKE_Fortran_FLAGS_COVERAGE --coverage)
  SET(CMAKE_EXE_LINKER_FLAGS_COVERAGE "${CMAKE_EXE_LINKER_FLAGS_DEBUG} --coverage -fprofile-arcs -ftest-coverage ")
  SET(CMAKE_SHARED_LINKER_FLAGS_COVERAGE "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} --coverage -fprofile-arcs -ftest-coverage ")

MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_COVERAGE
    CMAKE_C_FLAGS_COVERAGE
    CMAKE_Fortran_FLAGS_COVERAGE
    CMAKE_EXE_LINKER_FLAGS_COVERAGE
    CMAKE_SHARED_LINKER_FLAGS_COVERAGE )
ENDIF()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
  string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
  set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
  set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
  set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

if(MAKE_PYTHON_WRAPPER)
  set(CMAKE_SWIG_OUTDIR ${CMAKE_BINARY_DIR}/lib)
endif()
if(CMAKE_Fortran_COMPILER)
  set(CMAKE_Fortran_MODULE_DIRECTORY ${CMAKE_BINARY_DIR}/mod)
endif()

add_library(WorldBuilder ${SOURCES})

if(MSVC)
  SET(CMAKE_EXE_LINKER_FLAGS "/WHOLEARCHIVE:${CMAKE_CURRENT_BINARY_DIR}/lib/WorldBuilder.lib")
endif()

add_executable(WorldBuilderApp "${CMAKE_CURRENT_SOURCE_DIR}/app/main.cc")
add_executable(WorldBuilderVisualization "${CMAKE_CURRENT_SOURCE_DIR}/visualization/main.cc")

# Make sure that the whole library is loaded, so the registration is done correctly.
if(NOT APPLE AND NOT MSVC)
  SET(GWB_LIBRARY_WHOLE -Wl,--whole-archive WorldBuilder -Wl,--no-whole-archive)
elseif(MSVC)
  SET(GWB_LIBRARY_WHOLE WorldBuilder)
else()
  SET(GWB_LIBRARY_WHOLE -Wl,-force_load WorldBuilder)
endif()
target_link_libraries (WorldBuilderApp ${GWB_LIBRARY_WHOLE})
if(NOT APPLE)
  target_link_libraries (WorldBuilderVisualization ${GWB_LIBRARY_WHOLE}) 
else()
  target_link_libraries (WorldBuilderVisualization ${GWB_LIBRARY_WHOLE}) 
endif()

if(MAKE_PYTHON_WRAPPER)

  if(POLICY CMP0078)
    SET(TARGET gwb)
  else()
    SET(TARGET _gwb)
  endif()

  # Add swig module
  SET(CMAKE_SWIG_FLAGS "-py3")
  SET_SOURCE_FILES_PROPERTIES(SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/source/wrapper_cpp.i PROPERTIES CPLUSPLUS ON)
  if(${CMAKE_VERSION} VERSION_LESS "3.8.0") 
    swig_add_module(gwb python ${CMAKE_CURRENT_SOURCE_DIR}/source/wrapper_cpp.i)
  else()
    swig_add_library(gwb LANGUAGE python SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/source/wrapper_cpp.i)
  endif()

  # see https://github.com/swig/swig/issues/1259 for more info.
  if(SWIG_VERSION VERSION_LESS "4.0.1" AND NOT MSVC)
    set_target_properties(${TARGET} PROPERTIES COMPILE_FLAGS "-Wno-cast-qual")
  endif()

  # visual studio has a problem with finding the debug version of python (pythonxx_d.dll),
  # so force release usage.
  if(WIN32 AND MSVC)
    SET(CMAKE_SWIG_FLAGS "-D_SWIG_WIN32 /WHOLEARCHIVE:${CMAKE_CURRENT_BINARY_DIR}/lib/WorldBuilder.lib")
    swig_link_libraries(gwb ${GWB_LIBRARY_WHOLE} ${Python3_LIBRARY_RELEASE})

    SET_TARGET_PROPERTIES(${TARGET} PROPERTIES LINK_FLAGS "/WHOLEARCHIVE:WorldBuilder")

  else()
    swig_link_libraries(gwb ${GWB_LIBRARY_WHOLE} ${Python3_LIBRARIES})
  endif()
  
  set_target_properties(${TARGET} PROPERTIES PREFIX "_" OUTPUT_NAME "gwb")

  # Files to install with Python
  if(WIN32)
    set(PYTHON_INSTALL_FILES
            ${CMAKE_CURRENT_BINARY_DIR}/lib/gwb.py
            ${CMAKE_CURRENT_BINARY_DIR}/lib/_gwb.pyd)
  else()
    set(PYTHON_INSTALL_FILES
            ${CMAKE_CURRENT_BINARY_DIR}/lib/gwb.py
            ${CMAKE_CURRENT_BINARY_DIR}/lib/_gwb.so)
  endif()
  # Configure setup.py and copy to output directory
  set(SETUP_PY_IN ${CMAKE_CURRENT_SOURCE_DIR}/cmake/setup.py.in)
  set(SETUP_PY_OUT ${CMAKE_CURRENT_BINARY_DIR}/lib/setup.py)
  configure_file(${SETUP_PY_IN} ${SETUP_PY_OUT})

  # Install target to call setup.py
  add_custom_target(install-python
          DEPENDS _gwb
          COMMAND ${Python3_EXECUTABLE} ${SETUP_PY_OUT} install)
endif()

# binary:
install(CODE "message(Installing...)")
install (TARGETS WorldBuilder EXPORT WorldBuilder  DESTINATION bin)
install (EXPORT WorldBuilder  DESTINATION bin)

if(CMAKE_Fortran_COMPILER)
  install (FILES ${CMAKE_CURRENT_BINARY_DIR}/mod/worldbuilder.mod DESTINATION include)
endif()

if(MAKE_PYTHON_WRAPPER)
  install(CODE "execute_process(COMMAND ${Python3_EXECUTABLE} ${SETUP_PY_OUT} install)")
endif()

# headers:
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
  DESTINATION include
  COMPONENT includes
  FILES_MATCHING PATTERN "*.h")

enable_testing()
add_subdirectory(tests)
