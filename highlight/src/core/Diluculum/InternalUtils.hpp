/******************************************************************************\
* InternalUtils.hpp                                                            *
* Miscellaneous utilities used internally by Diluculum.                        *
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

#ifndef _DILUCULUM_INTERNAL_UTILS_HPP_
#define _DILUCULUM_INTERNAL_UTILS_HPP_

#include <Diluculum/LuaState.hpp>


namespace Diluculum
{
   namespace Impl
   {
      /** Calls the function on the top of the stack, passing the given
       *  parameters. Returns the values returned by the called function.
       *  @param ls The Lua state from where the Lua function will be taken, and
       *         where the function will be executed.
       *  @param params The parameters to be passed to the function.
       */
      LuaValueList CallFunctionOnTop (lua_State* ls, const LuaValueList& params);

      /** Throws an exception if the status code passed as parameter corresponds
       *  to an error code from a function from the Lua API.  The exception
       *  thrown is of the proper type, that is, of the subclass of \c LuaError
       *  that best describes the error. Furthermore, the string associated with
       *  the exception (the \c what parameter) is set to whatever Lua returned
       *  as the error message.
       *  <p>If the number passed as parameter is 0 (which is the code for "no
       *  error"), the function does nothing.
       *  @param statusCode The return code for some Lua API function; that's
       *         the value we want to check for "errorness".
       *  @param ls The Lua state on which the function that generated the
       *         status code being tested was called. This is used just to get a
       *         nice error message, if an error did actually happen.
       *  @throw LuaRunTimeError If <tt>retCode == LUA_ERRRUN</tt>.
       *  @throw LuaFileError If <tt>retCode == LUA_ERRFILE</tt>.
       *  @throw LuaRunTimeError If <tt>retCode == LUA_ERRRUN</tt>.
       *  @throw LuaSyntaxError If <tt>retCode == LUA_ERRSYNTAX</tt>.
       *  @throw LuaMemoryError If <tt>retCode == LUA_ERRMEM</tt>.
       *  @throw LuaError If \c retCode is not a recognized Lua error code.
       */
      void ThrowOnLuaError (lua_State* ls, int statusCode);

      /** The \c lua_Writer used in the calls to \c lua_dump() when converting a
       * function implemented in Lua to a \c LuaFunction.
       */
      int LuaFunctionWriter(lua_State* luaState, const void* data, size_t size,
                            void* func);

      /** The \c lua_Reader used to get Lua bytecode from a \c LuaFunction. This
       *  is used by \c LuaState::call();
       */
      const char* LuaFunctionReader(lua_State* luaState, void* func,
                                    size_t* size);
   }

} // namespace Diluculum


#endif // _DILUCULUM_INTERNAL_UTILS_HPP_
