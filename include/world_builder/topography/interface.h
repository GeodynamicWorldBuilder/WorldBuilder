/*
  Copyright (C) 2018-2025 by the authors of the World Builder code.

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

#ifndef WORLD_BUILDER_TOPOGRAPHY_INTERFACE_H
#define WORLD_BUILDER_TOPOGRAPHY_INTERFACE_H

#include "world_builder/topography/interface.h"

#include "world_builder/parameters.h"
#include "world_builder/types/string.h"
#include "world_builder/objects/natural_coordinate.h"


namespace WorldBuilder
{
  class World;

  namespace Topography
  {

    class ObjectFactory;

    /**
     * This class is an interface for the topography.
     */
    class Interface
    {
      public:
        /**
         * Destructor
         */
        virtual
        ~Interface() = default;

        /**
         * declare and read in the world builder file into the topography class
         */
        static
        void declare_entries(Parameters &prm,
                             const std::string &parent_name,
                             const std::vector<std::string> &required_entries);

        /**
         * declare and read in the world builder file into the topography class
         */
        virtual
        void parse_entries(Parameters &prm) = 0;

        /**
         * A function to register a new type. This is part of the automatic
         * registration of the object factory.
         */
        static void registerType(const std::string &name,
                                 void ( * /*declare_entries*/)(Parameters &, const std::string &),
                                 ObjectFactory *factory);

        /**
         * A function to create a new type. This is part of the automatic
         * registration of the object factory.
         */
        static std::unique_ptr<Interface> create(const std::string &name, WorldBuilder::World *world);

        enum SurfaceType
        {
          reference_surface,
          deformed_surface
        };

        SurfaceType surface_type;

        /**
         * declare and read in the world builder file into the parameters class
         */
        virtual
        SurfaceType
        get_surface_type() = 0;

        /**
         * Returns a temperature based on the given position, depth in the model,
         * gravity and current temperature.
         */
        virtual
        double get_topography(const Point<3> &position,
                              const Objects::NaturalCoordinate &position_in_natural_coordinates,
                              const double depth) const = 0;

      protected:
        /**
         * A pointer to the world class to retrieve variables.
         */
        WorldBuilder::World *world;



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
#define WB_REGISTER_TOPOGRAPHY(klass,name) \
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

  } // namespace Topography
} // namespace WorldBuilder

#endif
