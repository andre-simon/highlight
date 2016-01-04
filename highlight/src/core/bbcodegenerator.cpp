/***************************************************************************
                    bbcodegenerator.cpp  -  description
                             -------------------
    begin                : Jul 21 2009
    copyright            : (C) 2004-2009 by Andre Simon
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


#include <sstream>

#include "bbcodegenerator.h"

namespace highlight
{

BBCodeGenerator::BBCodeGenerator() : CodeGenerator ( BBCODE )
{
    newLineTag = "\n";
    spacer = " ";
}

BBCodeGenerator::~BBCodeGenerator() {}

string BBCodeGenerator::getHeader()
{
    return string();
}

void BBCodeGenerator::printBody()
{
    *out << "[size="<<getBaseFontSize()<<"]"; // TODO hier pt?
    processRootState();
    *out << "[/size]";
}

string BBCodeGenerator::getFooter()
{
    return string();
}

string  BBCodeGenerator::getOpenTag (const ElementStyle & elem )
{
    ostringstream s;

    s << "[color=#";
    s  << elem.getColour().getRed ( HTML )
       << elem.getColour().getGreen ( HTML )
       << elem.getColour().getBlue ( HTML )
       << "]";

    if ( elem.isBold() ) s << "[b]";
    if ( elem.isItalic() ) s << "[i]";
    if ( elem.isUnderline() ) s << "[u]";
    return  s.str();
}

string  BBCodeGenerator::getCloseTag ( const ElementStyle &elem )
{
    ostringstream s;
    if ( elem.isUnderline() ) s << "[/u]";
    if ( elem.isItalic() ) s << "[/i]";
    if ( elem.isBold() ) s << "[/b]";
    s << "[/color]";
    return  s.str();
}

void BBCodeGenerator::initOutputTags ()
{
    openTags.push_back ( "");
    openTags.push_back ( getOpenTag ( docStyle.getStringStyle() ) );
    openTags.push_back ( getOpenTag ( docStyle.getNumberStyle() ) );
    openTags.push_back ( getOpenTag ( docStyle.getSingleLineCommentStyle() ) );
    openTags.push_back ( getOpenTag ( docStyle.getCommentStyle() ) );
    openTags.push_back ( getOpenTag ( docStyle.getEscapeCharStyle() ) );
    openTags.push_back ( getOpenTag ( docStyle.getPreProcessorStyle() ) );
    openTags.push_back ( getOpenTag ( docStyle.getPreProcStringStyle() ) );
    openTags.push_back ( getOpenTag ( docStyle.getLineStyle() ) );
    openTags.push_back ( getOpenTag ( docStyle.getOperatorStyle() ) );
    openTags.push_back ( getOpenTag ( docStyle.getInterpolationStyle() ) );

    closeTags.push_back ( "" );
    closeTags.push_back ( getCloseTag ( docStyle.getStringStyle() ) );
    closeTags.push_back ( getCloseTag ( docStyle.getNumberStyle() ) );
    closeTags.push_back ( getCloseTag ( docStyle.getSingleLineCommentStyle() ) );
    closeTags.push_back ( getCloseTag ( docStyle.getCommentStyle() ) );
    closeTags.push_back ( getCloseTag ( docStyle.getEscapeCharStyle() ) );
    closeTags.push_back ( getCloseTag ( docStyle.getPreProcessorStyle() ) );
    closeTags.push_back ( getCloseTag ( docStyle.getPreProcStringStyle() ) );
    closeTags.push_back ( getCloseTag ( docStyle.getLineStyle() ) );
    closeTags.push_back ( getCloseTag ( docStyle.getOperatorStyle() ) );
    closeTags.push_back ( getCloseTag ( docStyle.getInterpolationStyle() ) );
}

string BBCodeGenerator::maskCharacter ( unsigned char c )
{
    return string ( 1, c );
}

string BBCodeGenerator::getKeywordOpenTag ( unsigned int styleID )
{
    return getOpenTag (docStyle.getKeywordStyle ( currentSyntax->getKeywordClasses() [styleID] ) );
}

string BBCodeGenerator::getKeywordCloseTag ( unsigned int styleID )
{
    return getCloseTag ( docStyle.getKeywordStyle ( currentSyntax->getKeywordClasses() [styleID] ) );
}

}
