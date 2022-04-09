/*
  Copyright (C) 2018-2021 by the authors of the World Builder code.

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

#include "world_builder/types/interface.h"



namespace WorldBuilder
{

  std::ostream &Types::operator<< (std::ostream &os, const Types::type &type)
  {
    switch (type)
      {
        case Types::type::None:
          os << "None";
          break;
        case Types::type::Array:
          os << "Array";
          break;
        case Types::type::Bool:
          os << "Bool";
          break;
        case Types::type::String:
          os << "String";
          break;
        case Types::type::Double:
          os << "Double";
          break;
        case Types::type::Int:
          os << "Int";
          break;
        case Types::type::UnsignedInt:
          os << "UnsignedInt";
          break;
        case Types::type::Object:
          os << "Object";
          break;
        case Types::type::List:
          os << "List";
          break;
        case Types::type::Point2D:
          os << "Point2D";
          break;
        case Types::type::Point3D:
          os << "Point3D";
          break;
        case Types::type::CoordinateSystem:
          os << "CoordinateSystem";
          break;
        case Types::type::PluginSystem:
          os << "PluginSystem";
          break;
        case Types::type::Segment:
          os << "Segment";
          break;
        case Types::type::ConstantLayer:
          os << "ConstantLayer";
          break;
        case Types::type::ValueAtPoints:
          os << "ValueAtPoints";
          break;
        case Types::type::OneOf:
          os << "OneOf";
          break;
        default:
          ;
      }
    return os;
  }

  namespace Types
  {
    Interface::Interface()


      = default;

    Interface::~Interface ()
      = default;


    type
    Interface::get_type() const
    {
      return type_name;
    }
  } // namespace Types
} // namespace WorldBuilder

