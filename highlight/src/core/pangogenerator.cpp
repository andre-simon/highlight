/***************************************************************************
 PangoGenerator.cpp  -  description
                             -------------------
    begin                : Sept 5 2014
    copyright            : (C) 2014 by Dominik Schmidt
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

#include "pangogenerator.h"

namespace highlight
{

string PangoGenerator::getOpenTag ( const ElementStyle & elem )
{
    return "<span "+getAttributes ( elem ) + ">";
}

string PangoGenerator::getAttributes ( const ElementStyle & elem )
{
    ostringstream s;
    s << "foreground=\"#"
      << ( elem.getColour().getRed ( HTML ) )
      << ( elem.getColour().getGreen ( HTML ) )
      << ( elem.getColour().getBlue ( HTML ) )
      << "\""
      << ( elem.isBold() ?     " weight=\"bold\"" :"" )
      << ( elem.isItalic() ?   " style=\"italic\"" :"" )
      << ( elem.isUnderline() ? " underline=\"single\"" :"" );
    return s.str();
}

PangoGenerator::PangoGenerator() : CodeGenerator ( PANGO )
{

    newLineTag = "\n";
    spacer = " ";
}

PangoGenerator::~PangoGenerator() {}

void PangoGenerator::initOutputTags()
{
    openTags.push_back ( "" );
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
    for (unsigned int i=1; i<NUMBER_BUILTIN_STATES; i++ ) {
        closeTags.push_back ( "</span>" );
    }
}

string PangoGenerator::getHeader()
{
    return string();
}

void PangoGenerator::printBody()
{
    int fontSize=0;
    StringTools::str2num<int> ( fontSize, this->getBaseFontSize(), std::dec );

    *out << "<span size=\""<<( ( fontSize ) ? fontSize*1024: 10*1024 ) << "\" "
         << "font_family=\"" << getBaseFont() << "\""
         <<">";
    processRootState();
    *out << "</span>";
}

string PangoGenerator::getFooter()
{
    return string();
}

string PangoGenerator::maskCharacter ( unsigned char c )
{
    switch ( c ) {
    case '<' :
        return "&lt;";
        break;
    case '>' :
        return "&gt;";
        break;
    case '&' :
        return "&amp;";
        break;
    default :
        return string ( 1, c );
    }
}

string PangoGenerator::getKeywordOpenTag ( unsigned int styleID )
{
    return getOpenTag (docStyle.getKeywordStyle ( currentSyntax->getKeywordClasses() [styleID] ) );
}

string PangoGenerator::getKeywordCloseTag ( unsigned int styleID )
{
    return "</span>";
}

}
