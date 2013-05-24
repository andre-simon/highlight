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

	possibleDirs.push_back ( Platform::getHomePath() + "/.highlight/" );
	if ( !userDefinedDir.empty() ) possibleDirs.push_back ( userDefinedDir );

#ifdef HL_DATA_DIR
	possibleDirs.push_back ( HL_DATA_DIR );
#else
	possibleDirs.push_back ( LSB_DATA_DIR );
#endif
	
#ifdef HL_CONFIG_DIR
	possibleDirs.push_back ( HL_CONFIG_DIR);
#else
	possibleDirs.push_back ( LSB_CFG_DIR);
#endif
	

	
	/*
	for ( unsigned int i=0;i<possibleDirs.size();i++ )
	{
		if ( Platform::fileExists ( possibleDirs[i] ) )
		{
			dataDir=possibleDirs[i];
			found = true; break;
		}
	}
	*/
	return true;
#else
	//dataDir=userDefinedDir;
	return true;
#endif
}

DataDir::DataDir()
{
}

const string DataDir::searchFile(const string path){
	for ( unsigned int i=0;i<possibleDirs.size();i++ )
	{
	  //cerr << "searching "<<possibleDirs[i]<< path<<"\n";
		if ( Platform::fileExists ( possibleDirs[i] + path ) )
			return possibleDirs[i]+ path;
		
	}
	return path;
}

const void DataDir::printConfigPaths(){
	for ( unsigned int i=0;i<possibleDirs.size();i++ )
	{
		if ( Platform::fileExists ( possibleDirs[i] ) )
			cout <<possibleDirs[i]<<"\n";
		
	}
}

const string DataDir::getLangPath ( const string & file )
{

	//return dataDir+"langDefs"+Platform::pathSeparator+file;
	return searchFile(string("langDefs")+Platform::pathSeparator+file);
}

const string DataDir::getThemePath ( const string & file)
{
	//return dataDir+"themes"+Platform::pathSeparator+file;
	return searchFile(string("themes")+Platform::pathSeparator+file);
}

const string DataDir::getFiletypesConfPath (const string & file)
{
	
	//return dataDir+"filetypes.conf";
	return searchFile(file + ".conf");
}

// FIXME remaining getters are crap

const string  DataDir::getI18nDir()
{
#ifndef _WIN32
#ifdef HL_DATA_DIR
	string dataDir= HL_DATA_DIR ;
#else
	string dataDir=  LSB_DATA_DIR;
#endif	
#else
	string dataDir;
#endif
	return dataDir+"gui_files"+Platform::pathSeparator+"i18n"+Platform::pathSeparator;
}

const string  DataDir::getExtDir()
{
#ifndef _WIN32
#ifdef HL_DATA_DIR
	string dataDir= HL_DATA_DIR ;
#else
	string dataDir=  LSB_DATA_DIR;
#endif	
#else
	string dataDir;
#endif
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
