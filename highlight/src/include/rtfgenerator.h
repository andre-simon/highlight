/***************************************************************************
                          rtfcode.h  -  description
                             -------------------
    begin                : Die Jul 9 2002
    copyright            : (C) 2002-2016 by Andre Simon
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


#ifndef RTFGENERATOR_H
#define RTFGENERATOR_H

#include <string>

#include "codegenerator.h"

namespace highlight
{

/**
   PageSize contains the RTF page dimensions.
*/
struct PageSize {
    /// RTF page width
    int width;
    /// RTF page height
    int height;

    PageSize()
    {
    }

    /** Constructor to define page dimensions
        @param w width
        @param h height*/
    PageSize ( int w, int h )
    {
        width=w;
        height = h;
    }

};

/** mapping of page size names and dimensions */
typedef  map<string, struct PageSize> PagesizeMap;

/**
   \brief This class generates RTF.

   It contains information about the resulting document structure (document
   header and footer), the colour system, white space handling and text
   formatting attributes.

* @author Andre Simon
*/

class RtfGenerator : public highlight::CodeGenerator
{
public:

    RtfGenerator();

    ~RtfGenerator();

    /**  Define RTF page size
         @param ps RTF page size (a3, a4, a5, b4, b5, b6, letter, legal) */
    void setRTFPageSize ( const string & ps );

    /** @param cs flag to enable character styles*/
    void setRTFCharStyles ( bool cs );
    
    /** @param pc flag to enable page color output */
    void setRTFPageColor ( bool pc );

private:

    /** prints document header
     */
    string getHeader();

    /** Prints document footer*/
    string getFooter();

    /** Prints document body*/
    void printBody();

    /** initialize tags in specific format according to colouring information provided in DucumentStyle */
    void initOutputTags();

    /** Map of several pagesizes */
    PagesizeMap psMap;

    /** name of page size which is mapped to page dimensions*/
    string pageSize;

    /** flag to add character styles */
    bool addCharStyles;

    /** flag to add page color */
    bool addPageColor;
    
    /** flag to enable Unicode conversion */
    bool isUtf8;
    
    /** utf-8 to utf-16 conversion variables */
    unsigned long utf16Char;
    size_t utf8SeqLen;
    
    /** @return escaped character*/
    virtual string maskCharacter ( unsigned char );

    /**\return text formatting attributes in RTF format */
    string  getAttributes ( const ElementStyle & col );

    /** @param styleNumber number of current style
        @param elem associated element style
        @return RTF formatting seqence (colour index + bold + italic)*/
    string getOpenTag ( int styleNumber,const ElementStyle &elem );

    /** @param styleNumber number of current style
        @param elem associated element style
        @param styleName style name
        @return RTF character style definition */
    string getCharStyle ( int styleNumber,const ElementStyle &elem, const string&styleName );

    /** @param elem associated element style
        @return RTF formatting sequnce to close element formatting */
    string getCloseTag ( const ElementStyle &elem );

    /** @param styleID current style ID
        @return matching sequence to begin a new element formatting*/
    string getKeywordOpenTag ( unsigned int styleID );

    /** @param styleID current style ID
        @return matching  sequence to stop element formatting*/
    string getKeywordCloseTag ( unsigned int styleID );
};

}
#endif
