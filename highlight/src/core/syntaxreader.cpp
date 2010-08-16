/***************************************************************************
                          syntaxreader.cpp  -  description
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


#include <memory>

#include "syntaxreader.h"
#include "stringtools.h"
#include "enums.h"


using namespace std;

namespace highlight
{

const string SyntaxReader::REGEX_IDENTIFIER =
    "[a-zA-Z_]\\w*";

const string SyntaxReader::REGEX_NUMBER =
    "(?:0x|0X)[0-9a-fA-F]+|\\d*[\\.]?\\d+(?:[eE][\\-\\+]\\d+)?[lLuU]*";

const string SyntaxReader::REGEX_ESCSEQ =
    "\\\\u\\p{XDigit}{4}|\\\\\\d{3}|\\\\x\\p{XDigit}{2}|\\\\[ntvbrfa\\\\\\?'\"]";

int RegexElement::instanceCnt=0;

SyntaxReader::SyntaxReader() :
        ignoreCase ( false ),
        disableHighlighting ( false ),
        allowNestedComments ( true ),
        reformatCode ( false ),
        validateStateChangeFct(NULL),
        luaState(NULL)
{

}

SyntaxReader::~SyntaxReader()
{
    reset();

    for (unsigned int i=0;i<pluginChunks.size();i++){
      delete pluginChunks[i];
    }
}

int SyntaxReader::isKeyword ( const string &s )
{
    return ( s.length() && keywords.count ( s ) ) ? keywords[s] : 0;
}

void SyntaxReader::restoreLangEndDelim(const string& langPath) {
    if ( !langPath.empty()&& exitDelimiters.count(langPath) )
    {
        Pattern* p = Pattern::compile ( exitDelimiters[langPath]);
        if ( p!=NULL ) {
            regex.insert (regex.begin(),1, new RegexElement ( EMBEDDED_CODE_END,EMBEDDED_CODE_END, p ) );
        }
    }
}

unsigned int SyntaxReader::generateNewKWClass ( const string& newClassName )
{
    unsigned int newClassID=0;
    bool found=false;
    while ( newClassID<keywordClasses.size() && !found )
    {
        found = ( newClassName==keywordClasses[newClassID++] );
    }
    if ( !found )
    {
        newClassID++;
        keywordClasses.push_back ( newClassName );
    }
    return newClassID;
}


bool SyntaxReader::readFlag(const Diluculum::LuaVariable& var) {
    if (var.value()==Diluculum::Nil) return false;
    return var.value().asBoolean();
}


void  SyntaxReader::initLuaState(Diluculum::LuaState& ls, const string& langDefPath){
          // initialize Lua state with variables which can be used within scripts
        string::size_type Pos = langDefPath.find_last_of ( Platform::pathSeparator );
        ls["HL_LANG_DIR"] =langDefPath.substr ( 0, Pos+1 );

        ls["Identifiers"]=REGEX_IDENTIFIER;
        ls["Digits"]=REGEX_NUMBER;

	//nitialize environment for hook functions
	ls["HL_STANDARD"]=STANDARD;
	ls["HL_STRING"]=STRING;
	ls["HL_NUMBER"]=NUMBER;
	ls["HL_LINE_COMMENT"]=SL_COMMENT;
	ls["HL_BLOCK_COMMENT"]=ML_COMMENT;
	ls["HL_ESC_SEQ"]=ESC_CHAR;
	ls["HL_PREPROC"]=DIRECTIVE;
	ls["HL_PREPROC_STRING"]=DIRECTIVE_STRING;
	ls["HL_OPERATOR"]=SYMBOL;
	ls["HL_LINENUMBER"]=LINENUMBER;
	ls["HL_KEYWORD"]=KEYWORD;
	ls["HL_STRING_END"]=STRING_END;
	ls["HL_LINE_COMMENT_END"]=SL_COMMENT_END;
	ls["HL_BLOCK_COMMENT_END"]=ML_COMMENT_END;
	ls["HL_ESC_SEQ_END"]=ESC_CHAR_END;
	ls["HL_PREPROC_END"]=DIRECTIVE_END;
	ls["HL_OPERATOR_END"]=SYMBOL_END;
	ls["HL_KEYWORD_END"]=KEYWORD_END;
	ls["HL_EMBEDDED_CODE_BEGIN"]=EMBEDDED_CODE_BEGIN;
	ls["HL_EMBEDDED_CODE_END"]=EMBEDDED_CODE_END;
	ls["HL_IDENTIFIER_BEGIN"]=IDENTIFIER_BEGIN;
	ls["HL_IDENTIFIER_END"]=IDENTIFIER_END;
	ls["HL_UNKNOWN"]=_UNKNOWN;

}

LoadResult SyntaxReader::load ( const string& langDefPath, bool clear )
{
    if ( clear )  reset();

    currentPath=langDefPath;
    disableHighlighting=false;

    try {

	if (luaState) delete luaState;
	luaState=new Diluculum::LuaState();

	Diluculum::LuaState& ls=*luaState;
	initLuaState(ls, langDefPath);

        // ececute script and read values
        ls.doFile (langDefPath);

	Diluculum::LuaValueMap globals = ls.globals();

        langDesc = ls["Description"].value().asString();

	if (pluginChunks.size()){
	  Diluculum::LuaValueList params;
	  params.push_back(langDesc);
	  for (unsigned int i=0;i<pluginChunks.size();i++){
	    ls.call(*pluginChunks[i], params, "syntax user function");
	  }
	}

        ignoreCase=readFlag(ls["IgnoreCase"]);
        reformatCode=readFlag(ls["EnableIndentation"]);
        disableHighlighting=readFlag(ls["DisableHighlighting"]);

        int idx=1;
        int keywordIdx=0;
        int kwId=0;
        char kwName[5]={0};
        while (ls["Keywords"][idx].value() !=Diluculum::Nil) {
            keywordIdx=ls["Keywords"][idx]["Id"].value().asNumber();
            snprintf(kwName, sizeof(kwName), "kw%c", ('a'+keywordIdx-1)); // TODO kwa -> kw1...
            kwId= generateNewKWClass ( kwName );

            if (ls["Keywords"][idx]["List"].value()!=Diluculum::Nil) {
                int listIdx=1;
                Diluculum::LuaVariable luaList=ls["Keywords"][idx]["List"];
                while (luaList[listIdx].value()!=Diluculum::Nil) {
                    keywords.insert ( make_pair ( luaList[listIdx].value().asString(), kwId ) );
                    ++listIdx;
                }
            } else if (ls["Keywords"][idx]["Regex"].value()!=Diluculum::Nil) {
                string reString=StringTools::trim(ls["Keywords"][idx]["Regex"].value().asString());
                int captGroup=-1;
                if (ls["Keywords"][idx]["Group"].value()!=Diluculum::Nil) {
                    captGroup=ls["Keywords"][idx]["Group"].value().asNumber();
                }

                Pattern* p = Pattern::compile ( reString );
                if ( p!=NULL )
                    regex.push_back ( new RegexElement ( KEYWORD, KEYWORD_END, p, kwId, captGroup ) );
                else {
                    failedRegex = reString;
                    return LOAD_FAILED_REGEX;
                }
            }
            idx++;
        }

        if (globals.count("Comments")) {

            int listIdx=1;
            int openDelimId=0;
            int closeDelimId=0;
            while (ls["Comments"][listIdx].value()!=Diluculum::Nil) {
                if (ls["Comments"][listIdx]["Block"].value().asBoolean()) {

                    if (ls["Comments"][listIdx]["Nested"].value()!=Diluculum::Nil)
                        allowNestedComments = ls["Comments"][listIdx]["Nested"].value().asBoolean();

                    string openDelim=StringTools::trim(ls["Comments"][listIdx]["Delimiter"][1].value().asString());
                    Pattern* p4 = Pattern::compile ( openDelim );
                    if ( p4!=NULL ) {
                        RegexElement* elem=new RegexElement ( ML_COMMENT,ML_COMMENT_END, p4, 0, -1 );
                        openDelimId=elem->instanceId;
                        regex.push_back ( elem );
                    }
                    else {
                        failedRegex = openDelim;
                        return LOAD_FAILED_REGEX;
                    }

                    string closeDelim=StringTools::trim(ls["Comments"][listIdx]["Delimiter"][2].value().asString());
                    Pattern* p5 = Pattern::compile (closeDelim  );
                    if ( p5!=NULL ) {
                        RegexElement* elem= new RegexElement ( ML_COMMENT_END,ML_COMMENT_END, p5, 0, -1 );
                        closeDelimId=elem->instanceId;
                        regex.push_back ( elem);
                    }
                    else {
                        failedRegex = closeDelim;
                        return LOAD_FAILED_REGEX;
                    }

                    delimiterDistinct[openDelimId]=openDelim!=closeDelim;
                    delimiterDistinct[closeDelimId]=openDelim!=closeDelim;
                    delimIds2[closeDelimId]=openDelimId;

                } else {

                    Pattern* p6 = Pattern::compile ( StringTools::trim(ls["Comments"][listIdx]["Delimiter"][1].value().asString()) );
                    if ( p6!=NULL )
                        regex.push_back ( new RegexElement ( SL_COMMENT, SL_COMMENT_END, p6, 0, -1 ) );
                    else {
                        failedRegex = ls["Comments"][listIdx]["Delimiter"][1].value().asString();
                        return LOAD_FAILED_REGEX;
                    }
                }
                ++listIdx;
            }

        }

        //move behind comment section because of fortran comments (^cC.*$)
        string re_digit = StringTools::trim(ls["Digits"].value().asString());
        string re_identifier= StringTools::trim(ls["Identifiers"].value().asString());

        // insert identifier and number regex after keyword regexes
        regex.push_back ( new RegexElement ( IDENTIFIER_BEGIN, IDENTIFIER_END,
                                             Pattern::compile ( re_identifier ) ) );
        regex.push_back ( new RegexElement ( NUMBER, NUMBER_END,
                                             Pattern::compile ( re_digit ) ) );

        if (globals.count("Strings")) {

            if (ls["Strings"]["RawPrefix"].value()!=Diluculum::Nil) {
                rawStringPrefix=ls["Strings"]["RawPrefix"].value().asString().at(0);
            }

            if (ls["Strings"]["Delimiter"].value()!=Diluculum::Nil) {
                Pattern* p = Pattern::compile (StringTools::trim( ls["Strings"]["Delimiter"].value().asString()) );
                if ( p!=NULL ) {
                    RegexElement* elem=new RegexElement ( STRING,STRING_END, p, 0, -1 );
                    delimiterDistinct[elem->instanceId]=true;
                    regex.push_back (elem );
                }
                else {
                    failedRegex = ls["Strings"]["Delimiter"].value().asString();
                    return LOAD_FAILED_REGEX;
                }

            }

            if (ls["Strings"]["DelimiterPairs"].value()!=Diluculum::Nil) {

                int listIdx=1;
                int openDelimId=0;
                int closeDelimId=0;
                while (ls["Strings"]["DelimiterPairs"][listIdx].value()!=Diluculum::Nil) {

                    string openDelim=StringTools::trim(ls["Strings"]["DelimiterPairs"][listIdx]["Open"].value().asString());
                    Pattern* p10 = Pattern::compile ( openDelim );
                    if ( p10!=NULL ) {
                        RegexElement* elem =new RegexElement(STRING, STRING_END, p10, 0, -1);
                        openDelimId=elem->instanceId;
                        regex.push_back( elem );
                    }
                    else {
                        failedRegex = openDelim;
                        return LOAD_FAILED_REGEX;
                    }

                    string closeDelim=StringTools::trim(ls["Strings"]["DelimiterPairs"][listIdx]["Close"].value().asString());
                    Pattern* p11 = Pattern::compile ( closeDelim );
                    if ( p11!=NULL ) {
                        RegexElement* elem = new RegexElement(STRING_END, STRING_END, p11, 0, -1);
                        closeDelimId=elem->instanceId;
                        regex.push_back( elem );
                    }
                    else {
                        failedRegex = closeDelim;
                        return LOAD_FAILED_REGEX;
                    }

                    delimIds2[closeDelimId]=openDelimId;

		    if (ls["Strings"]["DelimiterPairs"][listIdx]["Raw"].value()!=Diluculum::Nil){
		      rawStringOpenDelims[openDelimId]=ls["Strings"]["DelimiterPairs"][listIdx]["Raw"].value().asBoolean();
		    }

                    ++listIdx;
                }

            }

            string  escRegex=(ls["Strings"]["Escape"].value()==Diluculum::Nil)?REGEX_ESCSEQ:ls["Strings"]["Escape"].value().asString();
            Pattern* p2 = Pattern::compile (StringTools::trim(escRegex) );
            if ( p2!=NULL )
                regex.push_back ( new RegexElement ( ESC_CHAR,ESC_CHAR_END, p2, 0, -1 ) );
            else {
                failedRegex = ls["Strings"]["Escape"].value().asString();
                return LOAD_FAILED_REGEX;
            }
        }

        if (globals.count("PreProcessor")) {
            Pattern* p = Pattern::compile ( StringTools::trim(ls["PreProcessor"]["Prefix"].value().asString()) );
            if ( p!=NULL )
                regex.push_back ( new RegexElement ( DIRECTIVE,DIRECTIVE_END, p, 0, -1 ) );
            else {
                failedRegex = ls["PreProcessor"]["Prefix"].value().asString();
                return LOAD_FAILED_REGEX;
            }
            if (ls["PreProcessor"]["Continuation"].value()!=Diluculum::Nil) {
                continuationChar=ls["PreProcessor"]["Continuation"].value().asString().at(0);
            }
        }

        if (globals.count("Operators")) {
            Pattern* p = Pattern::compile (StringTools::trim( ls["Operators"].value().asString()) );
            if ( p!=NULL )
                regex.push_back ( new RegexElement ( SYMBOL,SYMBOL_END, p, 0, -1 ) );
            else {
                failedRegex = ls["Operators"].value().asString();
                return LOAD_FAILED_REGEX;
            }
        }

        if (globals.count("NestedSections")) {

            int listIdx=1;
            while (ls["NestedSections"][listIdx].value()!=Diluculum::Nil) {

                string lang= ls["NestedSections"][listIdx]["Lang"].value().asString();

                string openDelim=StringTools::trim(ls["NestedSections"][listIdx]["Delimiter"][1].value().asString());
                Pattern* p8 = Pattern::compile ( openDelim );
                if ( p8!=NULL )
                    //regex.push_back ( new RegexElement ( ML_COMMENT,ML_COMMENT_END, p4, 0, -1 ) );
                    regex.insert(regex.begin(), 1, new RegexElement(EMBEDDED_CODE_BEGIN, EMBEDDED_CODE_BEGIN, p8, 0, -1, lang));
                else {
                    failedRegex = openDelim;
                    return LOAD_FAILED_REGEX;
                }

                string closeDelim=StringTools::trim(ls["NestedSections"][listIdx]["Delimiter"][2].value().asString());
                exitDelimiters[getNewPath(lang)] = closeDelim;

                ++listIdx;
            }

        }

        // load hook functions
        if (globals.count("OnStateChange")) {
            validateStateChangeFct=new Diluculum::LuaFunction(ls["OnStateChange"].value().asFunction());
        }

    } catch (Diluculum::LuaError err) {
        luaError = string(err.what());
        return LOAD_FAILED_LUA;
    }
    return LOAD_OK;
}

void SyntaxReader::reset()
{
    keywords.clear();
    keywordClasses.clear();
    delimiterDistinct.clear();
    langDesc.clear();
    ignoreCase= false;
    allowNestedComments= reformatCode = false;
    rawStringPrefix = continuationChar = '\0';
    disableHighlighting=false;

    // TODO eigene methode
    for ( vector<RegexElement*>::iterator it=regex.begin(); it!=regex.end();it++ )
    {
        delete *it;
    }
    if (validateStateChangeFct) delete validateStateChangeFct; validateStateChangeFct=NULL;

    if (luaState) delete luaState; luaState=NULL;

    regex.clear();
    failedRegex.clear();
}

string SyntaxReader::getNewPath(const string& lang) {
    string::size_type Pos = currentPath.find_last_of ( Platform::pathSeparator );
    return currentPath.substr ( 0, Pos+1 ) + lang + ".lang";
}


int SyntaxReader::getOpenDelimiterID ( const string& token, State s) {
    for (unsigned int i=0; i<getRegexElements().size(); i++ )  {
        RegexElement *regexElem = getRegexElements() [i];
        if (regexElem->open==s ) {
            auto_ptr<Matcher> matcher (regexElem->rePattern->createMatcher ( token ));
            if (matcher->matches()) {

                return regexElem->instanceId;
            }
        }
    }
    return 0;
}

bool SyntaxReader::matchesOpenDelimiter ( const string& token, State s, int openDelimId) {

    for (unsigned int i=0; i<getRegexElements().size(); i++ )  {
        RegexElement *regexElem = getRegexElements() [i];
        if (regexElem->open==s ) {
            auto_ptr<Matcher> matcher (regexElem->rePattern->createMatcher ( token ));
            if (matcher->matches()&&delimIds2[regexElem->instanceId]==openDelimId) {
                return true;
            }
        }
    }
    return false;
}

}
