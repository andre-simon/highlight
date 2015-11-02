/***************************************************************************
                       xterm256generator.h  -  description
                             -------------------
    begin                : Oct 13 2006
    copyright            : (C) 2006-2007 by Andre Simon
    email                : andre.simon1@gmx.de
 ***************************************************************************/


/*
This file is part of Highlight.

Highlight is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Highlight is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Highlight.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef XTERM256GENERATOR_H
#define XTERM256GENERATOR_H

#include <string>

#include "codegenerator.h"


namespace highlight
{

/**
   \brief This class generates xterm 256 color escape sequences.

   It contains information about the resulting document structure (document
   header and footer), the colour system, white space handling and text
   formatting attributes.

* @author Andre Simon
*/

class Xterm256Generator : public highlight::CodeGenerator
{
public:
    Xterm256Generator();
    ~Xterm256Generator();

private:

    /** prints document header
     */
    string getHeader();

    /** Prints document footer*/
    string getFooter();

    /** Prints document body*/
    void printBody();

    /** \return escaped character*/
    virtual string maskCharacter ( unsigned char );

    /** initialize tags in specific format according to colouring information provided in DucumentStyle */
    void initOutputTags();

    /** @param style associated element style
        @return formatting seqence */
    string getOpenTag (const ElementStyle &style );

    /** @param styleID current style ID
        @return matching sequence to begin a new element formatting*/
    string getKeywordOpenTag ( unsigned int styleID );

    /** @param styleID current style ID
        @return matching sequence to close element formatting*/
    string getKeywordCloseTag ( unsigned int styleID );

    /** convert an xterm color value (0-253) to 3 unsigned chars rgb
        @param color xterm color
        @param rgb RGB destination string */
    void xterm2rgb ( unsigned char color, unsigned char* rgb );

    /** fill the colortable for use with rgb2xterm */
    void maketable();

    /** selects the nearest xterm color for a 3xBYTE rgb value
        @param RGB colour string */
    unsigned char rgb2xterm ( unsigned char* rgb );

    /// Flag to determine if colourtable is calculated
    static bool initialized;

    /// color tzable for nearest match calculation
    static unsigned char colortable[254][3];

    /// the 6 value iterations in the xterm color cube
    static const unsigned char valuerange[] ;

    /// 16 basic colors
    static const unsigned char basic16[16][3];
};

}
#endif
