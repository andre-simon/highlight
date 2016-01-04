/***************************************************************************
                          stringtools.cpp  -  description
                             -------------------
    begin                : Mon Dec 10 2001
    copyright            : (C) 2001-2010 by Andre Simon
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

#include "stringtools.h"

namespace StringTools
{

string change_case ( const string & s, const KeywordCase kcase ) throw()
{
    string r ( s );
    switch ( kcase ) {
    case CASE_UNCHANGED:
        break;
    case CASE_CAPITALIZE:
    case CASE_LOWER:
        for ( unsigned int i = 0; i < r.size(); ++i )
            r[i] = tolower ( r[i] );
        if ( kcase == CASE_CAPITALIZE && r.size() )
            r[0] = toupper ( r[0] );
        break;
    case CASE_UPPER:
        for ( unsigned int i = 0; i < r.size(); ++i )
            r[i] = toupper ( r[i] );
        break;
    }
    return r;
}


string trimRight ( const string &value )
{
    string::size_type where = value.find_last_not_of ( " \t\r" );

    if ( where == string::npos )
        // string has nothing but space
        return string();

    if ( where == ( value.length() - 1 ) )
        // string has no trailing space, don't copy its contents
        return value;

    return value.substr ( 0, where + 1 );
}

string trim(const string& s)
{
    string str(s);
    string::size_type pos = str.find_last_not_of(' ');
    if(pos != string::npos) {
        str.erase(pos + 1);
        pos = str.find_first_not_of(' ');
        if(pos != string::npos) str.erase(0, pos);
    } else str.erase(str.begin(), str.end());
    return str;
}

vector<string> splitString ( const string& s, unsigned char delim )
{
    string::size_type pos=s.find ( delim ), oldPos=0;
    vector <string> results;

    if ( pos ==string::npos ) {
        if ( !s.empty() ) results.push_back ( s );
        return results;
    }

    do {
        if ( oldPos-pos ) results.push_back ( s.substr ( oldPos, pos-oldPos ) );
        oldPos=pos+1;
        pos=s.find ( delim, pos+1 );
    } while ( pos!=string::npos );
    results.push_back ( s.substr ( oldPos ) );

    return results;
}

int calcWeight(const string&s)
{
    int sum=0;
    for (unsigned int i=0; i<s.size(); i++) {
        sum+=(i%2)? int(s[i])*3 : int(s[i]);
    }
    return sum;
}

string getPathAcronym(const string&path, char delim)
{
    string acronym;
    size_t pos=0;
    while ((pos=path.find(delim, pos))!=string::npos) {
        pos++;
        if (pos<path.size() && path[pos]!=delim) {
            acronym+= path[pos];
        }
    }
    if(!acronym.empty()) acronym.replace(acronym.end()-1, acronym.end(), "_");
    return acronym;
}

}
