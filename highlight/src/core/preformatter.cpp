/***************************************************************************
                        PreFormatter.cpp  -  description
                             -------------------
    begin                : Mo Jan 03 2005
    copyright            : (C) 2005-2007 by Andre Simon
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


#include "preformatter.h"

#include <iostream>
#include "stringtools.h"

namespace highlight
{

const std::string PreFormatter::LB_CHARS = " \t[](){}-+<>.:,;";
const std::string PreFormatter::WS_CHARS = " \n\r\t";
const std::string PreFormatter::INDENT_MARKERS = "{(=";

PreFormatter::PreFormatter() :
    maxLineLength ( 80 ),
    index ( 0 ),
    numberSpaces ( 0 ),
    lineNumber ( 0 ),
    wsPrefixLength ( string::npos ),
    hasMore ( false ),
    indentAfterOpenBraces ( true ),
    redefineWsPrefix ( false ),
    wrapLines ( false ),
    replaceTabs ( false )
{
}

PreFormatter::~PreFormatter()
{
}

bool PreFormatter::hasMoreLines()
{
    return hasMore;
}

bool PreFormatter::indentCode()
{
    return indentAfterOpenBraces;
}

void PreFormatter::setLine ( const std::string& newLine )
{

    line=newLine;

    if ( replaceTabs && numberSpaces ) {
        size_t tabPos=line.find ( '\t' );
        while ( tabPos!=string::npos ) {
            line.replace ( tabPos , 1, numberSpaces - ( tabPos % numberSpaces ) , ' ' );
            tabPos = line.find ( '\t', tabPos+1 );
        }
    }

    if ( wrapLines ) {
        wsPrefix.clear();
        index=0;
        wsPrefixLength=string::npos;
        hasMore=true;
        redefineWsPrefix=false;
    }
}

std::string PreFormatter::getNextLine()
{

    if ( !wrapLines ) {
        hasMore = false;
        return line;
    }

    ++lineNumber;

    if ( !index && line.length() > maxLineLength ) { // erster Durchlauf...
        // wenn moeglich an oeffnender Klammer oder Geichheitszeichen ausrichten
        if ( indentAfterOpenBraces ) {
            wsPrefixLength=line.find_first_of ( INDENT_MARKERS );
        }
        // sonst die Einrckung der Originalzeile beibehalten
        if ( wsPrefixLength==string::npos || wsPrefixLength-index>maxLineLength ) {
            wsPrefixLength=line.find_first_not_of ( WS_CHARS );
        } else {
            // wsPrefix in allen neu umgebrochenen Zeilen durch Spaces ersetzen
            redefineWsPrefix=true;
            //  Position hinter oeffnende Klammer springen
            wsPrefixLength=line.find_first_not_of ( WS_CHARS,wsPrefixLength+1 );
        }

        if ( wsPrefixLength!=string::npos ) {
            index = wsPrefixLength;
            // Falls Anzahl der Whitespaces am beginn der ersten zeile groesser
            // als Max. Zeilenlaenge, Whitespaces verwerfen
            if ( wsPrefixLength>maxLineLength ) {
                wsPrefixLength=0;
                return string();
            } else {
                wsPrefix=line.substr ( 0, wsPrefixLength );
            }
        }
        // Zeile enthaelt nur Whitespace; verwerfen
        else {
            hasMore= false;
            return string();
        }
    } else {
        if ( redefineWsPrefix ) {
            wsPrefix.clear();
            wsPrefix.append ( wsPrefixLength, ' ' );
        }
        redefineWsPrefix=false;
    }

    string resultString;

    // Position, ab der rueckwaerts nach Umbruchmglichkeit gesucht wird
    unsigned int searchEndPos = maxLineLength - wsPrefixLength;

    // letztes Teilstueck der Zeile ausgeben; Parsen beenden
    if ( line.length()-index < searchEndPos ) {
        hasMore=false;
        resultString= ( index>0 ) ? wsPrefix + line.substr ( index ) : line.substr ( index );
        return resultString;
    }

    // Umbrechposition suchen
    size_t lbPos = line.find_last_of ( LB_CHARS, index+searchEndPos );
    if ( lbPos <= index || lbPos == string::npos ) {
        // nichts gefunden, hart umbrechen
        lbPos = index + searchEndPos;
    }
    // Einrckung der Originalzeile erhalten
    resultString+=wsPrefix;
    // Neue Zeile erzeugen
    resultString += line.substr ( index, lbPos-index+1 );

    // Whitespace am neuen Zeilenbeginn ignorieren, ausser beim ersten Durchlauf
    size_t newIndex=line.find_first_not_of ( WS_CHARS, lbPos+1 );
    index= ( newIndex!=string::npos ) ?newIndex:line.length();

    hasMore=index!=line.length(); // unnoetigen Leerstring vermeiden

    if ( hasMore ) wrappedLines.insert ( lineNumber ); // diese Zeile wurde umgebrochen

    return resultString;
}

void PreFormatter::setWrapLineLength ( unsigned int maxLineLength )
{
    this->maxLineLength = maxLineLength;
}
void PreFormatter::setWrapIndentBraces ( bool indentAfterOpenBraces )
{
    this->indentAfterOpenBraces = indentAfterOpenBraces;
}

void PreFormatter::setNumberSpaces ( unsigned int num )
{
    numberSpaces = num;
}

}
