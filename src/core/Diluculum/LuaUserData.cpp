/******************************************************************************\
* LuaUserData.cpp                                                              *
* A C++ equivalent of a Lua userdata.                                          *
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

#include <Diluculum/LuaUserData.hpp>
#include <cstring>


namespace Diluculum
{
   // - LuaUserData::LuaUserData -----------------------------------------------
   LuaUserData::LuaUserData (size_t size)
      : size_(size), data_ (new char[size_])
   { }


   LuaUserData::LuaUserData (const LuaUserData& other)
      : size_(other.getSize()), data_ (new char[size_])
   {
      memcpy (data_.get(), other.getData(), getSize());
   }



   // - LuaUserData::operator= -------------------------------------------------
   const LuaUserData& LuaUserData::operator= (const LuaUserData& rhs)
   {
      size_ = rhs.getSize();
      data_.reset (new char[getSize()]);
      memcpy (getData(), rhs.getData(), getSize());
      return *this;
   }



   // - LuaUserData::operator> -------------------------------------------------
   bool LuaUserData::operator> (const LuaUserData& rhs) const
   {
      if (getSize() > rhs.getSize())
         return true;
      else if (getSize() < rhs.getSize())
         return false;
      else // getSize() == rhs.getSize()
         return memcmp (getData(), rhs.getData(), getSize()) > 0;
   }



   // - LuaUserData::operator< -------------------------------------------------
   bool LuaUserData::operator< (const LuaUserData& rhs) const
   {
      if (getSize() < rhs.getSize())
         return true;
      else if (getSize() > rhs.getSize())
         return false;
      else // getSize() == rhs.getSize()
         return memcmp (getData(), rhs.getData(), getSize()) < 0;
   }



   // - LuaUserData::operator== ------------------------------------------------
   bool LuaUserData::operator== (const LuaUserData& rhs) const
   {
      return getSize() == rhs.getSize()
         && memcmp (getData(), rhs.getData(), getSize()) == 0;
   }



   // - LuaUserData::operator!= ------------------------------------------------
   bool LuaUserData::operator!= (const LuaUserData& rhs) const
   {
      return getSize() != rhs.getSize()
         || memcmp (getData(), rhs.getData(), getSize()) != 0;
   }

} // namespace Diluculum
