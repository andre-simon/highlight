/***************************************************************************
                          themereader.h  -  description
                             -------------------
    begin                : Son Nov 10 2002
    copyright            : (C) 2002-2010 by Andre Simon
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


#ifndef THEMEREADER_H
#define THEMEREADER_H

#include <string>
#include <Diluculum/LuaFunction.hpp>
#include <Diluculum/LuaVariable.hpp>
#include "elementstyle.h"
#include "stylecolour.h"

using namespace std;

namespace highlight
{

/** maps keyword class names and the corresponding formatting information*/
typedef map <string, ElementStyle> KeywordStyles;

/** iterator for keyword styles*/
typedef KeywordStyles::const_iterator KSIterator;

/** \brief Contains information about document formatting properties.
 * @author Andre Simon
 */

class ThemeReader
{
private:
    ElementStyle comment, slcomment, str, dstr,
                 escapeChar, number, directive, line, operators, interpolation;
    ElementStyle defaultElem;
    ElementStyle canvas;

    string errorMsg;
    string desc;
    string themeInjections;

    vector<Diluculum::LuaFunction*> pluginChunks;

    bool fileOK;

    KeywordStyles keywordStyles;

    void initStyle(ElementStyle& style, const Diluculum::LuaVariable& var);

public:
    /** Constructor */
    ThemeReader();
    ~ThemeReader();

    /** load style definition
          \param styleDefinitionFile Style definition path
          \return True if successfull */
    bool load ( const string & styleDefinitionFile, OutputType outputType=HTML );

    void addUserChunk(const Diluculum::LuaFunction& chunk)
    {

        pluginChunks.push_back(new Diluculum::LuaFunction(chunk));
    }

    /** \return class names defined in the theme file */
    vector <string> getClassNames() const;

    /** \return keyword styles */
    KeywordStyles getKeywordStyles() const;

    /** \return Font size */
    string getErrorMessage() const;

    string getDescription() const
    {
        return desc;
    }

    string getInjections() const;

    /** \return Background colour*/
    Colour getBgColour() const;

    /** \return Style of default (unrecognized) strings */
    ElementStyle getDefaultStyle() const;

    /** \return Comment style*/
    ElementStyle getCommentStyle() const;

    /** \return Single line comment style*/
    ElementStyle getSingleLineCommentStyle() const;

    /** \return String style*/
    ElementStyle getStringStyle() const;

    /** \return Directive line string style*/
    ElementStyle getPreProcStringStyle() const;

    /** \return Escape character style*/
    ElementStyle getEscapeCharStyle() const;

    /** \return String interpolation style*/
    ElementStyle getInterpolationStyle() const;

    /** \return Number style*/
    ElementStyle getNumberStyle() const;

    /** \return Directive style*/
    ElementStyle getPreProcessorStyle() const;

    /** \return Type style*/
    ElementStyle getTypeStyle() const;

    /** \return Line number style*/
    ElementStyle getLineStyle() const;

    /** \return Operator style*/
    ElementStyle getOperatorStyle() const;

    /** \param className Name of keyword class (eg kwa, kwb, .., kwd)
        \return keyword style of the given className
    */
    ElementStyle getKeywordStyle ( const string &className ) ;

    /** \return True if language definition was found */
    bool found() const ;
};

}

#endif
