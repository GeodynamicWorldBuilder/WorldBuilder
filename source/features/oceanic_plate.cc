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

#include <world_builder/features/oceanic_plate.h>
#include <world_builder/utilities.h>
#include <world_builder/assert.h>
#include <world_builder/nan.h>
#include <world_builder/parameters.h>


namespace WorldBuilder
{
  using namespace Utilities;

  namespace Features
  {
    OceanicPlate::OceanicPlate(WorldBuilder::World *world_)
      :
      temperature_submodule_constant_depth(NaN::DSNAN),
      temperature_submodule_constant_temperature(NaN::DSNAN),
      composition_submodule_constant_depth(NaN::DSNAN),
      composition_submodule_constant_composition(NaN::ISNAN)
    {
      this->world = world_;
      this->name = "continental plate";
    }

    OceanicPlate::~OceanicPlate()
    { }


    void
    OceanicPlate::decare_entries()
    {
      Parameters &prm = this->world->parameters;

      const CoordinateSystem coordinate_system = prm.coordinate_system->natural_coordinate_system();

      this->declare_interface_entries(prm, coordinate_system);

      prm.enter_subsection("temperature model");
      {
        prm.load_entry("name", true, Types::String("","The name of the temperature model."));
        temperature_submodule_name = prm.get_string("name");

        if (temperature_submodule_name == "constant")
          {
            prm.load_entry("depth", true, Types::Double(NaN::DSNAN,"The depth in meters to which the temperature of this feature is present."));
            temperature_submodule_constant_depth = prm.get_double("depth");

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
                                        "A list of 2d points which define the location of the ridge."));
            std::vector<const Types::Point<2>* > temp_ridge_points = prm.get_array<const Types::Point<2> >("ridge points");

            const double dtr = prm.coordinate_system->natural_coordinate_system() == spherical ? const_pi / 180.0 : 1.0;
            WBAssertThrow(temp_ridge_points.size() >= 2,
                          "Need at least two points to form the ridge of the oceanic plate, "
                          << temp_ridge_points.size() << " points where given.");
            temperature_submodule_plate_model_ridge_points.resize(temp_ridge_points.size(),Point<2>(coordinate_system));
            for (unsigned int i = 0; i < temp_ridge_points.size(); ++i)
              {
                temperature_submodule_plate_model_ridge_points[i] = temp_ridge_points[i]->value * dtr;
              }

            prm.load_entry("spreading velocity", true, Types::Double(NaN::DSNAN,
                                                                     "The spreading velocity of the plate in meter per year. "
                                                                     "This is the velocity with which one side moves away from the ridge."));
            // directly convert from meter per year to meter per second.
            temperature_submodule_plate_model_spreading_velocity = prm.get_double("spreading velocity")/31557600;
          }

      }
      prm.leave_subsection();

      prm.enter_subsection("composition model");
      {
        prm.load_entry("name", true, Types::String("","The name of the composition model used."));
        composition_submodule_name = prm.get_string("name");

        if (composition_submodule_name == "constant")
          {
            prm.load_entry("depth", true, Types::Double(NaN::DSNAN,"The depth in meters to which the composition of this feature is present."));
            composition_submodule_constant_depth = prm.get_double("depth");

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
    OceanicPlate::temperature(const Point<3> &position,
                              const double depth,
                              const double gravity_norm,
                              double temperature) const
    {
      WorldBuilder::Utilities::NaturalCoordinate natural_coordinate = WorldBuilder::Utilities::NaturalCoordinate(position,
                                                                      *(world->parameters.coordinate_system));


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
                  bottom_temperature =  this->world->parameters.get_double("potential mantle temperature") *
                                        std::exp(((this->world->parameters.get_double("thermal expansion coefficient") * gravity_norm) /
                                                  this->world->parameters.get_double("specific heat")) * depth);
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
                  bottom_temperature =  this->world->parameters.get_double("potential mantle temperature") *
                                        std::exp(((this->world->parameters.get_double("thermal expansion coefficient") * gravity_norm) /
                                                  this->world->parameters.get_double("specific heat")) * depth);
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
                                ((2 / (double(i) * const_pi)) * std::sin((double(i) * const_pi * depth) / max_depth) *
                                 std::exp((((spreading_velocity * max_depth)/(2 * thermal_diffusivity)) -
                                           std::sqrt(((spreading_velocity*spreading_velocity*max_depth*max_depth) /
                                                      (4*thermal_diffusivity*thermal_diffusivity)) + double(i) * double(i) * const_pi * const_pi)) *
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

      return temperature;
    }

    double
    OceanicPlate::composition(const Point<3> &position,
                              const double depth,
                              const unsigned int composition_number,
                              double composition) const
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
                  return composition_submodule_constant_value;
                }
              else
                return 0.0;
            }

        }
      else if (composition_submodule_name == "constant layers")
        {
          // find out what layer we are in.
          double total_thickness = 0;
          for (unsigned int i = 0; i < composition_submodule_constant_layers_compositions.size(); ++i)
            {
              WorldBuilder::Utilities::NaturalCoordinate natural_coordinate = WorldBuilder::Utilities::NaturalCoordinate(position,*(world->parameters.coordinate_system));

              // Check wether we are in the correct layer
              if (depth >= total_thickness
                  && depth < total_thickness + composition_submodule_constant_layers_thicknesses[i]
                  && Utilities::polygon_contains_point(coordinates,
                                                       Point<2>(natural_coordinate.get_surface_coordinates(),
                                                                world->parameters.coordinate_system->natural_coordinate_system())))
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
                  else
                    return 0.0;
                }
              total_thickness += composition_submodule_constant_layers_thicknesses[i];
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

    /**
     * Register plugin
     */
    WB_REGISTER_FEATURE(OceanicPlate, oceanic plate)
  }
}

