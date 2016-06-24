/***************************************************************************
                          dataDir.cpp  -  description
                             -------------------
    begin                : Sam March 1 2003
    copyright            : (C) 2003-2013 by Andre Simon
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


string DataDir::LSB_DATA_DIR="/usr/share/highlight/";
string DataDir::LSB_CFG_DIR="/etc/highlight/";
string DataDir::LSB_DOC_DIR="/usr/share/doc/highlight/";


void DataDir::initSearchDirectories ( const string &userDefinedDir )
{

#ifndef WIN32

    possibleDirs.push_back ( Platform::getHomePath() + "/.highlight/" );
    if ( !userDefinedDir.empty() ) possibleDirs.push_back ( userDefinedDir );
    
    char* hlEnvPath=getenv("HIGHLIGHT_DATADIR");
    if (hlEnvPath!=NULL){
      possibleDirs.push_back ( hlEnvPath );
    }
    
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

#else
    possibleDirs.push_back(Platform::getAppPath()); //not needed because of fallback in searchFile
#endif
}

void DataDir:: searchDataDir( const string &userDefinedDir )
{
    initSearchDirectories(userDefinedDir);
}

const string DataDir::searchFile(const string path)
{
    for ( unsigned int i=0; i<possibleDirs.size(); i++ ) {
        // cerr << "searching "<<possibleDirs[i]<< path<<"\n";
        if ( Platform::fileExists ( possibleDirs[i] + path ) )
            return possibleDirs[i]+ path;
    }
    return path;
}

const void DataDir::printConfigPaths()
{
    for ( unsigned int i=0; i<possibleDirs.size(); i++ ) {
        if ( Platform::fileExists ( possibleDirs[i] ) )
            cout <<possibleDirs[i]<<"\n";
    }
}

const string DataDir::getLangPath ( const string & file )
{
    return searchFile(string("langDefs")+Platform::pathSeparator+file);
}

const string DataDir::getThemePath ( const string & file)
{
    return searchFile(string("themes")+Platform::pathSeparator+file);
}

const string DataDir::getPluginPath ( const string & file)
{
    return searchFile(string("plugins")+Platform::pathSeparator+file);
}

const string DataDir::getFiletypesConfPath (const string & file)
{
    return searchFile(file + ".conf");
}

const string DataDir::getThemePath ( )
{
    return getSystemDataPath()+"themes"+Platform::pathSeparator;
}

const string DataDir::getLangPath ( )
{
    return getSystemDataPath()+"langDefs"+Platform::pathSeparator;
}

const string DataDir::getPluginPath ( )
{
    return getSystemDataPath()+"plugins"+Platform::pathSeparator;
}

const string DataDir::getSystemDataPath ( )
{
#ifndef WIN32
#ifdef HL_DATA_DIR
    return HL_DATA_DIR;
#else
    return LSB_DATA_DIR;
#endif
#else
    return Platform::getAppPath();
#endif
}

const string  DataDir::getI18nDir()
{
    return getSystemDataPath()+"gui_files"+Platform::pathSeparator+"i18n"+Platform::pathSeparator;
}

const string  DataDir::getExtDir()
{
    return getSystemDataPath() +"gui_files"+Platform::pathSeparator+"ext"+Platform::pathSeparator;
}

const string DataDir::getDocDir()
{
    return getSystemDataPath() ;
}
