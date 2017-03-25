/******************************************************************************\
* Types.hpp                                                                    *
* The place for assorted types.                                                *
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

#ifndef _DILUCULUM_TYPES_HPP_
#define _DILUCULUM_TYPES_HPP_

#include <map>
#include <vector>


namespace Diluculum
{
   class LuaValue;

   /** A list of <tt>LuaValue</tt>s. Used, for example, to represent the return
    *  value of a Lua function call. In this case, the first return value is
    *  stored at the 0th \c vector position, the second return value at the 1st
    *  \c vector position and so on.
    */
   typedef std::vector<LuaValue> LuaValueList;

   /** Type mapping from <tt>LuaValue</tt>s to <tt>LuaValue</tt>s. Think of it
    *  as a C++ approximation of a Lua table.
    */
   typedef std::map<LuaValue, LuaValue> LuaValueMap;

} // namespace Diluculum

#endif // _DILUCULUM_TYPES_HPP_
