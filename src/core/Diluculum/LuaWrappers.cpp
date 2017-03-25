/******************************************************************************\
* LuaWrappers.cpp                                                              *
* Making C++ stuff accessible from Lua.                                        *
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

#include <Diluculum/LuaWrappers.hpp>
#include <cassert>


namespace Diluculum
{
   namespace Impl
   {
      // - ReportErrorFromCFunction --------------------------------------------
      void ReportErrorFromCFunction (lua_State* ls, const::std::string& what)
      {
         lua_Debug ar;
         int ret = lua_getstack (ls, 0, &ar);

         assert (ret != 0 && "'lua_getstack()' wasn't supposed to return '0' "
                 "here. Possible error cause: 'ReportErrorFromCFunction()' "
                 "wasn't called from a Lua function implemented in C.");

         ret = lua_getinfo (ls, "n", &ar);

         assert (ret != 0 && "'lua_getinfo()' wasn't supposed to return '1' "
                 "here. *Nothing* could go wrong at this point! Oh, well...");

         const std::string msg = std::string("Error found when calling '")
            + ar.name + "': " + what;

         lua_pushstring (ls, msg.c_str());
         lua_error (ls);
      }
   }
}
