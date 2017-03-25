/******************************************************************************\
* LuaExceptions.hpp                                                            *
* Exceptions related to Lua errors.                                            *
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

#ifndef _DILUCULUM_LUA_EXCEPTIONS_HPP_
#define _DILUCULUM_LUA_EXCEPTIONS_HPP_

#include <stdexcept>
#include <Diluculum/Types.hpp>
#include <Diluculum/LuaValue.hpp>


namespace Diluculum
{
   /// A generic Lua-related error.
   class LuaError: public std::runtime_error
   {
      public:
         /** Constructs a \c LuaError object.
          *  @param what The message associated with the error.
          */
         LuaError (const char* what)
            : std::runtime_error (what)
         { }
   };



   /// A Lua run-time error.
   class LuaRunTimeError: public LuaError
   {
      public:
         /** Constructs a \c LuaRunTimeError object.
          *  @param what The message associated with the error. Typically will
          *              be the error message issued by the Lua interpreter.
          */
         LuaRunTimeError (const char* what)
            : LuaError (what)
         { }
   };



   /// A Lua file-related error.
   class LuaFileError: public LuaError
   {
      public:
         /** Constructs a \c LuaFileError object.
          *  @param what The message associated with the error.
          */
         LuaFileError (const char* what)
            : LuaError (what)
         { }
   };



   /// An error while interpreting a chunk of Lua code.
   class LuaSyntaxError: public LuaError
   {
      public:
         /** Constructs a \c LuaSyntaxError object.
          *  @param what The message associated with the error. Typically will be
          *              the error message issued by the Lua interpreter.
          */
         LuaSyntaxError (const char* what)
            : LuaError (what)
         { }
   };



   /// A memory error reported by Lua.
   class LuaMemoryError: public LuaError
   {
      public:
         /** Constructs a \c LuaMemoryError object.
          *  @param what The message associated with the error.
          */
         LuaMemoryError (const char* what)
            : LuaError (what)
         { }
   };



   /// An error while executing the Lua error handler.
   class LuaErrorError: public LuaError
   {
      public:
         /** Constructs a \c LuaErrorError object.
          *  @param what The message associated with the error.
          */
         LuaErrorError (const char* what)
            : LuaError (what)
         { }
   };



   /// An error related to types when doing some Lua-related processing.
   class LuaTypeError: public LuaError
   {
      public:
         /** Constructs a \c LuaTypeError object.
          *  @param what The message associated with the error.
          */
         LuaTypeError (const char* what)
            : LuaError (what)
         { }
   };



   /** An error that happens when a certain type is expected but another one is
    *  found.
    */
   class TypeMismatchError: public LuaError
   {
      public:
         /** Constructs a \c TypeMismatchError object.
          *  @param expectedType The type that was expected.
          *  @param foundType The type that was actually found.
          */
         TypeMismatchError (const std::string& expectedType,
                            const std::string& foundType);

         /** Destroys a \c TypeMismatchError object.
          *  @note This was defined just to pretend that the destructor does not
          *        throw any exception. While this is something that I cannot
          *        guarantee (at least with this implementation), I believe this
          *        not a very dangerous lie.
          */
         ~TypeMismatchError() throw() { };

         /// Returns the type that was expected.
         std::string getExpectedType() const { return expectedType_; }

         /// Returns the type that was actually found.
         std::string getFoundType() const { return foundType_; }

      private:
         /// The type that was expected.
         std::string expectedType_;

         /// The type that was actually found.
         std::string foundType_;
   };

} // namespace Diluculum


#endif // _DILUCULUM_LUA_EXCEPTIONS_HPP_
