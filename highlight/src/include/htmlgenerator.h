/***************************************************************************
                          htmlgenerator.h  -  description
                             -------------------
    begin                : Wed Nov 28 2001
    copyright            : (C) 2001-2015 by Andre Simon
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



#ifndef HTMLGENERATOR_H
#define HTMLGENERATOR_H

#include <string>

#include "codegenerator.h"

#include "stylecolour.h"
#include "elementstyle.h"

namespace highlight
{

/**
   \brief This class generates HTML.

   It contains information about the resulting document structure (document
   header and footer), the colour system, white space handling and text
   formatting attributes.

* @author Andre Simon
*/

class HtmlGenerator  : public highlight::CodeGenerator
{
public:

    HtmlGenerator();

    /** Destructor, virtual as it is base for xhtmlgenerator*/
    virtual ~HtmlGenerator() {};

    /** Print style definitions to external file
     \param outFile Path of external style definition
     */
    bool printExternalStyle ( const string &outFile );

    /** Print index file with all input file names
      \param fileList List of output file names
      \param outPath Output path
    */
    bool printIndexFile ( const vector<string> & fileList, const string &outPath );

    /**
      \param  b set true if anchors should be attached to line numbers
    */
    void setHTMLAttachAnchors ( bool b )
    {
        attachAnchors = b;
    }

    /**
      \param  prefix anchor prefix
    */
    void setHTMLAnchorPrefix ( const string & prefix )
    {
        anchorPrefix = prefix;
    }

    /**
      \param  b if true line numbers should be replaced by list items
    */
    void setHTMLOrderedList ( bool b ) ;

    /**
      \param  b if true CSS formatting will be inserted into each tag
    */
    void setHTMLInlineCSS ( bool b )
    {
        useInlineCSS = b;
    }

    /**
      \param  b if true fragmented output will be enclosed in pre tag
    */
    void setHTMLEnclosePreTag ( bool b )
    {
        enclosePreTag = b;
    }

    /**
      \param name CSS Class name
    */
    void setHTMLClassName ( const string& name )
    {
        cssClassName  = (StringTools::change_case ( name ) =="none") ? "" : name;
    }

protected:

    string brTag,       ///< break tag
           hrTag,       ///< horizontal ruler tag
           idAttr,      ///< ID tag
           fileSuffix,   ///< filename extension
           cssClassName; ///< css class name prefix

    /** caches style definition */
    string styleDefinitionCache;

    /** line count should be replaced by ordered list*/
    bool orderedList;

    /** CSS definition should be outputted inline */
    bool useInlineCSS;

    /** pre tag should be outputted in fragment mode*/
    bool enclosePreTag;

    /** \return CSS definition */
    string  getStyleDefinition();

    /** \return Content of user defined style file */
    string readUserStyleDef();

    /** \param title Dociment title
        \return Start of file header */
    virtual string getHeaderStart ( const string &title );

    /** \return Comment with program information */
    string getGeneratorComment();

private:

    /** insert line number in the beginning of the new line
    */
    virtual void insertLineNumber ( bool insertNewLine=true );

    /** Print document header
    */
    string getHeader();

    /** Print document body*/
    void printBody();

    /** Print document footer*/
    string getFooter();

    /** initialize tags in specific format according to colouring information provided in DucumentStyle */
    void initOutputTags();

    /**  \param styleName Style name
         \return Opening tag of the given style
    */
    string getOpenTag ( const string& styleName );

    string getOpenTag ( const ElementStyle& elem );

    /** \return escaped character*/
    virtual string maskCharacter ( unsigned char );

    /** test if anchors should be appied to line numbers*/
    bool attachAnchors;

    /**Optional anchor prefix */
    string anchorPrefix;

    /**\return text formatting attributes in HTML format */
    string  getAttributes ( const string & elemName, const ElementStyle & elem );

    /**  \param styleID Style ID
         \return Opening tag of the given style
    */
    string getKeywordOpenTag ( unsigned int styleID );

    /**  \param styleID Style ID
         \return Closing tag of the given style
    */
    string getKeywordCloseTag ( unsigned int styleID );

    /** @return Newline string */
    string getNewLine();
};

}

#endif
