/******************************************************************************\
* LuaFunction.hpp                                                              *
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

#ifndef _DILUCULUM_LUA_FUNCTION_HPP_
#define _DILUCULUM_LUA_FUNCTION_HPP_

#include <string>
#include <boost/scoped_array.hpp>
#include <lua.hpp>
#include <Diluculum/Types.hpp>


namespace Diluculum
{
   /** A C++ equivalent of a Lua function. This can store both C functions
    *  exported (or exportable) to Lua and pure Lua functions. In the first
    *  case, it stores a \c lua_CFunction. In the second case it stores data
    *  that would normally be in the Lua-space as a function implemented in Lua
    *  (one can think of it as a buffer that stores Lua bytecode that a Lua
    *  interpreter can understand as a function implemented in Lua). This is
    *  used as the value of a \c LuaValue when it holds a Lua function.
    *  @note A \c LuaFunction does not have any reference to a Lua
    *        interpreter. Thus, it does not make sense to call a \c LuaFunction
    *        object directly. To call a \c LuaFunction, use \c LuaState::call().
    */
   class LuaFunction
   {
      public:

         /** Constructs a \c LuaFunction from Lua source code (a Lua chunk). The
          *  default value (a "dummy" chunk, that doesn't return anything) can
          *  be useful for moments in which a default constructable \c
          *  LuaFunction is desired.
          *  @param luaChunk The Lua source of the chunk.
          *  @note The chunk can be seen as a vararg function. So, if one wants,
          *        for instance, a chunk corresponding to a function that
          *        returns its first parameter doubled, the following chunk can
          *        be used: <tt>"local p = {...}; return p[1]*2"</tt>.
          */
         LuaFunction (const::std::string& luaChunk = "");

         /** Constructs a \c LuaFunction from Lua bytecode. Typically, this
          *  constructor is not called directly by end users. Instead, end users
          *  use <tt>LuaState::operator[]</tt> to get a \c LuaVariable
          *  containing a \c LuaFunction value, which can be accessed via
          *  <tt>LuaVariable::value()</tt>.
          *  @param data A pointer to the beginning of the memory area
          *         containing Lua bytecode.
          *  @param size The number of bytecode bytes to read.
          */
         LuaFunction (const void* data, size_t size);

         /** Constructs a \c LuaFunction from a \c lua_CFunction (a Lua function
          * implemented in C).
          */
         LuaFunction (lua_CFunction func);

         /** The copy constructor. The newly constructed \c LuaFunction will
          *  have its own block of memory, with the same contents as the \c
          *  other. In other words, this constructor allocates and copies
          *  memory.
          */
         LuaFunction (const LuaFunction& other);

         /** Assigns a \c LuaFunction to this one. The memory currently
          *  allocated for \c this will be freed, new memory will be allocated,
          *  and the data stored in \c rhs will be copied to \c this.
          */
         const LuaFunction& operator= (const LuaFunction& rhs);

         /**
          * Checks if this \c LuaFunction holds a C function (instead of a
          * "pure" Lua function).
          */
         bool isCFunction() const { return functionType_ == LUA_C_FUNCTION; }

         /**
          * Returns the value of this \c LuaFunction as a C function.
          * @note Will \c assert() if this \c LuaFunction does not hold a C
          *       function.
          */
         lua_CFunction getCFunction() const;

         /** Returns the size, in bytes, of the data stored in this
          *  \c LuaFunction.
          */
         size_t getSize() const { return size_; }

         /// Returns a pointer to the data stored in this \c LuaFunction.
         void* getData() { return data_.get(); }

         /** Returns a \c const pointer to the data stored in this
          *  \c LuaFunction.
          */
         const void* getData() const { return data_.get(); }

         /// Sets the data stored in this \c LuaFunction.
         void setData(void* data, size_t size);

         /// Gets the "reader flag".
         bool getReaderFlag() const { return readerFlag_; }

         /// Sets the "reader flag" to a given value.
         void setReaderFlag(bool f) { readerFlag_ = f; }

         /** The "greater than" operator for \c LuaFunction.
          *  @note Given two <tt>LuaFunction</tt>s, the decision on which one is
          *        greater is somewhat arbitrary. Here, the function with larger
          *        size is considered greater. If both are equal, the decision
          *        is based on the contents of the stored data.
          */
         bool operator> (const LuaFunction& rhs) const;

         /** The "less than" operator for \c LuaFunction.
          *  @note Given two <tt>LuaFunction</tt>s, the decision on which one is
          *        lesser is somewhat arbitrary. The criterion is similar to the
          *        described for the "greater than" operator.
          */
         bool operator< (const LuaFunction& rhs) const;

         /** The "equal to" operator for \c LuaFunction.
          *  @note Two <tt>LuaFunctions</tt>s are considered equal if the data
          *        they store have the same size and the same contents.
          *  @todo In Lua, a function is considered equal only to itself. Things
          *        are different here. Does this have a reason to not be like in
          *        Lua?
          */
         bool operator== (const LuaFunction& rhs) const;

         /// The "different than" operator for \c LuaFunction.
         bool operator!= (const LuaFunction& rhs) const;

      private:
         /** The possible types of functions than possibly be stored in a
          *  \c LuaFunction.
          */
         enum TypeOfFunction
         {
            /// A Lua function implemented in C.
            LUA_C_FUNCTION,

            /// A Lua function implemented in Lua.
            LUA_LUA_FUNCTION
         };

         /// The type of function stored in this \c LuaFunction.
         TypeOfFunction functionType_;

         /// The number of bytes stored "in" \c data_.
         size_t size_;

         /** A (smart) pointer to the data owned by this
          * \c LuaFunction. Depending on \c functionType_, the data pointed to
          * by \c data may store a pointer to a \c lua_CFunction or Lua
          * bytecode.
          */
         boost::scoped_array<char> data_;

         /** A flag used when reading the bytecode data, in calls to \c
          *  lua_load() and its \c lua_Reader.
          */
         bool readerFlag_;
   };

} // namespace Diluculum

#endif // _DILUCULUM_LUA_FUNCTION_HPP_
