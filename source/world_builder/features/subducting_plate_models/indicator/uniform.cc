/*
  Copyright (C) 2018-2026 by the authors of the World Builder code.

  This file is part of the World Builder.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.
*/

#include "world_builder/features/subducting_plate_models/indicator/uniform.h"

#include "world_builder/nan.h"
#include "world_builder/types/array.h"
#include "world_builder/types/double.h"
#include "world_builder/types/object.h"
#include "world_builder/types/one_of.h"
#include "world_builder/types/string.h"
#include "world_builder/types/unsigned_int.h"
#include "world_builder/world.h"

namespace WorldBuilder
{
  using namespace Utilities;

  namespace Features
  {
    namespace SubductingPlateModels
    {
      namespace Indicator
      {
        Uniform::Uniform(World *world_)
          : min_distance_slab_top(NaN::DSNAN),
            max_distance_slab_top(NaN::DSNAN),
            operation(Operations::REPLACE)
        {
          world = world_;
          name = "uniform";
        }

        Uniform::~Uniform() = default;

        void Uniform::declare_entries(Parameters &prm, const std::string & /*unused*/)
        {
          prm.declare_entry("", Types::Object(),
                            "A distance-dependent indicator model. Sets a constant indicator value of 1 "
                            "for the selected user-defined indicators.");
          prm.declare_entry("min distance slab top", Types::Double(0),
                            "The minimum distance in meters from the slab top where this model applies.");
          prm.declare_entry("max distance slab top", Types::Double(std::numeric_limits<double>::max()),
                            "The maximum distance in meters from the slab top where this model applies.");
          prm.declare_entry("indicators", Types::Array(Types::OneOf(Types::UnsignedInt(0), Types::String("")), 0),
                            "A list of user-defined indicator indices or names. If omitted, indicator 0 is selected. "
                            "An explicitly empty list selects no indicators.");
          prm.declare_entry("operation", Types::String("replace", std::vector<std::string> {"replace", "replace defined only"}),
                            "Whether all indicator values are replaced or only explicitly selected indicators are replaced.");
        }

        void Uniform::parse_entries(Parameters &prm)
        {
          min_distance_slab_top = prm.get<double>("min distance slab top");
          max_distance_slab_top = prm.get<double>("max distance slab top");
          WBAssert(max_distance_slab_top >= min_distance_slab_top,
                   "max distance slab top needs to be larger than or equal to min distance slab top.");

          if (prm.check_entry("indicators"))
            indicators = prm.get_vector<unsigned int>("indicators", world->indicator_properties);
          else
            {
              constexpr unsigned int default_indicator = 0;
              WBAssertThrow(world->indicator_properties.find(default_indicator) != world->indicator_properties.end(),
                            "Invalid default indicator index " << default_indicator
                            << ": no matching entry exists in indicator properties.");
              indicators = {default_indicator};
            }

          operation = string_operations_to_enum(prm.get<std::string>("operation"));
        }

        double Uniform::get_indicator(const Point<3> & /*position*/,
                                      const double /*depth*/,
                                      const unsigned int indicator_number,
                                      double indicator,
                                      const double /*feature_min_depth*/,
                                      const double /*feature_max_depth*/,
                                      const Utilities::PointDistanceFromCurvedPlanes &distance_from_planes,
                                      const AdditionalParameters & /*additional_parameters*/) const
        {
          if (distance_from_planes.distance_from_plane >= min_distance_slab_top
              && distance_from_planes.distance_from_plane <= max_distance_slab_top)
            {
              for (const unsigned int selected_indicator : indicators)
                if (selected_indicator == indicator_number)
                  return apply_operation(operation, indicator, 1.0);

              if (operation == Operations::REPLACE)
                return 0.0;
            }

          return indicator;
        }

        WB_REGISTER_FEATURE_SUBDUCTING_PLATE_INDICATOR_MODEL(Uniform, uniform)
      } // namespace Indicator
    } // namespace SubductingPlateModels
  } // namespace Features
} // namespace WorldBuilder
