/***************************************************************************
                          syntaxreader.h  -  description
                             -------------------
    begin                : Wed Nov 28 2001
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


#ifndef SYNTAXREADER_H
#define SYNTAXREADER_H

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>

#include <Diluculum/LuaState.hpp>
#include <Diluculum/LuaVariable.hpp>
#include <Diluculum/LuaFunction.hpp>
#include "platform_fs.h"
#include "enums.h"
#include "re/Pattern.h"
#include "re/Matcher.h"

using namespace std;

namespace highlight
{

	class RegexElement;

	/** maps keywords and the corresponding class IDs*/
	typedef map <string, int> KeywordMap;

	/** maps embedded langiage names to exit delimiter regexes*/
	typedef map <string, string> EmbedLangDelimMap;

	/**\brief Contains specific data of the programming language being processed.

	* @author Andre  Simon
	*/

	class SyntaxReader
	{

		public:

			SyntaxReader();

			~SyntaxReader();

			/** \return Symbol string, containg all known symbols with the referencing state ids*/
			//const string &getSymbolString() const { return symbolString; }

			/** \return Failed regular expression */
			 string getFailedRegex() const { return failedRegex; }

			/** \return Failed Lua exception description */
			 string getLuaErrorText() const { return luaError; }

			/** \return Prefix of raw strings */
			unsigned char getRawStringPrefix() const { return rawStringPrefix; }

			/** \return Continuation Character */
			unsigned char getContinuationChar() const { return continuationChar; }

			/** \return true if syntax highlighting is enabled*/
			bool highlightingEnabled() const { return !disableHighlighting;}

			/** \return True if language is case sensitive */
			bool isIgnoreCase() const { return ignoreCase;}

			/** \param s String
			     \return class id of keyword, 0 if s is not a keyword */
			int isKeyword ( const string &s ) ;

			/** Load new language definition
			    Will only read a new language definition if the given
			      file path is not equal to the path of the current language definition.
			    \param langDefPath Path of language definition
			    \param clear Test if current data should be resetted to defaults
			    \return LoadResult  */
			LoadResult load( const string& langDefPath, bool clear=true );

			/** \return True if multi line comments may be nested */
			bool allowNestedMLComments() const { return allowNestedComments; }

			/** \return True if highlighting is disabled
                                    TODO remove method */
			bool highlightingDisabled() const  { return disableHighlighting; }

			/** \return True if the next load() call would load a new language definition
			    \param  langDefPath Path to language definition  */
			bool needsReload ( const string &langDefPath ) const { return currentPath!=langDefPath; }

			/** \return True if current language may be reformatted (c, c++, c#, java) */
			bool enableReformatting() const { return reformatCode;}

			/** \return True if escape sequences are allowed outsde of strings */
//			bool allowExtEscSeq() const { return allowExtEscape; }

			/** \return keywords*/
			const KeywordMap& getKeywords() const { return keywords; }

			/** \return keyword classes*/
			const vector<string>& getKeywordClasses() const { return keywordClasses;}

			/** \return regular expressions */
			const vector<RegexElement*>& getRegexElements() const {return regex;};

			/** \return description of the programming language */
			const string & getDescription () const {return langDesc;}

			/**  \param delimID delimiter id
			     \return true,  if no closing delimiter exists (open and close delimiters are equal)
			 */
			bool delimiterIsDistinct ( int delimID )
			{
				return delimiterDistinct[delimID];
			}

			/**  \param delimID delimiter id
			     \return true,  if delimiter indicates a raw string
			 */
			bool delimiterIsRawString ( int delimID )
			{
				return rawStringOpenDelims[delimID];
			}

			/**  Pairs of open/close delimiters have a unique ID to test if two tokens act as delimiters
			     \param token delimiter token
                             \param s State of delimiter
			     \return delimiter ID
			 */
			int getOpenDelimiterID ( const string& token, State s);

			/**  Pairs of open/close delimiters have a unique ID to test if two tokens act as delimiters
			     \param token delimiter token
                             \param s State of delimiter
                             \param openDelimId opening delimiter retrieved with getOpenDelimiterID
			     \return true if delimiter id of token matches openDelimID
			 */
			bool matchesOpenDelimiter ( const string& token, State s, int openDelimId);

/*
			string getDelimRegex(const string & lang){
				return exitDelimiters[lang];
			}
*/
			/** initializes end delimiter regex to switch back to host language
				\param langPath path of embedded language definition
			*/
			void restoreLangEndDelim(const string&langPath);

			/**
				\param lang language definition name  (no path, no ".lang" extension)
				\return absolute path based on the previously loaded definition
			*/
			string getNewPath(const string& lang);

			/**
				\return absolute path of currently loaded definition
			*/
			string getCurrentPath() const { return currentPath;}

			/**
				\return pointer to state validation function
			*/
			Diluculum::LuaFunction* getValidateStateChangeFct() const {return validateStateChangeFct;}

			/**
				\return pointer to Lua state
			*/
			 Diluculum::LuaState* getLuaState() const { return luaState;}

			/**
				\param chunk Lua function to be added to the function list
			*/
			void addUserChunk(const Diluculum::LuaFunction& chunk){
			  userChunkFcts.push_back(new Diluculum::LuaFunction(chunk));
			}

			/**
				\param ls Lua state to be initialized with constants
				\param langDefPath absolute path of language definition
			*/
			static void initLuaState(Diluculum::LuaState& ls, const string& langDefPath);

		private:

			static const string REGEX_IDENTIFIER;
			static const string REGEX_NUMBER;
			static const string REGEX_ESCSEQ;

			// path to loaded language definition
			string currentPath;

			// Language description
			string langDesc;

			string failedRegex, luaError;

			KeywordMap keywords;

			vector <string> keywordClasses;

			vector <RegexElement*> regex;

			EmbedLangDelimMap exitDelimiters;

			// saves if delimiter pair consists of the same delimiter symbol
			map <int, bool> delimiterDistinct;

			map <int, bool> rawStringOpenDelims;

			map <int, int> delimIds2;

			// keywords are not case sensitive if set
			bool ignoreCase,

			// highlighting is disabled
			disableHighlighting,

			// Escape sequences are allowed outrside of strings
			//allowExtEscape,

			// allow nested multi line comment blocks
			allowNestedComments,

			// code formatting is enabled if set
			reformatCode;

			// character which is prefix of raw string (c#)
			unsigned char rawStringPrefix,

			//character which continues curreent style on next line
			continuationChar;

			/* reset members */
			void reset();

			// generate a unique class ID of the class name
			unsigned int generateNewKWClass ( const string& newClassName );

			bool readFlag(const Diluculum::LuaVariable& var) ;


			Diluculum::LuaFunction* validateStateChangeFct;

			Diluculum::LuaState* luaState; // make member to allow interaction with codeparser instance

			vector<Diluculum::LuaFunction*> userChunkFcts;

	};


	/**\brief Association of a regex with a state description

	  A RegexElement associates a regular expression with the state information
	  (opening and closing state, pattern, keyword class, keyword group id, language name)
	*/
	class RegexElement
	{
		public:
			RegexElement()
			:open ( STANDARD ), end ( STANDARD ), rePattern ( NULL ), kwClass ( 0 ),capturingGroup ( -1 ), langName(), instanceId(instanceCnt++)
			{

			}

			RegexElement ( State oState, State eState, Pattern *re, unsigned int cID=0, int group=-1, const string& name="" ) :
					open ( oState ), end ( eState ), rePattern ( re ), kwClass ( cID ), capturingGroup ( group ), langName(name),instanceId(instanceCnt++)
			{
				// cerr << "new re element "<<  rePattern->getPattern() <<" open: "<<open<<" end "<<end<<"\n";
				//cerr << "pattern:"<<rePattern->getPattern()<<"  instanceId:"<<instanceId<<"\n";
			}

			~RegexElement() { if ( rePattern ) delete rePattern; instanceCnt--; }

			State open, ///< opening state
			end;  ///< closing state
			Pattern *rePattern;          ///< regex pattern
			unsigned int kwClass;        ///< keyword class
			int capturingGroup;          ///< capturing group ID
			string langName;             ///< language name
			static int instanceCnt;
			int instanceId;

		private:
			RegexElement (const RegexElement& rhs){
				// does not work because Pattern misses copy constructor
				/*open=rhs.open;
				end=rhs.end;
				kwClass=rhs.kwClass;
				capturingGroup=rhs.capturingGroup;
				Pattern *pOrig=rePattern;
				rePattern=new Pattern(*rhs.rePattern);
				delete pOrig;*/
			}
			RegexElement& operator=(const RegexElement& rhs){
				// does not work because Pattern misses copy constructor
				/*open=rhs.open;
				end=rhs.end;
				kwClass=rhs.kwClass;
				capturingGroup=rhs.capturingGroup;
				Pattern *pOrig=rePattern;
				rePattern=new Pattern(*rhs.rePattern);
				delete pOrig;
				*/
				return *this;
			}
	};

	/**\brief Association of a regex and its relevant capturing group
	*/
	struct RegexDef
	{
		RegexDef() :capturingGroup ( -1 ) {}
		string reString;     ///< regex string
		int capturingGroup;  ///< capturing group which should be recognized as token
	};

}
#endif
