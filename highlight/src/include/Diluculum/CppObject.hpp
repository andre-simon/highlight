/******************************************************************************\
* CppObject.hpp                                                                *
* A struct stored as Lua userdata when exporting or instantiating objects.     *
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


#ifndef _DILUCULUM_CPP_OBJECT_HPP_
#define _DILUCULUM_CPP_OBJECT_HPP_

namespace Diluculum
{
   namespace Impl
   {
      /** The data that is stored as userdata when a C++ object is exported to
       *  or instantiated in Lua.
       */
      struct CppObject
      {
         public:
            /// Pointer to the actual object.
            void* ptr;

            /** Should the \c ptr be <tt>delete</tt>d when the userdata is
             *  garbage-collected in Lua? Essentially, if the object is
             *  instantiated in Lua, it should be; if it is instantiated in C++,
             *  it doesn't.
             */
            bool deleteMe;
      };

   } // namespace Impl

} // namespace Diluculum

#endif // _DILUCULUM_CPP_OBJECT_HPP_
