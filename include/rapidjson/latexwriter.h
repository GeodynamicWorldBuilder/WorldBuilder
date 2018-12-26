// Tencent is pleased to support the open source community by making RapidJSON available.
//
// Copyright (C) 2015 THL A29 Limited, a Tencent company, and Milo Yip. All rights reserved.
//
// Licensed under the MIT License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// http://opensource.org/licenses/MIT
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#ifndef RAPIDJSON_LATEXWRITER_H_
#define RAPIDJSON_lATEXWRITER_H_

#include "writer.h"
#include <iostream>

#ifdef __GNUC__
RAPIDJSON_DIAG_PUSH
RAPIDJSON_DIAG_OFF(effc++)
#endif

#if defined(__clang__)
RAPIDJSON_DIAG_PUSH
RAPIDJSON_DIAG_OFF(c++98-compat)
#endif

RAPIDJSON_NAMESPACE_BEGIN

//! Combination of LatexWriter format flags.
/*! \see LatexWriter::SetFormatOptions
 */
enum LatexFormatOptions
{
  lFormatDefault = 0,         //!< Default pretty formatting.
  lFormatSingleLineArray = 1  //!< Format arrays on a single line.
};

//! Writer with indentation and spacing.
/*!
    \tparam OutputStream Type of output os.
    \tparam SourceEncoding Encoding of source string.
    \tparam TargetEncoding Encoding of output stream.
    \tparam StackAllocator Type of allocator for allocating memory of stack.
*/
template<typename OutputStream, typename SourceEncoding = UTF8<>, typename TargetEncoding = UTF8<>, typename StackAllocator = CrtAllocator, unsigned writeFlags = kWriteDefaultFlags>
class LatexWriter : public Writer<OutputStream, SourceEncoding, TargetEncoding, StackAllocator, writeFlags>
{
  public:
    typedef Writer<OutputStream, SourceEncoding, TargetEncoding, StackAllocator, writeFlags> Base;
    typedef typename Base::Ch Ch;

    //! Constructor
    /*! \param os Output stream.
        \param allocator User supplied allocator. If it is null, it will create a private one.
        \param levelDepth Initial capacity of stack.
    */
    explicit LatexWriter(OutputStream &os, StackAllocator *allocator = 0, size_t levelDepth = Base::kDefaultLevelDepth) :
      Base(os, allocator, levelDepth), indentChar_(' '), indentCharCount_(4), formatOptions_(lFormatDefault) {}


    explicit LatexWriter(StackAllocator *allocator = 0, size_t levelDepth = Base::kDefaultLevelDepth) :
      Base(allocator, levelDepth), indentChar_(' '), indentCharCount_(4) {}

#if RAPIDJSON_HAS_CXX11_RVALUE_REFS
    LatexWriter(LatexWriter &&rhs) :
      Base(std::forward<LatexWriter>(rhs)), indentChar_(rhs.indentChar_), indentCharCount_(rhs.indentCharCount_), formatOptions_(rhs.formatOptions_) {}
#endif

    //! Set custom indentation.
    /*! \param indentChar       Character for indentation. Must be whitespace character (' ', '\\t', '\\n', '\\r').
        \param indentCharCount  Number of indent characters for each indentation level.
        \note The default indentation is 4 spaces.
    */
    LatexWriter &SetIndent(Ch indentChar, unsigned indentCharCount)
    {
      RAPIDJSON_ASSERT(indentChar == ' ' || indentChar == '\t' || indentChar == '\n' || indentChar == '\r');
      indentChar_ = indentChar;
      indentCharCount_ = indentCharCount;
      return *this;
    }

    //! Set pretty writer formatting options.
    /*! \param options Formatting options.
    */
    LatexWriter &SetFormatOptions(LatexFormatOptions options)
    {
      formatOptions_ = options;
      return *this;
    }

    /*! @name Implementation of Handler
        \see Handler
    */
    //@{

    bool Null()
    {
      //LatexPrefix(kNullType);
      return Base::EndValue(Base::WriteNull());
    }
    bool Bool(bool b)
    {
      //LatexPrefix(b ? kTrueType : kFalseType);
      Base::EndValue(Base::WriteBool(b));
      Base::os_->Put('\n');
      return true;
    }
    bool Int(int i)
    {
      //LatexPrefix(kNumberType);
      Base::EndValue(Base::WriteInt(i));
      Base::os_->Put('\n');
      return true;
    }
    bool Uint(unsigned u)
    {
      //LatexPrefix(kNumberType);
      Base::EndValue(Base::WriteUint(u));
      Base::os_->Put('\n');
      return true;
    }
    bool Int64(int64_t i64)
    {
      //LatexPrefix(kNumberType);
      Base::EndValue(Base::WriteInt64(i64));
      Base::os_->Put('\n');
      return true;
    }
    bool Uint64(uint64_t u64)
    {
      //LatexPrefix(kNumberType);
      Base::EndValue(Base::WriteUint64(u64));
      Base::os_->Put('\n');
      return true;
    }
    bool Double(double d)
    {
      //LatexPrefix(kNumberType);
      Base::EndValue(Base::WriteDouble(d));
      Base::os_->Put('\n');
      return true;
    }

    bool RawNumber(const Ch *str, SizeType length, bool copy = false)
    {
      RAPIDJSON_ASSERT(str != 0);
      (void)copy;
      //LatexPrefix(kNumberType);
      Base::EndValue(Base::WriteString(str, length, false));
      Base::os_->Put('\n');
      return true;
    }

    bool String(const Ch *str, SizeType length, bool copy = false)
    {
      RAPIDJSON_ASSERT(str != 0);
      (void)copy;
      //LatexPrefix(kStringType);
      Base::EndValue(Base::WriteString(str, length, false));
      Base::os_->Put('\n');
      return true;
    }

#if RAPIDJSON_HAS_STDSTRING
    bool String(const std::basic_string<Ch> &str)
    {
      return String(str.data(), SizeType(str.size()));
    }
#endif

    bool StartObject()
    {
    	std::string begin = "";
    	if(level_type.size() == 0)
    	{
    		// root object
      	  begin = "startobject root: \\section{" + get_path() + "} \\begin{itemize}";
      	  level_type.push_back(0);
    	}
    	else if(level_type.back() == 2)
    	{
    		path.push_back("1");
    		level_type.push_back(0);
    		begin = "startobject type=2";
    	}
        Base::os_->Put('\n');

      	Base::WriteString(begin.c_str(), begin.size(), false);
      ////LatexPrefix(kObjectType);
      /*if(set_enum_ == true)
      {
    	  new (Base::level_stack_.template Push<typename Base::Level>()) typename Base::Level(true);
    	  //this->os_->Put('\\'); // "\x5C"
    	  std::string begin = "\\end{itemize} \\section{" + get_path() + "}";
    	  Base::WriteString(begin.c_str(), begin.size(), false);
          //Base::os_->Put('\n');
    	  //this->os_->Put(begin.c_str());
          // reset set_enum_;
          set_enum_ = false;
      }
      else
      {
    	  new (Base::level_stack_.template Push<typename Base::Level>()) typename Base::Level(false);

    	  std::string begin = "\\begin{itemize}";
    	  Base::WriteString(begin.c_str(), begin.size(), false);
          //Base::os_->Put('\n');
    	  //this->os_->Put(begin.c_str());
      }*/
      return true; //Base::WriteStartObject();
    }

    bool Key(const Ch *str, SizeType length, bool copy = false)
    {
        std::string vector = "[";
        for(unsigned int i = 0; i < level_type.size(); ++i)
      	  vector += std::to_string(level_type[i]) + ", ";
        vector += "]";
    	std::string item = "\\item {\\bf " + std::string(str) + "}: ";
    	if(std::string(str) == "properties")
    	{
    		set_enum_ = true;
    		item = "";
    		//std::cout << "level_type.size() = " << level_type.size() << std::endl;
    		for(unsigned int i = 0; i < level_type.size(); ++i)
    		{
    			if(level_type.back() == 0)
    			{
    			item += " key type=P0 \\end{itemize}";
    			}
    			else if(level_type.back() == 1)
    			{
    				item += " key type=P1";
    				if(path.size() != 0)
    				path.pop_back();
    				break;
    			}else if(level_type.back() == 2)
    			{
    				item += " key type=P2";
    				if(path.size() != 0)
    				path.pop_back();
    			}
    			// otherwise assert?
    		}
    		level_type.resize(0);

    	      //item += "\\section{" + get_path() + "} \\begin{itemize}";
    		// 0 means itemize, 1 means section, 2 means oneof
    		level_type.push_back(1);
        	//path.push_back(str);
    	}
    	else if(std::string(str) == "oneOf")
    	{
    		path.push_back("oneOf");
    		//path.push_back("1");
    		level_type.push_back(2);
    		item = " key type=oneOf \\section{" + get_path() + "} \\begin{itemize}";
    	}
    	else if(std::string(str) == "items")
    	{
    		item = " key type=items \\begin{itemize}";
    		level_type.push_back(0);
    	}
    	else if(level_type.back() == 1)
		{
			//if(path.size() != 0)
				//path.pop_back();
			path.push_back(str);
			item = " key type=1 \\section{" + get_path() + "} \\begin{itemize}";
			level_type.push_back(0);
		}
    	else if(level_type.back() == 2)
    	{
			path.push_back(str);
			item = " key type=2 \\section{" + get_path() + "} \\begin{itemize}";
			level_type.push_back(0);
    	}
    	item += vector;
    	//else
    	//{
  	  //std::string item =  level_type.back() == 1 ? "\\section{" + get_path() + "} \\begin{itemize}" : "\\item {\\bf " + std::string(str) + "}: ";

    	Base::WriteString(item.c_str(), item.length(), false);


    	//}

      return true;
    }

#if RAPIDJSON_HAS_STDSTRING
    bool Key(const std::basic_string<Ch> &str)
    {
      return Key(str.data(), SizeType(str.size()));
    }
#endif

    bool EndObject(SizeType memberCount = 0)
    {
      (void)memberCount;
      //RAPIDJSON_ASSERT(Base::level_stack_.GetSize() >= sizeof(typename Base::Level)); // not inside an Object
      //RAPIDJSON_ASSERT(!Base::level_stack_.template Top<typename Base::Level>()->inArray); // currently inside an Array, not Object
      //RAPIDJSON_ASSERT(0 == Base::level_stack_.template Top<typename Base::Level>()->valueCount % 2); // Object has a Key without a Value

      // (ab)using the internal system.
     /* bool used_enum = Base::level_stack_.template Top<typename Base::Level>()->inArray;

      bool empty = Base::level_stack_.template Pop<typename Base::Level>(1)->valueCount == 0;

      Base::os_->Put('\n');
      if (!empty)
        {
          //Base::os_->Put('\n');
          WriteIndent();
        }*/
      //bool used_enum = true;

      //if(used_enum == true)
      {
    	  //this->os_->Put('\\');
    	  //std::string end = "\\end{enumerate}";
    	  //path.pop_back();
    	  //Base::WriteString(end.c_str(), end.size(), false);
      }
      //else
      std::string vector = "[";
      for(unsigned int i = 0; i < level_type.size(); ++i)
    	  vector += std::to_string(level_type[i]) + ", ";
      vector += "]";
      //std::cout << std::endl;
      std::string end = "";
      if(level_type.back() == 0)
      {
    	  if(path.size() != 0)
    	    path.pop_back();

    	  if(level_type.size() != 0)
    	    level_type.pop_back();
    	  end = " end type=0 \\end{itemize}";
      }
      else if(level_type.back() == 1)
      {
    	  end = "end type=1";
    	  if(path.size() != 0)
    	    path.pop_back();

    	  if(level_type.size() != 0)
    	    level_type.pop_back();
      }
      else if(level_type.back() == 2)
      {
    	  end = "end type=2";
    	  if(level_type.size() != 0)
    	  level_type.pop_back();
    	  if(path.size() != 0)
    	  path.pop_back();
      }
      end += vector;
      Base::WriteString(end.c_str(), end.size(), false);
      return true;
    }

    bool StartArray()
    {
      //LatexPrefix(kArrayType);
      //new (Base::level_stack_.template Push<typename Base::Level>()) typename Base::Level(true);
      //return Base::WriteStartArray();
    	return true;
    }

    bool EndArray(SizeType memberCount = 0)
    {
      (void)memberCount;
      /*RAPIDJSON_ASSERT(Base::level_stack_.GetSize() >= sizeof(typename Base::Level));
      RAPIDJSON_ASSERT(Base::level_stack_.template Top<typename Base::Level>()->inArray);
      bool empty = Base::level_stack_.template Pop<typename Base::Level>(1)->valueCount == 0;

      if (!empty && !(formatOptions_ & lFormatSingleLineArray))
        {
          Base::os_->Put('\n');
          //WriteIndent();
        }
      bool ret = Base::EndValue(Base::WriteEndArray());
      (void)ret;
      RAPIDJSON_ASSERT(ret == true);
      if (Base::level_stack_.Empty()) // end of json text
        Base::Flush();*/
      if(level_type.size() != 0 && level_type.back() == 2)
    	  level_type.pop_back();
      return true;
    }

    //@}

    /*! @name Convenience extensions */
    //@{

    //! Simpler but slower overload.
    bool String(const Ch *str)
    {
      return String(str, internal::StrLen(str));
    }
    bool Key(const Ch *str)
    {
      return Key(str, internal::StrLen(str));
    }

    //@}

    //! Write a raw JSON value.
    /*!
        For user to write a stringified JSON as a value.

        \param json A well-formed JSON value. It should not contain null character within [0, length - 1] range.
        \param length Length of the json.
        \param type Type of the root of json.
        \note When using LatexWriter::RawValue(), the result json may not be indented correctly.
    */
    bool RawValue(const Ch *json, size_t length, Type type)
    {
      RAPIDJSON_ASSERT(json != 0);
      //LatexPrefix(type);
      return Base::EndValue(Base::WriteRawValue(json, length));
    }

  protected:
    void LatexPrefix(Type type)
    {
      (void)type;
      if (Base::level_stack_.GetSize() != 0)   // this value is not at root
        {
          typename Base::Level *level = Base::level_stack_.template Top<typename Base::Level>();

          if (level->inArray)
            {
              if (level->valueCount > 0)
                {
                  //Base::os_->Put(','); // add comma if it is not the first element in array
                  if (formatOptions_ & lFormatSingleLineArray)
                    Base::os_->Put(' ');
                }

              if (!(formatOptions_ & lFormatSingleLineArray))
                {
                  Base::os_->Put('\n');
                  WriteIndent();
                }
            }
          else    // in object
            {
              if (level->valueCount > 0)
                {
                  if (level->valueCount % 2 == 0)
                    {
                      //Base::os_->Put(',');
                      Base::os_->Put('\n');
                    }
                  else
                    {
                      Base::os_->Put(':');
                      if(type == kObjectType)
                      {
                    	  Base::os_->Put('\n');
                    	  WriteIndent();
                      }
                      Base::os_->Put(' ');
                    }
                }
              else
                Base::os_->Put('\n');

              if (level->valueCount % 2 == 0)
                WriteIndent();
            }
          if (!level->inArray && level->valueCount % 2 == 0)
            RAPIDJSON_ASSERT(type == kStringType);  // if it's in object, then even number should be a name
          level->valueCount++;
        }
      else
        {
          RAPIDJSON_ASSERT(!Base::hasRoot_);  // Should only has one and only one root.
          Base::hasRoot_ = true;
        }
    }

    void WriteIndent()
    {
      size_t count = (Base::level_stack_.GetSize() / sizeof(typename Base::Level)) * indentCharCount_;
      PutN(*Base::os_, static_cast<typename OutputStream::Ch>(indentChar_), count);
    }

    std::string get_path()
    {
    	std::string return_path;
    	for(auto string : path)
    	{
    		return_path += "/" + string;
    	}
    	return return_path == "" ? "/" : return_path;
    }

    Ch indentChar_;
    unsigned indentCharCount_;
    LatexFormatOptions formatOptions_;
    std::vector<unsigned int> level_type;
    bool set_enum_ = false;

  private:
    std::vector<std::string> path;
    // Prohibit copy constructor & assignment operator.
    LatexWriter(const LatexWriter &);
    LatexWriter &operator=(const LatexWriter &);
};

RAPIDJSON_NAMESPACE_END

#if defined(__clang__)
RAPIDJSON_DIAG_POP
#endif

#ifdef __GNUC__
RAPIDJSON_DIAG_POP
#endif

#endif // RAPIDJSON_RAPIDJSON_H_
