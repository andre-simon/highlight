/******************************************************************************\
* LuaUtils.cpp                                                                 *
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

#include <cstring>
#include <Diluculum/LuaUtils.hpp>
#include <Diluculum/LuaExceptions.hpp>
#include <boost/lexical_cast.hpp>
#include "InternalUtils.hpp"


namespace Diluculum
{
   // - ToLuaValue -------------------------------------------------------------
   LuaValue ToLuaValue (lua_State* state, int index)
   {
      switch (lua_type (state, index))
      {
         case LUA_TNIL:
            return Nil;

         case LUA_TNUMBER:
            return lua_tonumber (state, index);

         case LUA_TBOOLEAN:
            // this (instead of a cast) avoids a warning on Visual C++
            return lua_toboolean (state, index) != 0;

         case LUA_TSTRING:
            return std::string(lua_tostring (state, index),
#if LUA_VERSION_NUM >= 502
                lua_rawlen(state, index));
#else
                lua_objlen(state, index));
#endif
         case LUA_TUSERDATA:
         {
            void* addr = lua_touserdata (state, index);
#if LUA_VERSION_NUM >= 502
	    size_t size = lua_rawlen(state, index);
#else
            size_t size = lua_objlen (state, index);
#endif
            LuaUserData ud (size);
            memcpy (ud.getData(), addr, size);
            return ud;
         }

         case LUA_TTABLE:
         {
            // Make the index positive if necessary (using a negative index here
            // will be *bad*, because the stack will be changed in the
            // 'lua_next()' and a negative index will mess everything).
            if (index < 0)
               index = lua_gettop(state) + index + 1;

            // Traverse the table adding the key/value pairs to 'ret'
            LuaValueMap ret;

            lua_pushnil (state);
            while (lua_next (state, index) != 0)
            {
               ret[ToLuaValue (state, -2)] = ToLuaValue (state, -1);
               lua_pop (state, 1);
            }

            // Alright, return the result
            return ret;
         }

         case LUA_TFUNCTION:
         {
            if (lua_iscfunction (state, index))
            {
               return lua_tocfunction (state, index);
            }
            else
            {
               LuaFunction func("", 0);
               lua_pushvalue (state, index);

#if LUA_VERSION_NUM >= 503
               lua_dump(state, Impl::LuaFunctionWriter, &func, 0);
#else
               lua_dump(state, Impl::LuaFunctionWriter, &func);
#endif
               lua_pop(state, 1);
               return func;
            }
         }

         default:
         {
            throw LuaTypeError(
               ("Unsupported type found in call to 'ToLuaValue()': "
                + boost::lexical_cast<std::string>(lua_type (state, index))
                + " (typename: \'" + luaL_typename (state, index)
                + "')").c_str());
         }
      }
   }



   // - PushLuaValue -----------------------------------------------------------
   void PushLuaValue (lua_State* state, const LuaValue& value)
   {
      switch (value.type())
      {
         case LUA_TNIL:
            lua_pushnil (state);
            break;

         case LUA_TNUMBER:
            lua_pushnumber (state, value.asNumber());
            break;

         case LUA_TSTRING:
         {
            const std::string& tmp = value.asString();
            lua_pushlstring (state, tmp.c_str(), tmp.length());
            break;
         }

         case LUA_TBOOLEAN:
            lua_pushboolean (state, value.asBoolean());
            break;

         case LUA_TUSERDATA:
         {
            size_t size = value.asUserData().getSize();
            void* addr = lua_newuserdata (state, size);
            memcpy (addr, value.asUserData().getData(), size);
            break;
         }

         case LUA_TTABLE:
         {
            lua_newtable (state);

            typedef LuaValueMap::const_iterator iter_t;
            const LuaValueMap table = value.asTable();
            for (iter_t p = table.begin(); p != table.end(); ++p)
            {
               if (p->first != Nil) // Ignore 'Nil'-indexed entries
               {
                  PushLuaValue (state, p->first);
                  PushLuaValue (state, p->second);
                  lua_settable (state, -3);
               }
            }

            break;
         }

         case LUA_TFUNCTION:
         {
            const LuaFunction& f = value.asFunction();
            if (f.isCFunction())
            {
               lua_pushcfunction (state, f.getCFunction());
            }
            else
            {
               LuaFunction* pf = const_cast<LuaFunction*>(&f); // yikes!
               pf->setReaderFlag (false);
               int status = lua_load (state, Impl::LuaFunctionReader, pf,
#if LUA_VERSION_NUM >= 502
                                      "Diluculum Lua chunk", NULL);
#else
                                      "Diluculum Lua chunk");
#endif
               Impl::ThrowOnLuaError (state, status);
            }
            break;
         }

         default:
         {
            throw LuaTypeError(
               ("Unsupported type found in call to 'PushLuaValue()': "
                + boost::lexical_cast<std::string>(value.type())
                + " (typename: \'" + value.typeName() + "')").c_str());
         }
      }
   }

} // namespace Diluculum
