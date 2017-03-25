/******************************************************************************\
* LuaState                                                                     *
* A pleasant way to use a Lua state in C++.                                    *
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


#ifndef _DILUCULUM_LUA_STATE_HPP_
#define _DILUCULUM_LUA_STATE_HPP_

#include <lua.hpp>
#include <string>
#include <vector>
#include <Diluculum/LuaExceptions.hpp>
#include <Diluculum/LuaValue.hpp>
#include <Diluculum/LuaVariable.hpp>
#include <Diluculum/Types.hpp>


namespace Diluculum
{

   /** \c LuaState: The Next Generation. The pleasant way to do perform relevant
    *  operations on a Lua state.
    *  <p>(My previous implementation of a class named \c LuaState was pretty
    *  low-level. It was essentially a C++ wrapper around a <tt>lua_State*</tt>,
    *  without higher level operations. This is an attempt to allow me to do
    *  the kind of things I do most of the time without much effort.)
    */
   class LuaState
   {
      public:
         /** Constructs a \c LuaState that owns a <tt>lua_State*</tt>. In other
          *  words, this will create the underlying Lua state on construction
          *  and destroy it when this \c LuaState is destroyed.
          *  @param loadStdLib If \c true (the default), makes all
          *         the Lua standard libraries available.
          *  @throw LuaError If something goes wrong.
          */
         explicit LuaState (bool loadStdLib = true);

         /** Constructs a \c LuaState that doesn't own the underlying Lua state.
          *  In other words, this \c LuaState will use a user-supplied
          *  <tt>lua_State*</tt> and its destructor will not \c lua_close() it.
          *  @param state The <tt>lua_State*</tt> that will be used by this
          *         \c LuaState.
          *  @param loadStdLib If \c true, makes all the Lua standard libraries
          *         available (default is \c false, because it is very likely
          *         that the libraries are already opened in the Lua state
          *         passed as the first parameter).
          *  @throw LuaError If something goes wrong.
          */
         explicit LuaState (lua_State* state, bool loadStdLib = false);

         /** Destructs a \c LuaState. If this \c LuaState owns the underlying \c
          *  lua_State*, \c lua_close() will be called on it. See the
          *  constructors' documentation for details on the \c lua_State*
          *  ownership.
          */
         virtual ~LuaState();

         /** Executes the file passed as parameter and returns all the values
          *  returned by this execution. Notice that when a \c LuaValueList is
          *  assigned to a \c LuaValue, just the first value in the list is
          *  assigned, so this is handy for situations when only the first
          *  return value is desired.
          *  @param fileName The file to be executed.
          *  @return All the values returned by the file execution.
          *  @throw LuaError \c LuaError or any of its subclasses can be thrown.
          *         In particular, \c LuaTypeError will be thrown if the
          *         execution returns a type not supported by \c LuaType.
          */
         LuaValueList doFile (const std::string& fileName)
         { return doStringOrFile (false, fileName); }

         /** Executes the string passed as parameter and returns all the values
          *  returned by this execution. Notice that when a \c LuaValueList is
          *  assigned to a \c LuaValue, just the first value in the list is
          *  assigned, so this is handy for situations when only the first
          *  return value is desired.
          *  @param what The string to be interpreted.
          *  @return All the values returned by the execution of \c what.
          *  @throw LuaError \c LuaError or any of its subclasses can be thrown.
          *         In particular, \c LuaTypeError will be thrown if the
          *         execution returns a type not supported by \c LuaType.
          */
         LuaValueList doString (const std::string& what)
         { return doStringOrFile (true, what); }

         /** Calls a given Lua function on this Lua state.
          *  @param func The function to be called.
          *  @param params the list of parameters to pass to the function.
          *  @param chunkName The string to use as the "chunk name" in the
          *         call. This is something added to error messages, in order to
          *         make easier to stop where the error was.
          *  @throw LuaError (or any of its subclasses), if some error is found
          *         during the function execution.
          */
         LuaValueList call (LuaFunction& func,
                            const LuaValueList& params,
                            const std::string& chunkName = "Diluculum chunk");

         /** Returns a \c LuaVariable representing the global variable named
          *  \c variable. Since the returned value also has a subscript
          *  operator, this is a handy way to access variables stored in tables.
          *  Also, since the \c LuaVariable has read/write support, this can be
          *  used even to modify the variables stored in the \c LuaState.
          *  @param variable The name of the global variable to be accessed.
          *  @returns The global variable named \c variable. If no such variable
          *           exists, returns a variable containing \c Nil.
          *  @note This operator does not throw exceptions.
          *  @note Trying to access "_G" (the Lua globals table) with
          *        <tt>LuaState::operator[]</tt> will not work (debug builds
          *        will \c assert()). Please use \c globals() instead.
          */
         LuaVariable operator[] (const std::string& variable);

         /**
          * Provides access to the table of global variables.
          * @note The returned table will not contain "_G" not "package",
          *       because including them would result in tables referencing
          *       themselves in a infinitely recursive manner. In Lua, tables
          *       are reference types, so this recursion is OK. In Diluculum,
          *       tables are value types, so this would result in a crash.
          * @return The table of global variables in this Lua state.
          */
         LuaValueMap globals();

         /// Returns the encapsulated <tt>lua_State*</tt>.
         lua_State* getState() { return state_; }

      private:
         /** Since The implementation of \c doString and \c doFile() are quite
          *  similar, it looked like a good idea to use the same function to
          *  implement both at a lower level. This is it.
          *  @param isString If \c true, means that it is desired to execute the
          *         contents of a string. If \c false, means that it is desired
          *         to execute the contents of a file.
          *  @param str Can be either a string of Lua code to be executed or a
          *         file with Lua code to be executed. The exact interpretation
          *         of this parameter depends on the first parameter,
          *         \c isString.
          *  @throw LuaError \c LuaError or any of its subclasses can be thrown.
          *         In particular, \c LuaTypeError will be thrown if the
          *         execution returns a type not supported by \c LuaType.
          */
         LuaValueList doStringOrFile (bool isString, const std::string& str);

         /// The underlying \c lua_State*.
         lua_State* state_;

         /** Does this \c LuaState owns \c state_? (This used by the destructor
          *  to decide whether it has to \c lua_close() it or not.)
          */
         const bool ownsState_;
   };

} // namespace Diluculum

#endif // #ifndef _DILUCULUM_LUA_STATE_HPP_
