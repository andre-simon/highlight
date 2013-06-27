/******************************************************************************\
* LuaUtils.hpp                                                                 *
* Some utilities related to Lua.                                               *
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

#ifndef _DILUCULUM_LUA_UTILS_HPP_
#define _DILUCULUM_LUA_UTILS_HPP_

#include <Diluculum/LuaValue.hpp>

namespace Diluculum
{

   /** Converts and returns the element at index \c index on the stack to a
    *  \c LuaValue. This keeps the Lua stack untouched. Oh, yes, and it accepts
    *  both positive and negative indices, just like the standard functions on
    *  the Lua C API.
    *  @throw LuaTypeError If the element at \c index cannot be converted to a
    *         \c LuaValue. This can happen if the value at that position is, for
    *         example, a "Lua Thread" that is not supported by \c LuaValue.
    */
   LuaValue ToLuaValue (lua_State* state, int index);

   /** Pushes the value stored at \c value into the Lua stack of \c state. For
    *  most types, this is equivalent to simply calling the appropriate
    *  <tt>lua_push*()</tt> function. For other types, like tables and Lua
    *  functions, the implementation is more complicated.
    *  @note If \c value holds a table, then any entry that happens to have
    *        \c Nil as key will be ignored. (Since Lua does not support \c nil
    *        as a table index.)
    */
   void PushLuaValue (lua_State* state, const LuaValue& value);

} // namespace Diluculum

#endif // _DILUCULUM_LUA_UTILS_HPP_
