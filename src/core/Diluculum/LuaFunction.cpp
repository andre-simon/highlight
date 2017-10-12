/******************************************************************************\
* LuaFunction.cpp                                                              *
* A C++ equivalent of a Lua function.                                          *
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

#include <Diluculum/LuaFunction.hpp>
#include <cstring>


namespace Diluculum
{
   // - LuaFunction::LuaFunction -----------------------------------------------
   LuaFunction::LuaFunction (const std::string& luaChunk)
      : functionType_(LUA_LUA_FUNCTION), size_(luaChunk.size())
   {
      data_.typeLuaFunction = new char[size_];
      memcpy(data_.typeLuaFunction, luaChunk.c_str(), size_);
   }

   LuaFunction::LuaFunction (const void* data, size_t size)
      : functionType_(LUA_LUA_FUNCTION), size_(size)
   {
      data_.typeLuaFunction = new char[size_];
      memcpy(data_.typeLuaFunction, data, size);
   }

   LuaFunction::LuaFunction (lua_CFunction func)
      : functionType_(LUA_C_FUNCTION), size_(sizeof(lua_CFunction))
   {
      data_.typeCFunction = func;
   }

   LuaFunction::LuaFunction (const LuaFunction& other)
      : functionType_(other.functionType_),
        size_(other.getSize())
   {
      switch (functionType_)
      {
         case LUA_LUA_FUNCTION:
            data_.typeLuaFunction = new char[getSize()];
            memcpy (getData(), other.getData(), getSize());
            break;

         default:
            // no constructor needed.
            memcpy (&data_, &other.data_, sizeof(PossibleTypes));
            break;
      }
   }



   // - LuaFunction::getCFunction ----------------------------------------------
   lua_CFunction LuaFunction::getCFunction() const
   {
      assert(functionType_ == LUA_C_FUNCTION
             && "Called LuaFunction::getCFunction() for a non-C function.");

      return data_.typeCFunction;
   }



   // - LuaFunction::setData ---------------------------------------------------
   void LuaFunction::setData (void* data, size_t size)
   {
      assert(functionType_ == LUA_LUA_FUNCTION
             && "Called LuaFunction::setData() for a non-Lua function.");

      size_ = size;
      delete[] data_.typeLuaFunction;
      data_.typeLuaFunction = new char[size];
      memcpy(data_.typeLuaFunction, data, size);
   }



   // - LuaFunction::operator= -------------------------------------------------
   const LuaFunction& LuaFunction::operator= (const LuaFunction& rhs)
   {
      destroyObjectAtData();

      size_ = rhs.getSize();
      functionType_ = rhs.functionType_;

      switch (functionType_)
      {
         case LUA_LUA_FUNCTION:
            data_.typeLuaFunction = new char[getSize()];
            memcpy (getData(), rhs.getData(), getSize());
            break;

         default:
            // no constructor needed.
            memcpy (&data_, &rhs.data_, sizeof(PossibleTypes));
            break;
      }

      return *this;
   }



   // - LuaFunction::operator> -------------------------------------------------
   bool LuaFunction::operator> (const LuaFunction& rhs) const
   {
      if (functionType_ > rhs.functionType_)
         return true;
      if (functionType_ < rhs.functionType_)
         return false;
      else // functionType_ == rhs.functionType_
      {
         if (functionType_ == LUA_C_FUNCTION)
            return memcmp (&data_.typeCFunction, &rhs.data_.typeCFunction, sizeof(lua_CFunction)) > 0;
         else if (functionType_ == LUA_LUA_FUNCTION)
            if (getSize() > rhs.getSize())
               return true;
            else if (getSize() < rhs.getSize())
               return false;
            else // getSize() == rhs.getSize()
               return memcmp (getData(), rhs.getData(), getSize()) > 0;
         else
         {
            assert (false && "Unsupported type found at a call "
                    "to 'LuaFunction::operator>()'");
            return false; // make the compiler happy.
         }
      }
   }



   // - LuaFunction::operator< -------------------------------------------------
   bool LuaFunction::operator< (const LuaFunction& rhs) const
   {
      if (functionType_ < rhs.functionType_)
         return true;
      else if (functionType_ > rhs.functionType_)
         return false;
      else // functionType_ == rhs.functionType_
      {
         if (functionType_ == LUA_C_FUNCTION)
            return memcmp (&data_.typeCFunction, &rhs.data_.typeCFunction, sizeof(lua_CFunction)) < 0;
         else if (functionType_ == LUA_LUA_FUNCTION)
         {
            if (getSize() < rhs.getSize())
               return true;
            else if (getSize() > rhs.getSize())
               return false;
            else // getSize() == rhs.getSize()
               return memcmp (getData(), rhs.getData(), getSize()) < 0;
         }
         else
         {
            assert (false && "Unsupported type found at a call "
                    "to 'LuaFunction::operator<()'");
            return false; // make the compiler happy.
         }
      }
   }



   // - LuaFunction::operator== ------------------------------------------------
   bool LuaFunction::operator== (const LuaFunction& rhs) const
   {
      if (functionType_ != rhs.functionType_)
         return false;
      else switch (functionType_)
      {
         case LUA_C_FUNCTION:
            return getCFunction() == rhs.getCFunction();

         case LUA_LUA_FUNCTION:
            return getSize() == rhs.getSize()
               && memcmp (getData(), rhs.getData(), getSize()) == 0;

         default:
         {
            assert(
               false
               && "Invalid type found in a call to 'LuaFunction::operator==()'.");
            return false; // make compilers happy
         }
      }
   }



   // - LuaFunction::operator!= ------------------------------------------------
   bool LuaFunction::operator!= (const LuaFunction& rhs) const
   {
      if (functionType_ != rhs.functionType_)
         return true;
      else switch (functionType_)
      {
         case LUA_C_FUNCTION:
            return getCFunction() != rhs.getCFunction();

         case LUA_LUA_FUNCTION:
            return getSize() != rhs.getSize()
               || memcmp (getData(), rhs.getData(), getSize()) != 0;

         default:
         {
            assert(
               false
               && "Invalid type found in a call to 'LuaFunction::operator!=()'.");
            return false; // make compilers happy
         }
      }
   }



   // - LuaFunction::destroyObjectAtData ---------------------------------------
   void LuaFunction::destroyObjectAtData()
   {
      switch (functionType_)
      {
         case LUA_LUA_FUNCTION:
            delete[] data_.typeLuaFunction;
            break;

         default:
            // no destructor needed.
            break;
      }
   }

} // namespace Diluculum
