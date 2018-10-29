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

#include <boost/algorithm/string.hpp>

#include <world_builder/features/interface.h>
#include <world_builder/features/continental_plate.h>
#include <world_builder/features/fault.h>
#include <world_builder/features/oceanic_plate.h>
#include <world_builder/features/subducting_plate.h>
#include <world_builder/features/mantle_layer.h>
#include <world_builder/assert.h>
#include <world_builder/utilities.h>


namespace WorldBuilder
{
  using namespace Utilities;

  namespace Features
  {
    Interface::Interface()
    {}

    Interface::~Interface ()
    {}

    void
    Interface::declare_interface_entries(Parameters &prm, const CoordinateSystem coordinate_system)
    {
      // Get the name
      prm.load_entry("name", true, Types::String("","The name which the user has given to the feature."));
      name = prm.get_string("name");

      // Get the list of coordinates where the subduction zone is located
      bool set = prm.load_entry("coordinates", true, Types::Array(
                                  Types::Point<2>(Point<2>(0,0,coordinate_system),"desciption point cross section"),
                                  "An array of Points representing an array of coordinates where the feature is located."));

      WBAssertThrow(set == true, "A list of coordinates is required for every feature.");

      std::vector<Types::Point<2>> typed_coordinates =  prm.get_array<Types::Point<2> >("coordinates");

      original_number_of_coordinates = typed_coordinates.size();
      coordinates.resize(original_number_of_coordinates, Point<2>(coordinate_system));
      for (unsigned int i = 0; i < original_number_of_coordinates; ++i)
        {
          coordinates[i] = typed_coordinates[i].value *
                           (coordinate_system == CoordinateSystem::spherical ? const_pi / 180.0 : 1.0);
        }

      // perform interpolation if required.
      prm.leave_subsection();
      prm.leave_subsection();
      std::string interpolation = prm.get_string("interpolation");

      // the one_dimensional_coordinates is always needed, so fill it.
      std::vector<double> one_dimensional_coordinates_local(original_number_of_coordinates,0.0);
      for (unsigned int j=0; j<original_number_of_coordinates; ++j)
        {
          one_dimensional_coordinates_local[j] = j;
        }

      if (interpolation != "none")
        {
          WBAssertThrow(interpolation == "linear" || interpolation == "monotone spline",
                        "For interpolation, linear and monotone spline are the onlyl allowed values.");

          double minimum_points_per_distance = prm.get_double("minimum points per distance") *
                                               (coordinate_system == CoordinateSystem::spherical ? const_pi / 180.0 : 1.0);

          if (minimum_points_per_distance > 0)
            {
              std::vector<double> x_list(original_number_of_coordinates,0.0);
              std::vector<double> y_list(original_number_of_coordinates,0.0);
              std::vector<Point<2> > coordinate_list_local = coordinates;
              for (unsigned int j=0; j<original_number_of_coordinates; ++j)
                {
                  x_list[j] = coordinates[j][0];
                  y_list[j] = coordinates[j][1];
                }

              Utilities::interpolation x_spline, y_spline;
              x_spline.set_points(one_dimensional_coordinates_local, x_list, interpolation == "linear" ? false : true);
              y_spline.set_points(one_dimensional_coordinates_local, y_list, interpolation == "linear" ? false : true);

              unsigned int additional_parts = 0;
              for (unsigned int i_plane=0; i_plane<original_number_of_coordinates-1; ++i_plane)
                {
                  const Point<2> P1 (x_spline(one_dimensional_coordinates_local[i_plane + additional_parts]),
                                     y_spline(one_dimensional_coordinates_local[i_plane + additional_parts]),
                                     coordinate_system);

                  const Point<2> P2 (x_spline(one_dimensional_coordinates_local[i_plane + additional_parts + 1]),
                                     y_spline(one_dimensional_coordinates_local[i_plane  + additional_parts+ 1]),
                                     coordinate_system);

                  const double length = (P1 - P2).norm();
                  const int parts = std::ceil(length / minimum_points_per_distance);
                  for (int j = 1; j < parts; j++)
                    {
                      const double x_position3 = i_plane+(double(j)/double(parts));
                      const Point<2> P3(x_spline(x_position3), y_spline(x_position3), coordinate_system);
                      one_dimensional_coordinates_local.insert(one_dimensional_coordinates_local.begin() + additional_parts + i_plane + 1, x_position3);
                      coordinate_list_local.insert(coordinate_list_local.begin() + additional_parts + i_plane + 1, P3);
                      additional_parts++;
                    }
                }
              coordinates = coordinate_list_local;
            }
        }
      one_dimensional_coordinates = one_dimensional_coordinates_local;
      prm.enter_subsection("objects");
      prm.enter_subsection(name);
    }

    void
    Interface::registerType(
      const std::string &name, ObjectFactory *factory)
    {
      get_factory_map()[name] = factory;
    }

    std::unique_ptr<Interface>
    Interface::create(const std::string &name, WorldBuilder::World *world)
    {
      std::string lower_case_name;
      std::transform(name.begin(),
                     name.end(),
                     std::back_inserter(lower_case_name),
                     ::tolower);;

      // Have a nice assert message to check whether a plugin exists in the case
      // of a debug compilation.
      WBAssertThrow(get_factory_map().find(lower_case_name) != get_factory_map().end(),
                    "Internal error: Plugin with name '" << lower_case_name << "' is not found. "
                    "The size of factories is " << get_factory_map().size() << ".");

      // Using at() because the [] will just insert values
      // which is undesirable in this case. An exception is
      // thrown when the name is not present.
      return get_factory_map().at(lower_case_name)->create(world);
    }

  }
}

