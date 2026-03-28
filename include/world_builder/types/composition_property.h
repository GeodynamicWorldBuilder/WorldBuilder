/*
  Copyright (C) 2018-2026 by the authors of the World Builder code.

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

#ifndef WORLD_BUILDER_TYPES_COMPOSITIONPROPERTY_H
#define WORLD_BUILDER_TYPES_COMPOSITIONPROPERTY_H


#include "world_builder/types/interface.h"


namespace WorldBuilder
{
  class Parameters;

  namespace Types
  {
    /**
    * Schema type for a single composition-property object.
    * Expected keys are:
    * - index (required)
    * - name (optional, defaults to index as string)
    * - reference density (optional, defaults to 3300)
    */
    class CompositionProperty final: public Interface
    {
      public:

        /**
         * A constructor used for the load_entry function
         */
        CompositionProperty();

        CompositionProperty(const CompositionProperty &) = default;

        ~CompositionProperty() override final;

        void write_schema(Parameters &prm,
                          const std::string &name,
                          const std::string &documentation) const override final;

        // Static accessor for default reference density
        static constexpr double get_default_reference_density()
        {
          return 3300.0;
        }

        std::vector<std::string> required;
        double default_reference_density;

      protected:
        CompositionProperty *clone_impl() const override final
        {
          return new CompositionProperty(*this);
        };

      private:

    };
  } // namespace Types
} // namespace WorldBuilder

#endif
