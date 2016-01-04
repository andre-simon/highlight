/***************************************************************************
                   xterm256generator.cpp  -  description
                             -------------------
    begin                : Oct 13 2006
    copyright            : (C) 2006 by Andre Simon
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


#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sstream>

#include "xterm256generator.h"
#include "charcodes.h"
#include "version.h"


namespace highlight
{
Xterm256Generator::Xterm256Generator() : CodeGenerator ( XTERM256 )
{

    newLineTag = "\n";
    spacer = " ";
}

Xterm256Generator::~Xterm256Generator() {}

string Xterm256Generator::getHeader()
{
    return string();
}

void Xterm256Generator::printBody()
{
    processRootState();
}

string Xterm256Generator::getFooter()
{
    return string();
}

string Xterm256Generator::maskCharacter ( unsigned char c )
{
    return string ( 1, c );
}

void Xterm256Generator::initOutputTags ( )
{
    openTags.push_back ( getOpenTag ( docStyle.getDefaultStyle() ) );
    openTags.push_back ( getOpenTag ( docStyle.getStringStyle() ) );
    openTags.push_back ( getOpenTag ( docStyle.getNumberStyle() ) );
    openTags.push_back ( getOpenTag ( docStyle.getSingleLineCommentStyle() ) );
    openTags.push_back ( getOpenTag ( docStyle.getCommentStyle() ) );
    openTags.push_back ( getOpenTag ( docStyle.getEscapeCharStyle() ) );
    openTags.push_back ( getOpenTag ( docStyle.getPreProcessorStyle() ) );
    openTags.push_back ( getOpenTag ( docStyle.getPreProcStringStyle() ) );
    openTags.push_back ( getOpenTag ( docStyle.getLineStyle() ) );
    openTags.push_back ( getOpenTag ( docStyle.getOperatorStyle() ) );
    openTags.push_back ( getOpenTag ( docStyle.getInterpolationStyle()) );

    for (unsigned int i=0; i<NUMBER_BUILTIN_STATES; i++ ) {
        closeTags.push_back ( "\033[m" );
    }
}

string  Xterm256Generator::getOpenTag ( const ElementStyle &col )
{

    Colour c= col.getColour();
    unsigned char  rgb[3];
    rgb[0] = ( unsigned char ) strtoll ( c.getRed ( HTML ).c_str(), NULL, 16 );
    rgb[1] = ( unsigned char ) strtoll ( c.getGreen ( HTML ).c_str(), NULL, 16 );
    rgb[2] = ( unsigned char ) strtoll ( c.getBlue ( HTML ).c_str(), NULL, 16 );

    ostringstream s;
    //s  << "\033[38;5;"<< ( int ) rgb2xterm ( rgb ) << "m";
    s  << "\033[";

    if ( col.isBold() ) s << "1;";
    if ( col.isItalic() ) s << "3;";
    if ( col.isUnderline() ) s << "4;";

    s << "38;5;"<< ( int ) rgb2xterm ( rgb ) << "m";
    return  s.str();
}

string Xterm256Generator::getKeywordOpenTag ( unsigned int styleID )
{
    return getOpenTag ( docStyle.getKeywordStyle ( currentSyntax->getKeywordClasses() [styleID] ) );
}

string Xterm256Generator::getKeywordCloseTag ( unsigned int styleID )
{
    return "\033[m";
}

/* the following functions are based on Wolfgang Frischs xterm256 converter utility:
   http://frexx.de/xterm-256-notes/
*/

void Xterm256Generator::xterm2rgb ( unsigned char color, unsigned char* rgb )
{
    // 16 basic colors
    if ( color<16 ) {
        rgb[0] = basic16[color][0];
        rgb[1] = basic16[color][1];
        rgb[2] = basic16[color][2];
    }

    // color cube color
    if ( color>=16 && color<=232 ) {
        color-=16;
        rgb[0] = valuerange[ ( color/36 ) %6];
        rgb[1] = valuerange[ ( color/6 ) %6];
        rgb[2] = valuerange[color%6];
    }

    // gray tone
    if ( color>=233 && color<=253 ) {
        rgb[0]=rgb[1]=rgb[2] = 8+ ( color-232 ) *0x0a;
    }
}

void Xterm256Generator::maketable()
{
    unsigned char c, rgb[3];
    for ( c=0; c<=253; c++ ) {
        xterm2rgb ( c,rgb );
        colortable[c][0] = rgb[0];
        colortable[c][1] = rgb[1];
        colortable[c][2] = rgb[2];
    }
}

unsigned char Xterm256Generator::rgb2xterm ( unsigned char* rgb )
{
    unsigned char c, best_match=0;
    double d, smallest_distance;

    if ( !initialized ) {
        maketable();
        initialized = true;
    }

    smallest_distance = 10000000000.0;

    for ( c=0; c<=253; c++ ) {
        d = pow ( colortable[c][0]-rgb[0],2.0 ) +
            pow ( colortable[c][1]-rgb[1],2.0 ) +
            pow ( colortable[c][2]-rgb[2],2.0 );
        if ( d<smallest_distance ) {
            smallest_distance = d;
            best_match=c;
        }
    }
    return best_match;
}

bool Xterm256Generator::initialized=false;

unsigned char Xterm256Generator::colortable[254][3] = { {0} };

const unsigned char Xterm256Generator::valuerange[] = { 0x00, 0x5F, 0x87, 0xAF, 0xD7, 0xFF };

const unsigned char Xterm256Generator::basic16[16][3] = {
    { 0x00, 0x00, 0x00 }, // 0
    { 0xCD, 0x00, 0x00 }, // 1
    { 0x00, 0xCD, 0x00 }, // 2
    { 0xCD, 0xCD, 0x00 }, // 3
    { 0x00, 0x00, 0xEE }, // 4
    { 0xCD, 0x00, 0xCD }, // 5
    { 0x00, 0xCD, 0xCD }, // 6
    { 0xE5, 0xE5, 0xE5 }, // 7
    { 0x7F, 0x7F, 0x7F }, // 8
    { 0xFF, 0x00, 0x00 }, // 9
    { 0x00, 0xFF, 0x00 }, // 10
    { 0xFF, 0xFF, 0x00 }, // 11
    { 0x5C, 0x5C, 0xFF }, // 12
    { 0xFF, 0x00, 0xFF }, // 13
    { 0x00, 0xFF, 0xFF }, // 14
    { 0xFF, 0xFF, 0xFF }  // 15
};
}
