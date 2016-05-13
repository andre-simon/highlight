/***************************************************************************
                          syntaxreader.cpp  -  description
                             -------------------
    begin                : Wed Nov 28 2001
    copyright            : (C) 2001-2015 by Andre Simon
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

namespace highlight
{

const string SyntaxReader::REGEX_IDENTIFIER =
    "[a-zA-Z_]\\w*";

const string SyntaxReader::REGEX_NUMBER =
    "(?:0x|0X)[0-9a-fA-F]+|\\d*[\\.]?\\d+(?:[eE][\\-\\+]\\d+)?[lLuU]*";

const string SyntaxReader::REGEX_ESCSEQ =
    "\\\\u[[:xdigit:]]{4}|\\\\\\d{3}|\\\\x[[:xdigit:]]{2}|\\\\[ntvbrfa\\\\\\?'\"]";

EmbedLangDelimMap SyntaxReader::exitDelimiters;
vector<Diluculum::LuaFunction*> SyntaxReader::pluginChunks;

int RegexElement::instanceCnt=0;


SyntaxReader::SyntaxReader() :
    ignoreCase ( false ),
    disableHighlighting ( false ),
    allowNestedComments ( true ),
    reformatCode ( false ),
    assertEqualLength(false),
    rawStringPrefix(0),
    continuationChar(0),
    validateStateChangeFct(NULL),
    decorateFct(NULL),
    decorateLineBeginFct(NULL),
    decorateLineEndFct(NULL),
    luaState(NULL)
{

}

SyntaxReader::~SyntaxReader()
{
    for ( vector<RegexElement*>::iterator it=regex.begin(); it!=regex.end(); it++ ) {
        delete *it;
    }
    if (validateStateChangeFct) delete validateStateChangeFct;
    if (decorateFct) delete decorateFct;
    if (decorateLineBeginFct) delete decorateLineBeginFct;
    if (decorateLineEndFct) delete decorateLineEndFct;
    if (luaState) delete luaState;
    for (unsigned int i=0; i<pluginChunks.size(); i++) {
        delete pluginChunks[i];
    }
    pluginChunks.clear();
}

void  SyntaxReader::initLuaState(Diluculum::LuaState& ls, const string& langDefPath, const string& pluginParameter, OutputType type )
{
    // initialize Lua state with variables which can be used within scripts
    string::size_type Pos = langDefPath.find_last_of ( Platform::pathSeparator );
    ls["HL_LANG_DIR"] =langDefPath.substr ( 0, Pos+1 );

    ls["HL_INPUT_FILE"] = ls["HL_PLUGIN_PARAM"] = pluginParameter;
    ls["HL_OUTPUT"] = type;

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
    ls["HL_INTERPOLATION"]=STRING_INTERPOLATION;
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
    ls["HL_INTERPOLATION_END"]=STRING_INTERPOLATION_END;
    ls["HL_UNKNOWN"]=_UNKNOWN;
    ls["HL_REJECT"]=_REJECT;
    ls["HL_FORMAT_HTML"]=HTML;
    ls["HL_FORMAT_XHTML"]=XHTML;
    ls["HL_FORMAT_TEX"]=TEX;
    ls["HL_FORMAT_LATEX"]=LATEX;
    ls["HL_FORMAT_RTF"]=RTF;
    ls["HL_FORMAT_ANSI"]=ANSI;
    ls["HL_FORMAT_XTERM256"]=XTERM256;
    ls["HL_FORMAT_HTML32"]=HTML32;
    ls["HL_FORMAT_SVG"]=SVG;
    ls["HL_FORMAT_BBCODE"]=BBCODE;
    ls["HL_FORMAT_PANGO"]=PANGO;
    ls["HL_FORMAT_ODT"]=ODTFLAT;
}

LoadResult SyntaxReader::load ( const string& langDefPath, const string& pluginReadFilePath, OutputType outputType, bool clear )
{
  
    currentPath=langDefPath;
    disableHighlighting=false;

    if (!Platform::fileExists(langDefPath)) {
        return LOAD_FAILED;
    }

    try {

        if (luaState) delete luaState;
        luaState=new Diluculum::LuaState();

        Diluculum::LuaState& ls=*luaState;
        initLuaState(ls, langDefPath, pluginReadFilePath, outputType);

        lua_register (ls.getState(),"AddKeyword",luaAddKeyword);

        SyntaxReader **s = (SyntaxReader **)lua_newuserdata(ls.getState(), sizeof(SyntaxReader *));
        *s=this;
        lua_setglobal(ls.getState(), GLOBAL_INSTANCE_NAME);

        // ececute script and read values
        ls.doFile (langDefPath);

        langDesc = ls["Description"].value().asString();

        if (pluginChunks.size()) {
            Diluculum::LuaValueList params;
            params.push_back(langDesc);
            for (unsigned int i=0; i<pluginChunks.size(); i++) {
                ls.call(*pluginChunks[i], params, "syntax user function");
            }
        }

        Diluculum::LuaValueMap globals = ls.globals();

        ignoreCase=readFlag(ls["IgnoreCase"]);
        reformatCode=readFlag(ls["EnableIndentation"]);
        disableHighlighting=readFlag(ls["DisableHighlighting"]);

        int idx=1;
        int kwId=0;
        while (ls["Keywords"][idx].value() !=Diluculum::Nil) {
            kwId= generateNewKWClass ( ls["Keywords"][idx]["Id"].value().asNumber() );

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
                regex.push_back ( new RegexElement ( KEYWORD, KEYWORD_END, reString, kwId, captGroup ) );
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
                    RegexElement* elem=new RegexElement ( ML_COMMENT,ML_COMMENT_END, openDelim, 0, -1 );
                    openDelimId=elem->instanceId;
                    regex.push_back ( elem );

                    string closeDelim=StringTools::trim(ls["Comments"][listIdx]["Delimiter"][2].value().asString());

                    elem= new RegexElement ( ML_COMMENT_END,ML_COMMENT_END, closeDelim, 0, -1 );
                    closeDelimId=elem->instanceId;
                    regex.push_back ( elem);

                    delimiterDistinct[openDelimId]=openDelim!=closeDelim;
                    delimiterDistinct[closeDelimId]=openDelim!=closeDelim;
                    delimIds2[closeDelimId]=openDelimId;

                } else {
                    regex.push_back ( new RegexElement ( SL_COMMENT, SL_COMMENT_END, 
                                                         StringTools::trim(ls["Comments"][listIdx]["Delimiter"][1].value().asString()), 0, -1 ) );
                }
                ++listIdx;
            }
        }

        //move behind comment section because of fortran comments (^cC.*$)
        string re_digit = StringTools::trim(ls["Digits"].value().asString());
        string re_identifier= StringTools::trim(ls["Identifiers"].value().asString());

        // insert identifier and number regex after keyword regexes
        regex.push_back ( new RegexElement ( IDENTIFIER_BEGIN, IDENTIFIER_END,
                                             re_identifier  ) );
        regex.push_back ( new RegexElement ( NUMBER, NUMBER_END,
                                             re_digit  ) );

        if (globals.count("Strings")) {

            if (ls["Strings"]["RawPrefix"].value()!=Diluculum::Nil) {
                rawStringPrefix=ls["Strings"]["RawPrefix"].value().asString().at(0);
            }

            if (ls["Strings"]["Delimiter"].value()!=Diluculum::Nil) {

                RegexElement* elem=new RegexElement ( STRING,STRING_END, StringTools::trim( ls["Strings"]["Delimiter"].value().asString()), 0, -1 );
                delimiterDistinct[elem->instanceId]=true;
                regex.push_back (elem );
            }
            if (ls["Strings"]["Interpolation"].value()!=Diluculum::Nil) {
                RegexElement* elem=new RegexElement ( STRING_INTERPOLATION, STRING_INTERPOLATION_END, 
                                                      StringTools::trim( ls["Strings"]["Interpolation"].value().asString()), 0, -1 );
                regex.push_back (elem );
            }

            if (ls["Strings"]["DelimiterPairs"].value()!=Diluculum::Nil) {

                int listIdx=1;
                int openDelimId=0;
                int closeDelimId=0;
                while (ls["Strings"]["DelimiterPairs"][listIdx].value()!=Diluculum::Nil) {

                    string openDelim=StringTools::trim(ls["Strings"]["DelimiterPairs"][listIdx]["Open"].value().asString());

                    RegexElement* elem =new RegexElement(STRING, STRING_END, openDelim, 0, -1);
                    openDelimId=elem->instanceId;
                    regex.push_back( elem );

                    string closeDelim=StringTools::trim(ls["Strings"]["DelimiterPairs"][listIdx]["Close"].value().asString());

                    elem = new RegexElement(STRING_END, STRING_END, closeDelim, 0, -1);
                    closeDelimId=elem->instanceId;
                    regex.push_back( elem );

                    delimIds2[closeDelimId]=openDelimId;

                    if (ls["Strings"]["DelimiterPairs"][listIdx]["Raw"].value()!=Diluculum::Nil) {
                        rawStringOpenDelims[openDelimId]=ls["Strings"]["DelimiterPairs"][listIdx]["Raw"].value().asBoolean();
                    }
                    ++listIdx;
                }
            }
            
            assertEqualLength=readFlag(ls["Strings"]["AssertEqualLength"]);

            string escRegex=(ls["Strings"]["Escape"].value()==Diluculum::Nil)?REGEX_ESCSEQ:ls["Strings"]["Escape"].value().asString();
            regex.push_back ( new RegexElement ( ESC_CHAR,ESC_CHAR_END, StringTools::trim(escRegex), 0, -1 ) );
        }

        if (globals.count("PreProcessor")) {

            regex.push_back ( new RegexElement ( DIRECTIVE,DIRECTIVE_END, StringTools::trim(ls["PreProcessor"]["Prefix"].value().asString()), 0, -1 ) );

            if (ls["PreProcessor"]["Continuation"].value()!=Diluculum::Nil) {
                continuationChar=ls["PreProcessor"]["Continuation"].value().asString().at(0);
            }
        }

        if (globals.count("Operators")) {
            regex.push_back ( new RegexElement ( SYMBOL,SYMBOL_END, StringTools::trim( ls["Operators"].value().asString()), 0, -1 ) );
        }

        if (globals.count("NestedSections")) {

            int listIdx=1;
            while (ls["NestedSections"][listIdx].value()!=Diluculum::Nil) {

                string lang= ls["NestedSections"][listIdx]["Lang"].value().asString();
                string openDelim=StringTools::trim(ls["NestedSections"][listIdx]["Delimiter"][1].value().asString());
                regex.insert(regex.begin(), 1, new RegexElement(EMBEDDED_CODE_BEGIN, EMBEDDED_CODE_BEGIN, openDelim, 0, -1, lang));

                string closeDelim=StringTools::trim(ls["NestedSections"][listIdx]["Delimiter"][2].value().asString());
                exitDelimiters[getNewPath(lang)] = closeDelim;

                ++listIdx;
            }
        }

        if (globals.count("HeaderInjection")) {
            headerInjection+= ls["HeaderInjection"].value().asString();
        }

        if (globals.count("FooterInjection")) {
            footerInjection+= ls["FooterInjection"].value().asString();
        }

        // load hook functions
        if (globals.count("OnStateChange")) {
            validateStateChangeFct=new Diluculum::LuaFunction(ls["OnStateChange"].value().asFunction());
        }
        if (globals.count("Decorate")) {
            decorateFct=new Diluculum::LuaFunction(ls["Decorate"].value().asFunction());
        }
        if (globals.count("DecorateLineBegin")) {
            decorateLineBeginFct=new Diluculum::LuaFunction(ls["DecorateLineBegin"].value().asFunction());
        }
        if (globals.count("DecorateLineEnd")) {
            decorateLineEndFct=new Diluculum::LuaFunction(ls["DecorateLineEnd"].value().asFunction());
        }
        
    } catch (Diluculum::LuaError err) {
        luaErrorMsg = string(err.what());
        return LOAD_FAILED_LUA;
    }
    return LOAD_OK;
}


void SyntaxReader::addKeyword(unsigned int groupID, const string& kw)
{
    if (!isKeyword ( kw )) {
        keywords.insert ( make_pair (kw, groupID ) );
    }
}

int SyntaxReader::isKeyword ( const string &s )
{
    return ( s.length() && keywords.count ( s ) ) ? keywords[s] : 0;
}

int SyntaxReader::luaAddKeyword (lua_State *L)
{
    int retVal=0;
    if (lua_gettop(L)==2) {
        const char*keyword=lua_tostring(L, 1);
        unsigned int kwgroupID=lua_tonumber(L, 2);
        lua_getglobal(L, GLOBAL_INSTANCE_NAME);
        SyntaxReader **a=reinterpret_cast<SyntaxReader **>(lua_touserdata(L, 3));
        if (*a) {
            (*a)->addKeyword(kwgroupID, keyword);
            retVal=1;
        }
    }
    lua_pushboolean(L, retVal);
    return 1;
}


void SyntaxReader::restoreLangEndDelim(const string& langPath)
{
    //TODO exitDelimiters be left static?
    if ( !langPath.empty()&& exitDelimiters.count(langPath) ) {
        regex.insert (regex.begin(),1, new RegexElement ( EMBEDDED_CODE_END,EMBEDDED_CODE_END, exitDelimiters[langPath] ) );
    }
}

unsigned int SyntaxReader::generateNewKWClass ( int classID )
{
    char className[5]= {0};
    snprintf(className, sizeof(className), "kw%c", ('a'+classID-1)); 
    
    unsigned int newClassID=0;
    bool found=false;
    while (!keywordClasses.empty() && newClassID<keywordClasses.size() && !found ) {
      found = ( className==keywordClasses.at(newClassID++) );
    }
    if ( !found ) {
        newClassID++;
        keywordClasses.push_back ( className );
    }

    return newClassID;
}


bool SyntaxReader::readFlag(const Diluculum::LuaVariable& var)
{
    if (var.value()==Diluculum::Nil) return false;
    return var.value().asBoolean();
}


string SyntaxReader::getNewPath(const string& lang)
{
    string::size_type Pos = currentPath.find_last_of ( Platform::pathSeparator );
    return currentPath.substr ( 0, Pos+1 ) + lang + ".lang";
}


int SyntaxReader::getOpenDelimiterID ( const string& token, State s)
{
    boost::xpressive::smatch what;
    for (unsigned int i=0; i<getRegexElements().size(); i++ )  {
        RegexElement *regexElem = getRegexElements() [i];
        if (regexElem->open==s ) {

            if( regex_match( token, what, regexElem->rex ) ) {
                return regexElem->instanceId;
            }
        }
    }
    return 0;
}

bool SyntaxReader::matchesOpenDelimiter ( const string& token, State s, int openDelimId)
{
    boost::xpressive::smatch what;
    for (unsigned int i=0; i<getRegexElements().size(); i++ )  {
        RegexElement *regexElem = getRegexElements() [i];
        if (regexElem->open==s ) {

            if( regex_match( token, what, regexElem->rex ) && delimIds2[regexElem->instanceId]==openDelimId) {
                return true;
            }

        }
    }
    return false;
}

}
