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
		//string dataDir;
		//string additionalConfDir;
		//bool fileExists ( const string& );
			vector <string> possibleDirs;

	public:

		DataDir();

		static string LSB_DATA_DIR;
		static string LSB_CFG_DIR;
		static string LSB_DOC_DIR;

		/** search for a valid installation directory
		    \param  userDefinedDir Directory defined by user
		    \return True if directory was found */
		bool searchDataDir ( const string &userDefinedDir );
		
		const void printConfigPaths();
		
		const string searchFile(const string path);



		/** add another installation directory, which is added to search path
		    \param  dir Directory defined by user */
		//void setAdditionalConfDir ( const string& dir );


		/** \return data Directory defined by user */
		//const string & getAdditionalConfDir();

		/** \return Data installation directory */
		//const string & getDir() ;

		/**  \param file filename
		     \param forceDefault set tue if additional directory should be ignored
		     \return Location of languafe definitions */
		const string getLangPath ( const string & file="") ;

		/** \param file filename
		    \param forceDefault set tue if additional directory should be ignored
		    \return Location of themes */
		const string getThemePath ( const string & file="" ) ;


		/** \param forceDefault set tue if additional directory should be ignored
		    \return Location of configuration files */
		//const string getConfDir ( bool forceDefault=false ) ;
		
		const string getFiletypesConfPath (const string &);

		/** \return Location of GUI menu translation files */
		const string getI18nDir();

		/** \return Location of GUI file extension filter files */
		const string getExtDir();

		/** \return Location of documentation (README) files (GUI) */
		const string getDocDir();
};

#endif
