/*
  Copyright (C) 2018 - 2021 by the authors of the World Builder code.

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

#include "world_builder/features/subducting_plate_models/composition/water_content.h"

#include "world_builder/nan.h"
#include "world_builder/types/array.h"
#include "world_builder/types/double.h"
#include "world_builder/types/object.h"
#include "world_builder/types/unsigned_int.h"
#include "world_builder/utilities.h"


namespace WorldBuilder
{

  using namespace Utilities;

  namespace Features
  {
    namespace SubductingPlateModels
    {
      namespace Composition
      {
        WaterContent::WaterContent(WorldBuilder::World *world_)
          :
          min_depth(NaN::DSNAN),
          max_depth(NaN::DSNAN),
          density(NaN::DSNAN)
        {
          this->world = world_;
          this->name = "water content";
        }

        WaterContent::~WaterContent()
          = default;

        void
        WaterContent::declare_entries(Parameters &prm, const std::string & /*unused*/)
        {
          // Document plugin and require entries if needed.
          // Add compositions to the required parameters.
          prm.declare_entry("", Types::Object({"compositions"}),
                            "WaterContent compositional model. Sets constant compositional field.");

          // Declare entries of this plugin
          prm.declare_entry("min distance slab top", Types::Double(0),
                            "todo The depth in meters from which the composition of this feature is present.");
          prm.declare_entry("max distance slab top", Types::Double(std::numeric_limits<double>::max()),
                            "todo The depth in meters to which the composition of this feature is present.");
          prm.declare_entry("density", Types::Double(3000.0),
                            "The reference density used to estimate the lithostatic pressure for calculating "
                            "the bound water content.");
          prm.declare_entry("compositions", Types::Array(Types::UnsignedInt(),0),
                            "A list with the labels of the composition which are present there.");
          prm.declare_entry("lithology",  Types::String("peridotite"),
                            "The lithology used to determine which polynomials to use for calculating the water content.");
          prm.declare_entry("initial water content", Types::Double(5),
                            "The value of the initial water content (in wt%) for the lithology at the trench. This is essentially the "
                            "max value applied to this lithology.");
          prm.declare_entry("operation", Types::String("replace", std::vector<std::string> {"replace", "replace defined only", "add", "subtract"}),
                            "Whether the value should replace any value previously defined at this location (replace) or "
                            "add the value to the previously define value. Replacing implies that all compositions not "
                            "explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.");

        }

        void
        WaterContent::parse_entries(Parameters &prm)
        {
          min_depth = prm.get<double>("min distance slab top");
          max_depth = prm.get<double>("max distance slab top");
          density = prm.get<double>("density");
          compositions = prm.get_vector<unsigned int>("compositions");
          max_water_content = prm.get<double>("initial water content");
          lithology_str = prm.get<std::string>("lithology");
          operation = string_operations_to_enum(prm.get<std::string>("operation"));
        }


        double
        WaterContent::calculate_water_content(double pressure,
                                              double temperature,
                                              unsigned int lithology_index) const
        {
          double inv_pressure = 1/pressure;
          double ln_LR_value = 0;
          double ln_c_sat_value = 0;
          double Td_value = 0;
          std::vector<double> LR_polynomial_coeffs;
          std::vector<double> c_sat_polynomial_coeffs;
          std::vector<double> Td_polynomial_coeffs;

          if (lithology_index == 0)
            {
              LR_polynomial_coeffs = LR_poly_peridotite;
              c_sat_polynomial_coeffs = c_sat_poly_peridotite;
              Td_polynomial_coeffs = Td_poly_peridotite;
            }

          if (lithology_index == 1)
            {
              LR_polynomial_coeffs = LR_poly_gabbro;
              c_sat_polynomial_coeffs = c_sat_poly_gabbro;
              Td_polynomial_coeffs = Td_poly_gabbro;
            }

          if (lithology_index == 2)
            {
              LR_polynomial_coeffs = LR_poly_MORB;
              c_sat_polynomial_coeffs = c_sat_poly_MORB;
              Td_polynomial_coeffs = Td_poly_MORB;
            }

          if (lithology_index == 3)
            {
              LR_polynomial_coeffs = LR_poly_sediment;
              c_sat_polynomial_coeffs = c_sat_poly_sediment;
              Td_polynomial_coeffs = Td_poly_sediment;
            }

          // Calculate the c_sat value from Tian et al., 2019
          if (lithology_index == 3)
            {
              for (unsigned int c_sat_index = 0; c_sat_index < c_sat_polynomial_coeffs.size(); ++c_sat_index)
                ln_c_sat_value += c_sat_polynomial_coeffs[c_sat_index] * (std::pow(std::log10(pressure), c_sat_polynomial_coeffs.size() - 1 - c_sat_index));
            }
          else
            {
              for (unsigned int c_sat_index = 0; c_sat_index < c_sat_polynomial_coeffs.size(); ++c_sat_index)
                ln_c_sat_value += c_sat_polynomial_coeffs[c_sat_index] * (std::pow(pressure, c_sat_polynomial_coeffs.size() - 1 - c_sat_index));
            }

          // Calculate the LR value from Tian et al., 2019
          for (unsigned int LR_coeff_index = 0; LR_coeff_index < LR_polynomial_coeffs.size(); ++LR_coeff_index)
            ln_LR_value += LR_polynomial_coeffs[LR_coeff_index] * (std::pow(inv_pressure, LR_polynomial_coeffs.size() - 1 - LR_coeff_index));

          // Calculate the Td value from Tian et al., 2019
          for (unsigned int Td_coeff_index = 0; Td_coeff_index < Td_polynomial_coeffs.size(); ++Td_coeff_index)
            Td_value += Td_polynomial_coeffs[Td_coeff_index] * (std::pow(pressure, Td_polynomial_coeffs.size() - 1 - Td_coeff_index));

          double partition_coeff = std::exp(ln_c_sat_value) * std::exp(std::exp(ln_LR_value) * (1/temperature - 1/Td_value));
          return partition_coeff;
        }


        double
        WaterContent::get_composition(const Point<3> &position_in_cartesian_coordinates,
                                      const double depth,
                                      const unsigned int composition_number,
                                      double composition,
                                      const double  /*feature_min_depth*/,
                                      const double  /*feature_max_depth*/,
                                      const WorldBuilder::Utilities::PointDistanceFromCurvedPlanes &distance_from_plane,
                                      // std::string lithology_str,
                                      const AdditionalParameters & /*additional_parameters*/) const
        {
          if (distance_from_plane.distance_from_plane <= max_depth && distance_from_plane.distance_from_plane >= min_depth)
            {
              unsigned int lithology_index;
              if (lithology_str == "peridotite")
                {
                  lithology_index = 0;
                }
              if (lithology_str == "gabbro")
                {
                  lithology_index = 1;
                }
              if (lithology_str == "MORB")
                {
                  lithology_index = 2;
                }
              if (lithology_str == "sediment")
                {
                  lithology_index = 3;
                }

              for (unsigned int i = 0; i < compositions.size(); ++i)
                {
                  if (compositions[i] == composition_number)
                    {
                      // return apply_operation(operation,composition, partition_coefficient);
                      return apply_operation(operation,composition,distance_from_plane.water_content[lithology_index]);
                    }
                }

              if (operation == Operations::REPLACE)
                {
                  return 0.0;
                }
            }
          return composition;
        }
        WB_REGISTER_FEATURE_SUBDUCTING_PLATE_COMPOSITION_MODEL(WaterContent, water content)
      } // namespace Composition
    } // namespace SubductingPlateModels
  } // namespace Features
} // namespace WorldBuilder


