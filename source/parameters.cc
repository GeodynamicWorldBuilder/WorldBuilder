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

#include <rapidjson/istreamwrapper.h>
#include "rapidjson/pointer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/latexwriter.h"
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

    StringBuffer buffer;
    {
      std::ofstream myfile;
      myfile.open ("../declarations.tex");
      LatexWriter<StringBuffer, UTF8<>, UTF8<>, CrtAllocator, kWriteNanAndInfFlag> writer(buffer);
      declarations.Accept(writer);

      myfile << buffer.GetString();
    }

    path_level =0;
    // Now read in the world builder file into a file stream and
    // put it into a the rapidjason document
    std::ifstream json_input_stream(filename.c_str());

    // Get world builder file and check whether it exists
    WBAssertThrow(json_input_stream.good(),
                  "Could not find the world builder file at the specified location: " + filename);

    WBAssert(json_input_stream, "Could not read the world builder file.");

    rapidjson::IStreamWrapper isw(json_input_stream);

    // relaxing sytax by allowing comments () for now, maybe also allow trailing commas and (kParseTrailingCommasFlag) and nan's, inf etc (kParseNanAndInfFlag)?
    //WBAssertThrow(!parameters.ParseStream<kParseCommentsFlag>(isw).HasParseError(), "Parsing erros world builder file");

    WBAssertThrow(!(parameters.ParseStream<kParseCommentsFlag | kParseNanAndInfFlag>(isw).HasParseError()),
                  "Parsing errors world builder file: Error(offset " << (unsigned)parameters.GetErrorOffset()
                  << "): " << GetParseError_En(parameters.GetParseError()));

    WBAssertThrow(parameters.IsObject(), "World builder file is is not an object.");
    json_input_stream.close();


    SchemaDocument schema(declarations);
    SchemaValidator validator(schema);

    if (!parameters.Accept(validator))
      {
        // Input JSON is invalid according to the schema
        // Output diagnostic information
        buffer.Clear();
        std::stringstream string;
        validator.GetInvalidSchemaPointer().StringifyUriFragment(buffer);
        string << "Invalid schema: " << buffer.GetString() << std::endl;
        string << "Invalid keyword: " << validator.GetInvalidSchemaKeyword();
        buffer.Clear();
        validator.GetInvalidDocumentPointer().StringifyUriFragment(buffer);
        string << "Invalid schema: " << buffer.GetString() << std::endl;
        PrettyWriter<StringBuffer> writer(buffer);
        validator.GetError().Accept(writer);
        WBAssertThrow(false, string.str() << "Error document: " << std::endl << buffer.GetString());
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
                WBAssertThrow(false, "Could not convert values of " << base << " into Point<2>, because it could not convert the sub-elements into doubles.");
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
    WBAssertThrow(Pointer((strict_base).c_str()).Get(parameters) != NULL, "Could not find " << strict_base << " in parameters.");
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
    vector.resize(0);
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
    vector.resize(0);
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
    vector.resize(0);
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
    //vector.resize(0);
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
    std::string collapse = "";
    for (unsigned int i = 0; i < path.size() && i < max_size; i++)
      {
        collapse +=  "/" + path[i];
      }
    return collapse;
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
            collapse += "/properties";
          }
        else
          {
            collapse += "/" + path[i];
          }
      }
    return collapse;
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

