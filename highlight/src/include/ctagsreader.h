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


#ifndef CTAGSREADER_H
#define CTAGSREADER_H

#include <string>
#include <sstream>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

/** TagInfo contains information about a ctags tag */
class TagInfo
{
	public:
		TagInfo() {};
		string getKind() const;
		string file, kind, name_space;

};

/** Maps parameter keys to values*/
typedef map<string, TagInfo> TagsMap;

class CTagsReader
{
	public:

		CTagsReader() {};
		~CTagsReader() {};

		/** load ctags file
		\param ctags_path Path to ctags file
		\return true if successfull
		*/
		bool load ( const string & ctags_path );

		/** test if tag infoation exists
		\param tagname tag ot highlight token
		\return true if tag information exists
		*/
		bool tagExists ( const string& tagname )
		{
			return tags.count ( tagname ) >0;
		}

		/** return tag info, call tagExists first to avoid growing map
		\param tagname tag ot highlight token
		\return tag info
		*/
		TagInfo getTagInfo ( const string &tagname )
		{
			return tags[tagname];
		}

	private:

		TagsMap tags;
};

/*
		c	class name
		d	define (from #define XXX)
		e	enumerator
		f	function or method name
		F	file name
		g	enumeration name
		m	member (of structure or class data)
		p	function prototype
		s	structure name
		t	typedef
		u	union name
		v	variable

*/

#endif
