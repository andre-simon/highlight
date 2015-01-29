/******************************************************************************\
* LuaVariable.cpp                                                              *
* A variable living in a Lua interpreter.                                      *
*                                                                              *
*                                                                              *
* Copyright (C) 2005-2010 by Leandro Motta Barros.                             *
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
#include <boost/lexical_cast.hpp>
#include <Diluculum/LuaState.hpp>
#include <Diluculum/LuaUserData.hpp>
#include <Diluculum/LuaUtils.hpp>
#include <Diluculum/LuaVariable.hpp>
#include "InternalUtils.hpp"


namespace Diluculum
{
   // - LuaVariable::LuaVariable -----------------------------------------------
   LuaVariable::LuaVariable (lua_State* state, const LuaValue& key,
                             const KeyList& predKeys)
      : state_(state), keys_(predKeys)
   {
      keys_.push_back (key);
   }



   // - LuaVariable::operator= -------------------------------------------------
   const LuaValue& LuaVariable::operator= (const LuaValue& rhs)
   {
      pushLastTable();
      PushLuaValue (state_, keys_.back());
      PushLuaValue (state_, rhs);
      lua_settable (state_, -3);
      lua_pop (state_, 1);

      return rhs;
   }



   // - LuaVariable::value -----------------------------------------------------
   LuaValue LuaVariable::value() const
   {
      pushTheReferencedValue();
      LuaValue ret = ToLuaValue (state_, -1);
      lua_pop (state_, 1);
      return ret;
   }



   // - LuaVariable::operator[] ------------------------------------------------
   LuaVariable LuaVariable::operator[] (const LuaValue& key) const
   {
      return LuaVariable (state_, key, keys_);
   }



   // - LuaVariable::operator() ------------------------------------------------
   LuaValueList LuaVariable::operator() (const LuaValueList& params)
   {
      pushTheReferencedValue();
      return Impl::CallFunctionOnTop (state_, params);
   }

   LuaValueList LuaVariable::operator()()
   {
      return (*this)(LuaValueList());
   }

   LuaValueList LuaVariable::operator() (const LuaValue& param)
   {
      LuaValueList params;
      params.push_back (param);
      return (*this)(params);
   }

   LuaValueList LuaVariable::operator() (const LuaValue& param1,
                                         const LuaValue& param2)
   {
      LuaValueList params;
      params.push_back (param1);
      params.push_back (param2);
      return (*this)(params);
   }

   LuaValueList LuaVariable::operator() (const LuaValue& param1,
                                         const LuaValue& param2,
                                         const LuaValue& param3)
   {
      LuaValueList params;
      params.push_back (param1);
      params.push_back (param2);
      params.push_back (param3);
      return (*this)(params);
   }

   LuaValueList LuaVariable::operator() (const LuaValue& param1,
                                         const LuaValue& param2,
                                         const LuaValue& param3,
                                         const LuaValue& param4)
   {
      LuaValueList params;
      params.push_back (param1);
      params.push_back (param2);
      params.push_back (param3);
      params.push_back (param4);
      return (*this)(params);
   }

   LuaValueList LuaVariable::operator() (const LuaValue& param1,
                                         const LuaValue& param2,
                                         const LuaValue& param3,
                                         const LuaValue& param4,
                                         const LuaValue& param5)
   {
      LuaValueList params;
      params.push_back (param1);
      params.push_back (param2);
      params.push_back (param3);
      params.push_back (param4);
      params.push_back (param5);
      return (*this)(params);
   }



   // - LuaVariable::pushLastTable ---------------------------------------------
   void LuaVariable::pushLastTable()
   {
      // Push the globals table onto the stack
#if LUA_VERSION_NUM >= 502
      lua_pushglobaltable (state_);
#else
      lua_pushstring (state_, "_G");
      lua_gettable (state_, LUA_GLOBALSINDEX);
#endif
      // Reach the "final" table (and leave it at the stack top)
      typedef KeyList::const_iterator iter_t;

      assert (keys_.size() > 0 && "At least one key should be present here.");

      iter_t end = keys_.end();
      --end;

      for (iter_t p = keys_.begin(); p != end; ++p)
      {
         PushLuaValue (state_, *p);
         lua_gettable (state_, -2);
         if (!lua_istable (state_, -1))
            throw TypeMismatchError ("table", luaL_typename (state_, -1));
         lua_remove (state_, -2);
      }
   }



   // - LuaVariable::pushTheReferencedValue ------------------------------------
   void LuaVariable::pushTheReferencedValue() const
   {
      assert (keys_.size() > 0 && "There should be at least one key here.");

#if LUA_VERSION_NUM >= 502
      lua_rawgeti (state_, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
#else
      int index = LUA_GLOBALSINDEX;
#endif
      typedef std::vector<LuaValue>::const_iterator iter_t;
      for (iter_t p = keys_.begin(); p != keys_.end(); ++p)
      {
         PushLuaValue (state_, *p);
#if LUA_VERSION_NUM >= 502
	  lua_gettable (state_, -2);
#else
         lua_gettable (state_, index);
#endif
         if (keys_.size() > 1 && p != keys_.end()-1 && !lua_istable(state_, -1))
            throw TypeMismatchError ("table", p->typeName());
#if LUA_VERSION_NUM >= 502
	  lua_remove (state_, -2);
#else
         if (index != LUA_GLOBALSINDEX)
            lua_remove (state_, -2);
         else
            index = -2;
#endif
      }
   }

} // namespace Diluculum
