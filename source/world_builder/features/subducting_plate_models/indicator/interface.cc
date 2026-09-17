/*
  Copyright (C) 2018-2026 by the authors of the World Builder code.

  This file is part of the World Builder.

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published
  by the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.
*/

#include "world_builder/features/subducting_plate_models/indicator/interface.h"

#include <algorithm>

namespace WorldBuilder
{
  namespace Features
  {
    namespace SubductingPlateModels
    {
      namespace Indicator
      {
        Interface::Interface() = default;
        Interface::~Interface() = default;

        void Interface::declare_entries(Parameters &prm,
                                        const std::string &parent_name,
                                        const std::vector<std::string> &required_entries)
        {
          prm.declare_model_entries("indicator", parent_name, get_declare_map(), required_entries);
        }

        void Interface::registerType(const std::string &name,
                                     void (*declare_entries)(Parameters &, const std::string &),
                                     ObjectFactory *factory)
        {
          get_factory_map()[name] = factory;
          get_declare_map()[name] = declare_entries;
        }

        std::unique_ptr<Interface> Interface::create(const std::string &name, World *world)
        {
          std::string lower_case_name;
          std::transform(name.begin(), name.end(), std::back_inserter(lower_case_name), ::tolower);

          WBAssertThrow(get_factory_map().find(lower_case_name) != get_factory_map().end(),
                        "Internal error: Plugin with name '" << lower_case_name << "' is not found. "
                        "The size of factories is " << get_factory_map().size() << '.');

          return get_factory_map().at(lower_case_name)->create(world);
        }
      } // namespace Indicator
    } // namespace SubductingPlateModels
  } // namespace Features
} // namespace WorldBuilder
