/*
  Copyright (C) 2018-2026 by the authors of the World Builder code.

  This file is part of the World Builder.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.
*/

#ifndef WORLD_BUILDER_FEATURES_SUBDUCTING_PLATE_MODELS_INDICATOR_INTERFACE_H
#define WORLD_BUILDER_FEATURES_SUBDUCTING_PLATE_MODELS_INDICATOR_INTERFACE_H

#include "world_builder/features/feature_utilities.h"
#include "world_builder/parameters.h"
#include "world_builder/utilities.h"

#include <map>
#include <vector>

namespace WorldBuilder
{
  class World;
  class Parameters;
  template <unsigned int dim> class Point;

  namespace Features
  {
    using namespace FeatureUtilities;

    namespace SubductingPlateModels
    {
      namespace Indicator
      {
        class ObjectFactory;

        class Interface
        {
          public:
            Interface();
            virtual ~Interface();

            static void declare_entries(Parameters &prm,
                                        const std::string &parent_name,
                                        const std::vector<std::string> &required_entries);

            virtual void parse_entries(Parameters &prm) = 0;

            virtual double get_indicator(const Point<3> &position,
                                         const double depth,
                                         const unsigned int indicator_number,
                                         double indicator,
                                         const double feature_min_depth,
                                         const double feature_max_depth,
                                         const Utilities::PointDistanceFromCurvedPlanes &distance_from_planes,
                                         const AdditionalParameters &additional_parameters) const = 0;

            static void registerType(const std::string &name,
                                     void (*declare_entries)(Parameters &, const std::string &),
                                     ObjectFactory *factory);

            static std::unique_ptr<Interface> create(const std::string &name, World *world);

            std::string get_name() const
            {
              return name;
            }

          protected:
            World *world;
            std::string name;

          private:
            static std::map<std::string, ObjectFactory *> &get_factory_map()
            {
              static std::map<std::string, ObjectFactory *> factories;
              return factories;
            }

            static std::map<std::string, void (*)(Parameters &, const std::string &)> &get_declare_map()
            {
              static std::map<std::string, void (*)(Parameters &, const std::string &)> declares;
              return declares;
            }
        };

        class ObjectFactory
        {
          public:
            virtual std::unique_ptr<Interface> create(World *world) = 0;
        };

#define WB_REGISTER_FEATURE_SUBDUCTING_PLATE_INDICATOR_MODEL(klass,name) \
  class klass##Factory : public ObjectFactory { \
    public: \
      klass##Factory() \
      { \
        Interface::registerType(#name, klass::declare_entries, this); \
      } \
      std::unique_ptr<Interface> create(World *world) override final { \
        return std::unique_ptr<Interface>(new klass(world)); \
      } \
  }; \
  static klass##Factory global_##klass##Factory;

      } // namespace Indicator
    } // namespace SubductingPlateModels
  } // namespace Features
} // namespace WorldBuilder

#endif
