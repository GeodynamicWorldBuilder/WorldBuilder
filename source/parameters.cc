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
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>

#include <boost/property_tree/json_parser.hpp>

#include <rapidjson/istreamwrapper.h>
#include "rapidjson/pointer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"

#include <world_builder/assert.h>
#include <world_builder/config.h>
#include <world_builder/parameters.h>
#include <world_builder/utilities.h>

#include <world_builder/types/point.h>
#include <world_builder/types/double.h>
#include <world_builder/types/string.h>
#include <world_builder/types/segment.h>
#include <world_builder/types/array.h>
#include <world_builder/types/unsigned_int.h>
#include <world_builder/types/plugin_system.h>

#include <world_builder/features/continental_plate_models/temperature/interface.h>
#include <world_builder/features/continental_plate_models/composition/interface.h>
#include <world_builder/features/oceanic_plate_models/temperature/interface.h>
#include <world_builder/features/oceanic_plate_models/composition/interface.h>
#include <world_builder/features/mantle_layer_models/temperature/interface.h>
#include <world_builder/features/mantle_layer_models/composition/interface.h>
#include <world_builder/features/subducting_plate_models/temperature/interface.h>
#include <world_builder/features/subducting_plate_models/composition/interface.h>

#include <world_builder/features/subducting_plate.h>
#include <world_builder/features/subducting_plate_models/temperature/interface.h>
#include <world_builder/features/subducting_plate_models/composition/interface.h>
#include <world_builder/features/fault.h>
#include <world_builder/features/fault_models/temperature/interface.h>
#include <world_builder/features/fault_models/composition/interface.h>

using boost::property_tree::ptree;
using namespace rapidjson;

namespace WorldBuilder
{
  Parameters::Parameters(World &world)
    :
    world(world)
  {
  }

  Parameters::~Parameters()
  {}

  void Parameters::initialize(std::string &filename)
  {
    {
      StringBuffer buffer;
      PrettyWriter<StringBuffer, UTF8<>, UTF8<>, CrtAllocator, kWriteNanAndInfFlag> writer(buffer);
      declarations.Accept(writer);

      //std::cout << buffer.GetString() << std::endl;
    }

    path_level =0;
    // Now read in the world builder file into a file stream and
    // put it into a boost property tree.
    std::ifstream json_input_stream(filename.c_str());

    // Get world builder file and check wether it exists
    WBAssertThrow(json_input_stream.good(),
                  "Could not find the world builder file at the specified location: " + filename);

    WBAssert(json_input_stream, "Could not read the world builder file.");

    //boost::property_tree::json_parser::read_json (json_input_stream, tree);
    std::ifstream json_input_stream2(filename.c_str());
    WBAssert(json_input_stream, "Could not read the world builder file.");

    rapidjson::IStreamWrapper isw(json_input_stream2);

    // relaxing sytax by allowing comments () for now, maybe also allow trailing commas and (kParseTrailingCommasFlag) and nan's, inf etc (kParseNanAndInfFlag)?
    //WBAssertThrow(!parameters.ParseStream<kParseCommentsFlag>(isw).HasParseError(), "Parsing erros world builder file");

    if (parameters.ParseStream<kParseCommentsFlag | kParseNanAndInfFlag>(isw).HasParseError())
      {
        fprintf(stderr, "\nError(offset %u): %s\n",
                (unsigned)parameters.GetErrorOffset(),
                GetParseError_En(parameters.GetParseError()));
        WBAssertThrow(false, "Parsing errors world builder file");

      }
    WBAssertThrow(parameters.IsObject(), "it is not an object.");
    json_input_stream.close();
    //boost::property_tree::json_parser::read_json (json_input_stream, tree);
    local_tree = &tree;


    SchemaDocument schema(declarations);
    SchemaValidator validator(schema);

    if (!parameters.Accept(validator))
      {
        // Input JSON is invalid according to the schema
        // Output diagnostic information
        StringBuffer sb;
        validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
        printf("Invalid schema: %s\n", sb.GetString());
        printf("Invalid keyword: %s\n", validator.GetInvalidSchemaKeyword());
        sb.Clear();
        validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
        printf("Invalid document: %s\n", sb.GetString());
        StringBuffer buffer;
        PrettyWriter<StringBuffer> writer(buffer);
        validator.GetError().Accept(writer);
        WBAssert(false, "Error document: " << std::endl << buffer.GetString());
      }
    else
      {
        //std::cout << "schema says it is correct, yay!" << std::endl;
      }
    {
      StringBuffer buffer;
      PrettyWriter<StringBuffer> writer(buffer);

      parameters.Accept(writer);
      //std::cout << "parameters file: " << std::endl << buffer.GetString() << std::endl;
    }

  }

  void
  Parameters::declare_entry(const std::string name,
                            const Types::Interface &type,
                            const std::string documentation)
  {
    type.write_schema(*this,name,documentation);
  }

  bool
  Parameters::check_entry(const std::string &name) const
  {
    return Pointer((this->get_full_json_path() + "/" + name).c_str()).Get(parameters) == NULL ? false : true;
  }


  template<>
  std::string
  Parameters::get(const std::string &name)
  {
    const std::string base = this->get_full_json_path();
    const Value *value = Pointer((base + "/" + name).c_str()).Get(parameters);

    bool required = false;
    if (Pointer((base + "/required").c_str()).Get(declarations) != NULL)
      {
        for (auto &v : Pointer((base + "/required").c_str()).Get(declarations)->GetArray())
          {
            if (v.GetString() == name)
              {
                required = true;
              }
          }
      }

    WBAssert(value != NULL || required == false,
             "Internal error: Value \"" << base << "/" << name << "/type\" not found in the input file, while it was set as required.");

    if (value == NULL)
      {
        value = Pointer((get_full_json_schema_path() + "/" + name + "/default value").c_str()).Get(declarations);
        WBAssert(value != NULL,
                 "internal error: could not retrieve the default value at: "
                 << base + "/" + name + "/default value");
      }

    return value->GetString();
  }

  template<>
  double
  Parameters::get(const std::string &name)
  {
    const std::string base = this->get_full_json_path();
    const Value *value = Pointer((base + "/" + name).c_str()).Get(parameters);

    bool required = false;
    if (Pointer((base + "/required").c_str()).Get(declarations) != NULL)
      {
        for (auto &v : Pointer((base + "/required").c_str()).Get(declarations)->GetArray())
          {
            if (v.GetString() == name)
              {
                required = true;
              }
          }
      }

    WBAssert(value != NULL || required == false,
             "Internal error: Value \"" << base << "/" << name << "/type\" not found in the input file, while it was set as required.");

    if (value == NULL)
      {
        //std::cout << "full json schema path = " << this->get_full_json_schema_path() << std::endl;
        value = Pointer((get_full_json_schema_path() + "/" + name + "/default value").c_str()).Get(declarations);
        WBAssert(value != NULL,
                 "internal error: could not retrieve the default value at: "
                 << get_full_json_schema_path() + "/" + name + "/default value, for value: " << base + "/" + name);
      }

    double return_value;
    try
      {
        return_value = value->GetDouble();
      }
    catch (...)
      {
        WBAssertThrow(false, "Could not convert values of " << base << " into doubles.");
      }
    return return_value;
  }

  template<>
  unsigned int
  Parameters::get(const std::string &name)
  {
    const std::string base = this->get_full_json_path();
    const Value *value = Pointer((base + "/" + name).c_str()).Get(parameters);

    bool required = false;
    if (Pointer((base + "/required").c_str()).Get(declarations) != NULL)
      {
        for (auto &v : Pointer((base + "/required").c_str()).Get(declarations)->GetArray())
          {
            if (v.GetString() == name)
              {
                required = true;
              }
          }
      }

    WBAssert(value != NULL || required == false,
             "Internal error: Value \"" << base << "/" << name << "/type\" not found in the input file, while it was set as required.");

    if (value == NULL)
      {
        value = Pointer((get_full_json_schema_path() + "/" + name + "/default value").c_str()).Get(declarations);
        WBAssert(value != NULL,
                 "internal error: could not retrieve the default value at: "
                 << base + "/" + name + "/default value");
      }

    return value->GetUint();
  }

  template<>
  bool
  Parameters::get(const std::string &name)
  {
    const std::string base = this->get_full_json_path();
    const Value *value = Pointer((base + "/" + name).c_str()).Get(parameters);

    bool required = false;
    if (Pointer((base + "/required").c_str()).Get(declarations) != NULL)
      {
        for (auto &v : Pointer((base + "/required").c_str()).Get(declarations)->GetArray())
          {
            if (v.GetString() == name)
              {
                required = true;
              }
          }
      }

    WBAssert(value != NULL || required == false,
             "Internal error: Value \"" << base << "/" << name << "/type\" not found in the input file, while it was set as required.");

    if (value == NULL)
      {
        value = Pointer((get_full_json_schema_path() + "/" + name + "/default value").c_str()).Get(declarations);
        WBAssert(value != NULL,
                 "internal error: could not retrieve the default value at: "
                 << base + "/" + name + "/default value");
      }

    return value->GetBool();
  }


  template<>
  Point<2>
  Parameters::get(const std::string &name)
  {

    const std::string strict_base = this->get_full_json_path();
    const Value *array = Pointer((strict_base + "/" + name).c_str()).Get(parameters);

    bool required = false;
    if (Pointer((strict_base + "/required").c_str()).Get(declarations) != NULL)
      {
        for (auto &v : Pointer((strict_base + "/required").c_str()).Get(declarations)->GetArray())
          {
            if (v.GetString() == name)
              {
                required = true;
              }
          }
      }

    WBAssert(array != NULL || required == false,
             "Internal error: Value \"" << strict_base << "/" << name << "/type\" not found in the input file, while it was set as required.");

    if (array != NULL)
      {
        //Value *array = Pointer((strict_base  + "/" + name).c_str()).Get(parameters);

        for (unsigned int i = 0; i < array->Size(); ++i )
          {
            const std::string base = strict_base + "/" + name;
            //let's assume that the file is correct, because it has been checked with the json schema.
            // So there are exactly two values.
            double value1, value2;

            try
              {
                value1 = Pointer((base + "/0").c_str()).Get(parameters)->GetDouble();
                value2 = Pointer((base + "/1").c_str()).Get(parameters)->GetDouble();
              }
            catch (...)
              {
                WBAssertThrow(false, "Could not convert values of " << base << " into doubles.");
              }
            return Point<2>(value1,value2,this->coordinate_system->natural_coordinate_system());
          }
      }
    WBAssertThrow(false, "default values not implemented in get<Point<2> >. Looked in: " + strict_base + "/" << name);

    return Point<2>(invalid);;
  }

  template<>
  std::vector<Point<2> >
  Parameters::get_vector(const std::string &name)
  {
    std::vector<Point<2> > vector;
    const std::string strict_base = this->get_full_json_path();
    if (Pointer((strict_base + "/" + name).c_str()).Get(parameters) != NULL)
      {
        Value *array = Pointer((strict_base  + "/" + name).c_str()).Get(parameters);

        for (unsigned int i = 0; i < array->Size(); ++i )
          {
            const std::string base = strict_base + "/" + name + "/" + std::to_string(i);
            //let's assume that the file is correct, because it has been checked with the json schema.
            // So there are exactly two values.
            double value1, value2;

            try
              {
                value1 = Pointer((base + "/0").c_str()).Get(parameters)->GetDouble();
                value2 = Pointer((base + "/1").c_str()).Get(parameters)->GetDouble();
              }
            catch (...)
              {
                WBAssertThrow(false, "Could not convert values of " << base << " into doubles.");
              }
            vector.push_back(Point<2>(value1,value2,this->coordinate_system->natural_coordinate_system()));
          }
      }
    return vector;
  }


  template<>
  std::vector<Objects::Segment<Features::SubductingPlateModels::Temperature::Interface,Features::SubductingPlateModels::Composition::Interface> >
  Parameters::get_vector(const std::string &name,
                         std::vector<std::shared_ptr<Features::SubductingPlateModels::Temperature::Interface> > &default_temperature_models,
                         std::vector<std::shared_ptr<Features::SubductingPlateModels::Composition::Interface> > &default_composition_models)
  {
    using namespace Features::SubductingPlateModels;
    std::vector<Objects::Segment<Temperature::Interface,Composition::Interface> > vector;
    this->enter_subsection(name);
    const std::string strict_base = this->get_full_json_path();
    if (Pointer((strict_base).c_str()).Get(parameters) != NULL)
      {
        // get the array of segments
        Value *array = Pointer((strict_base).c_str()).Get(parameters);

        for (unsigned int i = 0; i < array->Size(); ++i )
          {
            this->enter_subsection(std::to_string(i));
            const std::string base = this->get_full_json_path();
            // get one segment
            // length
            double length = Pointer((base + "/length").c_str()).Get(parameters)->GetDouble();

            // get thickness
            Value *point_array = Pointer((base  + "/thickness").c_str()).Get(parameters);
            Point<2> thickness(invalid);
            if (point_array != NULL)
              {
                if (point_array->Size() == 1)
                  {
                    // There is only one value, set it for both elements
                    double local0 = Pointer((base + "/thickness/0").c_str()).Get(parameters)->GetDouble();
                    thickness = Point<2>(local0,local0,invalid);
                  }
                else
                  {
                    double local0 = Pointer((base + "/thickness/0").c_str()).Get(parameters)->GetDouble();
                    double local1 = Pointer((base + "/thickness/1").c_str()).Get(parameters)->GetDouble();
                    thickness = Point<2>(local0,local1,invalid);
                  }
              }

            // get top trunctation (default is 0,0)
            point_array = Pointer((base  + "/top truncation").c_str()).Get(parameters);
            Point<2> top_trunctation(invalid);
            if (point_array != NULL)
              {
                if (point_array->Size() == 1)
                  {
                    // There is only one value, set it for both elements
                    double local0 = Pointer((base + "/top truncation/0").c_str()).Get(parameters)->GetDouble();
                    top_trunctation = Point<2>(local0,local0,invalid);
                  }
                else
                  {
                    double local0 = Pointer((base + "/top truncation/0").c_str()).Get(parameters)->GetDouble();
                    double local1 = Pointer((base + "/top truncation/1").c_str()).Get(parameters)->GetDouble();
                    top_trunctation = Point<2>(local0,local1,invalid);
                  }
              }
            // get thickness
            point_array = Pointer((base  + "/angle").c_str()).Get(parameters);
            Point<2> angle(invalid);
            if (point_array != NULL)
              {
                if (point_array->Size() == 1)
                  {
                    // There is only one value, set it for both elements
                    double local0 = Pointer((base + "/angle/0").c_str()).Get(parameters)->GetDouble();
                    angle = Point<2>(local0,local0,invalid);
                  }
                else
                  {
                    double local0 = Pointer((base + "/angle/0").c_str()).Get(parameters)->GetDouble();
                    double local1 = Pointer((base + "/angle/1").c_str()).Get(parameters)->GetDouble();
                    angle = Point<2>(local0,local1,invalid);
                  }
              }

            // Get temperature models
            std::vector<std::shared_ptr<Temperature::Interface> > temperature_models;

            //This is a value to look back in the path elements. It is a
            // bit arbetrary, so might want to look in the future for an alternative.
            const unsigned int searchback = path.size() > 5 && path[path.size()-4] == "sections" ? 4 : 2;
            if (this->get_shared_pointers<Temperature::Interface>("temperature models", temperature_models) == false)
              {
                temperature_models = default_temperature_models;

                // we only need to do something when there is a temperature model defined
                if (Pointer((this->get_full_json_path(path.size()-searchback) + "/temperature models").c_str()).Get(parameters) != NULL)
                  {
                    // copy the value, this unfortunately removes it.
                    Value value1 = Value(Pointer((this->get_full_json_path(path.size()-searchback) + "/temperature models").c_str()).Get(parameters)->GetArray());

                    // now copy it
                    Value value2;
                    value2.CopyFrom(value1, parameters.GetAllocator());

                    // now we should have 2x the same value, so put it back and place it in the correct location.
                    Pointer((this->get_full_json_path(path.size()-searchback) + "/temperature models").c_str()).Set(parameters, value1);//.Get(parameters)->Set("temperature models", value1, parameters.GetAllocator());

                    Pointer((base).c_str()).Get(parameters)->AddMember("temperature models", value2, parameters.GetAllocator());
                  }
              }

            std::vector<std::shared_ptr<Composition::Interface> > composition_models;
            if (this->get_shared_pointers<Composition::Interface>("composition models", composition_models) == false)
              {
                composition_models = default_composition_models;

                // we only need to do something when there is a composition model defined
                if (Pointer((this->get_full_json_path(path.size()-searchback) + "/composition models").c_str()).Get(parameters) != NULL)
                  {
                    // copy the value, this unfortunately removes it.
                    Value value1 = Value(Pointer((this->get_full_json_path(path.size()-searchback) + "/composition models").c_str()).Get(parameters)->GetArray());

                    // now copy it
                    Value value2;
                    value2.CopyFrom(value1, parameters.GetAllocator());

                    // now we should have 2x the same value, so put it back and place it in the correct location.
                    Pointer((this->get_full_json_path(path.size()-searchback) + "/composition models").c_str()).Set(parameters, value1);//.Get(parameters)->Set("temperature models", value1, parameters.GetAllocator());

                    Pointer((base).c_str()).Get(parameters)->AddMember("composition models", value2, parameters.GetAllocator());
                  }
              }
            vector.push_back(Objects::Segment<Temperature::Interface,Composition::Interface>(length, thickness, top_trunctation, angle, temperature_models, composition_models));

            this->leave_subsection();
          }
      }
    this->leave_subsection();
    return vector;
  }


  template<>
  std::vector<Objects::Segment<Features::FaultModels::Temperature::Interface,Features::FaultModels::Composition::Interface> >
  Parameters::get_vector(const std::string &name,
                         std::vector<std::shared_ptr<Features::FaultModels::Temperature::Interface> > &default_temperature_models,
                         std::vector<std::shared_ptr<Features::FaultModels::Composition::Interface> > &default_composition_models)
  {
    using namespace Features::FaultModels;
    std::vector<Objects::Segment<Temperature::Interface,Composition::Interface> > vector;
    this->enter_subsection(name);
    const std::string strict_base = this->get_full_json_path();
    if (Pointer((strict_base).c_str()).Get(parameters) != NULL)
      {
        // get the array of segments
        Value *array = Pointer((strict_base).c_str()).Get(parameters);

        for (unsigned int i = 0; i < array->Size(); ++i )
          {
            this->enter_subsection(std::to_string(i));
            const std::string base = this->get_full_json_path();
            // get one segment
            // length
            double length = Pointer((base + "/length").c_str()).Get(parameters)->GetDouble();

            // get thickness
            Value *point_array = Pointer((base  + "/thickness").c_str()).Get(parameters);
            Point<2> thickness(invalid);
            if (point_array != NULL)
              {
                if (point_array->Size() == 1)
                  {
                    // There is only one value, set it for both elements
                    double local0 = Pointer((base + "/thickness/0").c_str()).Get(parameters)->GetDouble();
                    thickness = Point<2>(local0,local0,invalid);
                  }
                else
                  {
                    double local0 = Pointer((base + "/thickness/0").c_str()).Get(parameters)->GetDouble();
                    double local1 = Pointer((base + "/thickness/1").c_str()).Get(parameters)->GetDouble();
                    thickness = Point<2>(local0,local1,invalid);
                  }
              }

            // get top trunctation (default is 0,0)
            point_array = Pointer((base  + "/top truncation").c_str()).Get(parameters);
            Point<2> top_trunctation(invalid);
            if (point_array != NULL)
              {
                if (point_array->Size() == 1)
                  {
                    // There is only one value, set it for both elements
                    double local0 = Pointer((base + "/top truncation/0").c_str()).Get(parameters)->GetDouble();
                    top_trunctation = Point<2>(local0,local0,invalid);
                  }
                else
                  {
                    double local0 = Pointer((base + "/top truncation/0").c_str()).Get(parameters)->GetDouble();
                    double local1 = Pointer((base + "/top truncation/1").c_str()).Get(parameters)->GetDouble();
                    top_trunctation = Point<2>(local0,local1,invalid);
                  }
              }
            // get thickness
            point_array = Pointer((base  + "/angle").c_str()).Get(parameters);
            Point<2> angle(invalid);
            if (point_array != NULL)
              {
                if (point_array->Size() == 1)
                  {
                    // There is only one value, set it for both elements
                    double local0 = Pointer((base + "/angle/0").c_str()).Get(parameters)->GetDouble();
                    angle = Point<2>(local0,local0,invalid);
                  }
                else
                  {
                    double local0 = Pointer((base + "/angle/0").c_str()).Get(parameters)->GetDouble();
                    double local1 = Pointer((base + "/angle/1").c_str()).Get(parameters)->GetDouble();
                    angle = Point<2>(local0,local1,invalid);
                  }
              }

            // Get temperature models
            std::vector<std::shared_ptr<Temperature::Interface> > temperature_models;

            //This is a value to look back in the path elements. It is a
            // bit arbetrary, so might want to look in the future for an alternative.
            const unsigned int searchback = path.size() > 5 && path[path.size()-4] == "sections" ? 4 : 2;
            if (this->get_shared_pointers<Temperature::Interface>("temperature models", temperature_models) == false)
              {
                temperature_models = default_temperature_models;

                // we only need to do something when there is a temperature model defined
                if (Pointer((this->get_full_json_path(path.size()-searchback) + "/temperature models").c_str()).Get(parameters) != NULL)
                  {
                    // copy the value, this unfortunately removes it.
                    Value value1 = Value(Pointer((this->get_full_json_path(path.size()-searchback) + "/temperature models").c_str()).Get(parameters)->GetArray());

                    // now copy it
                    Value value2;
                    value2.CopyFrom(value1, parameters.GetAllocator());

                    // now we should have 2x the same value, so put it back and place it in the correct location.
                    Pointer((this->get_full_json_path(path.size()-searchback) + "/temperature models").c_str()).Set(parameters, value1);//.Get(parameters)->Set("temperature models", value1, parameters.GetAllocator());

                    Pointer((base).c_str()).Get(parameters)->AddMember("temperature models", value2, parameters.GetAllocator());
                  }
              }

            std::vector<std::shared_ptr<Composition::Interface> > composition_models;
            if (this->get_shared_pointers<Composition::Interface>("composition models", composition_models) == false)
              {
                composition_models = default_composition_models;

                // we only need to do something when there is a composition model defined
                if (Pointer((this->get_full_json_path(path.size()-searchback) + "/composition models").c_str()).Get(parameters) != NULL)
                  {
                    // copy the value, this unfortunately removes it.
                    Value value1 = Value(Pointer((this->get_full_json_path(path.size()-searchback) + "/composition models").c_str()).Get(parameters)->GetArray());

                    // now copy it
                    Value value2;
                    value2.CopyFrom(value1, parameters.GetAllocator());

                    // now we should have 2x the same value, so put it back and place it in the correct location.
                    Pointer((this->get_full_json_path(path.size()-searchback) + "/composition models").c_str()).Set(parameters, value1);//.Get(parameters)->Set("temperature models", value1, parameters.GetAllocator());

                    Pointer((base).c_str()).Get(parameters)->AddMember("composition models", value2, parameters.GetAllocator());
                  }
              }
            vector.push_back(Objects::Segment<Temperature::Interface,Composition::Interface>(length, thickness, top_trunctation, angle, temperature_models, composition_models));

            this->leave_subsection();
          }
      }
    this->leave_subsection();
    return vector;
  }

  template<>
  std::vector<double>
  Parameters::get_vector(const std::string &name)
  {
    std::vector<double> vector;
    const std::string strict_base = this->get_full_json_path();
    if (Pointer((strict_base + "/" + name).c_str()).Get(parameters) != NULL)
      {
        Value *array = Pointer((strict_base  + "/" + name).c_str()).Get(parameters);

        for (unsigned int i = 0; i < array->Size(); ++i )
          {
            const std::string base = strict_base + "/" + name + "/" + std::to_string(i);

            vector.push_back(Pointer(base.c_str()).Get(parameters)->GetDouble());
          }
      }
    else
      {
        unsigned int min_size = Pointer((this->get_full_json_schema_path()  + "/" + name + "/minItems").c_str()).Get(declarations)->GetUint();

        double default_value = Pointer((this->get_full_json_schema_path()  + "/" + name + "/items/default value").c_str()).Get(declarations)->GetDouble();

        // set to min size
        for (unsigned int i = 0; i < min_size; ++i)
          {
            vector.push_back(default_value);
          }
      }
    return vector;
  }

  template<>
  std::vector<unsigned int>
  Parameters::get_vector(const std::string &name)
  {
    std::vector<unsigned int> vector;
    const std::string strict_base = this->get_full_json_path();
    if (Pointer((strict_base + "/" + name).c_str()).Get(parameters) != NULL)
      {
        Value *array = Pointer((strict_base  + "/" + name).c_str()).Get(parameters);

        for (unsigned int i = 0; i < array->Size(); ++i )
          {
            const std::string base = strict_base + "/" + name + "/" + std::to_string(i);

            vector.push_back(Pointer(base.c_str()).Get(parameters)->GetUint());
          }
      }
    else
      {
        unsigned int min_size = Pointer((this->get_full_json_schema_path()  + "/" + name + "/minItems").c_str()).Get(declarations)->GetUint();

        unsigned int default_value = Pointer((this->get_full_json_schema_path()  + "/" + name + "/items/default value").c_str()).Get(declarations)->GetUint();

        // set to min size
        for (unsigned int i = 0; i < min_size; ++i)
          {
            vector.push_back(default_value);
          }
      }
    return vector;
  }

  template<class T>
  std::unique_ptr<T>
  Parameters::get_unique_pointer(const std::string &name)
  {
    const std::string base = this->get_full_json_path();
    Value *value = Pointer((base + "/" + name + "/model").c_str()).Get(parameters);

    bool required = false;
    if (Pointer((base + "/required").c_str()).Get(declarations) != NULL)
      for (auto &v : Pointer((base + "/required").c_str()).Get(declarations)->GetArray())
        {
          if (v.GetString() == name)
            {
              required = true;
            }
        }

    WBAssert(value != NULL || required == false,
             "Internal error: Value \"" << base << "/" << name << "/model\" not found in the input file, while it was set as required.");

    if (value == NULL)
      {
        value = Pointer((get_full_json_schema_path() + "/" + name + "/default value").c_str()).Get(declarations);
        WBAssert(value != NULL,
                 "internal error: could not retrieve the default value at: "
                 << base + "/" + name + "/default value. Make sure the value has been declared.");
      }

    return T::create(value->GetString(),&world);
  }

  template<class T>
  bool
  Parameters::get_unique_pointers(const std::string &name, std::vector<std::unique_ptr<T> > &vector)
  {
    const std::string strict_base = this->get_full_json_path();
    if (Pointer((strict_base + "/" + name).c_str()).Get(parameters) != NULL)
      {
        Value *array = Pointer((strict_base  + "/" + name).c_str()).Get(parameters);

        for (unsigned int i = 0; i < array->Size(); ++i )
          {
            const std::string base = strict_base + "/" + name + "/" + std::to_string(i);

            std::string value = Pointer((base + "/model").c_str()).Get(parameters)->GetString();

            vector.push_back(std::move(T::create(value, &world)));
          }
      }
    else
      {
        return false;
      }

    return true;
  }

  template<>
  bool
  Parameters::get_unique_pointers(const std::string &name, std::vector<std::unique_ptr<Features::SubductingPlate> > &vector)
  {
    const std::string strict_base = this->get_full_json_path();
    if (Pointer((strict_base + "/" + name).c_str()).Get(parameters) != NULL)
      {
        Value *array = Pointer((strict_base  + "/" + name).c_str()).Get(parameters);

        for (unsigned int i = 0; i < array->Size(); ++i )
          {
            vector.push_back(std::move(std::unique_ptr<Features::SubductingPlate>(new Features::SubductingPlate(&world))));
          }
      }
    else
      {
        return false;
      }

    return true;
  }

  template<>
  bool
  Parameters::get_unique_pointers(const std::string &name, std::vector<std::unique_ptr<Features::Fault> > &vector)
  {
    const std::string strict_base = this->get_full_json_path();
    if (Pointer((strict_base + "/" + name).c_str()).Get(parameters) != NULL)
      {
        Value *array = Pointer((strict_base  + "/" + name).c_str()).Get(parameters);

        for (unsigned int i = 0; i < array->Size(); ++i )
          {
            vector.push_back(std::move(std::unique_ptr<Features::Fault>(new Features::Fault(&world))));
          }
      }
    else
      {
        return false;
      }

    return true;
  }



  template<class T>
  bool
  Parameters::get_shared_pointers(const std::string &name, std::vector<std::shared_ptr<T> > &vector)
  {
    const std::string strict_base = this->get_full_json_path();
    if (Pointer((strict_base + "/" + name).c_str()).Get(parameters) != NULL)
      {
        Value *array = Pointer((strict_base  + "/" + name).c_str()).Get(parameters);

        for (unsigned int i = 0; i < array->Size(); ++i )
          {
            const std::string base = strict_base + "/" + name + "/" + std::to_string(i);

            std::string value = Pointer((base + "/model").c_str()).Get(parameters)->GetString();

            vector.push_back(std::move(T::create(value, &world)));
          }
      }
    else
      {
        return false;
      }

    return true;
  }
  /*
  bool
  Parameters::load_entry(const std::string &name, const bool required, const Types::Interface &type)
  {
    //Todo: the loading of the entry should actually be performed by the type
    unsigned int location;
    return this->load_entry(name, required, type, location);
  }

  bool
  Parameters::load_entry(const std::string &name, const bool required, const Types::Interface &type, unsigned int &location)
  {
    bool found_value = false;

    const std::string path_plus_name = (get_full_path() == "") ? name : (get_full_path() + path_seperator + name);

    if (type.get_type() == Types::type::UnsignedInt)
      {
        // First check whether the value is in the tree. If not Assert when the value is required,
        // otherwise set found_value to false.
        boost::optional<std::string> value_tree = Utilities::get_from_ptree_abs(*local_tree,
                                                                                get_relative_path_without_arrays(),
                                                                                (name.front() == '[' && name.back() == ']' ? "" : name),
                                                                                required,
                                                                                path_seperator);

        found_value = value_tree ? true : false;

        WBAssertThrow((found_value == true && required == true) || required == false,
                      "Could not find " + get_full_path() + ", while it is set as required.");

        // Store the value
        //const Types::UnsignedInt &natural_type = dynamic_cast<const Types::UnsignedInt &>(type);

        //const unsigned int value = found_value == true ? Utilities::string_to_unsigned_int(value_tree.get())
        //                         : natural_type.default_value;

        //vector_unsigned_int.push_back(Types::UnsignedInt(value,natural_type.default_value,natural_type.description));
        location = vector_unsigned_int.size()-1;
        string_to_type_map[path_plus_name] = location;

      }
    else if (type.get_type() == Types::type::Double)
      {
        // First check whether the value is in the tree. If not Assert when the value is required,
        // otherwise set found_value to false.
        boost::optional<std::string> value_tree = Utilities::get_from_ptree_abs(*local_tree,
                                                                                get_relative_path_without_arrays(),
                                                                                (name.front() == '[' && name.back() == ']' ? "" : name),
                                                                                required,
                                                                                path_seperator);
        found_value = value_tree ? true : false;

        WBAssertThrow((found_value == true && required == true) || required == false,
                      "Could not find " + get_full_path() + ", while it is set as required.");


        // The value is present and we have retrieved it. Now store it
        const Types::Double &natural_type = dynamic_cast<const Types::Double &>(type);
        const double value = found_value == true ? Utilities::string_to_double(value_tree.get()) : natural_type.default_value;
        vector_double.push_back(Types::Double(value,natural_type.default_value,natural_type.description));
        location = vector_double.size()-1;
        string_to_type_map[path_plus_name] = location;

      }
    else if (type.get_type() == Types::type::String)
      {
        // First check whether the value is in the tree. If not Assert when the value is required,
        // otherwise set found_value to false.
        boost::optional<std::string> value_tree = Utilities::get_from_ptree_abs(*local_tree,
                                                                                get_relative_path_without_arrays(),
                                                                                (name.front() == '[' && name.back() == ']' ? "" : name),
                                                                                required,
                                                                                path_seperator);


        found_value = value_tree ? true : false;

        WBAssertThrow((found_value == true && required == true) || required == false,
                      "Could not find " + get_full_path() + ", while it is set as required.");

        // The value is present and we have retrieved it. Now store it
        const Types::String &natural_type = dynamic_cast<const Types::String &>(type);
        const std::string value = value_tree ? value_tree.get() : natural_type.default_value;
        vector_string.push_back(Types::String(natural_type.default_value,natural_type.description));

        vector_string[vector_string.size()-1].set_value(value);
        location = vector_string.size()-1;
        string_to_type_map[path_plus_name] = location;

      }
    else if (type.get_type() == Types::type::Segment)
      {
        /*
         // First check whether the value is in the tree. If not Assert when the value is required,
         // otherwise set found_value to false.
         const Types::Segment &natural_type = dynamic_cast<const Types::Segment &>(type);

         // Check length value
         boost::optional<std::string> length_value_tree =
           Utilities::get_from_ptree_abs(*local_tree,
                                         get_relative_path_without_arrays(),
                                         "length",
                                         required,
                                         path_seperator);

         found_value = length_value_tree ? true : false;

         WBAssertThrow((found_value == true && required == true) || required == false,
                       "Could not find " + get_full_path() + path_seperator + "length" + ", while it is set as required.");

         // Check thickness value as a 2d point. Todo: see if it can be recursed to POINT2D
         WorldBuilder::Point<2> point_thickness(std::array<double,2> {0,0},
                                                this->coordinate_system->natural_coordinate_system());

         std::string path_plus_name_without_arrays = "thickness";//((get_relative_path_without_arrays() == "") ? "" : (get_relative_path_without_arrays() + path_seperator + ""))
         //+ (name.front() == '[' && name.back() == ']' ? "" : "thickness");

         boost::optional<ptree &> child = local_tree->get_child_optional(path_plus_name_without_arrays);

         found_value = child ? true : false;

         WBAssertThrow((found_value == true && required == true) || required == false,
                       "Could not find " + get_full_path() + path_seperator + "thickness" + ", while it is set as required.");

         unsigned int diff = path.size()-path_level;
         path_level+=diff;
         if (found_value == true)
           {
             unsigned int current_size = 0;
             for (boost::property_tree::ptree::iterator it = child.get().begin(); it != child.get().end(); ++it)
               {
                 // First check whether the value is in the tree. If not Assert when the value is required,
                 // otherwise return false.
                 boost::optional<std::string> value_tree = Utilities::get_from_ptree_abs(it->second,
                                                                                         get_relative_path_without_arrays(),
                                                                                         "", required, path_seperator);

                 found_value = value_tree ? true : false;

                 WBAssertThrow((found_value == true && required == true) || required == false,
                               "Could not find " + get_full_path() + ", while it is set as required.");
                 //Todo: this looks strange to me. A value should always be found, or something really weird happened...

                 const double value = found_value == true ? Utilities::string_to_double(value_tree.get()) : natural_type.default_value_thickness[current_size];

                 point_thickness[current_size] = value;
                 current_size++;
               }

             // if only one value is filled in, set it for both.
             if (current_size == 1)
               {
                 point_thickness[1] = point_thickness[0];
               }
             WBAssertThrow(current_size == 1 || current_size == 2,
                           "The entry " + get_full_path() + path_seperator + "thickness" +
                           " should contain one or two values, but the size was not 1 or 2, it was "
                           << current_size << ".");
           }
         else
           {
             point_thickness = natural_type.default_value_thickness;
           }
         path_level -= diff;


         // Check angle value as a 2d point. Todo: see if it can be recursed to POINT2D
         WorldBuilder::Point<2> point_angle(std::array<double,2> {0,0},
                                            this->coordinate_system->natural_coordinate_system());

         path_plus_name_without_arrays = "angle";//((get_relative_path_without_arrays() == "") ? "" : (get_relative_path_without_arrays() + path_seperator + ""))
         //        + (name.front() == '[' && name.back() == ']' ? "" : "angle");

         child = local_tree->get_child_optional(path_plus_name_without_arrays);

         found_value = child ? true : false;

         WBAssertThrow((found_value == true && required == true) || required == false,
                       "Could not find " + get_full_path() + path_seperator + "angle" + ", while it is set as required.");

         //unsigned int diff = path.size()-path_level;
         path_level+=diff;
         if (found_value == true)
           {
             unsigned int current_size = 0;
             for (boost::property_tree::ptree::iterator it = child.get().begin(); it != child.get().end(); ++it)
               {
                 // First check whether the value is in the tree. If not Assert when the value is required,
                 // otherwise return false.
                 boost::optional<std::string> value_tree = Utilities::get_from_ptree_abs(it->second,
                                                                                         get_relative_path_without_arrays(),
                                                                                         "", required, path_seperator);

                 found_value = value_tree ? true : false;

                 WBAssertThrow((found_value == true && required == true) || required == false,
                               "Could not find " + get_full_path() + ", while it is set as required.");
                 const double value = found_value == true ? Utilities::string_to_double(value_tree.get()) : natural_type.default_value_angle[current_size];

                 point_angle[current_size] = value;
                 current_size++;
               }

             // if only one value is filled in, set it for both.
             if (current_size == 1)
               {
                 point_angle[1] = point_angle[0];
               }
             WBAssertThrow(current_size == 1 || current_size == 2,
                           "The entry " + get_full_path() + path_seperator + "angle" +
                           " should contain one or two values, but the size was not 1 or 2, it was "
                           << current_size << ".");
           }
         else
           {
             point_angle = natural_type.default_value_angle;
           }
         path_level -= diff;

         // The values are present and we have retrieved them. Now store it into a Segment type.
         const double value_length = length_value_tree ? Utilities::string_to_double(length_value_tree.get()) : natural_type.default_value_length;

         vector_segment.push_back(Types::Segment(value_length, natural_type.default_value_length,
                                                 point_thickness, natural_type.default_value_thickness,
                                                 point_angle, natural_type.default_value_angle,
                                                 natural_type.description));

         //vector_string[vector_string.size()-1].set_value(value);
         location = vector_segment.size()-1;
         string_to_type_map[path_plus_name] = location;* /

      }
    else if (type.get_type() == Types::type::ConstantLayer)
      {
        // First check whether the value is in the tree. If not Assert when the value is required,
        // otherwise set found_value to false.
        const Types::ConstantLayer &natural_type = dynamic_cast<const Types::ConstantLayer &>(type);

        // Check composition value
        std::vector<unsigned int> compositions;
        boost::optional<ptree &> child = local_tree->get_child_optional("compositions");

        found_value = child ? true : false;

        WBAssertThrow((found_value == true && required == true) || required == false,
                      "Could not find " + get_full_path() + path_seperator + name + path_seperator +
                      " compositions, while it is set as required.");

        if (child)
          {
            for (boost::property_tree::ptree::iterator it = child.get().begin(); it != child.get().end(); ++it)
              {
                compositions.push_back(Utilities::string_to_int(it->second.get<std::string>("")));
              }
          }
        else
          {
            compositions = natural_type.default_value_composition;
          }

        // Check the value, which may be the value indicating composition or the temperature
        std::vector<double> fractions;
        child = local_tree->get_child_optional("fractions");

        found_value = child ? true : false;

        // fractions are not required
        //WBAssertThrow((found_value == true && required == true) || required == false,
        //          "Could not find " + get_full_path() + path_seperator + name + path_seperator + " fractions, while it is set as required.");

        if (child)
          {
            for (boost::property_tree::ptree::iterator it = child.get().begin(); it != child.get().end(); ++it)
              {
                fractions.push_back(Utilities::string_to_double(it->second.get<std::string>("")));
              }
          }
        else
          {
            fractions = natural_type.default_value;
          }

        WBAssertThrow(compositions.size() == fractions.size(),
                      "The amount of provided compositions in the constant layer should match the amount of given fractions in "
                      << get_full_path() + path_seperator + name + ". "
                      << "Compositions size = " << compositions.size() << ", fractions size = " << fractions.size() << ".");

        // Check thickness value
        boost::optional<std::string> thickness_value_tree =
          Utilities::get_from_ptree_abs(*local_tree,
                                        get_relative_path_without_arrays(),
                                        "thickness",
                                        required,
                                        path_seperator);


        found_value = thickness_value_tree ? true : false;

        WBAssertThrow((found_value == true && required == true) || required == false,
                      "Could not find " + get_full_path() + path_seperator + "composition" + ", while it is set as required.");



        // The values are present and we have retrieved them. Now store it into a ConstantLayer type.
        //const int value_composition = composition_value_tree ? Utilities::string_to_int(composition_value_tree.get()) : natural_type.default_value_composition;
        //const double value = value_tree ? Utilities::string_to_double(value_tree.get()) : natural_type.default_value;
        const double value_thickness = thickness_value_tree ? Utilities::string_to_double(thickness_value_tree.get()) : natural_type.default_value_thickness;

        vector_constant_layer.push_back(Types::ConstantLayer(compositions, natural_type.default_value_composition,
                                                             fractions, natural_type.default_value,
                                                             value_thickness, natural_type.default_value_thickness,
                                                             natural_type.description));

        //vector_string[vector_string.size()-1].set_value(value);
        location = vector_constant_layer.size()-1;
        string_to_type_map[path_plus_name] = location;

      }
    else if (type.get_type() == Types::type::CoordinateSystem)
      {
        const Types::CoordinateSystem &natural_type = dynamic_cast<const Types::CoordinateSystem &>(type);
        const std::string path_tmp = (get_full_path() == "") ? name : (get_full_path() + path_seperator + name);

        boost::optional<ptree &> child = local_tree->get_child_optional(path_tmp);

        found_value = child ? true : false;

        WBAssertThrow((found_value == false && required == true) || required == false, "Could not find " + path_tmp + ", while it is set as required.");
        // only one entry allowed. For now we take the first one
        // Todo: assert when there are more entries
        std::string system = found_value == true ? child.get().begin()->first : natural_type.default_value;
        enter_subsection(name);
        {
          enter_subsection(system);
          {
            coordinate_system = CoordinateSystems::Interface::create(system, &world);

            //coordinate_system->declare_entries();
          }
          leave_subsection();
        }
        leave_subsection();


      }
    else if (type.get_type() == Types::type::PluginSystem)
      {
        // Todo: redesing the path_level system.
        enter_subsection(name);
        {
          path_level++;
          features.push_back(Features::Interface::create(name,&world));
          features.back()->parse_entries(*this);
          path_level--;
        }
        leave_subsection();


        found_value = true;
      }
    else if (type.get_type() == Types::type::List)
      {
        const std::string path_plus_name_without_arrays = ((get_full_path_without_arrays() == "")
                                                           ?
                                                           name
                                                           :
                                                           (get_full_path_without_arrays() + path_seperator + name));

        boost::optional<ptree &> child = local_tree->get_child_optional(path_plus_name_without_arrays);

        found_value = child ? true : false;

        WBAssertThrow((found_value == true && required == true) || required == false,
                      "Could not find " + get_full_path() + path_seperator + name + ", while it is set as required.");
        if (child)
          {
            const Types::List &natural_type = dynamic_cast<const Types::List &>(type);
            // Todo: assert size of inner_type is one
            vector_list.push_back(Types::List(name, std::vector<unsigned int>(), natural_type.inner_type, natural_type.description));
            location = vector_list.size()-1;
            string_to_type_map[path_plus_name] = location;

            enter_subsection(name);
            {
              path_level++;
              for (boost::property_tree::ptree::iterator it = child.get().begin(); it != child.get().end(); ++it)
                {
                  //TODO add a mangle for the name
                  ptree *parent = local_tree;
                  local_tree = &(it->second);
                  unsigned int child_location;
                  found_value = this->load_entry(it->first, required, *natural_type.inner_type_ptr, child_location);
                  vector_list[location].inner_type_index.push_back(child_location);
                  local_tree = parent;
                }
              path_level--;
            }
            leave_subsection();
          }
        else
          {
            // TODO: set default value
          }
      }
    else if (type.get_type() == Types::type::Array)
      {

        const std::string path_plus_name_without_arrays = ((get_relative_path_without_arrays() == "") ? "" : (get_relative_path_without_arrays() + path_seperator + ""))
                                                          + (name.front() == '[' && name.back() == ']' ? path_seperator : name);

        boost::optional<ptree &> child = local_tree->get_child_optional(path_plus_name_without_arrays);

        found_value = child ? true : false;

        WBAssertThrow((found_value == true && required == true) || required == false, "Could not find " + get_full_path() + path_seperator + name + ", while it is set as required.");


        if (child)
          {
            const Types::Array &natural_type = dynamic_cast<const Types::Array &>(type);

            vector_array.push_back(Types::Array(std::vector<unsigned int>(), natural_type.inner_type,natural_type.description));

            location = vector_array.size()-1;
            string_to_type_map[path_plus_name] = location;

            enter_subsection(name);
            {
              unsigned int current_size = 0;
              //path_level++;
              for (boost::property_tree::ptree::iterator it = child.get().begin(); it != child.get().end(); ++it)
                {
                  unsigned int diff = path.size()-path_level;
                  path_level+=diff;
                  //TODO add a mangle for the name
                  ptree *parent = local_tree;
                  local_tree = &(it->second);
                  unsigned int child_location;
                  found_value = this->load_entry("["+ std::to_string(current_size) + "]", required, *natural_type.inner_type_ptr, child_location);
                  vector_array[location].inner_type_index.push_back(child_location);
                  local_tree = parent;
                  current_size++;
                  path_level -= diff;
                }
              //path_level--;
            }
            leave_subsection();
          }
        else
          {
            const Types::Array &natural_type = dynamic_cast<const Types::Array &>(type);

            vector_array.push_back(Types::Array(std::vector<unsigned int>(), natural_type.inner_type,natural_type.description));

            location = vector_array.size()-1;
            string_to_type_map[path_plus_name] = location;
            unsigned int child_location;
            this->set_entry("[0]", *natural_type.inner_type_ptr, child_location);
            vector_array[location].inner_type_index.push_back(child_location);
          }
      }
    else if (type.get_type() == Types::type::Point2D)
      {
        WBAssertThrow(this->coordinate_system != NULL, "The coordinate system must be loaded before points are loaded.");
        const std::string path_plus_name_without_arrays = ((get_relative_path_without_arrays() == "") ? "" : (get_relative_path_without_arrays() + path_seperator + ""))
                                                          + (name.front() == '[' && name.back() == ']' ? "" : name);

        boost::optional<ptree &> child = local_tree->get_child_optional(path_plus_name_without_arrays);

        found_value = child ? true : false;

        WBAssertThrow((found_value == true && required == true) || required == false,
                      "Could not find " + get_full_path() + path_seperator + name + ", while it is set as required.");

        unsigned int diff = path.size()-path_level;
        path_level+=diff;
        const Types::Point<2> &natural_type = dynamic_cast<const Types::Point<2> &>(type);
        if (found_value == true)
          {
            WorldBuilder::Point<2> point(std::array<double,2> {0,0},
                                         this->coordinate_system->natural_coordinate_system());

            unsigned int current_size = 0;
            for (boost::property_tree::ptree::iterator it = child.get().begin(); it != child.get().end(); ++it)
              {
                // First check whether the value is in the tree. If not Assert when the value is required,
                // otherwise return false.
                boost::optional<std::string> value_tree = Utilities::get_from_ptree_abs(it->second,
                                                                                        get_relative_path_without_arrays(),
                                                                                        "", required, path_seperator);

                found_value = value_tree ? true : false;

                WBAssertThrow((found_value == true && required == true) || required == false,
                              "Could not find " + get_full_path() + ", while it is set as required.");
                const double value = found_value == true ? Utilities::string_to_double(value_tree.get()) : natural_type.default_value[current_size];

                point[current_size] = value;
                current_size++;
              }
            WBAssertThrow(current_size == 2,
                          "The entry " + get_full_path() + path_seperator + name +
                          " should represent a 2d point, but the size was not 2, it was "
                          << current_size << ".");

            vector_point_2d.push_back(Types::Point<2>(point, point,natural_type.description));
            location = vector_point_2d.size()-1;
            string_to_type_map[path_plus_name] = location;
          }
        else
          {
            vector_point_2d.push_back(natural_type);
            location = vector_point_2d.size()-1;
            string_to_type_map[path_plus_name] = location;
          }
        path_level -= diff;
      }
    else if (type.get_type() == Types::type::Point3D)
      {
        WBAssertThrow(this->coordinate_system != NULL, "The coordinate system must be loaded before points are loaded.");
        const std::string path_plus_name_without_arrays = ((get_relative_path_without_arrays() == "") ? "" : (get_relative_path_without_arrays() + path_seperator + ""))
                                                          + (name.front() == '[' && name.back() == ']' ? "" : name);

        boost::optional<ptree &> child = local_tree->get_child_optional(path_plus_name_without_arrays);

        found_value = child ? true : false;

        WBAssertThrow((found_value == true && required == true) || required == false,
                      "Could not find " + get_full_path() + path_seperator + name + ", while it is set as required.");

        unsigned int diff = path.size()-path_level;
        path_level+=diff;
        const Types::Point<3> &natural_type = dynamic_cast<const Types::Point<3> &>(type);
        if (found_value == true)
          {
            WorldBuilder::Point<3> point(std::array<double,3> {0,0,0},
                                         this->coordinate_system->natural_coordinate_system());
            unsigned int current_size = 0;
            for (boost::property_tree::ptree::iterator it = child.get().begin(); it != child.get().end(); ++it)
              {
                // First check whether the value is in the tree. If not Assert when the value is required,
                // otherwise return false.
                boost::optional<std::string> value_tree = Utilities::get_from_ptree_abs(it->second,
                                                                                        get_relative_path_without_arrays(),
                                                                                        "", required, path_seperator);

                found_value = value_tree ? true : false;

                WBAssertThrow((found_value == true && required == true) || required == false,
                              "Could not find " + get_full_path() + ", while it is set as required.");
                const double value = found_value == true ? Utilities::string_to_double(value_tree.get()) : natural_type.default_value[current_size];

                point[current_size] = value;
                current_size++;
              }
            WBAssertThrow(current_size == 3,
                          "The entry " + get_full_path() + path_seperator + name +
                          " should represent a 3d point, but the size was not 3, it was "
                          << current_size << ".");

            vector_point_3d.push_back(Types::Point<3>(point, point,natural_type.description));
            location = vector_point_3d.size()-1;
            string_to_type_map[path_plus_name] = location;
          }
        else
          {
            vector_point_3d.push_back(natural_type);
            location = vector_point_3d.size()-1;
            string_to_type_map[path_plus_name] = location;
          }
        path_level -= diff;
      }
    else
      {
        WBAssertThrow(false,"Type not defined: " << (int)type.get_type());
      }

    return found_value;
  }

  void
  Parameters::set_entry(const std::string &name, const Types::Interface &type)
  {
    unsigned int location;
    this->set_entry(name, type, location);
  }

  void
  Parameters::set_entry(const std::string &name, const Types::Interface &type, unsigned int &location)
  {

    const std::string path_plus_name = (get_full_path() == "") ? name : (get_full_path() + path_seperator + name);

    if (type.get_type() == Types::type::Double)
      {
        // The value is present and we have retrieved it. Now store it
        const Types::Double &natural_type = dynamic_cast<const Types::Double &>(type);
        vector_double.push_back(natural_type);
        location = vector_double.size()-1;
        string_to_type_map[path_plus_name] = location;
      }
    else if (type.get_type() == Types::type::UnsignedInt)
      {
        // The value is present and we have retrieved it. Now store it
        const Types::UnsignedInt &natural_type = dynamic_cast<const Types::UnsignedInt &>(type);
        vector_unsigned_int.push_back(natural_type);
        string_to_type_map[path_plus_name] = vector_unsigned_int.size()-1;
      }
    else if (type.get_type() == Types::type::String)
      {
        // The value is present and we have retrieved it. Now store it
        const Types::String &natural_type = dynamic_cast<const Types::String &>(type);
        vector_string.push_back(natural_type);
        string_to_type_map[path_plus_name] = vector_string.size()-1;
      }
    else if (type.get_type() == Types::type::Segment)
      {
        // The value is present and we have retrieved it. Now store it
        // const Types::Segment &natural_type = dynamic_cast<const Types::Segment &>(type);
        //vector_segment.push_back(natural_type);
        //string_to_type_map[path_plus_name] = vector_string.size()-1;
      }
    else if (type.get_type() == Types::type::CoordinateSystem)
      {

        WBAssert(false, "A Coordinate system can not be directly set, use the load_entry function.");
        / *const Types::CoordinateSystem &natural_type = dynamic_cast<const Types::CoordinateSystem &>(type);
        std::string system = natural_type.;
        coordinate_system = CoordinateSystems::create_coordinate_system(system);* /
      }

    else if (type.get_type() == Types::type::PluginSystem)
      {
        WBAssert(false, "A Feature can not be directly set, use the load_entry function.");
      }
    / *else if (type.get_type() == Types::type::List)
      {
        const std::string path_plus_name_without_arrays = ((get_current_path_without_arrays() == "") ? name : (get_current_path_without_arrays() + path_seperator + name));


        const Types::List &natural_type = dynamic_cast<const Types::List &>(type);
        vector_list.push_back(natural_type);
        string_to_type_map[get_full_path() + path_seperator + name] = vector_list.size()-1;

        for (unsigned int i = 0; i < natural_type.inner_type.size(); ++i)
          {
            //TODO add a mangle for the name
            this->set_entry(local_path,  natural_type.name, *(natural_type.inner_type[i]));
          }

      }* /
    else if (type.get_type() == Types::type::Array)
      {
        const Types::Array &natural_type = dynamic_cast<const Types::Array &>(type);
        vector_array.push_back(Types::Array(natural_type.inner_type_index,natural_type.get_type(),natural_type.description));
        location = vector_array.size()-1;
        string_to_type_map[path_plus_name] = location;


        //unsigned int size = natural_type.inner_type.size();
        for (unsigned int i = 0; i < natural_type.inner_type_index.size(); ++i)
          {
            //TODO add a mangle for the name
            unsigned int child_location;
            this->set_entry("["+ std::to_string(i) + "]", *(natural_type.inner_type_ptr), child_location);
            vector_array[location].inner_type_index.push_back(child_location);
          }
      }
    else if (type.get_type() == Types::type::Point2D)
      {
        // 2d
        const Types::Point<2> &natural_type = dynamic_cast<const Types::Point<2> &>(type);

        vector_point_2d.push_back(natural_type);
        location = vector_point_2d.size()-1;
        string_to_type_map[path_plus_name] = location;
      }
    else if (type.get_type() == Types::type::Point3D)
      {
        const Types::Point<3> &natural_type = dynamic_cast<const Types::Point<3> &>(type);

        vector_point_3d.push_back(natural_type);
        location = vector_point_3d.size()-1;
        string_to_type_map[path_plus_name] = location;
      }
    else
      {
        WBAssertThrow(false,"Type not defined: " << (int)type.get_type());
      }
  }*/

  void
  Parameters::enter_subsection(const std::string name)
  {
    path.push_back(name);
    //TODO: WBAssert(is path valid?)
  }

  void
  Parameters::leave_subsection()
  {
    path.pop_back();
  }



  std::string
  Parameters::get_full_json_path(unsigned int max_size) const
  {
    std::string collapse = "";//"/";
    for (unsigned int i = 0; i < path.size() && i < max_size; i++)
      {
        collapse +=  "/" + path[i];// + "/";
      }
    return collapse;//.substr(0,collapse.size()-1);
  }

  std::string
  Parameters::get_full_json_schema_path() const
  {
    std::string collapse = "/properties";
    for (unsigned int i = 0; i < path.size(); i++)
      {
        // first get the type
        //WBAssert(Pointer((collapse + "/" + path[i] + "/type").c_str()).Get(declarations) != NULL, "Internal error: could not find " << collapse + "/" + path[i] + "/type");

        std::string base_path = Pointer((collapse + "/" + path[i] + "/type").c_str()).Get(declarations) != NULL
                                ?
                                collapse + "/" + path[i]
                                :
                                collapse;
        std::string type = Pointer((base_path + "/type").c_str()).Get(declarations)->GetString();

        if (type == "array")
          {
            // the type is an array. Arrays always have an items, but can also
            // have a oneOf (todo: or anyOf ...). Find out whether this is the case
            //collapse += path[i] + "/items";
            if (Pointer((base_path + "/items/oneOf").c_str()).Get(declarations) != NULL)
              {
                // it has a structure with oneOf. Find out which of the entries is needed.
                // This means we have to take a sneak peak to figure out how to get to the
                // next value.
                unsigned int size = Pointer((base_path + "/items/oneOf").c_str()).Get(declarations)->Size();
                bool found = false;
                unsigned int index = 0;
                for (; index < size; ++index)
                  {
                    std::string declarations_string = Pointer((base_path + "/items/oneOf/" + std::to_string(index)
                                                               + "/properties/model/enum/0").c_str()).Get(declarations)->GetString();

                    // we need to get the json path relevant for the current declaration string
                    // we are interested in, which requires an offset of 2.
                    WBAssert(Pointer((get_full_json_path(i+2) + "/model").c_str()).Get(parameters) != NULL, "Could not find model in: " << get_full_json_path(i+2) + "/model");
                    std::string parameters_string = Pointer((get_full_json_path(i+2) + "/model").c_str()).Get(parameters)->GetString();
                    //std::cout << "flag 2: i = " << i << ", get_full_json_path(i+2) = " << get_full_json_path(i+2) << std::endl;
                    //std::cout << "declarations_string = " << declarations_string << ", parameters_string = " << parameters_string << std::endl;
                    // currently in our case these are always objects, so go directly to find the option we need.
                    if (declarations_string == parameters_string)
                      {
                        // found it for index i;
                        found = true;
                        break;
                      }
                  }
                WBAssert(found == true,
                         "Internal error: This is an array with several possible values, "
                         "but could not find the correct value " << collapse + "/" + path[i] + "/items/oneOf");
                collapse += "/" + path[i] + "/items/oneOf/" + std::to_string(index) + "/properties";
                // add one to i, to skip the array
                ++i;
              }
            else
              {
                collapse = base_path + "/items";
              }
          }
        else if (type == "object")
          {
            //WBAssertThrow(false, "Not implemented");
            // the type is an object. object or have a properties or
            // have a oneOf (todo: or anyOf ...). Find out whether this is the case
            //collapse += path[i] + "/pro";
            /*if (Pointer((collapse + "/" + path[i] + "/items/oneOf").c_str()).Get(declarations) != NULL)
              {
                // it has a structure with oneOf. Find out which of the entries is needed.
                // This means we have to take a sneak peak to figure out how to get to the
                // next value.
                unsigned int size = Pointer((collapse + path[i] + "/items/oneOf").c_str()).Get(declarations)->Size();
                bool found = false;
                unsigned int index = 0;
                for (; index < size; ++index)
                  {
                    // currently in our case these are always objects, so go directly to find the option we need.
                    if (Pointer((collapse + path[i] + "/items/oneOf/" + std::to_string(index)
                                 + "/" + path[i+1]).c_str()).Get(declarations) != NULL)
                      {
                        // found it for index i;
                        found = true;
                        break;
                      }
                  }
                WBAssert(found == true,
                         "Internal error: This is an array with several possible values, "
                         "but could not find the correct value " << collapse + path[i] + "/items/oneOf");
                collapse += path[i] + "/oneOf/" + std::to_string(index);
              }
            else
              {*/
            //++i;
            collapse += "/properties";// + path[i];
            //++i;
            //}
          }
        else
          {
            collapse += "/" + path[i];
          }
      }
    return collapse;//substr(0,collapse.size());
  }





  /**
   * Todo: Returns a vector of pointers to the Point<3> Type based on the provided name.
   * Note that the variable with this name has to be loaded before this function is called.
   */
  template std::unique_ptr<CoordinateSystems::Interface> Parameters::get_unique_pointer<CoordinateSystems::Interface>(const std::string &name);

  /**
   * Todo: Returns a vector of pointers to the Point<3> Type based on the provided name.
   * Note that the variable with this name has to be loaded before this function is called.
   */
  template bool
  Parameters::get_unique_pointers<Features::Interface>(const std::string &name,
                                                       std::vector<std::unique_ptr<Features::Interface> > &vector);

  /**
   * Todo: Returns a vector of pointers to the Point<3> Type based on the provided name.
   * Note that the variable with this name has to be loaded before this function is called.
   */
  template bool
  Parameters::get_unique_pointers<Features::ContinentalPlateModels::Temperature::Interface>(const std::string &name,
      std::vector<std::unique_ptr<Features::ContinentalPlateModels::Temperature::Interface> > &vector);

  /**
  * Todo: Returns a vector of pointers to the Point<3> Type based on the provided name.
  * Note that the variable with this name has to be loaded before this function is called.
  */
  template bool
  Parameters::get_unique_pointers<Features::ContinentalPlateModels::Composition::Interface>(const std::string &name,
      std::vector<std::unique_ptr<Features::ContinentalPlateModels::Composition::Interface> > &vector);

  /**
   * Todo: Returns a vector of pointers to the Point<3> Type based on the provided name.
   * Note that the variable with this name has to be loaded before this function is called.
   */
  template bool
  Parameters::get_unique_pointers<Features::OceanicPlateModels::Temperature::Interface>(const std::string &name,
      std::vector<std::unique_ptr<Features::OceanicPlateModels::Temperature::Interface> > &vector);

  /**
  * Todo: Returns a vector of pointers to the Point<3> Type based on the provided name.
  * Note that the variable with this name has to be loaded before this function is called.
  */
  template bool
  Parameters::get_unique_pointers<Features::OceanicPlateModels::Composition::Interface>(const std::string &name,
      std::vector<std::unique_ptr<Features::OceanicPlateModels::Composition::Interface> > &vector);

  /**
   * Todo: Returns a vector of pointers to the Point<3> Type based on the provided name.
   * Note that the variable with this name has to be loaded before this function is called.
   */
  template bool
  Parameters::get_unique_pointers<Features::MantleLayerModels::Temperature::Interface>(const std::string &name,
      std::vector<std::unique_ptr<Features::MantleLayerModels::Temperature::Interface> > &vector);

  /**
  * Todo: Returns a vector of pointers to the Point<3> Type based on the provided name.
  * Note that the variable with this name has to be loaded before this function is called.
  */
  template bool
  Parameters::get_unique_pointers<Features::MantleLayerModels::Composition::Interface>(const std::string &name,
      std::vector<std::unique_ptr<Features::MantleLayerModels::Composition::Interface> > &vector);

  /**
   * Todo: Returns a vector of pointers to the Point<3> Type based on the provided name.
   * Note that the variable with this name has to be loaded before this function is called.
   */
  template bool
  Parameters::get_unique_pointers<Features::SubductingPlateModels::Temperature::Interface>(const std::string &name,
      std::vector<std::unique_ptr<Features::SubductingPlateModels::Temperature::Interface> > &vector);

  /**
  * Todo: Returns a vector of pointers to the Point<3> Type based on the provided name.
  * Note that the variable with this name has to be loaded before this function is called.
  */
  template bool
  Parameters::get_unique_pointers<Features::SubductingPlateModels::Composition::Interface>(const std::string &name,
      std::vector<std::unique_ptr<Features::SubductingPlateModels::Composition::Interface> > &vector);

  /**
   * Todo: Returns a vector of pointers to the Point<3> Type based on the provided name.
   * Note that the variable with this name has to be loaded before this function is called.
   */
  template bool
  Parameters::get_unique_pointers<Features::FaultModels::Temperature::Interface>(const std::string &name,
                                                                                 std::vector<std::unique_ptr<Features::FaultModels::Temperature::Interface> > &vector);

  /**
  * Todo: Returns a vector of pointers to the Point<3> Type based on the provided name.
  * Note that the variable with this name has to be loaded before this function is called.
  */
  template bool
  Parameters::get_unique_pointers<Features::FaultModels::Composition::Interface>(const std::string &name,
                                                                                 std::vector<std::unique_ptr<Features::FaultModels::Composition::Interface> > &vector);



  /**
   * Todo: Returns a vector of pointers to the Point<3> Type based on the provided name.
   * Note that the variable with this name has to be loaded before this function is called.
   */
  template bool
  Parameters::get_shared_pointers<Features::SubductingPlateModels::Temperature::Interface>(const std::string &name,
      std::vector<std::shared_ptr<Features::SubductingPlateModels::Temperature::Interface> > &vector);

  /**
  * Todo: Returns a vector of pointers to the Point<3> Type based on the provided name.
  * Note that the variable with this name has to be loaded before this function is called.
  */
  template bool
  Parameters::get_shared_pointers<Features::SubductingPlateModels::Composition::Interface>(const std::string &name,
      std::vector<std::shared_ptr<Features::SubductingPlateModels::Composition::Interface> > &vector);


  /**
   * Todo: Returns a vector of pointers to the Point<3> Type based on the provided name.
   * Note that the variable with this name has to be loaded before this function is called.
   */
  template bool
  Parameters::get_shared_pointers<Features::FaultModels::Temperature::Interface>(const std::string &name,
                                                                                 std::vector<std::shared_ptr<Features::FaultModels::Temperature::Interface> > &vector);

  /**
  * Todo: Returns a vector of pointers to the Point<3> Type based on the provided name.
  * Note that the variable with this name has to be loaded before this function is called.
  */
  template bool
  Parameters::get_shared_pointers<Features::FaultModels::Composition::Interface>(const std::string &name,
                                                                                 std::vector<std::shared_ptr<Features::FaultModels::Composition::Interface> > &vector);





}

