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
    dim(NaN::ISNAN),
    //surface_rotation_angle(0.0),
    // minimum_parts_per_distance_unit(NaN::ISNAN),
    //minimum_distance_points(NaN::ISNAN),
    //potential_mantle_temperature(1600),
    //thermal_expansion_coefficient_alpha(3.5e-5),
    parameters(filename,*this)
    //specific_heat_Cp(1250)
  {
    std::cout << "flag c1" << std::endl;
    this->declare_and_parse(parameters);
    std::cout << "flag c2" << std::endl;
  }

  World::~World()
  {
    //delete coordinate_system;

    /*for (unsigned int i = 0; i < features.size(); ++i)
      {
        delete features[i];
      }*/

  }

  void World::declare_and_parse(Parameters &parameters)
  {
    parameters.load_entry("Potential mantle temperature", false, Types::Double(1600,"test"));
    parameters.load_entry("Thermal expansion coefficient alpha", false, Types::Double(3.5e-5,"test"));
    parameters.load_entry("specific heat Cp", false, Types::Double(1250,"test"));
    parameters.load_entry("Surface rotation angle", false, Types::Double(0,"test"));

    /*parameters.enter_subsection("Coordinate system");
    {
      parameters.declare_entry("name", true, Types::String("lala", "description"));
      std::cout << parameters.vector_string[parameters.string_to_type_map["name"].second].value << std::endl;
    }
    parameters.leave_subsection();*/

    parameters.load_entry("Coordinate system", false, Types::CoordinateSystem("cartesian","description"));

    //parameters.declare_entry("Surface rotation point", true, Types::Array(Types::Double("0", "descp double"),"descp srp"));
    parameters.load_entry("Surface rotation point", true, Types::Point<2>(Point<2>(2,3), "descp double"));

    bool set = false;
    // TODO: Improve this.
    try
    {
    parameters.load_entry("Cross section", true,
                                     Types::Array(
                                       Types::Point<2>(Point<2>(0,0),"desciption point cross section"),
                                       "description points array"));
    }
    catch(...)
	{
    	set = false;
	}

    if (set)
      {
        dim = 2;

        std::cout << "dim =2" << std::endl;
        // Todo: check that there are exactly two points
        // Todo: merge this into one line
        const Types::Array &cross_section_natural = this->parameters.get_array("Cross section");
        std::vector<Types::Point<2> > cross_section;
        for (unsigned int i = 0; i < cross_section_natural.inner_type_index.size(); ++i)
          cross_section.push_back(this->parameters.vector_point_2d[cross_section_natural.inner_type_index[i]]);

        WBAssertThrow(cross_section.size() == 2, "The cross section should contain two points, but it contains "
                      << cross_section.size() << " points.");

        std::cout << "loaded the cross section." << std::endl;
        /**
         * pre-compute stuff for the cross section
         */

        Point<2> surface_coord_conversions = (cross_section[0]-cross_section[1]);
        surface_coord_conversions *= 1/(surface_coord_conversions.norm());
        parameters.set_entry("Surface coordinate conversions",
                             Types::Point<2>(surface_coord_conversions, surface_coord_conversions, "description"));

        //const Point<2> diff_points = cross_section[0]-cross_section[1];
        //const double one_over_cross_section_length = 1/(diff_points.norm());

        //std::vector<std::unique_ptr<Types::Double> > double_vector {std::make_shared<Types::Double>(diff_points[0] * one_over_cross_section_length, ""),
        //                                                          std::make_shared<Types::Double>(diff_points[1] * one_over_cross_section_length, "")};
        //parameters.set_entry("Surface coordinate conversions",
        //                    Types::Array(std::vector<std::unique_ptr<Types::Interface> >{double_vector.begin(), double_vector.end()}, "description"));
      }
    else
      {
        dim = 3;
      }

    parameters.load_entry("Surface objects", true, Types::List(
                            Types::Feature("These are the features"), "description of list"));


  }

  void
  World::read(ptree &tree)
  {
    /*//todo: wrap this into a convenient function
    boost::optional<ptree &> child;

    / **
     * get the cross section
     * /
    child = tree.get_child_optional("Cross section");
    if (child)
      {
        dim = 2;
        for (boost::property_tree::ptree::const_iterator it = child.get().begin(); it != child.get().end(); ++it)
          {
            std::vector<double> tmp;
            boost::optional<const ptree &> child2 = it->second.get_child("");
            WBAssertThrow (child, "Cross section: This should be a 2d array, but only one dimension found.");
            for (boost::property_tree::ptree::const_iterator it2 = child2.get().begin(); it2 != child2.get().end(); ++it2)
              {
                tmp.push_back(stod(it2->second.get<std::string>("")));
              }
            WBAssertThrow (tmp.size() == 2, "Cross section: These represent 2d coordinates, but there are " <<
                           tmp.size() <<
                           " coordinates specified.");

            std::array<double,2> tmp_array;
            std::copy(tmp.begin(), tmp.end(), tmp_array.begin());
            cross_section.push_back(tmp_array);
          }
        WBAssertThrow (cross_section.size() == 2, "Cross section: This feature requires at exactly 2 coordinates, but " <<
                       cross_section.size() <<
                       " where provided.");

        / **
         * pre-compute stuff for the cross section
         * /
        const Point<2> diff_points = Point<2>(cross_section[0])-Point<2>(cross_section[1]);
        const double one_over_cross_section_length = 1/(diff_points.norm());
        surface_coord_conversions = {diff_points[0] *one_over_cross_section_length,diff_points[1] *one_over_cross_section_length};
      }
    else
      {
        dim = 3;
      }


    // Get the rotation angle
    surface_rotation_angle = string_to_double(get_from_ptree(tree, "", "Surface rotation angle"));

    / **
     * Get the point to rotate about when the rotation angle is not zero.
     * /
    if (std::abs(surface_rotation_angle) < std::numeric_limits<double>::epsilon())
      {
        child = tree.get_child("Surface rotation point");
        WBAssertThrow (child, "Entry undeclared:  Surface rotation point. "
                       "Need a surface rotation point when rotation the surface.");
        for (boost::property_tree::ptree::const_iterator it = child.get().begin(); it != child.get().end(); ++it)
          {
            surface_rotation_point.push_back(Utilities::string_to_double(it->second.get<std::string>("")));
          }
        WBAssertThrow (surface_rotation_point.size() == 2, "Only 2d coordinates allowed for Surface rotation point.");
      }

    // Get variables needed for the interpolation (todo: not yet used).
    minimum_parts_per_distance_unit = string_to_unsigned_int(get_from_ptree(tree, "", "Minimum parts per distance unit"));
    minimum_distance_points = string_to_double(get_from_ptree(tree, "", "Minimum distance points"));

    // Now reading the main paramters from the file
    child = tree.get_child("Surface objects");
    WBAssertThrow (child, "Entry undeclared:  Surface rotation point");
    for (boost::property_tree::ptree::const_iterator it = child.get().begin(); it != child.get().end(); ++it)
      {
        features.push_back(Features::create_feature(it->first,*this));
        std::string path = "Surface objects" + World::path_seperator + it->first;
        features.back()->read(it->second,path);
      }


    child = tree.get_child_optional("Coordinate system");
    if (child)
      {
        const boost::optional<std::string> value = child.get().get_optional<std::string>("name");

        WBAssertThrow (value, "Entry undeclared:  Coordinate system.name");
        coordinate_system = CoordinateSystems::create_coordinate_system(value.get());
      }
    else
      {
        coordinate_system = CoordinateSystems::create_coordinate_system("cartesian");
      }

    */

  }

  double
  World::temperature(const std::array<double,2> &point, const double depth, const double gravity_norm) const
  {
    // turn it into a 3d coordinate and call the 3d temperature function
    WBAssertThrow(dim == 2, "This function can only be called when the cross section "
                  "variable in the world builder file has been set. Dim is "
                  << dim << ".");

    // Todo: merge this into one line
    const Types::Array &cross_section_natural = parameters.get_array("Cross section");

    WBAssert(cross_section_natural.inner_type_index.size() == 2, "Internal error: Cross section natural should contain two points, but it contains " << cross_section_natural.inner_type_index.size() <<  " points.");

    std::vector<Types::Point<2> > cross_section;
    for (unsigned int i = 0; i < cross_section_natural.inner_type_index.size(); ++i)
      cross_section.push_back(parameters.vector_point_2d[cross_section_natural.inner_type_index[i]]);

    WBAssert(cross_section.size() == 2, "Internal error: Cross section should contain two points, but it contains " << cross_section.size() <<  " points.");

    const WorldBuilder::Point<2> &surface_coord_conversions = this->parameters.get_point<2>("Surface coordinate conversions");

    Point<3> coord_3d(cross_section[0][0] + point[0] * surface_coord_conversions[0],
                      cross_section[0][1] + point[0] * surface_coord_conversions[1],
                      point[1]);

    return temperature(coord_3d.get_array(), depth, gravity_norm);
  }

  double
  World::temperature(const std::array<double,3> &point_, const double depth, const double gravity_norm) const
  {
    Point<3> point(point_);

    double temperature = this->parameters.get_double("Potential mantle temperature") +
                         (((this->parameters.get_double("Potential mantle temperature") * this->parameters.get_double("Thermal expansion coefficient alpha") * gravity_norm) /
                           this->parameters.get_double("specific heat Cp")) * 1000.0) * ((depth) / 1000.0);


    for (std::vector<std::unique_ptr<Features::Interface> >::const_iterator it = parameters.features.begin(); it != parameters.features.end(); ++it)
      {
        temperature = (*it)->temperature(point,depth,gravity_norm,temperature);
      }

    return temperature;
  }

  bool
  World::composition(const std::array<double,2> &point, const double depth, const unsigned int composition_number) const
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

    WBAssert(cross_section.size() == 2, "Internal error: Cross section should contain two points, but it contains " << cross_section.size() <<  " points.");

    const WorldBuilder::Point<2> &surface_coord_conversions = this->parameters.get_point<2>("Surface coordinate conversions");

    Point<3> coord_3d(cross_section[0][0] + point[0] * surface_coord_conversions[0],
                      cross_section[0][1] + point[0] * surface_coord_conversions[1],
                      point[1]);

    return composition(coord_3d.get_array(), depth, composition_number);
  }

  bool
  World::composition(const std::array<double,3> &point_, const double depth, const unsigned int composition_number) const
  {
    Point<3> point(point_);
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

