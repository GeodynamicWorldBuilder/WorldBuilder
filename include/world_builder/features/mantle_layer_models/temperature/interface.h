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

#ifndef _world_builder_features_mantle_layer_temperature_interface_h
#define _world_builder_features_mantle_layer_temperature_interface_h

#include <vector>
#include <map>

#include <world_builder/world.h>
#include <world_builder/parameters.h>
#include <world_builder/point.h>


namespace WorldBuilder
{
  class World;

  /**
   * This class is an interface for the specific plate tectonic feature classes,
   * such as continental plate, oceanic plate and subduction zone.
   */
  namespace Features
  {

    namespace MantleLayerModels
    {
      namespace Temperature
      {
        class ObjectFactory;

        class Interface
        {
          public:
            /**
             * constructor
             */
            Interface();

            /**
             * Destructor
             */
            virtual
            ~Interface();

            /**
             * declare and read in the world builder file into the parameters class
             */
            static
            void declare_entries(Parameters &prm,
                                 const std::string &parent_name,
                                 const std::vector<std::string> &required_entries);

            /**
             * declare and read in the world builder file into the parameters class
             */
            virtual
            void parse_entries(Parameters &prm) = 0;


            /**
             * takes temperature and position and returns a temperature.
             */
            virtual
            double get_temperature(const Point<3> &position,
                                   const double depth,
                                   const double gravity,
                                   double temperature,
                                   const double feature_min_depth,
                                   const double feature_max_depth) const = 0;
            /**
             * A function to register a new type. This is part of the automatic
             * registration of the object factory.
             */
            static void registerType(const std::string &name,
                                     void ( *)(Parameters &, const std::string &),
                                     ObjectFactory *factory);


            /**
             * A function to create a new type. This is part of the automatic
             * registration of the object factory.
             */
            static std::unique_ptr<Interface> create(const std::string &name, WorldBuilder::World *world);

            /**
             * Returns the name of the plugin
             */
            std::string get_name() const
            {
              return name;
            };

          protected:
            /**
             * A pointer to the world class to retrieve variables.
             */
            WorldBuilder::World *world;

            /**
             * The name of the feature type.
             */
            std::string name;

          private:
            static std::map<std::string, ObjectFactory *> &get_factory_map()
            {
              static std::map<std::string, ObjectFactory *> factories;
              return factories;
            }

            static std::map<std::string, void ( *)(Parameters &,const std::string &)> &get_declare_map()
            {
              static std::map<std::string, void ( *)(Parameters &,const std::string &)> declares;
              return declares;
            }

        };


        /**
         * A class to create new objects
         */
        class ObjectFactory
        {
          public:
            virtual std::unique_ptr<Interface> create(World *world) = 0;
        };

        /**
         * A macro which should be in every derived cpp file to automatically
         * register it. Because this is a library, we need some extra measures
         * to ensure that the static variable is actually initialized.
         */
#define WB_REGISTER_FEATURE_MANTLE_LAYER_TEMPERATURE_MODEL(klass,name) \
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

      }
    }
  }
}

#endif
