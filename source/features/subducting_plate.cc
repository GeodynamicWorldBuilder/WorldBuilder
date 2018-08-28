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

#include <world_builder/features/subducting_plate.h>
#include <world_builder/utilities.h>
#include <world_builder/assert.h>
#include <world_builder/nan.h>
#include <world_builder/parameters.h>


namespace WorldBuilder
{
  using namespace Utilities;

  namespace Features
  {
    SubductingPlate::SubductingPlate(WorldBuilder::World *world_)
      :
      reference_point(0,0,cartesian)
    {
      this->world = world_;
      this->name = "subducting plate";
    }

    SubductingPlate::~SubductingPlate()
    { }


    void
    SubductingPlate::decare_entries()
    {
      Parameters &prm = this->world->parameters;

      const CoordinateSystem coordinate_system = prm.coordinate_system->natural_coordinate_system();

      // Get the name
      prm.load_entry("name", true, Types::String("","The name which the user has given to the feature."));
      name = prm.get_string("name");

      // Get the list of coordinates where the subduction zone is located
      bool set = prm.load_entry("coordinates", true, Types::Array(
                                  Types::Point<2>(Point<2>(0,0,coordinate_system),"desciption point cross section"),
                                  "An array of Points representing an array of coordinates where the feature is located."));

      WBAssertThrow(set == true, "A list of coordinates is required for every feature.");

      std::vector<const Types::Point<2>* > typed_coordinates =  prm.get_array<const Types::Point<2> >("coordinates");

      coordinates.resize(typed_coordinates.size(), Point<2>(coordinate_system));
      for (unsigned int i = 0; i < typed_coordinates.size(); ++i)
        {
          coordinates[i] = typed_coordinates[i]->value *
                           (coordinate_system == CoordinateSystem::spherical ? M_PI / 180.0 : 1.0);
        }

      // Get the reference point
      prm.load_entry("reference point", true, Types::Point<2>(Point<2>(0,0,coordinate_system),
                                                              "A point which the plate subducts towards. When a coordinates of the subducting "
                                                              "plate form a line, it is undefined in what direction the plate should subduct "
                                                              "along that line. Through giving a point to which the plate should subduct "
                                                              "solves this problem."));

      reference_point = prm.get_point<2>("reference point");


      prm.load_entry("starting depth", false, Types::Double(0, "The depth below the surface at which this plate starts."));

      starting_depth = prm.get_double("starting depth");


      prm.load_entry("maximum depth", false, Types::Double(INFINITY, "The depth below the surface at which this plate may not be present."));

      maximum_depth = prm.get_double("maximum depth");


      prm.enter_subsection("segments");
      {

        // For now it is required to have a all segments, this requirement might be dropped in the future.
        prm.load_entry("all", true, Types::Array(Types::Segment(0,Point<2>(0,0,cartesian),Point<2>(0,0,cartesian), "A plate segment with a certain length, thickness and angle."),
                                                 "A list of plate segments."));

        std::vector<const Types::Segment *> all_segments = prm.get_array<const Types::Segment>("all");


        maximum_slab_thickness = 0;
        total_slab_length.resize(typed_coordinates.size());
        slab_segment_lengths.resize(typed_coordinates.size());
        slab_segment_thickness.resize(typed_coordinates.size());
        slab_segment_angles.resize(typed_coordinates.size());
        for (unsigned int coordinate_i = 0; coordinate_i < typed_coordinates.size(); ++coordinate_i)
          {
            total_slab_length[coordinate_i] = 0;
            for (unsigned int segment_i = 0; segment_i < all_segments.size(); segment_i++)
              {
                total_slab_length[coordinate_i] += all_segments[segment_i]->value_length;
                slab_segment_lengths[coordinate_i].push_back(all_segments[segment_i]->value_length);

                if (all_segments[segment_i]->value_thickness[0] > maximum_slab_thickness)
                  maximum_slab_thickness = all_segments[segment_i]->value_thickness[0];

                if (all_segments[segment_i]->value_thickness[1] > maximum_slab_thickness)
                  maximum_slab_thickness = all_segments[segment_i]->value_thickness[1];

                slab_segment_thickness[coordinate_i].push_back(all_segments[segment_i]->value_thickness);
                slab_segment_angles[coordinate_i].push_back(all_segments[segment_i]->value_angle * (M_PI/180));
              }

            if (total_slab_length[coordinate_i] > maximum_total_slab_length)
              maximum_total_slab_length = total_slab_length[coordinate_i];
          }
      }
      prm.leave_subsection();

      prm.enter_subsection("temperature submodule");
      {
        prm.load_entry("name", true, Types::String("","The name of the temperature submodule."));
        temperature_submodule_name = prm.get_string("name");

        if (temperature_submodule_name == "constant")
          {
            prm.load_entry("temperature", true, Types::Double(0,"The temperature in degree Kelvin which this feature should have"));
            temperature_submodule_constant_temperature = prm.get_double("temperature");
          }
        else if (temperature_submodule_name == "linear")
          {
            prm.load_entry("depth", true, Types::Double(NaN::DSNAN,"The depth in meters to which the temperature rises (or lowers) to."));
            temperature_submodule_linear_depth = prm.get_double("depth");

            prm.load_entry("top temperature", false, Types::Double(293.15,"The temperature in degree Kelvin a the top of this block. If this value is not set, the "));
            temperature_submodule_linear_top_temperature = prm.get_double("top temperature");


            prm.load_entry("bottom temperature", false, Types::Double(NaN::DQNAN,"The temperature in degree Kelvin a the bottom of this block."));
            temperature_submodule_linear_bottom_temperature = prm.get_double("bottom temperature");
          }
        else if (temperature_submodule_name == "plate model")
          {
            prm.load_entry("depth", true, Types::Double(NaN::DSNAN,"The depth in meters to which the temperature rises (or lowers) to."));
            temperature_submodule_plate_model_depth = prm.get_double("depth");

            prm.load_entry("top temperature", false, Types::Double(293.15,"The temperature in degree Kelvin a the top of this block. If this value is not set, the "));
            temperature_submodule_plate_model_top_temperature = prm.get_double("top temperature");


            prm.load_entry("bottom temperature", false, Types::Double(NaN::DQNAN,"The temperature in degree Kelvin a the bottom of this block."));
            temperature_submodule_plate_model_bottom_temperature = prm.get_double("bottom temperature");

            prm.load_entry("ridge points", true,
                           Types::Array(Types::Point<2>(Point<2>({0,0},coordinate_system),
                                                        "A 2d point on the line where the oceanic ridge is located."),
                                        "A list of 2d points which indicate the location of the ridge."));
            std::vector<const Types::Point<2>* > temp_ridge_points = prm.get_array<const Types::Point<2> >("ridge points");

            const double dtr = prm.coordinate_system->natural_coordinate_system() == spherical ? M_PI / 180.0 : 1.0;
            WBAssertThrow(temp_ridge_points.size() >= 2,
                          "Need at least two points to form the ridge of the oceanic plate, "
                          << temp_ridge_points.size() << " points where given.");
            temperature_submodule_plate_model_ridge_points.resize(temp_ridge_points.size(),Point<2>(coordinate_system));
            for (unsigned int i = 0; i < temp_ridge_points.size(); ++i)
              {
                temperature_submodule_plate_model_ridge_points[i] = temp_ridge_points[i]->value * dtr;
              }

            prm.load_entry("spreading velocity", true, Types::Double(NaN::DSNAN,
                                                                     "The spreading velocity of the plate in meter per year. Todo: find out whether this is the half or the whole spreading velocity."));
            // directly convert from meter per year to meter per second.
            temperature_submodule_plate_model_spreading_velocity = prm.get_double("spreading velocity")/31557600;
          }

      }
      prm.leave_subsection();

      prm.enter_subsection("composition submodule");
      {
        prm.load_entry("name", true, Types::String("","The name of the composition submodule used."));
        composition_submodule_name = prm.get_string("name");

        if (composition_submodule_name == "constant")
          {
            prm.load_entry("depth", true, Types::Double(NaN::DSNAN,"The depth in meters to which the composition of this feature is present."));
            composition_submodule_constant_depth = prm.get_double("depth");

            prm.load_entry("composition", true, Types::UnsignedInt(0,"The number of the composition that is present there."));
            composition_submodule_constant_composition = prm.get_unsigned_int("composition");
          }
      }
      prm.leave_subsection();
    }


    double
    SubductingPlate::temperature(const Point<3> &position,
                                 const double depth,
                                 const double gravity_norm,
                                 double temperature) const
    {
      WorldBuilder::Utilities::NaturalCoordinate natural_coordinate = WorldBuilder::Utilities::NaturalCoordinate(position,
                                                                      *(world->parameters.coordinate_system));

      // todo: explain
      const double starting_radius = natural_coordinate.get_depth_coordinate() + depth - starting_depth;

      // todo: explain and check -starting_depth
      //if (depth <= maximum_depth && depth >= starting_depth && depth <= maximum_total_slab_length + maximum_slab_thickness)
      {
        // todo: explain
        std::map<std::string,double> distance_from_planes =
          Utilities::distance_point_from_curved_planes(position,
                                                       reference_point,
                                                       coordinates,
                                                       slab_segment_lengths,
                                                       slab_segment_angles,
                                                       starting_radius,
                                                       this->world->parameters.coordinate_system,
                                                       false);

        const double distance_from_plane = distance_from_planes["distanceFromPlane"];
        const double distance_along_plane = distance_from_planes["distanceAlongPlane"];
        const double section_fraction = distance_from_planes["sectionFraction"];
        const unsigned int current_section = distance_from_planes["section"];
        const unsigned int next_section = current_section + 1;
        const unsigned int current_segment = distance_from_planes["segment"];
        const unsigned int next_segment = current_segment + 1;
        const double segment_fraction = distance_from_planes["segmentFraction"];

        //if(distance_from_plane < INFINITY)
        // std::cout << "distance_from_plane = " << distance_from_plane << ", distance_along_plane = " << distance_along_plane << std::endl;

        if (abs(distance_from_plane) < INFINITY || (distance_along_plane) < INFINITY)
          {
            // We want to do both section (horizontal) and segment (vertical) interpolation.

            const double thickness_up = slab_segment_thickness[current_section][current_segment][0]
                                        + section_fraction
                                        * (slab_segment_thickness[next_section][current_segment][0]
                                           - slab_segment_thickness[current_section][current_segment][0]);
            const double thickness_down = slab_segment_thickness[current_section][current_segment][1]
                                          + section_fraction
                                          * (slab_segment_thickness[next_section][current_segment][1]
                                             - slab_segment_thickness[current_section][current_segment][1]);
            const double thickness_local = thickness_up + segment_fraction * (thickness_down - thickness_up);

            const double max_slab_length = total_slab_length[current_section] +
                                           section_fraction *
                                           (total_slab_length[next_section] - total_slab_length[current_section]);

            // TODO: do some interpolation for the thickness.
            if (distance_from_plane > 0 &&
                distance_from_plane <= thickness_local &&
                distance_along_plane > 0 &&
                distance_along_plane <= max_slab_length)
              {
                // Inside the slab!
                return temperature_submodule_constant_temperature;
              }
          }
        //return -1;
      }
      /*
            // We are in the the area where the contintal plate
            if (temperature_submodule_name == "constant")
              {
                if (depth <= temperature_submodule_constant_depth &&
                    Utilities::polygon_contains_point(coordinates, Point<2>(natural_coordinate.get_surface_coordinates(),
                                                                            world->parameters.coordinate_system->natural_coordinate_system())))
                  {
                    // The constant temperature module should be used for this. Set the constant temperature.
                    return temperature_submodule_constant_temperature;
                  }

              }
            else if (temperature_submodule_name == "linear")
              {
                // The linear temperature module should be used for this.
                if (depth <= temperature_submodule_linear_depth &&
                    Utilities::polygon_contains_point(coordinates, Point<2>(natural_coordinate.get_surface_coordinates(),
                                                                            world->parameters.coordinate_system->natural_coordinate_system())))
                  {
                    double bottom_temperature = temperature_submodule_linear_bottom_temperature;
                    if (std::isnan(temperature_submodule_linear_bottom_temperature))
                      {
                        bottom_temperature =  this->world->parameters.get_double("Potential mantle temperature") +
                                              (((this->world->parameters.get_double("Potential mantle temperature") * this->world->parameters.get_double("Thermal expansion coefficient alpha") * gravity_norm) /
                                                this->world->parameters.get_double("specific heat Cp")) * 1000.0) * ((depth) / 1000.0);
                      }

                    return temperature_submodule_linear_top_temperature +
                           depth * ((bottom_temperature - temperature_submodule_linear_top_temperature) / temperature_submodule_linear_depth);
                  }
              }
            else if (temperature_submodule_name == "plate model")
              {
                // The plate model temperature module should be used for this.
                if (depth <= temperature_submodule_plate_model_depth &&
                    Utilities::polygon_contains_point(coordinates, Point<2>(natural_coordinate.get_surface_coordinates(),
                                                                            world->parameters.coordinate_system->natural_coordinate_system())))
                  {
                    double bottom_temperature = temperature_submodule_plate_model_bottom_temperature;
                    const double max_depth = temperature_submodule_plate_model_depth;

                    if (std::isnan(bottom_temperature))
                      {
                        bottom_temperature =  this->world->parameters.get_double("Potential mantle temperature") +
                                              (((this->world->parameters.get_double("Potential mantle temperature") * this->world->parameters.get_double("Thermal expansion coefficient alpha") * gravity_norm) /
                                                this->world->parameters.get_double("specific heat Cp")) * 1000.0) * ((max_depth) / 1000.0);
                      }

                    const int sommation_number = 100;
                    double distance_ridge = std::numeric_limits<double>::max();

                    const CoordinateSystem coordinate_system = world->parameters.coordinate_system->natural_coordinate_system();

                    for (unsigned int i_ridge = 0; i_ridge < temperature_submodule_plate_model_ridge_points.size()-1; i_ridge++)
                      {
                        const Point<2> segment_point0 = temperature_submodule_plate_model_ridge_points[i_ridge];
                        const Point<2> segment_point1 = temperature_submodule_plate_model_ridge_points[i_ridge+1];

                        const Point<2> check_point(natural_coordinate.get_surface_coordinates(),natural_coordinate.get_coordinate_system());
                        // based on http://geomalgorithms.com/a02-_lines.html
                        const Point<2> v = segment_point1 - segment_point0;
                        const Point<2> w = check_point - segment_point0;

                        const double c1 = (w[0] * v[0] + w[1] * v[1]);
                        const double c2 = (v[0] * v[0] + v[1] * v[1]);

                        Point<2> Pb(coordinate_system);
                        // This part is needed when we want to consider segments instead of lines
                        // If you want to have infinite lines, use only the else statement.

                        if (c1 <= 0)
                          Pb=segment_point0;
                        else if (c2 <= c1)
                          Pb=segment_point1;
                        else
                          Pb = segment_point0 + (c1 / c2) * v;

                        Point<3> compare_point(coordinate_system);

                        compare_point[0] = coordinate_system == cartesian ? Pb[0] :  natural_coordinate.get_depth_coordinate();
                        compare_point[1] = coordinate_system == cartesian ? Pb[1] : Pb[0];
                        compare_point[2] = coordinate_system == cartesian ? natural_coordinate.get_depth_coordinate() : Pb[1];

                        distance_ridge = std::min(distance_ridge,this->world->parameters.coordinate_system->distance_between_points_at_same_depth(Point<3>(natural_coordinate.get_coordinates(),natural_coordinate.get_coordinate_system()),compare_point));

                      }

                    // some aliases
                    const double top_temperature = temperature_submodule_plate_model_top_temperature;
                    const double spreading_velocity = temperature_submodule_plate_model_spreading_velocity;
                    const double thermal_diffusivity = this->world->parameters.get_double("thermal diffusivity");
                    const double age = distance_ridge / spreading_velocity;
                    temperature = top_temperature + (bottom_temperature - top_temperature) * (depth / max_depth);

                    for (int i = 1; i<sommation_number+1; ++i)
                      {
                        temperature = temperature + (bottom_temperature - top_temperature) *
                                      ((2 / (double(i) * M_PI)) * std::sin((double(i) * M_PI * depth) / max_depth) *
                                       std::exp((((spreading_velocity * max_depth)/(2 * thermal_diffusivity)) -
                                                 std::sqrt(((spreading_velocity*spreading_velocity*max_depth*max_depth) /
                                                            (4*thermal_diffusivity*thermal_diffusivity)) + double(i) * double(i) * M_PI * M_PI)) *
                                                ((spreading_velocity * age) / max_depth)));

                      }

                    return temperature;
                  }

              }
            else if (temperature_submodule_name == "none")
              {
                return temperature;
              }
            else
              {
                WBAssertThrow(false,"Given temperature module does not exist: " + temperature_submodule_name);
              }
      */
      return temperature;
    }

    bool
    SubductingPlate::composition(const Point<3> &position,
                                 const double depth,
                                 const unsigned int composition_number,
                                 bool composition) const
    {
      if (composition_submodule_name == "constant")
        {
          WorldBuilder::Utilities::NaturalCoordinate natural_coordinate = WorldBuilder::Utilities::NaturalCoordinate(position,*(world->parameters.coordinate_system));
          // The constant temperature module should be used for this.
          if (depth <= composition_submodule_constant_depth &&
              Utilities::polygon_contains_point(coordinates, Point<2>(natural_coordinate.get_surface_coordinates(),world->parameters.coordinate_system->natural_coordinate_system())))
            {
              // We are in the the area where the contintal plate is defined. Set the constant temperature.
              if (composition_submodule_constant_composition == composition_number)
                {
                  return true;
                }
            }

        }
      else if (composition_submodule_name == "none")
        {
          return composition;
        }
      else
        {
          WBAssertThrow(false,"Given composition module does not exist: " + composition_submodule_name);
        }

      return composition;
    }
  }
}

