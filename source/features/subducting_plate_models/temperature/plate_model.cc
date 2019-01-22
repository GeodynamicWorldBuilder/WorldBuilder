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

#include <world_builder/utilities.h>
#include <world_builder/assert.h>
#include <world_builder/nan.h>
#include <world_builder/parameters.h>

#include <world_builder/types/array.h>
#include <world_builder/types/double.h>
#include <world_builder/types/point.h>
#include <world_builder/types/string.h>
#include <world_builder/types/object.h>
#include <world_builder/features/subducting_plate_models/temperature/plate_model.h>


namespace WorldBuilder
{
  using namespace Utilities;

  namespace Features
  {
    namespace SubductingPlateModels
    {
      namespace Temperature
      {
        PlateModel::PlateModel(WorldBuilder::World *world_)
          :
          min_depth(NaN::DSNAN),
          max_depth(NaN::DSNAN),
          //top_temperature(NaN::DSNAN),
          //bottom_temperature(NaN::DSNAN),
          //spreading_velocity(NaN::DSNAN),
          operation("")
        {
          this->world = world_;
          this->name = "plate model";
        }

        PlateModel::~PlateModel()
        { }

        void
        PlateModel::declare_entries(Parameters &prm, const std::string &)
        {

          // Add temperature to the required parameters.
          prm.declare_entry("", Types::Object({"plate velocity"}), "Temperature model object");


          prm.declare_entry("min distance slab top", Types::Double(0),
                            "todo The depth in meters from which the composition of this feature is present.");

          prm.declare_entry("max distance slab top", Types::Double(std::numeric_limits<double>::max()),
                            "todo The depth in meters to which the composition of this feature is present.");

          prm.declare_entry("density", Types::Double(3300),
                            "The reference density of the subducting plate in $kg/m^3$");

          prm.declare_entry("plate velocity", Types::Double(NaN::DQNAN),
                            "The velocity in meters per year with which the plate subducts in meters per year.");

          prm.declare_entry("thermal conductivity", Types::Double(2.0),
                            "The thermal conductivity of the subducting plate material in $W m^{-1} K^{-1}$.");

          prm.declare_entry("thermal expansion coefficient", Types::Double(3.5e-5),
                            "The thermal expansivity of the subducting plate material in $K^{-1}$.");

          prm.declare_entry("specific heat", Types::Double(1250),
                            "The specific heat of the subducting plate material in $J kg^{-1} K^{-1}$.");

          prm.declare_entry("operation", Types::String("replace", std::vector<std::string> {"replace", "add", "substract"}),
                            "Whether the value should replace any value previously defined at this location (replace), "
                            "add the value to the previously define value (add) or substract the value to the previously "
                            "define value (substract).");
        }

        void
        PlateModel::parse_entries(Parameters &prm)
        {

          min_depth = prm.get<double>("min distance slab top");
          max_depth = prm.get<double>("max distance slab top");
          operation = prm.get<std::string>("operation");
          //top_temperature = prm.get<double>("top temperature");
          //bottom_temperature = prm.get<double>("bottom temperature");
          //spreading_velocity = prm.get<double>("spreading velocity")/31557600;
          //ridge_coordinates = prm.get_vector<Point<2> >("ridge coordinates");


          density = prm.get<double>("density");

          plate_velocity = prm.get<double>("plate velocity");

          thermal_conductivity = prm.get<double>("thermal conductivity");

          thermal_expansion_coefficient = prm.get<double>("thermal expansion coefficient");

          specific_heat = prm.get<double>("specific heat");


          potential_mantle_temperature = this->world->potential_mantle_temperature;
          surface_temperature = this->world->surface_temperature;

          /*
           prm.load_entry("depth", true, Types::Double(NaN::DSNAN,"The depth in meters to which the temperature rises (or lowers) to."));
          temperature_submodule_plate_model_depth = prm.get_double("depth");

          prm.load_entry("top temperature", false, Types::Double(293.15,"The temperature in degree Kelvin a the top of this block. If this value is not set, the "));
          temperature_submodule_plate_model_top_temperature = prm.get_double("top temperature");


          prm.load_entry("bottom temperature", false, Types::Double(NaN::DQNAN,"The temperature in degree Kelvin a the bottom of this block."));
          temperature_submodule_plate_model_bottom_temperature = prm.get_double("bottom temperature");

          prm.load_entry("ridge points", true,
                         Types::Array(Types::Point<2>(Point<2>({0,0},coordinate_system),
                                                      "A 2d point on the line where the subducting ridge is located."),
                                      "A list of 2d points which define the location of the ridge."));
          std::vector<Types::Point<2> > temp_ridge_points = prm.get_array<Types::Point<2> >("ridge points");

          const double dtr = prm.coordinate_system->natural_coordinate_system() == spherical ? const_pi / 180.0 : 1.0;
          WBAssertThrow(temp_ridge_points.size() >= 2,
                        "Need at least two points to form the ridge of the subducting plate, "
                        << temp_ridge_points.size() << " points where given.");
          temperature_submodule_plate_model_ridge_points.resize(temp_ridge_points.size(),Point<2>(coordinate_system));
          for (unsigned int i = 0; i < temp_ridge_points.size(); ++i)
            {
              temperature_submodule_plate_model_ridge_points[i] = temp_ridge_points[i].value * dtr;
            }

          prm.load_entry("spreading velocity", true, Types::Double(NaN::DSNAN,
                                                                   "The spreading velocity of the plate in meter per year. "
                                                                   "This is the velocity with which one side moves away from the ridge."));
          // directly convert from meter per year to meter per second.
          temperature_submodule_plate_model_spreading_velocity = prm.get_double("spreading velocity")/31557600;
           */
        }


        double
        PlateModel::get_temperature(const Point<3> &,
                                    const double,
                                    const double gravity_norm,
                                    double temperature_,
                                    const double,
                                    const double,
                                    const std::map<std::string,double> &distance_from_planes) const
        {

          double temperature = temperature_;
          const double thickness_local = distance_from_planes.at("thicknessLocal");
          const double distance_from_plane = distance_from_planes.at("distanceFromPlane");
          const double distance_along_plane = distance_from_planes.at("distanceAlongPlane");
          const double average_angle = distance_from_planes.at("averageAngle");

          /*
           * We now use the McKenzie (1970) equation to determine the
           * temperature inside the slab. The McKenzie equation was
           * designed for a straight slab, but we have a potentially
           * curved slab. Because the angle is a required parameter, we
           * first tried a local angle. This gave weird effects of
           * apparent cooling when the slabs angle decreases. Now we
           * use an average angle, which works better.
           */
          const double R = (density * specific_heat
                            * (plate_velocity /(365.25 * 24.0 * 60.0 * 60.0))
                            * thickness_local) / (2.0 * thermal_conductivity);

          WBAssert(!std::isnan(R), "Internal error: R is not a number: " << R << ".");

          // gravity in original in cm/s^2, here in m/s^2, thickness original in km, here in meter. So 100/1000=0.1
          const double H = specific_heat
                           / (thermal_expansion_coefficient * gravity_norm * 0.1 * thickness_local);

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



          if (operation == "replace")
            return temperature;
          else if ("add")
            return temperature_ + temperature;
          else if ("substract")
            return temperature_ - temperature;


          return temperature_;
        }

        WB_REGISTER_FEATURE_CONTINENTAL_TEMPERATURE_MODEL(PlateModel, plate model)
      }
    }
  }
}

