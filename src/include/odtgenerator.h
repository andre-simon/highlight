/***************************************************************************
                         odtgenerator.h  -  description
                             -------------------
    begin                : Jul 19 2012
    copyright            : (C) 2004-2012 by Andre Simon
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


#ifndef ODTGENERATOR_H
#define ODTGENERATOR_H

#include <string>

#include "codegenerator.h"
#include "version.h"

namespace highlight
{

/**
   \brief This class generates OpenDocument Textfiles (Flat XML).

   It contains information about the resulting document structure (document
   header and footer), the colour system, white space handling and text
   formatting attributes.

* @author Andre Simon
*/

class ODTGenerator : public highlight::CodeGenerator
{
public:
    ODTGenerator();
    ~ODTGenerator();

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

    /** @return ODT open tags */
    string getOpenTag ( const string& styleName );

    /** initialize tags in specific format according to colouring information provided in DucumentStyle */
    void initOutputTags();

    /** @param styleID current style ID
        @return matching sequence to begin a new element formatting*/
    string getKeywordOpenTag ( unsigned int styleID );

    /** @param styleID current style ID
        @return matching  sequence to stop element formatting*/
    string getKeywordCloseTag ( unsigned int styleID );

    string styleDefinitionCache;

    string getStyleDefinition();

    string getAttributes ( const string &, const ElementStyle & );
};

}
#endif
