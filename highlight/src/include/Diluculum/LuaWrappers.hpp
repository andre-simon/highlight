/******************************************************************************\
* LuaWrappers.hpp                                                              *
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

#ifndef _DILUCULUM_LUA_WRAPPERS_HPP_
#define _DILUCULUM_LUA_WRAPPERS_HPP_

#include <algorithm>
#include <string>
#include <boost/bind.hpp>
#include <Diluculum/CppObject.hpp>
#include <Diluculum/LuaExceptions.hpp>
#include <Diluculum/LuaState.hpp>
#include <Diluculum/LuaUtils.hpp>


namespace Diluculum
{
   namespace Impl
   {
      /** Calls \c lua_error() with a proper error message. The error message is
       *  composed of things that may help to find out what's the error, like
       *  the function name.
       *  @param ls The <tt>lua_State*</tt> on which the function will operate.
       *  @param what A message to be included in the error message created by
       *         this function.
       *  @note This is not intended to be called by Diluculum users.
       */
      void ReportErrorFromCFunction (lua_State* ls, const::std::string& what);



      /** Helper class, used by the \c DILUCULUM_CLASS_METHOD() macro, as a
       *  means register a method in the table that represents a class being
       *  exported to Lua. Everything is done in the constructor. This is just
       *  a way to get some code executed in a macro call that happens at the
       *  global scope, outside of a function definition.
       */
      class ClassTableFiller
      {
         public:
            /** Adds the function \c func to the table \c classTable, with a key
             *  \c name.
             *  @param classTable The table representing the class being
             *         exported to Lua.
             *  @param name The name by which the method will be known in the
             *         Lua side.
             *  @param func The C function wrapping the method.
             */
            ClassTableFiller (Diluculum::LuaValueMap& classTable,
                              const std::string& name,
                              lua_CFunction func)
            {
               classTable[name] = func;
            }
      };
   }
}



/** Returns the name of the wrapper function that is created by
 *  \c DILUCULUM_WRAP_FUNCTION() for a given function name.
 *  @param FUNC The function whose wrapper name is desired.
 */
#define DILUCULUM_WRAPPER_FUNCTION(FUNC)        \
   Diluculum__ ## FUNC ## __Wrapper_Function



/** Creates a \c lua_CFunction that wraps a function with the signature like the
 *  following one:
 *  <p><tt>Diluculum::LuaValueList Func (const Diluculum::LuaValueList& params)</tt>
 *  <p>Notice that, thanks to the use of <tt>Diluculum::LuaValueList</tt>s, the
 *  wrapped function can effectively take and return an arbitrary number of
 *  values.
 *  @note The name of the created wrapper function is a decorated version of the
 *        \c FUNC parameter. The decoration scheme can be quite complicated and
 *        is subject to change in future releases of Diluculum, so don't try to
 *        use it directly. Use the \c DILUCULUM_WRAPPER_FUNCTION() macro to
 *        obtain it instead.
 *  @note The proper way to report errors from the function being wrapped is by
 *        <tt>throw</tt>ing a \c Diluculum::LuaError. The created wrapper
 *        function will handle these exceptions and "translate" them to a call
 *        to \c lua_error().
 *  @see DILUCULUM_WRAPPER_FUNCTION() To find out the name of the created
 *       wrapper function.
 *  @param FUNC The function to be wrapped.
 */
#define DILUCULUM_WRAP_FUNCTION(FUNC)                                         \
int DILUCULUM_WRAPPER_FUNCTION(FUNC) (lua_State* ls)                          \
{                                                                             \
   using std::for_each;                                                       \
   using boost::bind;                                                         \
   using Diluculum::PushLuaValue;                                             \
   using Diluculum::Impl::ReportErrorFromCFunction;                           \
                                                                              \
   try                                                                        \
   {                                                                          \
      /* Read parameters and empty the stack */                               \
      const int numParams = lua_gettop (ls);                                  \
      Diluculum::LuaValueList params;                                         \
      for (int i = 1; i <= numParams; ++i)                                    \
         params.push_back (Diluculum::ToLuaValue (ls, i));                    \
      lua_pop (ls, numParams);                                                \
                                                                              \
      /* Call the wrapped function */                                         \
      Diluculum::LuaValueList ret = FUNC (params);                            \
                                                                              \
      /* Push the return values and return */                                 \
      for_each (ret.begin(), ret.end(), bind (PushLuaValue, ls, _1));         \
                                                                              \
      return ret.size();                                                      \
   }                                                                          \
   catch (Diluculum::LuaError& e)                                             \
   {                                                                          \
      ReportErrorFromCFunction (ls, e.what());                                \
      return 0;                                                               \
   }                                                                          \
   catch(...)                                                                 \
   {                                                                          \
      ReportErrorFromCFunction (ls, "Unknown exception caught by wrapper.");  \
      return 0;                                                               \
   }                                                                          \
}



/** Returns the name of the table that represent the class \c CLASS.
 *  @note This is used internally. Users can ignore this macro.
 */
#define DILUCULUM_CLASS_TABLE(CLASS) \
Diluculum__Class_Table__ ## CLASS



/** Starts a block of class wrapping macro calls. This must be followed by calls
 *  to \c DILUCULUM_CLASS_METHOD() for each method to be exported to Lua and a
 *  final call to \c DILUCULUM_END_CLASS().
 *  @param CLASS The class being exported.
 */
#define DILUCULUM_BEGIN_CLASS(CLASS)                                          \
namespace                                                                     \
{                                                                             \
   /* the table representing the class */                                     \
   Diluculum::LuaValueMap DILUCULUM_CLASS_TABLE(CLASS);                       \
}                                                                             \
                                                                              \
/* The Constructor */                                                         \
int Diluculum__ ## CLASS ## __Constructor_Wrapper_Function (lua_State* ls)    \
{                                                                             \
   using Diluculum::PushLuaValue;                                             \
   using Diluculum::Impl::CppObject;                                          \
   using Diluculum::Impl::ReportErrorFromCFunction;                           \
                                                                              \
   try                                                                        \
   {                                                                          \
      /* Read parameters and empty the stack */                               \
      const int numParams = lua_gettop (ls);                                  \
      Diluculum::LuaValueList params;                                         \
      for (int i = 1; i <= numParams; ++i)                                    \
         params.push_back (Diluculum::ToLuaValue (ls, i));                    \
      lua_pop (ls, numParams);                                                \
                                                                              \
      /* Construct the object, wrap it in a userdata, and return */           \
      void* ud = lua_newuserdata (ls, sizeof(CppObject));                     \
      CppObject* cppObj = reinterpret_cast<CppObject*>(ud);                   \
      cppObj->ptr = new CLASS (params);                                       \
      cppObj->deleteMe = true;                                                \
                                                                              \
      lua_getglobal (ls, "__Diluculum__Class_Metatables");                    \
      lua_getfield (ls, -1, #CLASS);                                          \
      lua_setmetatable (ls, -3);                                              \
      lua_pop (ls, 1); /* pop the table of metatables */                      \
                                                                              \
      return 1;                                                               \
   }                                                                          \
   catch (Diluculum::LuaError& e)                                             \
   {                                                                          \
      ReportErrorFromCFunction (ls, e.what());                                \
      return 0;                                                               \
   }                                                                          \
   catch(...)                                                                 \
   {                                                                          \
      ReportErrorFromCFunction (ls, "Unknown exception caught by wrapper.");  \
      return 0;                                                               \
   }                                                                          \
}                                                                             \
                                                                              \
/* Destructor */                                                              \
int Diluculum__ ## CLASS ## __Destructor_Wrapper_Function (lua_State* ls)     \
{                                                                             \
   using Diluculum::Impl::CppObject;                                          \
                                                                              \
   CppObject* cppObj =                                                        \
      reinterpret_cast<CppObject*>(lua_touserdata (ls, -1));                  \
                                                                              \
   if (cppObj->deleteMe)                                                      \
   {                                                                          \
      cppObj->deleteMe = false; /* don't delete again when gc'ed! */          \
      CLASS* pObj = reinterpret_cast<CLASS*>(cppObj->ptr);                    \
      delete pObj;                                                            \
   }                                                                          \
                                                                              \
   return 0;                                                                  \
}



/** Returns the name of the function used to wrap a method \c METHOD of the
 *  class \c CLASS.
 *  @note This is used internally. Users can ignore this macro.
 */
#define DILUCULUM_METHOD_WRAPPER(CLASS, METHOD)                      \
   Diluculum__ ## CLASS ## __ ## METHOD ## __Method_Wrapper_Function



/** Exports a given class' method. This macro must be called between calls to
 *  \c DILUCULUM_BEGIN_CLASS() and \c DILUCULUM_END_CLASS().
 *  @param CLASS The class whose method is being exported.
 *  @param METHOD The method being exported.
 */
#define DILUCULUM_CLASS_METHOD(CLASS, METHOD)                                 \
int DILUCULUM_METHOD_WRAPPER(CLASS, METHOD) (lua_State* ls)                   \
{                                                                             \
   using std::for_each;                                                       \
   using boost::bind;                                                         \
   using Diluculum::PushLuaValue;                                             \
   using Diluculum::Impl::CppObject;                                          \
   using Diluculum::Impl::ReportErrorFromCFunction;                           \
                                                                              \
   try                                                                        \
   {                                                                          \
      /* Read parameters and empty the stack */                               \
      const int numParams = lua_gettop (ls);                                  \
      Diluculum::LuaValue ud = Diluculum::ToLuaValue (ls, 1);                 \
      Diluculum::LuaValueList params;                                         \
      for (int i = 2; i <= numParams; ++i)                                    \
         params.push_back (Diluculum::ToLuaValue (ls, i));                    \
      lua_pop (ls, numParams);                                                \
                                                                              \
      /* Get the object pointer and call the method */                        \
      CppObject* cppObj =                                                     \
         reinterpret_cast<CppObject*>(ud.asUserData().getData());             \
      CLASS* pObj = reinterpret_cast<CLASS*>(cppObj->ptr);                    \
                                                                              \
      Diluculum::LuaValueList ret = pObj->METHOD (params);                    \
                                                                              \
      /* Push the return values and return */                                 \
      for_each (ret.begin(), ret.end(), bind (PushLuaValue, ls, _1));         \
                                                                              \
      return ret.size();                                                      \
   }                                                                          \
   catch (Diluculum::LuaError& e)                                             \
   {                                                                          \
      ReportErrorFromCFunction (ls, e.what());                                \
      return 0;                                                               \
   }                                                                          \
   catch(...)                                                                 \
   {                                                                          \
      ReportErrorFromCFunction (ls, "Unknown exception caught by wrapper.");  \
      return 0;                                                               \
   }                                                                          \
}                                                                             \
                                                                              \
namespace                                                                     \
{                                                                             \
   Diluculum::Impl::ClassTableFiller                                          \
      Diluculum__ ## CLASS ## _ ## METHOD ## __ ## Filler(                    \
         DILUCULUM_CLASS_TABLE(CLASS),                                        \
         #METHOD,                                                             \
         DILUCULUM_METHOD_WRAPPER(CLASS, METHOD));                            \
}



/** Ends a block of class wrapping macro calls (which was opened by a call to
 *  \c DILUCULUM_BEGIN_CLASS()).
 *  @param CLASS The class being exported.
 */
#define DILUCULUM_END_CLASS(CLASS)                                            \
                                                                              \
/* The function used to register the class in a 'LuaState' */                 \
void Diluculum_Register_Class__ ## CLASS (Diluculum::LuaVariable className)   \
{                                                                             \
   Diluculum::LuaState ls (className.getState());                             \
                                                                              \
   if (ls["__Diluculum__Class_Metatables"].value().type() == LUA_TNIL)        \
     ls["__Diluculum__Class_Metatables"] = Diluculum::EmptyLuaValueMap;       \
                                                                              \
   static bool isInited = false;                                              \
   if (!isInited)                                                             \
   {                                                                          \
      isInited = true;                                                        \
                                                                              \
      DILUCULUM_CLASS_TABLE(CLASS)["classname"] = #CLASS;                     \
                                                                              \
      DILUCULUM_CLASS_TABLE(CLASS)["new"] =                                   \
         Diluculum__ ## CLASS ## __Constructor_Wrapper_Function;              \
                                                                              \
      DILUCULUM_CLASS_TABLE(CLASS)["delete"] =                                \
         Diluculum__ ## CLASS ## __Destructor_Wrapper_Function;               \
                                                                              \
      DILUCULUM_CLASS_TABLE(CLASS)["__gc"] =                                  \
         Diluculum__ ## CLASS ## __Destructor_Wrapper_Function;               \
                                                                              \
      DILUCULUM_CLASS_TABLE(CLASS)["__index"] = DILUCULUM_CLASS_TABLE(CLASS); \
   }                                                                          \
                                                                              \
   className = DILUCULUM_CLASS_TABLE(CLASS);                                  \
                                                                              \
   ls["__Diluculum__Class_Metatables"][#CLASS] =                              \
      DILUCULUM_CLASS_TABLE(CLASS);                                           \
} /* end of Diluculum_Register_Class__CLASS */



/** Registers a class in a given \c Diluculum::LuaState. The class must have
 *  been previously exported by calls to \c DILUCULUM_BEGIN_CLASS(),
 *  \c DILUCULUM_END_CLASS() and probably \c DILUCULUM_CLASS_METHOD().
 *  @param LUA_VARIABLE The \c Diluculum::LuaVariable that will store the class
 *         after this call.
 *  @param CLASS The class being registered.
 */
#define DILUCULUM_REGISTER_CLASS(LUA_VARIABLE, CLASS)  \
   Diluculum_Register_Class__ ## CLASS (LUA_VARIABLE);



/** Registers an object instantiated in C++ into a Lua state. This way, this
 *  object's methods can be called from Lua. The registered C++ object will
 *  \e not be destroyed when the corresponding Lua object is garbage-collected.
 *  Destroying it is responsibility of the programmer on the C++ side.
 *  @param LUA_VARIABLE The \c Diluculum::LuaVariable where the object will be
 *         stored. Notice that a \c Diluculum::LuaVariable contains a reference
 *         to a <tt>lua_State*</tt>, so the Lua state in which the object will
 *         be stored is passed here, too, albeit indirectly.
 *  @param CLASS The class of the object being registered. This class must have
 *         been previously registered in the target Lua state with a call to the
 *         \c DILUCULUM_REGISTER_CLASS() macro.
 *  @param OBJECT The object to be registered to the Lua state.
 */
#define DILUCULUM_REGISTER_OBJECT(LUA_VARIABLE, CLASS, OBJECT)                \
{                                                                             \
   /* leave the table where 'OBJECT' is to be stored at the stack top */      \
   LUA_VARIABLE.pushLastTable();                                              \
                                                                              \
   /* push the field where the object will be stored */                       \
   Diluculum::PushLuaValue (LUA_VARIABLE.getState(),                          \
                            LUA_VARIABLE.getKeys().back());                   \
                                                                              \
   /* create the userdata, set its metatable */                               \
   void* ud = lua_newuserdata (LUA_VARIABLE.getState(),                       \
                               sizeof(Diluculum::Impl::CppObject));           \
                                                                              \
   Diluculum::Impl::CppObject* cppObj =                                       \
      reinterpret_cast<Diluculum::Impl::CppObject*>(ud);                      \
                                                                              \
   cppObj->ptr = &OBJECT;                                                     \
   cppObj->deleteMe = false;                                                  \
                                                                              \
   lua_getglobal (LUA_VARIABLE.getState(), "__Diluculum__Class_Metatables");  \
   lua_getfield (LUA_VARIABLE.getState(), -1, #CLASS);                        \
   lua_setmetatable (LUA_VARIABLE.getState(), -3);                            \
                                                                              \
   lua_pop (LUA_VARIABLE.getState(), 1); /* pop the table of metatables */    \
                                                                              \
   /* store the userdata */                                                   \
   lua_settable (LUA_VARIABLE.getState(), -3);                                \
}



/** Starts a block declaring a dynamically loadable module, that is, a module
 *  that is expected to be compiled as a shared library. The block must be
 *  closed by a call to \c DILUCULUM_END_MODULE() and contain some calls to
 *  macros like \c DILUCULUM_MODULE_ADD_CLASS() and
 *  \c DILUCULUM_MODULE_ADD_FUNCTION() in its body.
 *  <p>For those who know the way things happen in Lua, this block will provide
 *  a \c luaopen_MyModule() function, which initializes the module when loaded.
 *  @param MODNAME The module name. This is how the module will be called in
 *         Lua. Also, the dynamic library to which the model will be compiled
 *         should be named like this (plus an extension like <tt>.so</tt> or
 *         <tt>.dll</tt>). Well, actually the rules for naming the module file
 *         are more complex than this. Check the
 *         <a href="http://www.lua.org/docs.html">Lua manual</a> for the dirty
 *         details.
 */
#define DILUCULUM_BEGIN_MODULE(MODNAME)                  \
extern "C" int luaopen_ ## MODNAME (lua_State *luaState) \
{                                                        \
   using Diluculum::LuaState;                            \
   using Diluculum::LuaVariable;                         \
   using Diluculum::EmptyLuaValueMap;                    \
   LuaState ls (luaState);                               \
                                                         \
   ls[#MODNAME] = EmptyLuaValueMap;                      \
   LuaVariable theModule = ls[#MODNAME];



/** Adds a class to the module. Must be called between calls to
 *  \c DILUCULUM_BEGIN_MODULE() and \c DILUCULUM_END_MODULE().
 *  @param CLASS The name of the class being added, as it is known in the
 *         C++ side.
 *  @param LUACLASS The name by which the class will be known in the Lua side.
 */
#define DILUCULUM_MODULE_ADD_CLASS(CLASS, LUACLASS)      \
   DILUCULUM_REGISTER_CLASS(theModule[LUACLASS], CLASS);



/** Adds a function to the module. Must be called between calls to
 *  \c DILUCULUM_BEGIN_MODULE() and \c DILUCULUM_END_MODULE().
 *  @param CFUNC The name of the function being added, as it is known in the
 *         C++ side.
 *  @param LUAFUNC The name by which the function will be known in the Lua side.
 */
#define DILUCULUM_MODULE_ADD_FUNCTION(CFUNC, LUAFUNC) \
   theModule[LUAFUNC] = CFUNC;



/** Closes a block (started by \c DILUCULUM_BEGIN_MODULE()) that defines a
 *  dynamically loadable Lua module.
 *  @bug In principle, this function should return one parameter: the table
 *       containing the module (this is more or less a convention). The current
 *       implementation not only ignores the convention: it also seems to return
 *       something invalid (function looks to be returning 1 without anything on
 *       the stack).
 */
#define DILUCULUM_END_MODULE() \
   return 1;                   \
}

#endif // _DILUCULUM_LUA_WRAPPERS_HPP_
