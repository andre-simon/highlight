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
      : functionType_(LUA_LUA_FUNCTION), size_(luaChunk.size()),
        data_(new char[size_])
   {
      memcpy(data_.get(), luaChunk.c_str(), size_);
   }

   LuaFunction::LuaFunction (const void* data, size_t size)
      : functionType_(LUA_LUA_FUNCTION), size_(size), data_(new char[size_])
   {
      memcpy(data_.get(), data, size);
   }

   LuaFunction::LuaFunction (lua_CFunction func)
      : functionType_(LUA_C_FUNCTION), size_(sizeof(lua_CFunction)),
        data_(new char[sizeof(lua_CFunction)])
   {
      memcpy(data_.get(), reinterpret_cast<lua_CFunction*>(&func),
             sizeof(lua_CFunction));
   }

   LuaFunction::LuaFunction (const LuaFunction& other)
      : functionType_(other.functionType_),
        size_(other.getSize()), data_(new char[size_])
   {
      memcpy (data_.get(), other.getData(), getSize());
   }



   // - LuaFunction::getCFunction ----------------------------------------------
   lua_CFunction LuaFunction::getCFunction() const
   {
      assert(functionType_ == LUA_C_FUNCTION
             && "Called LuaFunction::getCFunction() for a non-C function.");

      return *reinterpret_cast<lua_CFunction*>(data_.get());
   }



   // - LuaFunction::setData ---------------------------------------------------
   void LuaFunction::setData (void* data, size_t size)
   {
      size_ = size;
      data_.reset (new char[size]);
      memcpy(data_.get(), data, size);
   }



   // - LuaFunction::operator= -------------------------------------------------
   const LuaFunction& LuaFunction::operator= (const LuaFunction& rhs)
   {
      size_ = rhs.getSize();
      functionType_ = rhs.functionType_;
      data_.reset (new char[getSize()]);
      memcpy (getData(), rhs.getData(), getSize());
      return *this;
   }



   // - LuaFunction::operator> -------------------------------------------------
   bool LuaFunction::operator> (const LuaFunction& rhs) const
   {
      if (functionType_ > rhs.functionType_)
         return true;
      else if (getSize() > rhs.getSize())
         return true;
      else if (getSize() < rhs.getSize())
         return false;
      else // getSize() == rhs.getSize()
         return memcmp (getData(), rhs.getData(), getSize()) > 0;
   }



   // - LuaFunction::operator< -------------------------------------------------
   bool LuaFunction::operator< (const LuaFunction& rhs) const
   {
      if (functionType_ < rhs.functionType_)
         return true;
      else if (getSize() < rhs.getSize())
         return true;
      else if (getSize() > rhs.getSize())
         return false;
      else // getSize() == rhs.getSize()
         return memcmp (getData(), rhs.getData(), getSize()) < 0;
   }



   // - LuaFunction::operator== ------------------------------------------------
   bool LuaFunction::operator== (const LuaFunction& rhs) const
   {
      return functionType_ == rhs.functionType_
         && getSize() == rhs.getSize()
         && memcmp (getData(), rhs.getData(), getSize()) == 0;
   }



   // - LuaFunction::operator!= ------------------------------------------------
   bool LuaFunction::operator!= (const LuaFunction& rhs) const
   {
      return functionType_ != rhs.functionType_
         || getSize() != rhs.getSize()
         || memcmp (getData(), rhs.getData(), getSize()) != 0;
   }

} // namespace Diluculum
