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

      this->declare_interface_entries(prm, coordinate_system);

      // Get the reference point
      prm.load_entry("reference point", true, Types::Point<2>(Point<2>(0,0,coordinate_system),
                                                              "A point which the plate subducts towards. When a coordinates of the subducting "
                                                              "plate form a line, it is undefined in what direction the plate should subduct "
                                                              "along that line. Through giving a point to which the plate should subduct "
                                                              "solves this problem."));

      reference_point = prm.get_point<2>("reference point") * (coordinate_system == CoordinateSystem::spherical ? const_pi / 180.0 : 1.0);


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

        maximum_slab_thickness = 0;
        total_slab_length.resize(original_number_of_coordinates);
        slab_segment_lengths.resize(original_number_of_coordinates);
        slab_segment_thickness.resize(original_number_of_coordinates);
        slab_segment_angles.resize(original_number_of_coordinates);
        for (unsigned int coordinate_i = 0; coordinate_i < original_number_of_coordinates; ++coordinate_i)
          {
            // todo: remove the next line
            std::vector<Types::Segment> current_segment;

            // first check whether there is an overwrite for this coordinate
            bool overwrite = prm.load_entry(std::to_string(coordinate_i), false, Types::Array(Types::Segment(0,Point<2>(0,0,cartesian),Point<2>(0,0,cartesian),
                                            "A plate segment with a certain length, thickness and angle."),
                                            "A list of plate segments."));


            if (overwrite == true)
              {
                // there is a special case for this coordinate, so use it.
                current_segment = prm.get_array<Types::Segment>(std::to_string(coordinate_i));
              }
            else
              {
                // Need to get it again, because the load entry last time could
                // have changed the segment list, thereby invalidating the pointers.
                current_segment = prm.get_array<Types::Segment>("all");
              }

            total_slab_length[coordinate_i] = 0;
            for (unsigned int segment_i = 0; segment_i < current_segment.size(); segment_i++)
              {
                total_slab_length[coordinate_i] += current_segment[segment_i].value_length;
                slab_segment_lengths[coordinate_i].push_back(current_segment[segment_i].value_length);

                if (current_segment[segment_i].value_thickness[0] > maximum_slab_thickness)
                  maximum_slab_thickness = current_segment[segment_i].value_thickness[0];

                if (current_segment[segment_i].value_thickness[1] > maximum_slab_thickness)
                  maximum_slab_thickness = current_segment[segment_i].value_thickness[1];

                slab_segment_thickness[coordinate_i].push_back(current_segment[segment_i].value_thickness);
                slab_segment_angles[coordinate_i].push_back(current_segment[segment_i].value_angle * (const_pi/180));
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
            WBAssertThrow(temperature_submodule_name == "none","Subducting plate temperature model '" << temperature_submodule_name << "' not found.");
          }

      }
      prm.leave_subsection();

      prm.enter_subsection("composition model");
      {
        prm.load_entry("name", true, Types::String("","The name of the composition model used."));
        composition_submodule_name = prm.get_string("name");

        if (composition_submodule_name == "constant")
          {
            prm.load_entry("compositions", true,
                           Types::Array(Types::UnsignedInt(0,"The number of the composition that is present there."),
                                        "A list of compositions which are present"));
            std::vector<Types::UnsignedInt> temp_composition = prm.get_array<Types::UnsignedInt>("compositions");
            composition_submodule_constant_composition.resize(temp_composition.size());
            for (unsigned int i = 0; i < temp_composition.size(); ++i)
              {
                composition_submodule_constant_composition[i] = temp_composition[i].value;
              }

            prm.load_entry("fractions", false,
                           Types::Array(Types::Double(1.0,"The value between 0 and 1 of how much this composition is present."),
                                        "A list of compositional fractions corresponding to the compositions list."));
            std::vector<Types::Double> temp_fraction = prm.get_array<Types::Double>("fractions");
            composition_submodule_constant_value.resize(temp_fraction.size());
            for (unsigned int i = 0; i < temp_composition.size(); ++i)
              {
                composition_submodule_constant_value[i] = temp_fraction[i].value;
              }

            WBAssertThrow(composition_submodule_constant_composition.size() == composition_submodule_constant_value.size(),
                          "There are not the same amount of compositions and fractions.");
          }
        else if (composition_submodule_name == "constant layers")
          {
            // Load the layers.
            prm.load_entry("layers", true, Types::Array(Types::ConstantLayer({NaN::ISNAN}, {1.0},NaN::DSNAN,
                                                                             "A plate constant layer with a certain composition and thickness."),
                                                        "A list of layers."));

            std::vector<Types::ConstantLayer> constant_layers = prm.get_array<Types::ConstantLayer>("layers");

            composition_submodule_constant_layers_compositions.resize(constant_layers.size());
            composition_submodule_constant_layers_thicknesses.resize(constant_layers.size());
            composition_submodule_constant_layers_value.resize(constant_layers.size());

            for (unsigned int i = 0; i < constant_layers.size(); ++i)
              {
                composition_submodule_constant_layers_compositions[i] = constant_layers[i].value_composition;
                composition_submodule_constant_layers_thicknesses[i] = constant_layers[i].value_thickness;
                composition_submodule_constant_layers_value[i] = constant_layers[i].value;
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
                                                         false,
                                                         one_dimensional_coordinates);

          const double distance_from_plane = distance_from_planes["distanceFromPlane"];
          const double distance_along_plane = distance_from_planes["distanceAlongPlane"];
          const double average_angle = distance_from_planes["averageAngle"];
          const double section_fraction = distance_from_planes["sectionFraction"];
          const unsigned int current_section = one_dimensional_coordinates[distance_from_planes["section"]];
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

              const double potential_mantle_temperature = world->parameters.get_double("potential mantle temperature");
              const double surface_temperature = world->parameters.get_double("surface temperature");

              // TODO: do some interpolation for the thickness.
              if (distance_from_plane >= 0 &&
                  distance_from_plane <= thickness_local &&
                  distance_along_plane >= 0 &&
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
                       * temperature inside the slab. The McKenzie equation was
                       * designed for a straight slab, but we have a potentially
                       * curved slab. Because the angle is a required parameter, we
                       * first tried a local angle. This gave weird effects of
                       * apparent cooling when the slabs angle decreases. Now we
                       * use an average angle, which works better.
                       */
                      const double R = (temperature_submodule_plate_model_density * temperature_submodule_plate_model_specific_heat
                                        * (temperature_submodule_plate_model_plate_velocity /(365.25 * 24.0 * 60.0 * 60.0))
                                        * thickness_local) / (2.0 * temperature_submodule_plate_model_thermal_conductivity);

                      WBAssert(!std::isnan(R), "Internal error: R is not a number: " << R << ".");

                      // gravity in original in cm/s^2, here in m/s^2, thickness original in km, here in meter. So 100/1000=0.1
                      const double H = temperature_submodule_plate_model_specific_heat
                                       / (temperature_submodule_plate_model_Thermal_expansion_coefficient * gravity_norm * 0.1 * thickness_local);

                      WBAssert(!std::isnan(H), "Internal error: H is not a number: " << H << ".");

                      const int n_sum = 500;
                      // distance_from_plane can be zero, so protect division.
                      double z_scaled = 1 - (std::fabs(distance_from_plane) < 2.0 * std::numeric_limits<double>::epsilon() ?
                                             2.0 * std::numeric_limits<double>::epsilon()
                                             :
                                             distance_from_plane
                                             / thickness_local);

                      // distance_along_plane can be zero, so protect division.
                      double x_scaled = (std::fabs(distance_along_plane) < 2.0 * std::numeric_limits<double>::epsilon() ?
                                         2.0 *std::numeric_limits<double>::epsilon()
                                         :
                                         distance_along_plane)
                                        / thickness_local;
                      double temp = exp((x_scaled * sin(average_angle)
                                         - z_scaled * cos(average_angle)) / H);

                      WBAssert(!std::isnan(z_scaled), "Internal error: z_scaled is not a number: " << z_scaled << ".");
                      WBAssert(!std::isnan(x_scaled), "Internal error: x_scaled is not a number: " << x_scaled << ".");
                      WBAssert(!std::isnan(temp), "Internal error: temp is not a number: " << temp << ". In exponent: "
                               << (x_scaled * sin(average_angle) - z_scaled * cos(average_angle)) / H
                               << ", top: " << (x_scaled * sin(average_angle) - z_scaled * cos(average_angle))
                               << ", x_scaled = " << x_scaled << ", z_scaled = " << z_scaled << ", average_angle = " << average_angle);

                      double sum=0;
                      for (int i=1; i<=n_sum; i++)
                        {
                          sum += (std::pow((-1.0),i)/(i*const_pi)) *
                                 (exp((R - std::pow(R * R + i * i * const_pi * const_pi, 0.5)) * x_scaled))
                                 * (sin(i * const_pi * z_scaled));
                        }
                      temperature = temp * (potential_mantle_temperature + (surface_temperature - 273.15) * (z_scaled)
                                            + 2.0 * (potential_mantle_temperature - 273.15) * sum);

                      WBAssert(!std::isnan(temperature), "Internal error: temperature is not a number: " << temperature << ".");
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
                                                             false,
                                                             one_dimensional_coordinates);

              const double distance_from_plane = distance_from_planes["distanceFromPlane"];
              const double distance_along_plane = distance_from_planes["distanceAlongPlane"];
              const double section_fraction = distance_from_planes["sectionFraction"];
              const unsigned int current_section = one_dimensional_coordinates[distance_from_planes["section"]];
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
                  if (distance_from_plane >= 0 &&
                      distance_from_plane <= thickness_local &&
                      distance_along_plane >= 0 &&
                      distance_along_plane <= max_slab_length)
                    {
                      // Inside the slab!
                      if (composition_submodule_name == "constant")
                        {
                          // We are in the the area where the subducting plate is defined. Set the constant composition
                          const bool clear = true;
                          for (unsigned int i =0; i < composition_submodule_constant_composition.size(); ++i)
                            {
                              if (composition_submodule_constant_composition[i] == composition_number)
                                {
                                  return composition_submodule_constant_value[i];
                                }
                              else if (clear == true)
                                {
                                  composition = 0.0;
                                }
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
                                  const bool clear = true;
                                  for (unsigned int j =0; j < composition_submodule_constant_layers_compositions[i].size(); ++j)
                                    {
                                      if (composition_submodule_constant_layers_compositions[i][j] == composition_number)
                                        {
                                          return composition_submodule_constant_layers_value[i][j];
                                        }
                                      else if (clear == true)
                                        {
                                          composition = 0.0;
                                        }
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

