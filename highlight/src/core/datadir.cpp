/***************************************************************************
                          dataDir.cpp  -  description
                             -------------------
    begin                : Sam March 1 2003
    copyright            : (C) 2003 by Andre Simon
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


#include <string>
#include <fstream>
#include <vector>
#include "platform_fs.h"

#include "datadir.h"

using namespace std;

string DataDir::LSB_DATA_DIR="/usr/share/highlight/";
string DataDir::LSB_CFG_DIR="/etc/highlight/";
string DataDir::LSB_DOC_DIR="/usr/share/doc/highlight/";


bool DataDir::searchDataDir ( const string &userDefinedDir )
{

#ifndef _WIN32
	bool found = false;

	vector <string> possibleDirs;
	if ( !userDefinedDir.empty() ) possibleDirs.push_back ( userDefinedDir );

#ifdef HL_DATA_DIR
	possibleDirs.push_back ( HL_DATA_DIR );
#endif
	possibleDirs.push_back ( LSB_DATA_DIR );

	for ( unsigned int i=0;i<possibleDirs.size();i++ )
	{
		if ( Platform::fileExists ( possibleDirs[i] ) )
		{
			dataDir=possibleDirs[i];
			found = true; break;
		}
	}
	return found;
#else
	dataDir=userDefinedDir;
	return true;
#endif
}

DataDir::DataDir()
{
}


void DataDir::setAdditionalConfDir ( const string& dir )
{
	additionalConfDir=dir;
}

const string & DataDir::getAdditionalConfDir()
{
	return additionalConfDir;
}

const string &DataDir::getDir()
{
	return dataDir;
}

const string DataDir::getLangPath ( const string & file )
{

	return dataDir+"langDefs"+Platform::pathSeparator+file;
}

const string DataDir::getThemePath ( const string & file)
{

	return dataDir+"themes"+Platform::pathSeparator+file;
}

const string DataDir::getConfDir ( bool forceDefault )
{
	if ( !forceDefault && !additionalConfDir.empty() )
	{
		return additionalConfDir;
	}
#ifndef _WIN32
#ifdef HL_CONFIG_DIR
	return HL_CONFIG_DIR;
#else
	return LSB_CFG_DIR;
#endif
#else
	return getDir();
#endif
}


const string  DataDir::getI18nDir()
{
	return dataDir+"gui_files"+Platform::pathSeparator+"i18n"+Platform::pathSeparator;
}

const string  DataDir::getExtDir()
{
	return dataDir+"gui_files"+Platform::pathSeparator+"ext"+Platform::pathSeparator;
}

const string DataDir::getDocDir()
{
#ifndef _WIN32
#ifdef HL_DOC_DIR
	return HL_DOC_DIR;
#else
	return LSB_DOC_DIR;
#endif
#else
	return getDir();
#endif
}
/*
bool DataDir::fileExists ( const string&f )
{
	ifstream file ( f.c_str() );
	bool exists=!file.fail();
	file.close();
	return exists;
}
*/