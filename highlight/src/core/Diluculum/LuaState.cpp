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

#include <cassert>
#include <cstring>
#include <typeinfo>
#include <boost/lexical_cast.hpp>
#include <Diluculum/LuaState.hpp>
#include <Diluculum/LuaUtils.hpp>
#include "InternalUtils.hpp"


namespace Diluculum
{
   // - LuaState::LuaState -----------------------------------------------------
   LuaState::LuaState (bool loadStdLib)
      : state_(0), ownsState_(true)
   {
      state_ = luaL_newstate();
      if (state_ == 0)
         throw LuaError ("Error opening Lua state.");

      if (loadStdLib)
         luaL_openlibs (state_);
   }


   LuaState::LuaState (lua_State* state, bool loadStdLib)
      : state_(state), ownsState_(false)
   {
      if (state_ == 0)
         throw LuaError ("Constructor of 'LuaState' got a NULL pointer.");

      if (loadStdLib)
         luaL_openlibs (state_);
   }



   // - LuaState::~LuaState ----------------------------------------------------
   LuaState::~LuaState()
   {
      if (ownsState_ && state_ != 0)
         lua_close (state_);
   }



   // - LuaState::doStringOrFile -----------------------------------------------
   LuaValueList LuaState::doStringOrFile (bool isString, const std::string& str)
   {
      const int stackSizeAtBeginning = lua_gettop (state_);

      if (isString)
      {
         Impl::ThrowOnLuaError (state_, luaL_loadbuffer (state_, str.c_str(),
                                                         str.length(), "line"));
      }
      else
      {
         Impl::ThrowOnLuaError (state_, luaL_loadfile (state_, str.c_str()));
      }

      Impl::ThrowOnLuaError (state_, lua_pcall (state_, 0, LUA_MULTRET, 0));

      const int numResults = lua_gettop (state_) - stackSizeAtBeginning;

      LuaValueList results;

      for (int i = numResults; i > 0; --i)
         results.push_back (ToLuaValue (state_, -i));

      lua_pop (state_, numResults);

      return results;
   }



   // - LuaState::call ---------------------------------------------------------
   LuaValueList LuaState::call (LuaFunction& func,
                                const LuaValueList& params,
                                const std::string& chunkName)
   {
      func.setReaderFlag (false);
      PushLuaValue (state_, LuaValue (func));
      return Impl::CallFunctionOnTop (state_, params);
   }


   // - LuaState::operator[] ---------------------------------------------------
   LuaVariable LuaState::operator[] (const std::string& variable)
   {
      assert(variable != "_G" && "Can't access '_G'; use LuaState::globals().");

      return LuaVariable (state_, variable);
   }


   // - LuaState::globals ------------------------------------------------------
   LuaValueMap LuaState::globals()
   {
      // Traverse the globals table adding the key/value pairs to 'ret'
      LuaValueMap ret;

#if LUA_VERSION_NUM >= 502
      // Obtain global table
      lua_rawgeti (state_, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
#endif
      
      lua_pushnil (state_);
      
#if LUA_VERSION_NUM >= 502
      while (lua_next (state_, -2) != 0)
#else
      while (lua_next (state_, LUA_GLOBALSINDEX) != 0)
#endif
      {
         // Exclude from the results the tables that would result in infinite
         // recursion
         if (lua_type (state_, -2) == LUA_TSTRING)
         {
            const char* key = lua_tostring(state_, -2);
            if (strcmp(key, "_G") == 0
                || strcmp(key, "package") == 0)
            {
               lua_pop (state_, 1);
               continue;
            }
         }

         ret[ToLuaValue (state_, -2)] = ToLuaValue (state_, -1);

         lua_pop (state_, 1);
      }
      
#if LUA_VERSION_NUM >= 502
      // Remove global table
      lua_remove (state_, -2);
#endif
      // Alright, return the result
      return ret;
   }

} // namespace Diluculum
