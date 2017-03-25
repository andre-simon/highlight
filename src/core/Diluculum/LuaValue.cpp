/******************************************************************************\
* LuaValue.cpp                                                                 *
* A class that somewhat mimics a Lua value.                                    *
*                                                                              *
*                                                                              *
* Copyright (C) 2005-2013 by Leandro Motta Barros.                             *
*                                                                              *
* Permission is hereby granted, free of charge, to any person obtaining a copy *
* of this software and associated documentation files (the "Software"), to     *
* deal in the Software without restriction, including without limitation the   *
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or  *
* sell copies of the Software, and to permit persons to whom the Software is   *
* furnished to do so, subject to the following conditions:                     *
*                                                                              *
* The above copyright notice and this permission notice shall be included in   *
* all copies or substantial portions of the Software.                          *
*                                                                              *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR   *
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,     *
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE *
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER       *
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      *
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS *
* IN THE SOFTWARE.                                                             *
\******************************************************************************/

#include <cstring>
#include <Diluculum/LuaValue.hpp>
#include <Diluculum/LuaExceptions.hpp>


namespace Diluculum
{
   // - LuaValue::LuaValue -----------------------------------------------------
   LuaValue::LuaValue()
      : dataType_(LUA_TNIL)
   { }


   LuaValue::LuaValue (bool b)
      : dataType_(LUA_TBOOLEAN)
   {
      memcpy (data_, &b, sizeof(bool));
   }


   LuaValue::LuaValue (float n)
      : dataType_(LUA_TNUMBER)
   {
      lua_Number num = static_cast<lua_Number>(n);
      memcpy (data_, &num, sizeof(lua_Number));
   }


   LuaValue::LuaValue (double n)
      : dataType_(LUA_TNUMBER)
   {
      lua_Number num = static_cast<lua_Number>(n);
      memcpy (data_, &num, sizeof(lua_Number));
   }


   LuaValue::LuaValue (long double n)
      : dataType_(LUA_TNUMBER)
   {
      lua_Number num = static_cast<lua_Number>(n);
      memcpy (data_, &num, sizeof(lua_Number));
   }


   LuaValue::LuaValue (short n)
      : dataType_(LUA_TNUMBER)
   {
      lua_Number num = static_cast<lua_Number>(n);
      memcpy (data_, &num, sizeof(lua_Number));
   }


   LuaValue::LuaValue (unsigned short n)
      : dataType_(LUA_TNUMBER)
   {
      lua_Number num = static_cast<lua_Number>(n);
      memcpy (data_, &num, sizeof(lua_Number));
   }


   LuaValue::LuaValue (int n)
      : dataType_(LUA_TNUMBER)
   {
      lua_Number num = static_cast<lua_Number>(n);
      memcpy (data_, &num, sizeof(lua_Number));
   }


   LuaValue::LuaValue (unsigned n)
      : dataType_(LUA_TNUMBER)
   {
      lua_Number num = static_cast<lua_Number>(n);
      memcpy (data_, &num, sizeof(lua_Number));
   }


   LuaValue::LuaValue (long n)
      : dataType_(LUA_TNUMBER)
   {
      lua_Number num = static_cast<lua_Number>(n);
      memcpy (data_, &num, sizeof(lua_Number));
   }


   LuaValue::LuaValue (unsigned long n)
      : dataType_(LUA_TNUMBER)
   {
      lua_Number num = static_cast<lua_Number>(n);
      memcpy (data_, &num, sizeof(lua_Number));
   }


   LuaValue::LuaValue (const std::string& s)
      : dataType_(LUA_TSTRING)
   {
      new(data_) std::string(s);
   }


   LuaValue::LuaValue (const char* s)
      : dataType_(LUA_TSTRING)
   {
      new(data_) std::string(s);
   }


   LuaValue::LuaValue (const LuaValueMap& t)
      : dataType_(LUA_TTABLE)
   {
      new(data_) LuaValueMap(t);
   }


   LuaValue::LuaValue (lua_CFunction f)
      : dataType_(LUA_TFUNCTION)
   {
      new(data_) LuaFunction(f);
   }


   LuaValue::LuaValue (const LuaFunction& f)
      : dataType_(LUA_TFUNCTION)
   {
      new(data_) LuaFunction(f);
   }


   LuaValue::LuaValue (const LuaUserData& ud)
      : dataType_(LUA_TUSERDATA)
   {
      new(data_) LuaUserData(ud);
   }


   LuaValue::LuaValue (const LuaValueList& v)
      // Avoids possible memory corruption during destroyObjectAtData
      : dataType_(LUA_TNIL)
   {
      if (v.size() >= 1)
         *this = v[0];
      else
         *this = Nil;
   }


   LuaValue::LuaValue (const LuaValue& other)
      : dataType_ (other.dataType_)
   {
      switch (dataType_)
      {
         case LUA_TSTRING:
            new(data_) std::string(other.asString());
            break;

         case LUA_TTABLE:
            new(data_) LuaValueMap (other.asTable());
            break;

         case LUA_TUSERDATA:
            new(data_) LuaUserData (other.asUserData());
            break;

         case LUA_TFUNCTION:
            new(data_) LuaFunction (other.asFunction());
            break;

         default:
            // no constructor needed.
            memcpy (data_, other.data_, sizeof(PossibleTypes));
            break;
      }
   }



   // - LuaValue::operator= ----------------------------------------------------
   LuaValue& LuaValue::operator= (const LuaValue& rhs)
   {
      destroyObjectAtData();

      dataType_ = rhs.dataType_;

      switch (dataType_)
      {
         case LUA_TSTRING:
            new(data_) std::string (rhs.asString());
            break;

         case LUA_TTABLE:
            new(data_) LuaValueMap (rhs.asTable());
            break;

         case LUA_TUSERDATA:
            new(data_) LuaUserData (rhs.asUserData());
            break;

         case LUA_TFUNCTION:
            new(data_) LuaFunction (rhs.asFunction());
            break;

         default:
            // no constructor needed.
            memcpy (data_, rhs.data_, sizeof(PossibleTypes));
            break;
      }

      return *this;
   }


   const LuaValueList& LuaValue::operator= (const LuaValueList& rhs)
   {
      if (rhs.size() >= 1)
         *this = rhs[0];
      else
         *this = Nil;

      return rhs;
   }



   // - LuaValue::typeName -----------------------------------------------------
   std::string LuaValue::typeName() const
   {
      switch (dataType_)
      {
         case LUA_TNIL:
            return "nil";

         case LUA_TBOOLEAN:
            return "boolean";

         case LUA_TNUMBER:
            return "number";

         case LUA_TSTRING:
            return "string";

         case LUA_TTABLE:
            return "table";

         case LUA_TFUNCTION:
            return "function";

         case LUA_TUSERDATA:
            return "userdata";

         default: // can't happen
            assert (false
                    && "Invalid type found in a call to 'LuaValue::typeName()'.");
            return ""; // return something to make compilers happy.
      }
   }



   // - LuaValue::asNumber() ---------------------------------------------------
   lua_Number LuaValue::asNumber() const
   {
      if (dataType_ == LUA_TNUMBER)
      {
         const lua_Number* pn = reinterpret_cast<const lua_Number*>(&data_);
         return *pn;
      }
      else
      {
         throw TypeMismatchError ("number", typeName());
      }
   }



   // - LuaValue::asInteger() --------------------------------------------------
   lua_Integer LuaValue::asInteger() const
   {
      if (dataType_ == LUA_TNUMBER)
      {
         const lua_Number* num = reinterpret_cast<const lua_Number*>(&data_);
         lua_Integer res = static_cast<lua_Integer>(*num);
         return res;
      }
      else
      {
         throw TypeMismatchError ("number", typeName());
      }
   }



   // - LuaValue::asString -----------------------------------------------------
   const std::string& LuaValue::asString() const
   {
      if (dataType_ == LUA_TSTRING)
      {
         const std::string* ps = reinterpret_cast<const std::string*>(&data_);
         return *ps;
      }
      else
      {
         throw TypeMismatchError ("string", typeName());
      }
   }



   // - LuaValue::asBoolean ----------------------------------------------------
   bool LuaValue::asBoolean() const
   {
      if (dataType_ == LUA_TBOOLEAN)
      {
         const bool* pb = reinterpret_cast<const bool*>(&data_);
         return *pb;
      }
      else
      {
         throw TypeMismatchError ("boolean", typeName());
      }
   }



   // - LuaValue::asTable ------------------------------------------------------
   LuaValueMap LuaValue::asTable() const
   {
      if (dataType_ == LUA_TTABLE)
      {
         const LuaValueMap* pm = reinterpret_cast<const LuaValueMap*>(&data_);
         return *pm;
      }
      else
      {
         throw TypeMismatchError ("table", typeName());
      }
   }



   // - LuaValue::asFunction ---------------------------------------------------
   const LuaFunction& LuaValue::asFunction() const
   {
      if (dataType_ == LUA_TFUNCTION)
      {
         const LuaFunction* pf = reinterpret_cast<const LuaFunction*>(&data_);
         return *pf;
      }
      else
      {
         throw TypeMismatchError ("function", typeName());
      }
   }



   // - LuaValue::asUserData ---------------------------------------------------
   const LuaUserData& LuaValue::asUserData() const
   {
      if (dataType_ == LUA_TUSERDATA)
      {
         const LuaUserData* pd = reinterpret_cast<const LuaUserData*>(&data_);
         return *pd;
      }
      else
      {
         throw TypeMismatchError ("userdata", typeName());
      }
   }

   LuaUserData& LuaValue::asUserData()
   {
      if (dataType_ == LUA_TUSERDATA)
      {
         LuaUserData* pd = reinterpret_cast<LuaUserData*>(&data_);
         return *pd;
      }
      else
      {
         throw TypeMismatchError ("userdata", typeName());
      }
   }



   // - LuaValue::operator< ----------------------------------------------------
   bool LuaValue::operator< (const LuaValue& rhs) const
   {
      std::string lhsTypeName = typeName();
      std::string rhsTypeName = rhs.typeName();

      if (lhsTypeName < rhsTypeName)
         return true;
      else if (lhsTypeName > rhsTypeName)
         return false;
      else // lhsTypeName == rhsTypeName
      {
         if (lhsTypeName == "nil")
            return false;
         else if (lhsTypeName == "boolean")
            return asBoolean() < rhs.asBoolean();
         else if (lhsTypeName == "number")
            return asNumber() < rhs.asNumber();
         else if (lhsTypeName == "string")
            return asString() < rhs.asString();
         else if (lhsTypeName == "function")
            return asFunction() < rhs.asFunction();
         else if (lhsTypeName == "userdata")
            return asUserData() < rhs.asUserData();
         else if (lhsTypeName == "table")
         {
            const LuaValueMap lhsMap = asTable();
            const LuaValueMap rhsMap = rhs.asTable();

            if (lhsMap.size() < rhsMap.size())
               return true;
            else if (lhsMap.size() > rhsMap.size())
               return false;
            else // lhsMap.size() == rhsMap.size()
            {
               typedef LuaValueMap::const_iterator iter_t;

               iter_t pLHS = lhsMap.begin();
               iter_t pRHS = rhsMap.begin();
               const iter_t end = lhsMap.end();

               while (pLHS != end)
               {
                  // check the key first
                  if (pLHS->first < pRHS->first)
                     return true;
                  else if (pLHS->first > pRHS->first)
                     return false;

                  // then check the value
                  if (pLHS->second < pRHS->second)
                     return true;
                  else if (pLHS->second > pRHS->second)
                     return false;

                  // make the iterators iterate
                  ++pRHS;
                  ++pLHS;
               }
               return false;
            }
         }
         else
         {
            assert (false && "Unsupported type found at a call "
                    "to 'LuaValue::operator<()'");
            return false; // make the compiler happy.
         }
      }
   }



   // - LuaValue::operator> ----------------------------------------------------
   bool LuaValue::operator> (const LuaValue& rhs) const
   {
      std::string lhsTypeName = typeName();
      std::string rhsTypeName = rhs.typeName();

      if (lhsTypeName > rhsTypeName)
         return true;
      else if (lhsTypeName < rhsTypeName)
         return false;
      else // lhsTypeName == rhsTypeName
      {
         if (lhsTypeName == "nil")
            return false;
         else if (lhsTypeName == "boolean")
            return asBoolean() > rhs.asBoolean();
         else if (lhsTypeName == "number")
            return asNumber() > rhs.asNumber();
         else if (lhsTypeName == "string")
            return asString() > rhs.asString();
         else if (lhsTypeName == "function")
            return asFunction() > rhs.asFunction();
         else if (lhsTypeName == "userdata")
            return asUserData() > rhs.asUserData();
         else if (lhsTypeName == "table")
         {
            const LuaValueMap lhsMap = asTable();
            const LuaValueMap rhsMap = rhs.asTable();

            if (lhsMap.size() > rhsMap.size())
               return true;
            else if (lhsMap.size() < rhsMap.size())
               return false;
            else // lhsMap.size() == rhsMap.size()
            {
               typedef LuaValueMap::const_iterator iter_t;

               iter_t pLHS = lhsMap.begin();
               iter_t pRHS = rhsMap.begin();
               const iter_t end = lhsMap.end();

               while (pLHS != end)
               {
                  // check the key first
                  if (pLHS->first > pRHS->first)
                     return true;
                  else if (pLHS->first < pRHS->first)
                     return false;

                  // then check the value
                  if (pLHS->second > pRHS->second)
                     return true;
                  else if (pLHS->second < pRHS->second)
                     return false;

                  // make the iterators iterate
                  ++pRHS;
                  ++pLHS;
               }
               return false;
            }
         }
         else
         {
            assert (false && "Unsupported type found at a call "
                    "to 'LuaValue::operator>()'");
            return false; // make the compiler happy.
         }
      }
   }



   // - LuaValue::operator== ---------------------------------------------------
   bool LuaValue::operator== (const LuaValue& rhs) const
   {
      std::string lhsTypeName = typeName();
      std::string rhsTypeName = rhs.typeName();

      if (typeName() != rhs.typeName())
         return false;
      else switch (type())
      {
         case LUA_TNIL:
            return true;

         case LUA_TBOOLEAN:
            return asBoolean() == rhs.asBoolean();

         case LUA_TNUMBER:
            return asNumber() == rhs.asNumber();

         case LUA_TSTRING:
            return asString() == rhs.asString();

         case LUA_TTABLE:
            return asTable() == rhs.asTable();

         case LUA_TFUNCTION:
            return asFunction() == rhs.asFunction();

         case LUA_TUSERDATA:
            return asUserData() == rhs.asUserData();

         default:
         {
            assert(
               false
               && "Invalid type found in a call to 'LuaValue::operator==()'.");
            return 0; // make compilers happy
         }
      }
   }



   // - LuaValue::operator[] ---------------------------------------------------
   LuaValue& LuaValue::operator[] (const LuaValue& key)
   {
      if (type() != LUA_TTABLE)
         throw TypeMismatchError ("table", typeName());

      LuaValueMap* pTable = reinterpret_cast<LuaValueMap*>(data_);

      return (*pTable)[key];
   }



   const LuaValue& LuaValue::operator[] (const LuaValue& key) const
   {
      if (type() != LUA_TTABLE)
         throw TypeMismatchError ("table", typeName());

      const LuaValueMap* pTable = reinterpret_cast<const LuaValueMap*>(data_);

      LuaValueMap::const_iterator it = (*pTable).find(key);

      if (it == (*pTable).end())
         return Nil;

      return it->second;
   }



   // - LuaValue::destroyObjectAtData ------------------------------------------
   void LuaValue::destroyObjectAtData()
   {
      switch (dataType_)
      {
         case LUA_TSTRING:
         {
            std::string* ps = reinterpret_cast<std::string*>(data_);
            ps->~basic_string();
            break;
         }

         case LUA_TTABLE:
         {
            LuaValueMap* pm = reinterpret_cast<LuaValueMap*>(data_);
            pm->~LuaValueMap();
            break;
         }

         case LUA_TUSERDATA:
         {
            LuaUserData* pd = reinterpret_cast<LuaUserData*>(data_);
            pd->~LuaUserData();
            break;
         }

         case LUA_TFUNCTION:
         {
            LuaFunction* pf = reinterpret_cast<LuaFunction*>(data_);
            pf->~LuaFunction();
            break;
         }

         default:
            // no destructor needed.
            break;
      }
   }

} // namespace Diluculum
