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

      reference_point = prm.get_point<2>("reference point") * (coordinate_system == CoordinateSystem::spherical ? M_PI / 180.0 : 1.0);


      prm.load_entry("starting depth", false, Types::Double(0, "The depth below the surface at which this plate starts."));

      starting_depth = prm.get_double("starting depth");


      prm.load_entry("maximum depth", false, Types::Double(INFINITY, "The depth below the surface at which this plate may not be present."));

      maximum_depth = prm.get_double("maximum depth");


      prm.enter_subsection("segments");
      {

        // For now it is required to have a all segments, this requirement might be dropped in the future.
        prm.load_entry("all", true, Types::Array(Types::Segment(0,Point<2>(0,0,cartesian),Point<2>(0,0,cartesian),
                                                                "A plate segment with a certain length, thickness and angle."),
                                                 "A list of plate segments."));

        // todo: remove the next line
        std::vector<const Types::Segment *> all_segments = prm.get_array<const Types::Segment>("all");


        maximum_slab_thickness = 0;
        total_slab_length.resize(typed_coordinates.size());
        slab_segment_lengths.resize(typed_coordinates.size());
        slab_segment_thickness.resize(typed_coordinates.size());
        slab_segment_angles.resize(typed_coordinates.size());
        for (unsigned int coordinate_i = 0; coordinate_i < typed_coordinates.size(); ++coordinate_i)
          {
            // todo: remove the next line
            std::vector<const Types::Segment *> &current_segment = all_segments;

            // first check whether there is an overwrite for this coordinate
            bool overwrite = prm.load_entry(std::to_string(coordinate_i), false, Types::Array(Types::Segment(0,Point<2>(0,0,cartesian),Point<2>(0,0,cartesian),
                                            "A plate segment with a certain length, thickness and angle."),
                                            "A list of plate segments."));


            if (overwrite == true)
              {
                // there is a special case for this coordinate, so use it.
                current_segment = prm.get_array<const Types::Segment>(std::to_string(coordinate_i));
              }
            else
              {
                // Need to get it again, because the load entry last time could
                // have changed the segment list, thereby invalidating the pointers.
                current_segment = prm.get_array<const Types::Segment>("all");
              }

            total_slab_length[coordinate_i] = 0;
            for (unsigned int segment_i = 0; segment_i < current_segment.size(); segment_i++)
              {
                total_slab_length[coordinate_i] += current_segment[segment_i]->value_length;
                slab_segment_lengths[coordinate_i].push_back(current_segment[segment_i]->value_length);

                if (current_segment[segment_i]->value_thickness[0] > maximum_slab_thickness)
                  maximum_slab_thickness = current_segment[segment_i]->value_thickness[0];

                if (current_segment[segment_i]->value_thickness[1] > maximum_slab_thickness)
                  maximum_slab_thickness = current_segment[segment_i]->value_thickness[1];

                slab_segment_thickness[coordinate_i].push_back(current_segment[segment_i]->value_thickness);
                slab_segment_angles[coordinate_i].push_back(current_segment[segment_i]->value_angle * (M_PI/180));
              }

            if (total_slab_length[coordinate_i] > maximum_total_slab_length)
              maximum_total_slab_length = total_slab_length[coordinate_i];
          }
      }
      prm.leave_subsection();

      prm.enter_subsection("temperature model");
      {
        prm.load_entry("name", true, Types::String("","The name of the temperature model."));
        temperature_submodule_name = prm.get_string("name");

        if (temperature_submodule_name == "constant")
          {
            prm.load_entry("temperature", true, Types::Double(0,"The temperature in degree Kelvin which this feature should have"));
            temperature_submodule_constant_temperature = prm.get_double("temperature");
          }
        else if (temperature_submodule_name == "plate model")
          {
            prm.load_entry("density", true, Types::Double(NaN::DSNAN,"The reference density of the subducting plate."));
            temperature_submodule_plate_model_density = prm.get_double("density");

            prm.load_entry("plate velocity", true, Types::Double(NaN::DSNAN,"The velocity in meters per year with which the plate subducts."));
            temperature_submodule_plate_model_plate_velocity = prm.get_double("plate velocity");

            prm.load_entry("thermal conductivity", false, Types::Double(2.0,"The thermal conductivity of the subducting plate material in $W m^{-1} K^{-1}$."));
            temperature_submodule_plate_model_thermal_conductivity = prm.get_double("thermal conductivity");

            prm.load_entry("thermal expansion coefficient", false, Types::Double(3.5e-5,"The thermal expansivity of the subducting plate material in $K^{-1}$."));
            temperature_submodule_plate_model_Thermal_expansion_coefficient = prm.get_double("thermal expansion coefficient");

            prm.load_entry("specific heat", false, Types::Double(1250,"The specific heat of the subducting plate material in $J kg^{-1} K^{-1}$."));
            temperature_submodule_plate_model_specific_heat = prm.get_double("specific heat");
          }
        else
          {
            WBAssertThrow(false,"Subducting plate temperature model '" << temperature_submodule_name << "' not found.");
          }

      }
      prm.leave_subsection();

      prm.enter_subsection("composition model");
      {
        prm.load_entry("name", true, Types::String("","The name of the composition model used."));
        composition_submodule_name = prm.get_string("name");

        if (composition_submodule_name == "constant")
          {
            prm.load_entry("composition", true, Types::UnsignedInt(0,"The number of the composition that is present there."));
            composition_submodule_constant_composition = prm.get_unsigned_int("composition");

            prm.load_entry("value", false, Types::Double(1.0,"The value between 0 and 1 of how much this composition is present."));
            composition_submodule_constant_value = prm.get_double("value");
          }
        else if (composition_submodule_name == "constant layers")
          {
            // Load the layers.
            prm.load_entry("layers", true, Types::Array(Types::ConstantLayer(NaN::ISNAN,1.0,NaN::DSNAN,
                                                                             "A plate constant layer with a certain composition and thickness."),
                                                        "A list of layers."));

            std::vector<const Types::ConstantLayer *> constant_layers = prm.get_array<const Types::ConstantLayer>("layers");

            composition_submodule_constant_layers_compositions.resize(constant_layers.size());
            composition_submodule_constant_layers_thicknesses.resize(constant_layers.size());
            composition_submodule_constant_layers_value.resize(constant_layers.size());

            for (unsigned int i = 0; i < constant_layers.size(); ++i)
              {
                composition_submodule_constant_layers_compositions[i] = constant_layers[i]->value_composition;
                composition_submodule_constant_layers_thicknesses[i] = constant_layers[i]->value_thickness;
                composition_submodule_constant_layers_value[i] = constant_layers[i]->value;
              }
          }
        else
          {
            WBAssertThrow(composition_submodule_name == "none","Subducting plate temperature model '" << temperature_submodule_name << "' not found.");
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
      if (depth <= maximum_depth && depth >= starting_depth && depth <= maximum_total_slab_length + maximum_slab_thickness)
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
          //const unsigned int next_segment = current_segment + 1;
          const double segment_fraction = distance_from_planes["segmentFraction"];

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

              const double angle_up = slab_segment_angles[current_section][current_segment][0]
                                      + section_fraction
                                      * (slab_segment_angles[next_section][current_segment][0]
                                         - slab_segment_angles[current_section][current_segment][0]);
              const double angle_down = slab_segment_angles[current_section][current_segment][1]
                                        + section_fraction
                                        * (slab_segment_angles[next_section][current_segment][1]
                                           - slab_segment_angles[current_section][current_segment][1]);
              const double angle_local = angle_up + segment_fraction * (angle_down - angle_up);

              const double max_slab_length = total_slab_length[current_section] +
                                             section_fraction *
                                             (total_slab_length[next_section] - total_slab_length[current_section]);

              const double potential_mantle_temperature = world->parameters.get_double("potential mantle temperature");
              const double surface_temperature = world->parameters.get_double("surface temperature");

              // TODO: do some interpolation for the thickness.
              if (distance_from_plane > 0 &&
                  distance_from_plane <= thickness_local &&
                  distance_along_plane > 0 &&
                  distance_along_plane <= max_slab_length)
                {
                  // Inside the slab!
                  if (temperature_submodule_name == "constant")
                    {
                      return temperature_submodule_constant_temperature;
                    }
                  else if (temperature_submodule_name == "plate model")
                    {
                      /*
                       * We now use the McKenzie (1970) equation to determine the
                       * temperature inside the slab.
                       */
                      const double R = (temperature_submodule_plate_model_density * temperature_submodule_plate_model_specific_heat
                                        * (temperature_submodule_plate_model_plate_velocity /(365.25 * 24.0 * 60.0 * 60.0))
                                        * thickness_local) / (2.0 * temperature_submodule_plate_model_thermal_conductivity);

                      const double H = temperature_submodule_plate_model_specific_heat
                                       / (temperature_submodule_plate_model_Thermal_expansion_coefficient * gravity_norm * thickness_local);

                      const int n_sum = 500;
                      double temp = exp(((distance_along_plane / thickness_local) * sin(angle_local)
                                         - (distance_from_plane / thickness_local) * cos(angle_local)) / H);

                      double sum=0;
                      for (int i=1; i<=n_sum; i++)
                        {
                          sum += (std::pow((-1.0),i)/(i*M_PI)) *
                                 (exp((R - std::pow(std::pow(R, 2.0) + std::pow(i, 2.0) * std::pow(M_PI, 2.0), 0.5))
                                      *(distance_along_plane / thickness_local)))
                                 * (sin(i * M_PI * (1 - distance_from_plane / thickness_local)));
                        }
                      temperature = temp * (potential_mantle_temperature+2.0*(potential_mantle_temperature-surface_temperature) * sum);//segment;//distance_along_plane/thickness_local;//50+current_section;//

                    }
                }
            }
        }
      return temperature;
    }

    double
    SubductingPlate::composition(const Point<3> &position,
                                 const double depth,
                                 const unsigned int composition_number,
                                 double composition) const
    {
      if (composition_submodule_name == "none")
        {
          return composition;
        }
      else
        {
          WorldBuilder::Utilities::NaturalCoordinate natural_coordinate = WorldBuilder::Utilities::NaturalCoordinate(position,
                                                                          *(world->parameters.coordinate_system));
          // todo: explain
          const double starting_radius = natural_coordinate.get_depth_coordinate() + depth - starting_depth;

          // todo: explain and check -starting_depth
          if (depth <= maximum_depth && depth >= starting_depth && depth <= maximum_total_slab_length + maximum_slab_thickness)
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
              //const unsigned int next_segment = current_segment + 1;
              const double segment_fraction = distance_from_planes["segmentFraction"];

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
                      if (composition_submodule_name == "constant")
                        {
                          // We are in the the area where the subducting plate is defined. Set the constant composition
                          if (composition_submodule_constant_composition == composition_number)
                            {
                              return composition_submodule_constant_value;
                            }
                        }
                      else if (composition_submodule_name == "constant layers")
                        {
                          // find out what layer we are in.
                          double total_thickness = 0;
                          for (unsigned int i = 0; i < composition_submodule_constant_layers_compositions.size(); ++i)
                            {
                              if (distance_from_plane >= total_thickness
                                  && distance_from_plane < total_thickness + composition_submodule_constant_layers_thicknesses[i])
                                {
                                  // We are in a layer. Check whether this is the correct composition.
                                  // The composition_number is cast to an int to prevent a warning.
                                  // The reason composition_submodule_constant_layers_compositions is
                                  // unsigned int is so that it can be set to a negative value, which
                                  // is aways ignored.
                                  if (composition_submodule_constant_layers_compositions[i] == (int)composition_number)
                                    {
                                      return composition_submodule_constant_layers_value[i];
                                    }
                                }
                              total_thickness += composition_submodule_constant_layers_thicknesses[i];
                            }
                        }
                      else
                        {
                          WBAssertThrow(false,"Given composition module does not exist: " + composition_submodule_name);
                        }

                    }
                }
            }
        }

      return composition;
    }

    /**
     * Register plugin
     */
    WB_REGISTER_FEATURE(SubductingPlate, subducting plate)
  }
}

