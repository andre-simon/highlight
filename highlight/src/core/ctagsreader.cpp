/***************************************************************************
                          ctagsreader.h  -  description
                             -------------------
    begin                : Tue Oct 21 2008
    copyright            : (C) 2008 by Andre Simon
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


#include "ctagsreader.h"


string TagInfo::getKind() const
{
	if ( kind.empty() ) return "";
	switch ( kind[0] )
	{
		case 'c': return	"class";
		case 'd': return	"define";
		case 'e': return	"enumerator";
		case 'f': return	"function";
		case 'F': return	"file";
		case 'g': return	"enumeration name";
		case 'm': return	"member";
		case 'n': return	"namespace";
		case 'p': return	"function prototype";
		case 's': return	"structure name";
		case 't': return	"typedef";
		case 'u': return	"union name";
		case 'v': return	"variable";
		default: return "";
	}
}

bool CTagsReader::load ( const string & ctags_path )
{
	ifstream in ( ctags_path.c_str() );
	if ( in )
	{
		string line;
		string tagname,
		tagfile,
		dummy1,
		dummy2,
		kind,
		name_space,
		file;
		TagInfo info;
		line.reserve ( 250 );
		while ( getline ( in, line ) && line.size() )
		{
			if ( line[0]=='!' )
			{
				continue;
			}

			istringstream tagStream ( line );
			if ( !getline ( tagStream, tagname, '\t' ) ) continue;
			if ( !getline ( tagStream, tagfile, '\t' ) ) continue;
			if ( !getline ( tagStream, dummy1, '\t' ) ) continue;
			if ( !getline ( tagStream, dummy2, '\t' ) ) continue;
			if (dummy2.empty()) {
				while (dummy2.empty()) {
					if ( !getline ( tagStream, dummy2, '\t' ) ) continue;
				}
			}
			if ( !getline ( tagStream, kind, '\t' ) ) continue;
			name_space.clear(); file.clear();
			getline ( tagStream, name_space, '\t' );

			info.file=tagfile;
			info.kind=kind;
			info.name_space=name_space;
			//info.tag_exists=true;
			tags[tagname] = info;
		}
		in.close();
		return true;
	}
	return false;
}
