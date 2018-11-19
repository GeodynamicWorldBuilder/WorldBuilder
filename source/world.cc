/*
  Copyright (C) 2018 by the authors of the World Builder code.

  This file is part of the World Builder.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation, either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <sstream>

#include "rapidjson/pointer.h"

#include <world_builder/config.h>
#include <world_builder/world.h>
#include <world_builder/utilities.h>
#include <world_builder/assert.h>
#include <world_builder/point.h>
#include <world_builder/nan.h>
#include <world_builder/parameters.h>
#include <world_builder/types/interface.h>
#include <world_builder/types/coordinate_system.h>

#include <world_builder/types/array.h>
#include <world_builder/types/double.h>
#include <world_builder/types/list.h>
#include <world_builder/types/string.h>
#include <world_builder/types/point.h>
#include "../include/world_builder/types/plugin_system.h"
#include "../include/world_builder/types/plugin_system.h"


namespace WorldBuilder
{


  using namespace Utilities;

  World::World(std::string filename)
    :
    parameters(*this),
    dim(NaN::ISNAN)
  {
    std::cout << "flag 1 " << std::endl;
    this->declare_entries(parameters);
    std::cout << "flag 2 " << std::endl;
    parameters.initialize(filename);
    std::cout << "flag 3 " << std::endl;
    this->declare_and_parse(parameters);
    std::cout << "flag 4 " << std::endl;
  }

  World::~World()
  {}

  void World::declare_entries(Parameters &prm)
  {
    std::cout << "declare entry: " << std::endl;
    prm.declare_entry("version","",true,Types::String(),"The major and minor version number for which the input file was written.");
    prm.declare_entry("cross section","",true,Types::Array(Types::Point<2>(),2,2),"This is an array of two points along where the cross section is taken");

    prm.declare_entry("features","",true,Types::PluginSystem(Features::Interface::declare_entries),"A list of features.");


  }


  void World::declare_and_parse(Parameters &prm)
  {
    using namespace rapidjson;
    Document &doc = prm.parameters;


    /**
     * First load the major version number in the file and check the major
     * version number of the program.
     */
    WBAssertThrow(Pointer("/version").Get(doc) != NULL,
                  "An entry called version is required in a World Builder "
                  "Parameter file.");
    //WBAssertThrow(doc.HasMember("version"),
//            "An entry called version is required in a World Builder "
//      "Parameter file.");
    //prm.load_entry("version", Types::String("","The major version number for which the input file was written."));

    WBAssertThrow(Version::MAJOR == "0"
                  && prm.load_entry("version") == Version::MAJOR + "." + Version::MINOR
                  || Version::MAJOR != "0"
                  && prm.load_entry("version") == Version::MAJOR,
                  "The major and minor version combination (for major version 0) or the major "
                  "version (for major versions after 0) for which is input file was written "
                  "is not the same as the version of the World Builder you are running. This means "
                  "That there may have been incompatible changes made between the versions. "
                  "Verify those changes and wheter they affect your model. If this is not "
                  "the case, adjust the version number in the input file.");
//    WBAssertThrow(Pointer("/version").Get(doc)->GetUint() == Utilities::string_to_unsigned_int(Version::MAJOR),

    /**
     * Seconly load the coordinate system parameters.
     */
    prm.load_entry("coordinate system", false, Types::CoordinateSystem("cartesian","This determines the coordinate system"));

    const CoordinateSystem coordinate_system = prm.coordinate_system->natural_coordinate_system();

    /**
     * Temperature parameters.
     */
    prm.load_entry("potential mantle temperature", false,
                   Types::Double(1600,"The potential temperature of the mantle at the surface in Kelvin."));
    prm.load_entry("surface temperature", false,
                   Types::Double(293.15,"The temperature at the surface in Kelvin."));
    prm.load_entry("force surface temperature", false,
                   Types::String("false","Force the provided surface temperature to be set at the surface"));
    prm.load_entry("thermal expansion coefficient", false,
                   Types::Double(3.5e-5,"The thermal expansion coefficient in $K^{-1}$."));
    prm.load_entry("specific heat", false, Types::Double(1250,"The specific heat in $J kg^{-1} K^{-1}."));
    prm.load_entry("thermal diffusivity", false, Types::Double(0.804e-6,"Set the thermal diffusivity in $m^{2} s^{-1}$."));

    /**
     * Model rotation parameters.
     */
    prm.load_entry("surface rotation angle", false,
                   Types::Double(0,"The angle with which the model should be rotated around the surface rotation point."));
    prm.load_entry("surface rotation point", false,
                   Types::Point<2>(Point<2>(0,0, coordinate_system), "The point where should be rotated around."));

    /**
     * Model descretisation parameters.
     */
    prm.load_entry("minimum points per distance", false,
                   Types::Double(std::numeric_limits<double>::max(),"This enforces that there is at least every distance interval"
                                 "(in degree for spherical coordinates or meter in cartesian coordinates) a point."));
    prm.load_entry("interpolation", false,
                   Types::String("none", "What type of interpolation should be used to enforce the minimum points per "
                                 "distance parameter. Options are none, linear and monotone spline."));


    bool set = prm.load_entry("cross section", false,
                              Types::Array(
                                Types::Point<2>(Point<2>(0,0, coordinate_system),"A point in the cross section."),
                                "This is an array of two points along where the cross section is taken"));

    if (set)
      {
        dim = 2;
        std::vector<Types::Point<2> > cross_section = prm.get_array<Types::Point<2> >("cross section");

        WBAssertThrow(cross_section.size() == 2, "The cross section should contain two points, but it contains "
                      << cross_section.size() << " points.");

        /**
         * pre-compute stuff for the cross section
         */
        Point<2> surface_coord_conversions = (cross_section[0]-cross_section[1]) * (coordinate_system == spherical ? const_pi / 180.0 : 1.0);
        surface_coord_conversions *= -1/(surface_coord_conversions.norm());
        prm.set_entry("surface coordinate conversions",
                      Types::Point<2>(surface_coord_conversions, surface_coord_conversions, "An internal value which is precomputed."));
      }
    else
      {
        dim = 3;
      }

    prm.load_entry("features", true, Types::List(
                     Types::PluginSystem("These are the features"), "A list of features."));

  }

  double
  World::temperature(const std::array<double,2> &point,
                     const double depth,
                     const double gravity_norm) const
  {
    // turn it into a 3d coordinate and call the 3d temperature function
    WBAssertThrow(dim == 2, "This function can only be called when the cross section "
                  "variable in the world builder file has been set. Dim is "
                  << dim << ".");

    const CoordinateSystem coordinate_system = this->parameters.coordinate_system->natural_coordinate_system();

    Point<2> point_natural(point[0], point[1],coordinate_system);
    if (coordinate_system == spherical)
      {
        point_natural[1] = std::sqrt(point[0]*point[0]+point[1]*point[1]);
        point_natural[0] = std::atan2(point[1],point[0]);
      }
    // Todo: merge this into one line
    const Types::Array &cross_section_natural = parameters.get_array("cross section");

    WBAssert(cross_section_natural.inner_type_index.size() == 2,
             "Internal error: Cross section natural should contain two points, but it contains "
             << cross_section_natural.inner_type_index.size() <<  " points.");

    std::vector<Point<2> > cross_section;
    for (unsigned int i = 0; i < cross_section_natural.inner_type_index.size(); ++i)
      cross_section.push_back(parameters.vector_point_2d[cross_section_natural.inner_type_index[i]]  * (coordinate_system == spherical ? const_pi / 180.0 : 1.0));

    WBAssert(cross_section.size() == 2,
             "Internal error: Cross section should contain two points, but it contains "
             << cross_section.size() <<  " points.");

    const WorldBuilder::Point<2> &surface_coord_conversions = this->parameters.get_point<2>("surface coordinate conversions");

    Point<3> coord_3d(coordinate_system);
    if (coordinate_system == spherical)
      {
        coord_3d[0] = point_natural[1];
        coord_3d[1] = cross_section[0][0] + point_natural[0] * surface_coord_conversions[0];
        coord_3d[2] = cross_section[0][1] + point_natural[0] * surface_coord_conversions[1];
      }
    else
      {
        coord_3d[0] = cross_section[0][0] + point_natural[0] * surface_coord_conversions[0];
        coord_3d[1] = cross_section[0][1] + point_natural[0] * surface_coord_conversions[1];
        coord_3d[2] = point_natural[1];
      }


    std::array<double, 3> point_3d_cartesian = this->parameters.coordinate_system->natural_to_cartesian_coordinates(coord_3d.get_array());

    return temperature(point_3d_cartesian, depth, gravity_norm);
  }

  double
  World::temperature(const std::array<double,3> &point_,
                     const double depth,
                     const double gravity_norm) const
  {
    // We receive the cartesian points from the user.
    Point<3> point(point_,cartesian);

    if (std::fabs(depth) < 2.0 * std::numeric_limits<double>::epsilon() && this->parameters.get_string("force surface temperature") == "true")
      return this->parameters.get_double("surface temperature");

    double temperature = this->parameters.get_double("potential mantle temperature") *
                         std::exp(((this->parameters.get_double("thermal expansion coefficient") * gravity_norm) /
                                   this->parameters.get_double("specific heat")) * depth);


    for (std::vector<std::unique_ptr<Features::Interface> >::const_iterator it = parameters.features.begin(); it != parameters.features.end(); ++it)
      {
        temperature = (*it)->temperature(point,depth,gravity_norm,temperature);
      }

    return temperature;
  }

  double
  World::composition(const std::array<double,2> &point,
                     const double depth,
                     const unsigned int composition_number) const
  {
    // turn it into a 3d coordinate and call the 3d temperature function
    WBAssertThrow(dim == 2, "This function can only be called when the cross section "
                  "variable in the world builder file has been set. Dim is "
                  << dim << ".");

    const CoordinateSystem coordinate_system = this->parameters.coordinate_system->natural_coordinate_system();

    Point<2> point_natural(point[0], point[1],coordinate_system);
    if (coordinate_system == spherical)
      {
        point_natural[1] = std::sqrt(point[0]*point[0]+point[1]*point[1]);
        point_natural[0] = std::atan2(point[1],point[0]);
      }

    // Todo: merge this into one line
    const Types::Array &cross_section_natural = this->parameters.get_array("cross section");
    std::vector<Point<2> > cross_section;
    for (unsigned int i = 0; i < cross_section_natural.inner_type_index.size(); ++i)
      cross_section.push_back(this->parameters.vector_point_2d[cross_section_natural.inner_type_index[i]] * (coordinate_system == spherical ? const_pi / 180.0 : 1.0));

    WBAssert(cross_section.size() == 2, "Internal error: Cross section should contain two points, but it contains "
             << cross_section.size() <<  " points.");

    const WorldBuilder::Point<2> &surface_coord_conversions = this->parameters.get_point<2>("surface coordinate conversions");

    Point<3> coord_3d(coordinate_system);
    if (coordinate_system == spherical)
      {
        coord_3d[0] = point_natural[1];
        coord_3d[1] = cross_section[0][0] + point_natural[0] * surface_coord_conversions[0];
        coord_3d[2] = cross_section[0][1] + point_natural[0] * surface_coord_conversions[1];
      }
    else
      {
        coord_3d[0] = cross_section[0][0] + point_natural[0] * surface_coord_conversions[0];
        coord_3d[1] = cross_section[0][1] + point_natural[0] * surface_coord_conversions[1];
        coord_3d[2] = point_natural[1];
      }

    std::array<double, 3> point_3d_cartesian = this->parameters.coordinate_system->natural_to_cartesian_coordinates(coord_3d.get_array());

    return composition(point_3d_cartesian, depth, composition_number);
  }

  double
  World::composition(const std::array<double,3> &point_,
                     const double depth,
                     const unsigned int composition_number) const
  {
    // We receive the cartesian points from the user.
    Point<3> point(point_,cartesian);
    double composition = 0;
    for (std::vector<std::unique_ptr<Features::Interface> >::const_iterator it = parameters.features.begin(); it != parameters.features.end(); ++it)
      {
        composition = (*it)->composition(point,depth,composition_number, composition);
      }

    return composition;
  }

}

