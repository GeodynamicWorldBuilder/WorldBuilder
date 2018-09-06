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


#include <world_builder/world.h>
#include <world_builder/utilities.h>
#include <world_builder/assert.h>
#include <world_builder/point.h>
#include <world_builder/nan.h>
#include <world_builder/parameters.h>
#include <world_builder/types/interface.h>
#include <world_builder/types/feature.h>
#include <world_builder/types/coordinate_system.h>


namespace WorldBuilder
{

  using namespace Utilities;

  World::World(std::string filename)
    :
    parameters(filename,*this),
    dim(NaN::ISNAN)
  {
    this->declare_and_parse(parameters);
  }

  World::~World()
  {}

  void World::declare_and_parse(Parameters &prm)
  {
    /**
     * First load the coordinate system parameters.
     */
    prm.load_entry("Coordinate system", false, Types::CoordinateSystem("cartesian","This determines the coordinate system"));

    const CoordinateSystem coordinate_system = prm.coordinate_system->natural_coordinate_system();

    /**
     * Temperature parameters.
     */
    prm.load_entry("potential mantle temperature", false,
                   Types::Double(1600,"The potential temperature of the mantle at the surface in Kelvin"));
    prm.load_entry("surface temperature", false,
                           Types::Double(293,"The temperature at the surface in Kelvin"));
    prm.load_entry("Thermal expansion coefficient alpha", false,
                   Types::Double(3.5e-5,"The thermal expansion coefficient alpha. TODO: expand add units"));
    prm.load_entry("specific heat Cp", false, Types::Double(1250,"The specific heat Cp.  TODO: expand and add units"));
    prm.load_entry("thermal diffusivity", false, Types::Double(0.804e-6,"Set the thermal diffusivity. TODO: expand and add units "));

    /**
     * Model rotation parameters.
     */
    prm.load_entry("Surface rotation angle", false,
                   Types::Double(0,"The angle with which the model should be rotated around the Surface rotation point."));
    prm.load_entry("Surface rotation point", false,
                   Types::Point<2>(Point<2>(0,0, coordinate_system), "The point where should be rotated around."));



    bool set = prm.load_entry("Cross section", false,
                              Types::Array(
                                Types::Point<2>(Point<2>(0,0, coordinate_system),"A point in the cross section."),
                                "This is an array of two points along where the cross section is taken"));

    if (set)
      {
        dim = 2;
        std::vector<const Types::Point<2>* > cross_section = prm.get_array<const Types::Point<2> >("Cross section");

        WBAssertThrow(cross_section.size() == 2, "The cross section should contain two points, but it contains "
                      << cross_section.size() << " points.");

        /**
         * pre-compute stuff for the cross section
         */
        Point<2> surface_coord_conversions = (*cross_section[0]-*cross_section[1]) * (coordinate_system == spherical ? M_PI / 180.0 : 1.0);
        surface_coord_conversions *= -1/(surface_coord_conversions.norm());
        prm.set_entry("Surface coordinate conversions",
                      Types::Point<2>(surface_coord_conversions, surface_coord_conversions, "An internal value which is precomputed."));
      }
    else
      {
        dim = 3;
      }

    prm.load_entry("Surface objects", true, Types::List(
                     Types::Feature("These are the features"), "A list of features."));

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

    // Todo: merge this into one line
    const Types::Array &cross_section_natural = parameters.get_array("Cross section");

    WBAssert(cross_section_natural.inner_type_index.size() == 2,
             "Internal error: Cross section natural should contain two points, but it contains "
             << cross_section_natural.inner_type_index.size() <<  " points.");

    std::vector<Types::Point<2> > cross_section;
    for (unsigned int i = 0; i < cross_section_natural.inner_type_index.size(); ++i)
      cross_section.push_back(parameters.vector_point_2d[cross_section_natural.inner_type_index[i]]);

    WBAssert(cross_section.size() == 2,
             "Internal error: Cross section should contain two points, but it contains "
             << cross_section.size() <<  " points.");

    const WorldBuilder::Point<2> &surface_coord_conversions = this->parameters.get_point<2>("Surface coordinate conversions");

    Point<3> coord_3d(cross_section[0][0] + point[0] * surface_coord_conversions[0],
                      cross_section[0][1] + point[0] * surface_coord_conversions[1],
                      point[1],
                      parameters.coordinate_system->natural_coordinate_system());

    return temperature(coord_3d.get_array(), depth, gravity_norm);
  }

  double
  World::temperature(const std::array<double,3> &point_,
                     const double depth,
                     const double gravity_norm) const
  {
	  // We receive the cartesian points from the user.
    Point<3> point(point_,cartesian);

    double temperature = this->parameters.get_double("potential mantle temperature") +
                         (((this->parameters.get_double("potential mantle temperature") * this->parameters.get_double("Thermal expansion coefficient alpha") * gravity_norm) /
                           this->parameters.get_double("specific heat Cp")) * 1000.0) * ((depth) / 1000.0);


    for (std::vector<std::unique_ptr<Features::Interface> >::const_iterator it = parameters.features.begin(); it != parameters.features.end(); ++it)
      {
        temperature = (*it)->temperature(point,depth,gravity_norm,temperature);
      }

    return temperature;
  }

  bool
  World::composition(const std::array<double,2> &point,
                     const double depth,
                     const unsigned int composition_number) const
  {
    // turn it into a 3d coordinate and call the 3d temperature function
    WBAssertThrow(dim == 2, "This function can only be called when the cross section "
                  "variable in the world builder file has been set. Dim is "
                  << dim << ".");

    // Todo: merge this into one line
    const Types::Array &cross_section_natural = this->parameters.get_array("Cross section");
    std::vector<Types::Point<2> > cross_section;
    for (unsigned int i = 0; i < cross_section_natural.inner_type_index.size(); ++i)
      cross_section.push_back(this->parameters.vector_point_2d[cross_section_natural.inner_type_index[i]]);

    WBAssert(cross_section.size() == 2, "Internal error: Cross section should contain two points, but it contains "
             << cross_section.size() <<  " points.");

    const WorldBuilder::Point<2> &surface_coord_conversions = this->parameters.get_point<2>("Surface coordinate conversions");

    Point<3> coord_3d(cross_section[0][0] + point[0] * surface_coord_conversions[0],
                      cross_section[0][1] + point[0] * surface_coord_conversions[1],
                      point[1],
                      parameters.coordinate_system->natural_coordinate_system());

    return composition(coord_3d.get_array(), depth, composition_number);
  }

  bool
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

  /*WorldBuilder::CoordinateSystems::Interface &
  World::get_coordinate_system() const
  {
    return *coordinate_system;
  }*/
}

