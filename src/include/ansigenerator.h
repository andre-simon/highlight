/***************************************************************************
                         ansicode.h  -  description
                             -------------------
    begin                : Jul 5 2004
    copyright            : (C) 2004-2007 by Andre Simon
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


#ifndef ANSIGENERATOR_H
#define ANSIGENERATOR_H

#include <string>

#include "codegenerator.h"
#include "charcodes.h"
#include "version.h"

namespace highlight
{

/**
   \brief This class generates ANSI escape sequences.

   It contains information about the resulting document structure (document
   header and footer), the colour system, white space handling and text
   formatting attributes.

* @author Andre Simon
*/

class AnsiGenerator : public highlight::CodeGenerator
{
public:
    AnsiGenerator();
    ~AnsiGenerator();

    /** prints document header
     */
    string getHeader();

    /** Prints document footer*/
    string getFooter();

    /** Prints document body*/
    void printBody();

private:

    /** \return escaped character*/
    virtual string maskCharacter ( unsigned char );


    /** \return ANSI formatting sequences */
    string getOpenTag ( const string& font,
                        const string& fgCol, const string& bgCol="" );

    /** initialize tags in specific format according to colouring information provided in DucumentStyle */
    void initOutputTags();

    /** @param styleID current style ID
        @return matching sequence to begin a new element formatting*/
    string getKeywordOpenTag ( unsigned int styleID );

    /** @param styleID current style ID
        @return matching  sequence to stop element formatting*/
    string getKeywordCloseTag ( unsigned int styleID );
};

}
#endif
