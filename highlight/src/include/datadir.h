/***************************************************************************
                          datadir.h  -  description
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


#ifndef DATADIR_H
#define DATADIR_H

using namespace std;

/** \brief Manages access to installation directories.

    Apart from the standard installation directory, one can define additional
    search paths. If the additonal paths do not exist, the default paths are
    returned.
 **/

class DataDir
{
    vector <string> possibleDirs;

public:

    static string LSB_DATA_DIR;
    static string LSB_CFG_DIR;
    static string LSB_DOC_DIR;

    /** search for a valid installation directory
        \param  userDefinedDir Directory defined by user
        \return True if directory was found */
    void initSearchDirectories ( const string &userDefinedDir );

    // Deprecated
    void searchDataDir( const string &userDefinedDir );

    const void printConfigPaths();

    const string searchFile(const string path);

    /**  \param file file
         \return Location of given syntax definition */
    const string getLangPath ( const string & file) ;

    /** \return System themes location */
    const string getThemePath ( ) ;

    /** \return System syntax location */
    const string getLangPath ( ) ;

    /** \return System syntax location */
    const string getSystemDataPath ( ) ;

    /** \param file file
        \return Location of given theme */
    const string getThemePath ( const string & file ) ;

    const string getFiletypesConfPath (const string &);

    const string getPluginPath (const string &);

    const string getPluginPath ();

    /** \return Location of GUI menu translation files */
    const string getI18nDir();

    /** \return Location of GUI file extension filter files */
    const string getExtDir();

    /** \return Location of documentation (README) files (GUI) */
    const string getDocDir();
};

#endif
