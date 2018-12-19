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

#include <world_builder/types/array.h>
#include <world_builder/types/constant_layer.h>
#include <world_builder/types/double.h>
#include <world_builder/types/point.h>
#include <world_builder/types/string.h>
#include <world_builder/types/object.h>
#include <world_builder/types/plugin_system.h>
#include <world_builder/types/unsigned_int.h>

//temp
#include <rapidjson/istreamwrapper.h>
#include "rapidjson/pointer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"



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
    SubductingPlate::declare_entries(Parameters &prm,
                                     const std::string &parent_name,
                                     const std::vector<std::string> &required_entries)
    {
      if (parent_name == "items")
        prm.enter_subsection("properties");

      prm.declare_entry("min depth", Types::Double(0),
                        "The depth to which this feature is present");
      prm.declare_entry("max depth", Types::Double(std::numeric_limits<double>::max()),
                        "The depth to which this feature is present");
      prm.declare_entry("dip point", Types::Point<2>(),
                        "The depth to which this feature is present");
      /*        prm.enter_subsection("segments");
              {
                prm.enter_subsection("properties");
                {
                      prm.declare_entry("", Types::Object(), "Segment of subducting slab");

                      prm.declare_entry("length", Types::Double(0), "length of the segment");

                      prm.declare_entry("top truncation", Types::Point<2>(), "length of the segment");

                      prm.declare_entry("thickness", Types::Point<2>(), "length of the segment");

                      prm.declare_entry("angle", Types::Point<2>(), "length of the segment");
                }
                prm.leave_subsection();
              }
              prm.leave_subsection();*/

      prm.declare_entry("segments", Types::Array(Types::Segment(0,Point<2>(0,0,invalid),Point<2>(0,0,invalid),Point<2>(0,0,invalid),
                                                                Types::PluginSystem("", Features::SubductingPlateModels::Temperature::Interface::declare_entries, {"model"}),
                                                                Types::PluginSystem("", Features::SubductingPlateModels::Composition::Interface::declare_entries, {"model"}))),
                        "The depth to which this feature is present");

      prm.declare_entry("temperature models",
                        Types::PluginSystem("", Features::SubductingPlateModels::Temperature::Interface::declare_entries, {"model"}),
                        "A list of temperature models.");
      prm.declare_entry("composition models",
                        Types::PluginSystem("", Features::SubductingPlateModels::Composition::Interface::declare_entries, {"model"}),
                        "A list of composition models.");

      if (parent_name != "items")
        {
          // This only happens if we are not in sections
          prm.declare_entry("sections", Types::Array(Types::PluginSystem("",Features::SubductingPlate::declare_entries, {"coordinate"}, false)),"A list of feature properties for a coordinate.");
        }
      else
        {

          // this only happens in sections
          prm.declare_entry("coordinate", Types::UnsignedInt(0),
                            "The coordinate which should be overwritten");

          prm.leave_subsection();
        }
    }

    void
    SubductingPlate::parse_entries(Parameters &prm)
    {
      const CoordinateSystem coordinate_system = prm.coordinate_system->natural_coordinate_system();

      this->name = prm.get<std::string>("name");
      this->get_coordinates("coordinates", prm, coordinate_system);


      starting_depth = prm.get<double>("min depth");
      maximum_depth = prm.get<double>("max depth");

      const unsigned int n_sections = this->original_number_of_coordinates;

      reference_point = prm.get<Point<2> >("dip point");

      default_temperature_models.resize(0);
      default_composition_models.resize(0);
      bool found = prm.get_shared_pointers<Features::SubductingPlateModels::Temperature::Interface>("temperature models", default_temperature_models);
      found = prm.get_shared_pointers<Features::SubductingPlateModels::Composition::Interface>("composition models", default_composition_models);

      // initialize the default models
      /*prm.enter_subsection("temperature models");
      {
        for(unsigned int i = 0; i < default_temperature_models.size(); ++i)
        {
          prm.enter_subsection(std::to_string(i));
          {
            default_temperature_models[i]->parse_entries(prm);
          }
          prm.leave_subsection();
        }
      }
      prm.leave_subsection();


      prm.enter_subsection("composition models");
      {
        for(unsigned int i = 0; i < default_composition_models.size(); ++i)
        {
          prm.enter_subsection(std::to_string(i));
          {
            default_composition_models[i]->parse_entries(prm);
          }
          prm.leave_subsection();
        }
      }
      prm.leave_subsection();*/

      //std::vector<std::shared_ptr<Features::SubductingPlateModels::Temperature::Interface> > temperature_models;
      //std::vector<std::shared_ptr<Features::SubductingPlateModels::Composition::Interface>  > composition_models;

      // get the default segments.
      default_segment_vector = prm.get_vector<Objects::Segment<Features::SubductingPlateModels::Temperature::Interface,
      Features::SubductingPlateModels::Composition::Interface> >("segments", default_temperature_models, default_composition_models);


      // This vector stores segments to this coordiante/section.
      //First used (raw) pointers to the segment relevant to this coordinate/section,
      // but I do not trust it won't fail when memory is moved. So storing the all the data now.
      segment_vector.resize(0);
      segment_vector.resize(n_sections, default_segment_vector);

      prm.enter_subsection("segments");
      {
        for (unsigned int i = 0; i < default_segment_vector.size(); ++i)
          {
            prm.enter_subsection(std::to_string(i));
            {
              prm.enter_subsection("temperature models");
              {
                for (unsigned int j = 0; j < default_segment_vector[i].temperature_systems.size(); ++j)
                  {
                    prm.enter_subsection(std::to_string(j));
                    {
                      default_segment_vector[i].temperature_systems[j]->parse_entries(prm);
                    }
                    prm.leave_subsection();
                  }
              }
              prm.leave_subsection();


              prm.enter_subsection("composition models");
              {
                for (unsigned int j = 0; j < default_segment_vector[i].composition_systems.size(); ++j)
                  {
                    prm.enter_subsection(std::to_string(j));
                    {
                      default_segment_vector[i].composition_systems[j]->parse_entries(prm);
                    }
                    prm.leave_subsection();
                  }
              }
              prm.leave_subsection();
            }
            prm.leave_subsection();
          }
      }
      prm.leave_subsection();

      // now search whether a section is present, if so, replace the default segments.
      std::vector<std::unique_ptr<Features::SubductingPlate> > sections_vector;
      prm.get_unique_pointers("sections", sections_vector);

      prm.enter_subsection("sections");
      for (unsigned int i = 0; i < n_sections; ++i)
        {
          // first check whether this section/coordinate has a a special overwrite
          for (unsigned int i_sector = 0; i_sector < sections_vector.size(); ++i_sector)
            {
              prm.enter_subsection(std::to_string(i_sector));
              {
                const unsigned int change_coord_number = prm.get<unsigned int>("coordinate");

                segment_vector[change_coord_number] = prm.get_vector<Objects::Segment<Features::SubductingPlateModels::Temperature::Interface,
                                                      Features::SubductingPlateModels::Composition::Interface> >("segments", default_temperature_models, default_composition_models);

                prm.enter_subsection("segments");
                {
                  for (unsigned int i = 0; i < segment_vector[change_coord_number].size(); ++i)
                    {
                      prm.enter_subsection(std::to_string(i));
                      {
                        prm.enter_subsection("temperature models");
                        {
                          for (unsigned int j = 0; j < segment_vector[change_coord_number][i].temperature_systems.size(); ++j)
                            {
                              prm.enter_subsection(std::to_string(j));
                              {
                                segment_vector[change_coord_number][i].temperature_systems[j]->parse_entries(prm);
                              }
                              prm.leave_subsection();
                            }
                        }
                        prm.leave_subsection();


                        prm.enter_subsection("composition models");
                        {
                          for (unsigned int j = 0; j < segment_vector[change_coord_number][j].composition_systems.size(); ++j)
                            {
                              prm.enter_subsection(std::to_string(j));
                              {
                                segment_vector[change_coord_number][i].composition_systems[j]->parse_entries(prm);
                              }
                              prm.leave_subsection();
                            }
                        }
                        prm.leave_subsection();
                      }
                      prm.leave_subsection();
                    }
                }
                prm.leave_subsection();

              }
              prm.leave_subsection();
            }

        }
      prm.leave_subsection();

      maximum_slab_thickness = 0;
      maximum_total_slab_length = 0;
      total_slab_length.resize(original_number_of_coordinates);
      slab_segment_lengths.resize(original_number_of_coordinates);
      slab_segment_thickness.resize(original_number_of_coordinates);
      slab_segment_top_truncation.resize(original_number_of_coordinates);
      slab_segment_angles.resize(original_number_of_coordinates);

      for (unsigned int i = 0; i < segment_vector.size(); ++i)
        {
          double local_total_slab_length = 0;
          slab_segment_lengths[i].resize(segment_vector[i].size());
          slab_segment_thickness[i].resize(segment_vector[i].size(), Point<2>(invalid));
          slab_segment_top_truncation[i].resize(segment_vector[i].size(), Point<2>(invalid));
          slab_segment_angles[i].resize(segment_vector[i].size(), Point<2>(invalid));
          for (unsigned int j = 0; j < segment_vector[i].size(); ++j)
            {
              slab_segment_lengths[i][j] = segment_vector[i][j].value_length;
              local_total_slab_length += segment_vector[i][j].value_length;

              slab_segment_thickness[i][j] = segment_vector[i][j].value_thickness;
              slab_segment_top_truncation[i][j] = segment_vector[i][j].value_top_truncation;

              slab_segment_angles[i][j] = segment_vector[i][j].value_angle * (const_pi/180);
            }
          maximum_slab_thickness = std::max(maximum_slab_thickness, local_total_slab_length);
          total_slab_length[i] = local_total_slab_length;
          maximum_total_slab_length = std::max(maximum_total_slab_length, local_total_slab_length);
        }


      /* prm.get_unique_pointers<Features::SubductingPlateModels::Temperature::Interface>("temperature models", temperature_models);

       prm.enter_subsection("temperature models");
       {
         for (unsigned int i = 0; i < temperature_models.size(); ++i)
           {
             prm.enter_subsection(std::to_string(i));
             {
               temperature_models[i]->parse_entries(prm);
             }
             prm.leave_subsection();
           }
       }
       prm.leave_subsection();


       prm.get_unique_pointers<Features::SubductingPlateModels::Composition::Interface>("composition models", composition_models);

       prm.enter_subsection("composition models");
       {
         for (unsigned int i = 0; i < composition_models.size(); ++i)
           {
             prm.enter_subsection(std::to_string(i));
             {
               composition_models[i]->parse_entries(prm);
             }
             prm.leave_subsection();
           }
       }
       prm.leave_subsection();*/

      /*

      //Parameters &prm = this->world->parameters;

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
            prm.load_entry("operation", false, Types::String("replace","Wheter the temperature should be replaced or added."));
            temperature_submodule_constant_operation = prm.get_string("operation");
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
            //prm.load_entry("compositions", true,
            //             Types::Array(Types::UnsignedInt(0,"The number of the composition that is present there."),
            //                        "A list of compositions which are present"));
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
      prm.leave_subsection();*/
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

      WBAssert(starting_radius != 0, "Internal error: starting_radius can not be zero. "
               << "Position = " << position[0] << ":" << position[1] << ":" << position[2]
               << ", natural_coordinate.get_depth_coordinate() = " << natural_coordinate.get_depth_coordinate()
               << ", depth = " << depth
               << ", starting_depth " << starting_depth
              );

      // todo: explain and check -starting_depth
      if (depth <= maximum_depth && depth >= starting_depth && depth <= maximum_total_slab_length + maximum_slab_thickness)
        {
          /*WBAssert(coordinates.size() == slab_segment_lengths.size(),
                   "Internal error: The size of coordinates (" << coordinates.size()
                   << ") and slab_segment_lengths (" << slab_segment_lengths.size() << ") are different.");
          WBAssert(coordinates.size() == slab_segment_angles.size(),
                   "Internal error: The size of coordinates (" << coordinates.size()
                   << ") and slab_segment_angles (" << slab_segment_angles.size() << ") are different.");
          WBAssert(coordinates.size() == slab_segment_angles.size(),
                   "Internal error: The size of coordinates (" << coordinates.size()
                   << ") and one_dimensional_coordinates (" << one_dimensional_coordinates.size() << ") are different.");*/
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
              // first for thickness
              const double thickness_up = slab_segment_thickness[current_section][current_segment][0]
                                          + section_fraction
                                          * (slab_segment_thickness[next_section][current_segment][0]
                                             - slab_segment_thickness[current_section][current_segment][0]);
              const double thickness_down = slab_segment_thickness[current_section][current_segment][1]
                                            + section_fraction
                                            * (slab_segment_thickness[next_section][current_segment][1]
                                               - slab_segment_thickness[current_section][current_segment][1]);
              const double thickness_local = thickness_up + segment_fraction * (thickness_down - thickness_up);
              distance_from_planes["thicknessLocal"] = thickness_local;

              // secondly for top truncation
              const double top_truncation_up = slab_segment_top_truncation[current_section][current_segment][0]
                                               + section_fraction
                                               * (slab_segment_top_truncation[next_section][current_segment][0]
                                                  - slab_segment_top_truncation[current_section][current_segment][0]);
              const double top_truncation_down = slab_segment_top_truncation[current_section][current_segment][1]
                                                 + section_fraction
                                                 * (slab_segment_top_truncation[next_section][current_segment][1]
                                                    - slab_segment_top_truncation[current_section][current_segment][1]);
              const double top_truncation_local = top_truncation_up + segment_fraction * (top_truncation_down - top_truncation_up);

              // if the thickness is zero, we don't need to compute anything, so return.
              if (std::fabs(thickness_local) < 2.0 * std::numeric_limits<double>::epsilon())
                return temperature;

              // if the thickness is smaller than what is truncated off at the top, we don't need to compute anything, so return.
              if (thickness_local < top_truncation_local)
                return temperature;

              const double max_slab_length = total_slab_length[current_section] +
                                             section_fraction *
                                             (total_slab_length[next_section] - total_slab_length[current_section]);

              if (distance_from_plane >= top_truncation_local &&
                  distance_from_plane <= thickness_local &&
                  distance_along_plane >= 0 &&
                  distance_along_plane <= max_slab_length)
                {
                  // Inside the slab!
                  double temperature_current_section = temperature;
                  double temperature_next_section = temperature;

                  for (auto &temperature_model: segment_vector[current_section][current_segment].temperature_systems)
                    {
                      temperature_current_section = temperature_model->get_temperature(position,
                                                                                       depth,
                                                                                       gravity_norm,
                                                                                       temperature,
                                                                                       starting_depth,
                                                                                       maximum_depth,
                                                                                       distance_from_planes);

                      WBAssert(!std::isnan(temperature), "Temparture is not a number: " << temperature
                               << ", based on a temperature model with the name " << temperature_model->get_name());
                      WBAssert(std::isfinite(temperature), "Temparture is not a finite: " << temperature
                               << ", based on a temperature model with the name " << temperature_model->get_name());

                    }

                  for (auto &temperature_model: segment_vector[next_section][current_segment].temperature_systems)
                    {
                      temperature_next_section = temperature_model->get_temperature(position,
                                                                                    depth,
                                                                                    gravity_norm,
                                                                                    temperature,
                                                                                    starting_depth,
                                                                                    maximum_depth,
                                                                                    distance_from_planes);

                      WBAssert(!std::isnan(temperature), "Temparture is not a number: " << temperature
                               << ", based on a temperature model with the name " << temperature_model->get_name());
                      WBAssert(std::isfinite(temperature), "Temparture is not a finite: " << temperature
                               << ", based on a temperature model with the name " << temperature_model->get_name());

                    }

                  // linear interpolation between current and next section temperatures
                  temperature = temperature_current_section + section_fraction * (temperature_next_section - temperature_current_section);
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

                  // We want to do both section (horizontal) and segment (vertical) interpolation.
                  // first for thickness
                  const double thickness_up = slab_segment_thickness[current_section][current_segment][0]
                                              + section_fraction
                                              * (slab_segment_thickness[next_section][current_segment][0]
                                                 - slab_segment_thickness[current_section][current_segment][0]);
                  const double thickness_down = slab_segment_thickness[current_section][current_segment][1]
                                                + section_fraction
                                                * (slab_segment_thickness[next_section][current_segment][1]
                                                   - slab_segment_thickness[current_section][current_segment][1]);
                  const double thickness_local = thickness_up + segment_fraction * (thickness_down - thickness_up);
                  distance_from_planes["thicknessLocal"] = thickness_local;

                  // secondly for top truncation
                  const double top_truncation_up = slab_segment_top_truncation[current_section][current_segment][0]
                                                   + section_fraction
                                                   * (slab_segment_top_truncation[next_section][current_segment][0]
                                                      - slab_segment_top_truncation[current_section][current_segment][0]);
                  const double top_truncation_down = slab_segment_top_truncation[current_section][current_segment][1]
                                                     + section_fraction
                                                     * (slab_segment_top_truncation[next_section][current_segment][1]
                                                        - slab_segment_top_truncation[current_section][current_segment][1]);
                  const double top_truncation_local = top_truncation_up + segment_fraction * (top_truncation_down - top_truncation_up);

                  // if the thickness is zero, we don't need to compute anything, so return.
                  if (std::fabs(thickness_local) < 2.0 * std::numeric_limits<double>::epsilon())
                    return composition;

                  // if the thickness is smaller than what is truncated off at the top, we don't need to compute anything, so return.
                  if (thickness_local < top_truncation_local)
                    return composition;

                  const double max_slab_length = total_slab_length[current_section] +
                                                 section_fraction *
                                                 (total_slab_length[next_section] - total_slab_length[current_section]);

                  if (distance_from_plane >= top_truncation_local &&
                      distance_from_plane <= thickness_local &&
                      distance_along_plane >= 0 &&
                      distance_along_plane <= max_slab_length)
                    {
                      // Inside the slab!

                      double composition_current_section = composition;
                      double composition_next_section = composition;

                      for (auto &composition_model: segment_vector[current_section][current_segment].composition_systems)
                        {
                          composition_current_section = composition_model->get_composition(position,
                                                                                           depth,
                                                                                           composition_number,
                                                                                           composition_current_section,
                                                                                           starting_depth,
                                                                                           maximum_depth,
                                                                                           distance_from_planes);

                          WBAssert(!std::isnan(composition_current_section), "Composition_current_section is not a number: " << composition_current_section
                                   << ", based on a temperature model with the name " << composition_model->get_name());
                          WBAssert(std::isfinite(composition_current_section), "Composition_current_section is not a finite: " << composition_current_section
                                   << ", based on a temperature model with the name " << composition_model->get_name());

                        }

                      for (auto &composition_model: segment_vector[next_section][current_segment].composition_systems)
                        {
                          composition_next_section = composition_model->get_composition(position,
                                                                                        depth,
                                                                                        composition_number,
                                                                                        composition_next_section,
                                                                                        starting_depth,
                                                                                        maximum_depth,
                                                                                        distance_from_planes);

                          WBAssert(!std::isnan(composition_next_section), "Composition_next_section is not a number: " << composition_next_section
                                   << ", based on a temperature model with the name " << composition_model->get_name());
                          WBAssert(std::isfinite(composition_next_section), "Composition_next_section is not a finite: " << composition_next_section
                                   << ", based on a temperature model with the name " << composition_model->get_name());

                        }

                      // linear interpolation between current and next section temperatures
                      composition = composition_current_section + section_fraction * (composition_next_section - composition_current_section);

                      /*if (composition_submodule_name == "constant")
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
                        }*/

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

