/*
  Copyright (C) 2018-2026 by the authors of the World Builder code.

  This file is part of the World Builder.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef WORLD_BUILDER_FEATURES_SUBDUCTING_PLATE_MODELS_INDICATOR_UNIFORM_H
#define WORLD_BUILDER_FEATURES_SUBDUCTING_PLATE_MODELS_INDICATOR_UNIFORM_H

#include "world_builder/features/subducting_plate_models/indicator/interface.h"

namespace WorldBuilder
{
  namespace Features
  {
    namespace SubductingPlateModels
    {
      namespace Indicator
      {
        class Uniform final : public Interface
        {
          public:
            Uniform(World *world);
            ~Uniform() override final;

            static void declare_entries(Parameters &prm, const std::string &parent_name = "");
            void parse_entries(Parameters &prm) override final;

            double get_indicator(const Point<3> &position,
                                 const double depth,
                                 const unsigned int indicator_number,
                                 double indicator,
                                 const double feature_min_depth,
                                 const double feature_max_depth,
                                 const Utilities::PointDistanceFromCurvedPlanes &distance_from_planes,
                                 const AdditionalParameters &additional_parameters) const override final;

          private:
            double min_distance_slab_top;
            double max_distance_slab_top;
            std::vector<unsigned int> indicators;
            Operations operation;
        };
      } // namespace Indicator
    } // namespace SubductingPlateModels
  } // namespace Features
} // namespace WorldBuilder

#endif
