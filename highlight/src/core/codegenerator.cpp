/***************************************************************************
                          codegenerator.cpp  -  description
                             -------------------
    begin                : Die Jul 9 2002
    copyright            : (C) 2002-2015 by Andre Simon
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


#include <climits>
#include <memory>
#include <boost/xpressive/xpressive_dynamic.hpp>

#include "codegenerator.h"

#include "htmlgenerator.h"
#include "xhtmlgenerator.h"
#include "rtfgenerator.h"
#include "latexgenerator.h"
#include "texgenerator.h"
#include "svggenerator.h"
#include "bbcodegenerator.h"
#include "pangogenerator.h"
#include "odtgenerator.h"
#include "astyle/astyle.h"
#include "astyle/ASStreamIterator.h"

#if !defined (QT)
#include "ansigenerator.h"
#include "xterm256generator.h"
#endif

namespace highlight
{
const unsigned int CodeGenerator::NUMBER_BUILTIN_STATES = highlight::KEYWORD;

const string CodeGenerator::STY_NAME_STD="std";
const string CodeGenerator::STY_NAME_STR="str";
const string CodeGenerator::STY_NAME_NUM="num";
const string CodeGenerator::STY_NAME_SLC="slc";
const string CodeGenerator::STY_NAME_COM="com";
const string CodeGenerator::STY_NAME_ESC="esc";
const string CodeGenerator::STY_NAME_DIR="ppc"; //preprocessor
const string CodeGenerator::STY_NAME_DST="pps"; //preprocessor string
const string CodeGenerator::STY_NAME_LIN="lin";
const string CodeGenerator::STY_NAME_SYM="opt"; //operator
const string CodeGenerator::STY_NAME_IPL="ipl"; //interpolation

CodeGenerator * CodeGenerator::getInstance ( OutputType type )
{
    CodeGenerator* generator=NULL;
    switch ( type ) {
    case HTML:
        generator = new HtmlGenerator();
        break;
    case XHTML:
        generator = new XHtmlGenerator();
        break;
    case TEX:
        generator = new TexGenerator ();
        break;
    case LATEX:
        generator = new LatexGenerator();
        break;
    case RTF:
        generator = new RtfGenerator ();
        break;
    case SVG:
        generator = new SVGGenerator();
        break;
    case BBCODE:
        generator = new BBCodeGenerator();
        break;
    case PANGO:
        generator = new PangoGenerator();
        break;
    case ODTFLAT:
        generator = new ODTGenerator();
        break;
#if !defined (QT)
    case ANSI:
        generator = new AnsiGenerator();
        break;
    case XTERM256:
        generator = new Xterm256Generator();
        break;
#endif
    default:
        break;
    }
    return generator;
}


CodeGenerator::CodeGenerator ( highlight::OutputType type )
    :currentSyntax(NULL),
     in ( NULL ),
     out ( NULL ),
     encoding ( "none" ),
     docTitle ( "Source file" ),
     maskWs ( false ),
     excludeWs ( false ),
     fragmentOutput ( false ),
     keepInjections( false ),
     showLineNumbers ( false ),
     lineNumberFillZeroes ( false ),
     printNewLines(true),
     baseFontSize("10"),
     lineNumber ( 0 ),
     lineNumberOffset ( 0 ),
     includeStyleDef ( false ),
     lineIndex ( 0 ),
     lineNumberWidth ( 5 ),
     maxLineCnt ( UINT_MAX ),
     terminatingChar ( '\0' ),
     formatter ( NULL ),
     formattingEnabled ( false ),
     formattingPossible ( false ),
     validateInput ( false ),
     numberWrappedLines ( true ),	//until now, wrapped lines were always numbered, so this remains the default.
     noTrailingNewLine(false),
     resultOfHook(false),
     keywordCase ( StringTools::CASE_UNCHANGED ),
     eolDelimiter ('\n'),
     outputType ( type )
{
}


CodeGenerator::~CodeGenerator()
{
    delete formatter;

    for ( map<string, SyntaxReader*>::iterator it=syntaxReaders.begin(); it!=syntaxReaders.end(); it++ ) {
        delete it->second;
    }
}


bool CodeGenerator::initTheme ( const string& themePath )
{
    this->themePath=themePath;
    bool loadOK = docStyle.load ( themePath, outputType );
    initOutputTags();
    return loadOK;
}

const string& CodeGenerator::getStyleName()
{
    return themePath;
}

void CodeGenerator::setLineNumberWidth ( int w )
{
    lineNumberWidth=w;
}

int CodeGenerator::getLineNumberWidth()
{
    return lineNumberWidth;
}

void CodeGenerator::setPrintLineNumbers ( bool flag, unsigned int startCnt )
{
    showLineNumbers=flag;
    lineNumberOffset = startCnt-1;
}

bool CodeGenerator::getPrintLineNumbers()
{
    return showLineNumbers;
}

void CodeGenerator::setPrintZeroes ( bool flag )
{
    lineNumberFillZeroes=flag;
}

bool CodeGenerator::getPrintZeroes()
{
    return lineNumberFillZeroes;
}

void CodeGenerator::setIncludeStyle ( bool flag )
{
    includeStyleDef = flag;
}

void CodeGenerator::disableTrailingNL ( bool flag )
{
    noTrailingNewLine = flag;
}

void CodeGenerator::setStyleInputPath ( const string& path )
{
    styleInputPath = path;
}

void CodeGenerator::setStyleOutputPath ( const string& path )
{
    styleOutputPath = path;
}

void CodeGenerator::setPluginReadFile ( const string& path )
{
    pluginReadFile = path;
}

const string&  CodeGenerator::getStyleInputPath()
{
    return styleInputPath;
}

const string&  CodeGenerator::getStyleOutputPath()
{
    return styleOutputPath;
}

void CodeGenerator::setFragmentCode ( bool flag )
{
    fragmentOutput=flag;
}

bool CodeGenerator::getFragmentCode()
{
    return fragmentOutput;
}
void CodeGenerator::setKeepInjections ( bool flag )
{
  keepInjections=flag;
}

bool CodeGenerator::getKeepInjections()
{
  return keepInjections;
}
void CodeGenerator::setValidateInput ( bool flag )
{
    validateInput=flag;
}

bool CodeGenerator::getValidateInput()
{
    return validateInput;
}


void CodeGenerator::setNumberWrappedLines ( bool flag )
{
    numberWrappedLines=flag;
}

bool CodeGenerator::getNumberWrappedLines()
{
    return numberWrappedLines;
}

void CodeGenerator::setBaseFont ( const string& fontName )
{
    baseFont = fontName;
}

void CodeGenerator::setBaseFontSize ( const string& fontSize)
{
    baseFontSize = fontSize;
}

void CodeGenerator::setStartingNestedLang(const string &langName)
{
    embedLangStart = langName;
}

const string CodeGenerator::getBaseFont() const
{
    if ( !baseFont.empty() ) return baseFont;
    switch ( outputType ) {
    case HTML:
    case XHTML:
    case SVG:
        return "'Courier New',monospace";
        break;
    case LATEX:
        return "ttfamily";
        break;
    case TEX:
        return "tt";
        break;
    default:
        return "Courier New";
    }
}

const string CodeGenerator::getBaseFontSize()
{
    return baseFontSize;
}

void CodeGenerator::setTitle ( const string & title )
{
    if ( !title.empty() ) docTitle= title;
}

string CodeGenerator::getTitle()
{
    return docTitle;
}

void CodeGenerator::setEncoding ( const string& encodingName )
{
    encoding = encodingName;
}

bool CodeGenerator::formattingDisabled()
{
    return !formattingEnabled;
}

void CodeGenerator::setMaxInputLineCnt ( unsigned int cnt )
{
    maxLineCnt = cnt;
}

bool CodeGenerator::formattingIsPossible()
{
    return formattingPossible;
}

void CodeGenerator::setPreformatting ( WrapMode lineWrappingStyle,
                                       unsigned int lineLength,
                                       int numberSpaces )
{
    bool enableWrap = lineWrappingStyle!=WRAP_DISABLED;
    bool replaceTabs = numberSpaces > 0;

    if ( enableWrap || replaceTabs ) {
        preFormatter.setWrap ( enableWrap );
        preFormatter.setWrapIndentBraces ( lineWrappingStyle==WRAP_DEFAULT );
        preFormatter.setWrapLineLength ( lineLength );
        preFormatter.setReplaceTabs ( replaceTabs );
        preFormatter.setNumberSpaces ( numberSpaces );
    }
}

void CodeGenerator::setKeyWordCase ( StringTools::KeywordCase keyCase )
{
    keywordCase = keyCase;
}

void CodeGenerator::setEOLDelimiter(char delim)
{
    eolDelimiter = delim;
}

void CodeGenerator::reset()
{
    lineIndex = 0;
    lineNumber = 0;
    line.clear();
    preFormatter.reset();
    inFile.clear();
    outFile.clear();
    hostLangDefPath.clear();
    embedLangDefPath.clear();
    printNewLines=true;
}

string CodeGenerator::getThemeInitError()
{
    return  docStyle.getErrorMessage();
}

string CodeGenerator::getPluginScriptError()
{
    return userScriptError;
}

string CodeGenerator::getSyntaxRegexError()
{
    return (currentSyntax)? currentSyntax->getFailedRegex(): "syntax undef";
}
string CodeGenerator::getSyntaxLuaError()
{
    return (currentSyntax)? currentSyntax->getLuaErrorText(): "syntax undef";

}
string CodeGenerator::getSyntaxDescription()
{
    return (currentSyntax)? currentSyntax->getDescription(): "syntax undef";

}

unsigned int CodeGenerator::getLineNumber()
{
    return lineNumber;
}

bool CodeGenerator::readNewLine ( string &newLine )
{
    bool eof;
    if ( lineIndex ) terminatingChar=newLine[lineIndex-1];
    if ( formattingPossible && formattingEnabled ) {
        eof=!formatter->hasMoreLines();
        if ( !eof ) {
            newLine = formatter->nextLine();
        }
    } else {
        eof = ! getline ( *in, newLine, eolDelimiter );
    }

#ifndef _WIN32
    // drop CR of CRLF files
    if (!newLine.empty() && newLine[newLine.size() - 1] == '\r')
        newLine.erase(newLine.size() - 1);
#endif

    return eof || ( lineNumber == maxLineCnt );
}

void CodeGenerator::matchRegex ( const string &line )
{
    regexGroups.clear();
    int matchBegin=0;
    int groupID=0;

    // cycle through all regex, save the start and ending indices of matches to report them later
    for ( unsigned int i=0; i<currentSyntax->getRegexElements().size(); i++ ) {
        RegexElement *regexElem = currentSyntax->getRegexElements() [i];

        boost::xpressive::sregex_iterator cur( line.begin(), line.end(), regexElem->rex );
        boost::xpressive::sregex_iterator end;

        for( ; cur != end; ++cur )  {
            groupID = ( regexElem->capturingGroup<0 ) ? cur->size()-1 : regexElem->capturingGroup;
            matchBegin =  cur->position(groupID);
            regexGroups.insert (
                make_pair ( matchBegin+1, ReGroup ( regexElem->open, cur->length(groupID), regexElem->kwClass, regexElem->langName ) ) );
        }
    }
}

unsigned char CodeGenerator::getInputChar()
{
    bool eol = lineIndex == line.length();

    if ( eol ) {
        bool eof=false;
        if ( preFormatter.isEnabled() ) {
            if ( !preFormatter.hasMoreLines() ) {
                eof=readNewLine ( line );
                preFormatter.setLine ( line );
                ++lineNumber;
                numberCurrentLine = true;
            } else {
                if(numberWrappedLines)
                    ++lineNumber;
                numberCurrentLine = numberWrappedLines;
            }

            line = preFormatter.getNextLine();
        } else {
            eof=readNewLine ( line );
            ++lineNumber;

            numberCurrentLine = true;
        }
        lineIndex=0;
        matchRegex ( line );

        return ( eof ) ?'\0':'\n';
    }

    return line[lineIndex++];
}

State CodeGenerator::getCurrentState (State oldState)
{
    unsigned char c='\0';

    if ( token.length() ==0 ) {
        c=getInputChar();
    } else {
        lineIndex-= ( token.length()-1 );
        c=token[0];
    }
    if ( c=='\n' ) {
        return _EOL;   // End of line
    }

    if ( c=='\0' ) {
        return _EOF;   // End of file
    }

    if ( c==' ' || c=='\t' ) {
        token= c;
        return _WS;
    }

    // Test if a regular expression was found at the current position
    if ( !regexGroups.empty() ) {
        if ( regexGroups.count ( lineIndex ) ) {
            token = line.substr ( lineIndex-1, regexGroups[lineIndex].length );
    
            unsigned int oldIndex= lineIndex;
            if ( regexGroups[oldIndex].length>1 ) lineIndex+= regexGroups[oldIndex].length-1;


            if ( regexGroups[oldIndex].state==EMBEDDED_CODE_BEGIN) {
                embedLangDefPath = currentSyntax->getNewPath(regexGroups[oldIndex].name);
            }

            if ( regexGroups[oldIndex].state==IDENTIFIER_BEGIN || regexGroups[oldIndex].state==KEYWORD ) {
                string reservedWord= ( currentSyntax->isIgnoreCase() ) ? StringTools::change_case ( token ) :token;
                currentKeywordClass=currentSyntax->isKeyword ( reservedWord );
                if ( !currentKeywordClass && regexGroups[oldIndex].state==KEYWORD )
                    currentKeywordClass = regexGroups[oldIndex].kwClass;
                return validateState(( currentKeywordClass ) ? KEYWORD : STANDARD, oldState, currentKeywordClass);
            } else {
                return validateState(regexGroups[oldIndex].state, oldState, 0);
            }
        }
    }

    // Character not referring to any state
    token = c;
    return STANDARD;
}


State CodeGenerator::validateState(State newState, State oldState, unsigned int kwClass)
{

    if (currentSyntax->getValidateStateChangeFct()) {
        Diluculum::LuaValueList params;
        params.push_back(Diluculum::LuaValue(oldState));
        params.push_back(Diluculum::LuaValue(newState));
        params.push_back(Diluculum::LuaValue(token));
        params.push_back(Diluculum::LuaValue(kwClass));

        Diluculum::LuaValueList res=
            currentSyntax->getLuaState()->call ( *currentSyntax->getValidateStateChangeFct(),
                    params,"getValidateStateChangeFct call")  ;

        resultOfHook = res.size()>=1;
        if (resultOfHook) {
            State validatedState = (State)res[0].asNumber();
            if ( validatedState== _REJECT){
                // proceed using only the first character of the token
                lineIndex -= (token.length() -1);
                token=token.substr(0, 1); 
                return oldState;
            }
            return validatedState;
        }
    }
    resultOfHook  = false;
    return newState;
}

//it is faster to pass ostream reference
void CodeGenerator::maskString ( ostream& ss, const string & s )
{
    for ( unsigned int i=0; i< s.length(); i++ ) {
        ss << maskCharacter ( s[i] );
    }
}


Diluculum::LuaValueList CodeGenerator::callDecorateFct(const string&token){
    Diluculum::LuaValueList params;
    params.push_back(Diluculum::LuaValue(token));
    params.push_back(Diluculum::LuaValue(currentState));
    params.push_back(Diluculum::LuaValue(currentKeywordClass));

    return currentSyntax->getLuaState()->call ( *currentSyntax->getDecorateFct(),
                                               params,"getDecorateFct call")  ;
}

void CodeGenerator::printMaskedToken (bool flushWhiteSpace, StringTools::KeywordCase tcase )
{
    if ( flushWhiteSpace )
        flushWs();
    string caseToken = StringTools::change_case ( token, tcase );
    if (currentSyntax->getDecorateFct()) {
        
      Diluculum::LuaValueList res=callDecorateFct(caseToken);    
        if (res.size()==1) {
            *out<<res[0].asString();
        } else {
            maskString ( *out, caseToken );
        }
    } else {
      maskString ( *out, caseToken );
    }

    token.clear();
}

bool CodeGenerator::styleFound()
{
    return docStyle.found();
}

bool CodeGenerator::printIndexFile ( const vector<string> &fileList,
                                     const string &outPath )
{
    return true;
}

bool CodeGenerator::initIndentationScheme ( const string &indentScheme )
{

    if ( formatter!=NULL ) {
        return true;
    }

    if ( !indentScheme.size() ) return false;

    formatter=new astyle::ASFormatter();

    formatter->setParensHeaderPaddingMode(true);

    if ( indentScheme=="allman" || indentScheme=="bsd" || indentScheme=="ansi" ) {
        formatter->setFormattingStyle ( astyle::STYLE_ALLMAN );
    } else if ( indentScheme=="kr"||indentScheme=="k&r"||indentScheme=="k/r" ) {
        formatter->setFormattingStyle ( astyle::STYLE_KR );
    } else if ( indentScheme=="java" ) {
        formatter->setFormattingStyle ( astyle::STYLE_JAVA );
    } else if ( indentScheme=="stroustrup" ) {
        formatter->setFormattingStyle ( astyle::STYLE_STROUSTRUP );
    } else if ( indentScheme=="whitesmith" ) {
        formatter->setFormattingStyle ( astyle::STYLE_WHITESMITH );
    } else if ( indentScheme=="banner" ) {
        formatter->setFormattingStyle ( astyle::STYLE_BANNER );
    } else if ( indentScheme=="gnu" ) {
        formatter->setFormattingStyle ( astyle::STYLE_GNU );
    } else if ( indentScheme=="linux" ) {
        formatter->setFormattingStyle ( astyle::STYLE_LINUX );
    } else if ( indentScheme=="horstmann" ) {
        formatter->setFormattingStyle ( astyle::STYLE_HORSTMANN );
    } else if ( indentScheme=="otbs" ||  indentScheme=="1tbs") {
        formatter->setFormattingStyle ( astyle::STYLE_1TBS );
    } else if ( indentScheme=="google") {
        formatter->setFormattingStyle ( astyle::STYLE_GOOGLE );
    } else if ( indentScheme=="pico" ||  indentScheme=="a11") {
        formatter->setFormattingStyle ( astyle::STYLE_PICO );
    } else if ( indentScheme=="lisp" ||  indentScheme=="python"||  indentScheme=="a12") {
        formatter->setFormattingStyle ( astyle::STYLE_LISP );
    } else if ( indentScheme=="vtk") {
        formatter->setFormattingStyle ( astyle::STYLE_VTK );
    } else {
        return false;
    }
    return formattingEnabled=true;
}

LoadResult CodeGenerator::loadLanguage ( const string& langDefPath )
{

    bool reloadNecessary= currentSyntax ? currentSyntax->needsReload ( langDefPath ): true;
    LoadResult result=LOAD_OK;

    if ( reloadNecessary ) {
        if (syntaxReaders.count(langDefPath)) {
            currentSyntax=syntaxReaders[langDefPath];
            result=LOAD_OK;
        } else {
            currentSyntax=new SyntaxReader();
            result=currentSyntax->load(langDefPath, pluginReadFile, outputType);
            syntaxReaders[langDefPath]=currentSyntax;
        }

        if ( result==LOAD_OK ) {
            formattingPossible=currentSyntax->enableReformatting();

            if ( openTags.size() >NUMBER_BUILTIN_STATES ) {
                // remove dynamic keyword tag delimiters of the old language definition
                vector<string>::iterator keyStyleOpenBegin =
                    openTags.begin() + NUMBER_BUILTIN_STATES;
                vector<string>::iterator keyStyleCloseBegin =
                    closeTags.begin() + NUMBER_BUILTIN_STATES;
                openTags.erase ( keyStyleOpenBegin, openTags.end() );
                closeTags.erase ( keyStyleCloseBegin, closeTags.end() );
            }
            // add new keyword tag delimiters
            for ( unsigned int i=0; i< currentSyntax->getKeywordClasses().size(); i++ ) {
                openTags.push_back ( getKeywordOpenTag ( i ) );
                closeTags.push_back ( getKeywordCloseTag ( i ) );
            }
        }
    }
    return result;
}

bool CodeGenerator::validateInputStream()
{
    if ( !in ) return false;

    // it is not possible to move stream pointer back with stdin
    if ( ( int ) in->tellg() == -1 ) // -1 : stdin
        return true;

    // Sources: http://en.wikipedia.org/wiki/Magic_number_(programming)
    // Magic configuration of "file"
    // This is intended for web plugins - only check filetypes often found in the net
    char magic_gif[]    = {'G','I','F','8', 0};
    char magic_png[]    = {'\x89','P','N','G', 0};
    char magic_java[]   = {'\xCA','\xFE','\xBA','\xBE', 0};
    char magic_jpeg[]   = {'\xFF','\xD8','\xFF', 0};
    char magic_bmp[]    = {'B','M', 0};
    char magic_pdf[]    = {'%','P','D','F', 0};
    char magic_utf8[]   = {'\xEF','\xBB','\xBF',0};
    char magic_rar[]    = {'R','a','r','!', 0};
    char magic_zip[]    = {'P','K','\x03','\x04', 0};
    char magic_ace[]    = {'*','*','A','C','E','*','*', 0};
    char magic_tgz[]    = {'\x8b','\x1f', '\x00', '\x08', 0};
    char magic_bzip[]   = {'B','Z', 0};

    char* magic_table[] = {magic_utf8,
                           magic_gif, magic_png, magic_jpeg, magic_bmp, magic_pdf,
                           magic_java,
                           magic_rar, magic_zip, magic_ace, magic_tgz, magic_bzip,
                           0
                          };

    char buffer [10]= {0};
    in->read ( buffer,8 );  //only read the first 8 bytes of input stream

    int magic_index=0;
    while ( magic_table[magic_index] ) {
        if ( !strncmp ( buffer, magic_table[magic_index], strlen ( magic_table[magic_index] ) ) ) {
            break;
        }
        magic_index++;
    }
    int streamReadPos=0;
    if ( magic_table[magic_index] == magic_utf8 ) {
        //setEncoding("utf-8");
        streamReadPos=3; // remove UTF-8 magic number from output
    }

    in -> seekg ( streamReadPos, ios::beg );
    in-> clear();  // clear fail bit to continue reading

    return !magic_table[magic_index] // points to 0 if no pattern was found
           || magic_table[magic_index] == magic_utf8;
}

void CodeGenerator::printHeader(){
  if ( ! fragmentOutput ) 
    *out << getHeader();
  
  if ( !fragmentOutput || keepInjections) 
    *out << currentSyntax->getHeaderInjection();
}

void CodeGenerator::printFooter(){
  if ( !fragmentOutput || keepInjections) 
    *out << currentSyntax->getFooterInjection();
  
  if ( ! fragmentOutput )
    *out << getFooter();
}

ParseError CodeGenerator::generateFile ( const string &inFileName,
        const string &outFileName )
{
    if ( !docStyle.found() ) {
        return BAD_STYLE;
    }

    reset();

    ParseError error=PARSE_OK;

    inFile=inFileName;
    outFile=outFileName;
    in = ( inFileName.empty() ? &cin :new ifstream ( inFileName.c_str() ) );

    if ( validateInput )
        if ( !validateInputStream() ) error= BAD_INPUT;

    if ( !in->fail() && error==PARSE_OK ) {
        out = ( outFileName.empty() ? &cout :new ofstream ( outFileName.c_str() ) );
        if ( out->fail() ) {
            error=BAD_OUTPUT;
        }
    }

    if ( in->fail() ) {
        error=BAD_INPUT;
    }

    if ( error==PARSE_OK ) {
        if ( formatter != NULL ) {
            formatter->init ( new astyle::ASStreamIterator ( in ) );
        }
        printHeader();
        printBody();
        printFooter();
    }

    if ( !outFileName.empty() ) {
        delete out;
        out=NULL;
    }
    if ( !inFileName.empty() ) {
        delete in;
        in=NULL;
    }
    return error;
}

string CodeGenerator::generateString ( const string &input )
{

    if ( !docStyle.found() ) {
        return "";
    }

    reset();

    in = new istringstream ( input );
    out = new ostringstream ();

    if ( in->fail() || out->fail() ) {
        return "";
    }

    if ( formatter != NULL ) {
        formatter->init ( new astyle::ASStreamIterator ( in ) );
    }
    printHeader();
    printBody();
    printFooter();

    string result = static_cast<ostringstream*> ( out )->str();

    delete out;
    out=NULL;
    delete in;
    in=NULL;

    return result;
}

string CodeGenerator::generateStringFromFile ( const string &inFileName )
{

    if ( !docStyle.found() ) {
        return "";
    }

    reset();

    inFile = inFileName;
    in = new ifstream ( inFileName.c_str() );
    out = new ostringstream ();

    if ( in->fail() || out->fail() ) {
        return "";
    }

    if ( validateInput && !validateInputStream() ) {
        return "ERROR: detected binary input";
    }

    if ( formatter != NULL ) {
        formatter->init ( new astyle::ASStreamIterator ( in ) );
    }
    printHeader();
    printBody();
    printFooter();
    
    string result = static_cast<ostringstream*> ( out )->str();

    delete out;
    out=NULL;
    delete in;
    in=NULL;

    return result;
}

unsigned int CodeGenerator::getStyleID ( State s, unsigned int kwClassID )
{
    if ( s==KEYWORD && kwClassID ) {
        return NUMBER_BUILTIN_STATES + kwClassID-1;
    }
    return ( unsigned int ) s ;
}

void CodeGenerator::openTag ( State s )
{
    *out << openTags[ ( unsigned int ) s];
    currentState=s;
}

void CodeGenerator::closeTag ( State s )
{
    *out << closeTags[ ( unsigned int ) s];
    flushWs();
    currentState=_UNKNOWN;
}

void CodeGenerator::openKWTag ( unsigned int kwClassID )
{
    *out << openTags.at(getStyleID ( KEYWORD, kwClassID ) );
    currentState=KEYWORD;
}

void CodeGenerator::closeKWTag ( unsigned int kwClassID )
{
    *out << closeTags.at(getStyleID ( KEYWORD, kwClassID ) );
    flushWs();
    currentState=_UNKNOWN;
}

bool CodeGenerator::loadEmbeddedLang(const string&embedLangDefPath)
{
    //save path of host language
    if (hostLangDefPath.empty()) {
        hostLangDefPath =currentSyntax->getCurrentPath();
    }
    //load syntax of embedded langage
    LoadResult res = loadLanguage(embedLangDefPath);
    //pass end delimiter regex to syntax description
    currentSyntax->restoreLangEndDelim(embedLangDefPath);
    return res == LOAD_OK;
}

///////////////////////////////////////////////////////////////////////////////

void CodeGenerator::processRootState()
{
    bool eof=false,
         firstLine=true; // avoid newline before printing the first output line

    if ( currentSyntax->highlightingDisabled() ) {
        string line;
        while ( getline ( *in, line ) ) {
            ++lineNumber;
            insertLineNumber ( !firstLine );
            flushWs();
            firstLine=false;
            maskString ( *out, line );
        }
        *out << flush;
        return;
    }

    if (!embedLangStart.empty()) {
        if (!loadEmbeddedLang(currentSyntax->getNewPath(embedLangStart))) return;
    }

    State state=STANDARD;

    openTag ( STANDARD );
    do {
        // determine next state
        state= getCurrentState(STANDARD);

        // handle current state
        switch ( state ) {
        case KEYWORD:
            closeTag ( STANDARD );
            eof=processKeywordState ( state );
            openTag ( STANDARD );
            break;
        case NUMBER:
            closeTag ( STANDARD );
            eof=processNumberState();
            openTag ( STANDARD );
            break;
        case ML_COMMENT:
            closeTag ( STANDARD );
            eof=processMultiLineCommentState();
            openTag ( STANDARD );
            break;
        case SL_COMMENT:
            closeTag ( STANDARD );
            eof=processSingleLineCommentState();
            openTag ( STANDARD );
            break;
        case STRING:
            closeTag ( STANDARD );
            eof=processStringState ( STANDARD );
            openTag ( STANDARD );
            break;
        case DIRECTIVE:
            closeTag ( STANDARD );
            eof=processDirectiveState();
            openTag ( STANDARD );
            break;
        case ESC_CHAR:
            closeTag ( STANDARD );
            eof=processEscapeCharState();
            openTag ( STANDARD );
            break;
        case SYMBOL:
            closeTag ( STANDARD );
            eof=processSymbolState();
            openTag ( STANDARD );
            break;
        case EMBEDDED_CODE_BEGIN:
        case EMBEDDED_CODE_END:
            closeTag ( STANDARD );
            eof=processSyntaxChangeState(state);
            openTag ( STANDARD );
            break;
        case _EOL:
            insertLineNumber ( !firstLine );
            firstLine=false;
            break;
        case _EOF:
            eof=true;
            break;
        case _WS:
            processWsState();
            break;
        default:
            printMaskedToken ();
            break;
        }
    } while ( !eof );
    closeTag ( STANDARD );
    
    if (currentSyntax->getDecorateLineEndFct()) {
      Diluculum::LuaValueList res=callDecorateLineFct(false);    
      if (res.size()==1) {
        *out << res[0].asString();
      } 
    } 
    
    printNewLines = !noTrailingNewLine;
    *out << getNewLine();
    *out << flush;
}

bool CodeGenerator::processSyntaxChangeState(State myState)
{
    State newState=STANDARD;
    bool eof=false,
         exitState=false;
    openTag ( KEYWORD );
    do {
        if (myState==EMBEDDED_CODE_BEGIN) {
            if (!loadEmbeddedLang(embedLangDefPath)) {
                // exit or segfault
                return true;
            }
            //test current line again to match tokens of the embedded language
            matchRegex(line);
        } else if (myState==EMBEDDED_CODE_END) {
            // load host language syntax
            loadLanguage(hostLangDefPath);
            //test current line again to match tokens of the host language
            matchRegex(line);
        }

        printMaskedToken ( newState!=_WS );
        newState= getCurrentState(myState);
        switch ( newState ) {
        case _WS:
            processWsState();
            break;
        case _EOL:
            insertLineNumber();
            exitState=true;
            break;
        case _EOF:
            eof = true;
            break;
        default:
            exitState=true;
            break;
        }
    } while ( ( !exitState ) && ( !eof ) );

    closeTag ( KEYWORD );
    return eof;
}

bool CodeGenerator::processKeywordState ( State myState )
{
    State newState=STANDARD;
    unsigned int myClassID=currentKeywordClass;
    bool eof=false,
         exitState=false;

    openKWTag ( myClassID );
    do {
        printMaskedToken ( newState!=_WS,
                           ( currentSyntax->isIgnoreCase() ) ? keywordCase : StringTools::CASE_UNCHANGED );
        newState= getCurrentState(myState);
        switch ( newState ) {
        case _WS:
            processWsState();
            break;
        case _EOL:
            insertLineNumber();
            exitState=true;
            break;
        case _EOF:
            eof = true;
            break;
        case KEYWORD_END:
            exitState=true;
            break;
        default:
            exitState= ( myClassID!=currentKeywordClass ) || ( myState!=newState );
            break;
        }
    } while ( ( !exitState ) && ( !eof ) );

    closeKWTag ( myClassID );

    currentKeywordClass=0;
    return eof;
}

bool CodeGenerator::processNumberState()
{
    State newState=STANDARD;
    bool eof=false,
         exitState=false;
    openTag ( NUMBER );
    do {
        printMaskedToken ( newState!=_WS );
        newState= getCurrentState(NUMBER);
        switch ( newState ) {
        case _WS:
            processWsState();
            break;
        case _EOL:
            insertLineNumber();
            exitState=true;
            break;
        case _EOF:
            eof = true;
            break;
        default:
            exitState=newState!=NUMBER;
            break;
        }
    } while ( ( !exitState ) && ( !eof ) );

    closeTag ( NUMBER );
    return eof;
}

bool CodeGenerator::processMultiLineCommentState()
{
    int commentCount=1;
    int openDelimID=currentSyntax->getOpenDelimiterID ( token, ML_COMMENT);
    State newState=STANDARD;
    bool eof=false, exitState=false;
    openTag ( ML_COMMENT );
    do {
        printMaskedToken (newState!=_WS );
        newState= getCurrentState(ML_COMMENT);

        switch ( newState ) {
        case _WS:
            processWsState();
            break;
        case _EOL:
            wsBuffer += closeTags[ML_COMMENT];
            insertLineNumber();
            wsBuffer += openTags[ML_COMMENT];
            break;
        case _EOF:
            eof = true;
            break;
        case ML_COMMENT:

            if ( currentSyntax->allowNestedMLComments() ) {
                ++commentCount;
            }
            // if delimiters are equal, close the comment by continueing to
            // ML_COMMENT_END section
            if (currentSyntax->delimiterIsDistinct(currentSyntax->getOpenDelimiterID ( token, ML_COMMENT  ))) break;

        case ML_COMMENT_END:

            if (!currentSyntax->matchesOpenDelimiter (token,  ML_COMMENT_END, openDelimID)) {
                break;
            }
            commentCount--;
            if ( !commentCount ) {
                printMaskedToken();
                exitState=true;
            }
            break;
        default:
            break;
        }
    } while ( ( !exitState ) && ( !eof ) );

    closeTag ( ML_COMMENT );
    return eof;
}


bool CodeGenerator::processSingleLineCommentState()
{
    if ( checkSpecialCmd() ) {
        return in->bad(); // if input stream is bad, report eof to calling method
    }

    State newState=STANDARD;
    bool eof=false, exitState=false;

    openTag ( SL_COMMENT );
    do {
        printMaskedToken ( newState!=_WS );
        newState= getCurrentState(SL_COMMENT);

        switch ( newState ) {
        case _WS:
            processWsState();
            break;
        case _EOL:
            printMaskedToken();
            if ( preFormatter.isEnabled() && preFormatter.isWrappedLine ( lineNumber-1 ) ) {
                exitState=false;
            } else {
                exitState=true;
            }
            if ( !exitState ) wsBuffer += closeTags[SL_COMMENT];
            insertLineNumber();
            if ( !exitState ) wsBuffer += openTags[SL_COMMENT];

            break;
        case _EOF:
            eof = true;
            break;
        default:
            break;
        }
    } while ( ( !exitState ) && ( !eof ) );

    closeTag ( SL_COMMENT );
    return eof;
}

bool CodeGenerator::processDirectiveState()
{
    State  newState=STANDARD;
    bool eof=false, exitState=false;

    openTag ( DIRECTIVE );
    do {
        printMaskedToken ( newState!=_WS );
        newState= getCurrentState(DIRECTIVE);
        switch ( newState ) {
        case _WS:
            processWsState();
            break;
        case DIRECTIVE_END:
            printMaskedToken();
            exitState=true;
            break;
        case _EOL:
            printMaskedToken();
            if ( preFormatter.isEnabled() && preFormatter.isWrappedLine ( lineNumber-1 ) ) {
                exitState=false;
            } else {
                exitState= ( terminatingChar!=currentSyntax->getContinuationChar() );
            }
            if ( !exitState ) wsBuffer += closeTags[DIRECTIVE];
            insertLineNumber();
            if ( !exitState ) wsBuffer += openTags[DIRECTIVE];
            break;
        case ML_COMMENT:
            closeTag ( DIRECTIVE );
            eof= processMultiLineCommentState();
            openTag ( DIRECTIVE );
            break;
        case SL_COMMENT:
            closeTag ( DIRECTIVE );
            eof= processSingleLineCommentState();
            openTag ( DIRECTIVE );
            exitState=true;
            break;
        case STRING:
            closeTag ( DIRECTIVE );
            eof=processStringState ( DIRECTIVE );
            openTag ( DIRECTIVE );
            break;
        case _EOF:
            eof = true;
            break;
        default:
            break;
        }
    } while ( ( !exitState ) && ( !eof ) );

    closeTag ( DIRECTIVE );
    return eof;
}

bool CodeGenerator::processStringState ( State oldState )
{
    State newState=STANDARD;
    bool eof=false, exitState=false;
    bool returnedFromOtherState=false;

    State myState= ( oldState==DIRECTIVE ) ? DIRECTIVE_STRING : STRING;

    int openDelimID=currentSyntax->getOpenDelimiterID ( token, myState);
    string openDelim=token;

    //Raw String by definition:
    bool isRawString=currentSyntax->delimiterIsRawString(openDelimID);

    // Test if character before string open delimiter token equals to the
    // raw string prefix (Example: r" ", r""" """ in Python)

    //Raw String Prefix:
    if ( lineIndex>token.length() &&line[lineIndex-token.length()-1]==currentSyntax->getRawStringPrefix() ) {
        isRawString=true;
    }

    openTag ( myState );
    do {
        // true if last token was an escape char
        if ( !returnedFromOtherState ) {
            printMaskedToken (newState!=_WS );
        }
        returnedFromOtherState=false;
        newState= getCurrentState(myState);

        switch ( newState ) {
        case _WS:
            processWsState();
            break;
        case _EOL:
            wsBuffer += closeTags[myState];
            insertLineNumber();
            wsBuffer += openTags[myState];
            break;
        case STRING_END:
            if (resultOfHook || currentSyntax->matchesOpenDelimiter (token,  STRING_END, openDelimID)) {
              if (currentSyntax->assertDelimEqualLength()){
                exitState= openDelim.length()==token.length();
              }else {
                exitState= true;
              }
                printMaskedToken();
            }
            break;
        case STRING:
            // if there exist multiple string delimiters, close string if
            // current delimiter is equal to the opening delimiter
            exitState=currentSyntax->delimiterIsDistinct(currentSyntax->getOpenDelimiterID ( token, STRING  ))&&token==openDelim;
            printMaskedToken();
            break;
        case ESC_CHAR:
            if ( !isRawString ) {
                closeTag ( myState );
                eof=processEscapeCharState();
                openTag ( myState );
                returnedFromOtherState=true;
            } else {
                // FIXME not a fix for Python r"""\"""
                exitState=token.size()>1 && token[1] == openDelim[0];
                printMaskedToken();
            }
            break;
        case STRING_INTERPOLATION:
            closeTag ( myState );
            eof=processInterpolationState();
            openTag ( myState );
            returnedFromOtherState=true;
            break;
        case _EOF:
            eof = true;
            break;
        default:
            printMaskedToken();
            break;
        }
    } while ( ( !exitState ) && ( !eof ) );

    closeTag ( myState );
    return eof;
}

bool CodeGenerator::processSymbolState()
{

    State newState=STANDARD;
    bool eof=false,
         exitState=false;

    openTag ( SYMBOL );
    do {
        printMaskedToken ( newState!=_WS );
        newState= getCurrentState(SYMBOL);
        switch ( newState ) {
        case _WS:
            processWsState();
            break;
        case _EOL:
            insertLineNumber();
            exitState=true;
            break;
        case _EOF:
            eof = true;
            break;
        default:
            exitState=newState!=SYMBOL;
            break;
        }
    } while ( ( !exitState ) && ( !eof ) );

    closeTag ( SYMBOL );
    return eof;
}

bool CodeGenerator::processEscapeCharState()
{
    State newState=STANDARD;
    bool eof=false, exitState=false;
    openTag ( ESC_CHAR );
    do {
        printMaskedToken (newState!=_WS );
        newState= getCurrentState(ESC_CHAR);
        switch ( newState ) {
        case _EOL:
            insertLineNumber();
            exitState=true;
            break;
        case _WS:
            processWsState();
            break;
        case _EOF:
            eof = true;
            break;
        default:
            exitState=newState!=ESC_CHAR;
            break;
        }
    } while ( ( !exitState ) && ( !eof ) );

    closeTag ( ESC_CHAR );
    return eof;
}

bool CodeGenerator::processInterpolationState()
{
    State newState=STANDARD;
    bool eof=false, exitState=false;
    openTag ( STRING_INTERPOLATION );
    do {
        printMaskedToken (newState!=_WS );
        newState= getCurrentState(STRING_INTERPOLATION);
        switch ( newState ) {
        case _EOL:
            insertLineNumber();
            exitState=true;
            break;
        case _WS:
            processWsState();
            break;
        case _EOF:
            eof = true;
            break;
        default:
            exitState=newState!=STRING_INTERPOLATION;
            break;
        }
    } while ( ( !exitState ) && ( !eof ) );

    closeTag ( STRING_INTERPOLATION );
    return eof;
}

void CodeGenerator::processWsState()
{
    if ( !maskWs ) {
        wsBuffer += token;
        token.clear();
        return;
    }
    flushWs();
    int cntWs=0;
    lineIndex--;

    // while (iswspace(line[lineIndex])  ) {
    while ( line[lineIndex]==' ' || line[lineIndex]=='\t' ) {
        ++cntWs;
        ++lineIndex;
    }

    if ( cntWs>1 ) {
        unsigned int styleID=getStyleID ( currentState, currentKeywordClass );
        if ( excludeWs && styleID!=_UNKNOWN ) {
            *out << closeTags[styleID];
        }
        *out << maskWsBegin;
        for ( int i=0; i<cntWs; i++ ) {
            *out << spacer;
        }
        *out << maskWsEnd;
        if ( excludeWs && styleID!=_UNKNOWN ) {
            *out << openTags[styleID];
        }
    } else {
        *out << spacer; //Bugfix fehlender Space nach Strings
    }
    token.clear();
}

void CodeGenerator::flushWs()
{
    *out<<wsBuffer;
    wsBuffer.clear();
}

string CodeGenerator::getNewLine()
{
    return (printNewLines) ? newLineTag : "";
}

Diluculum::LuaValueList CodeGenerator::callDecorateLineFct(bool isLineStart){
       
   Diluculum::LuaValueList params;
   params.push_back(Diluculum::LuaValue(lineNumber));

    return currentSyntax->getLuaState()->call ( isLineStart ? 
           *currentSyntax->getDecorateLineBeginFct(): *currentSyntax->getDecorateLineEndFct(),
           params,"getDecorateLineFct call")  ;
  
}

void CodeGenerator::insertLineNumber ( bool insertNewLine )
{
    if ( insertNewLine ) {
      if (currentSyntax->getDecorateLineEndFct()) {
        Diluculum::LuaValueList res=callDecorateLineFct(false);    
        if (res.size()==1) {
          wsBuffer +=res[0].asString();
        } 
      } 
      
      wsBuffer += getNewLine();
    }

    
    if (currentSyntax->getDecorateLineBeginFct()) {
      Diluculum::LuaValueList res=callDecorateLineFct(true);    
      if (res.size()==1) {
        wsBuffer +=res[0].asString();
      } 
    } 
    
    if ( showLineNumbers ) {
        ostringstream os;
        ostringstream numberPrefix;

        os << setw ( getLineNumberWidth() ) << right;
        if( numberCurrentLine ) {
            if ( lineNumberFillZeroes ) {
                os.fill ( '0' );
            }
            os << lineNumber+lineNumberOffset;
        } else {
            os << "";
        }

        numberPrefix << openTags[LINENUMBER];
        maskString ( numberPrefix, os.str() );
        numberPrefix << spacer << closeTags[LINENUMBER];

        wsBuffer += numberPrefix.str();
    }
}

unsigned int CodeGenerator::getLineIndex()
{
    return lineIndex;
}

bool CodeGenerator::printExternalStyle ( const string &outFile )
{
    if ( !includeStyleDef ) {
        ostream *cssOutFile = ( outFile.empty() ? &cout :new ofstream ( outFile.c_str() ) );
        if ( !cssOutFile->fail() ) {
            *cssOutFile << styleCommentOpen
                        <<" Style definition file generated by highlight "
                        << HIGHLIGHT_VERSION << ", " << HIGHLIGHT_URL
                        << " " << styleCommentClose << "\n";

            *cssOutFile << "\n" << styleCommentOpen
                        << " Highlighting theme: "
                        << docStyle.getDescription() <<" "
                        << styleCommentClose << "\n\n"
                        << getStyleDefinition()
                        << "\n";
            *cssOutFile << readUserStyleDef();
            if ( !outFile.empty() ) delete cssOutFile;
        } else {
            return false;
        }
    }
    return true;
}

string CodeGenerator::readUserStyleDef()
{
    ostringstream ostr;
    if ( !styleInputPath.empty() ) {
        ifstream userStyleDef ( styleInputPath.c_str() );
        if ( userStyleDef ) {
            ostr 	<< "\n" << styleCommentOpen
                    << " Content of " << styleInputPath
                    << ": " <<styleCommentClose << "\n";
            string line;
            while ( getline ( userStyleDef, line ) ) {
                ostr << line << "\n";
            }
            userStyleDef.close();
        } else {
            ostr 	<< styleCommentOpen
                    << " ERROR: Could not include " << styleInputPath
                    << "." << styleCommentClose << "\n";
        }
    }

    string injections=docStyle.getInjections();
    if (!injections.empty()) {
        ostr 	<< "\n" << styleCommentOpen
                << " Plug-in theme injections: " <<styleCommentClose << "\n";
        ostr << injections<<"\n";
    }
    return ostr.str();
}

bool CodeGenerator::initPluginScript(const string& script)
{

    if (script.empty()) return true;

    try {
        userScriptError="";
        Diluculum::LuaState ls;
        ls.doFile (script);
        int listIdx=1;
        
        while (ls["Plugins"][listIdx].value() !=Diluculum::Nil) {

            // Theme plugins
            if (ls["Plugins"][listIdx]["Type"].value().asString()=="theme") {
                if (ls["Plugins"][listIdx]["Chunk"].value().type()==LUA_TFUNCTION) {
                    docStyle.addUserChunk(ls["Plugins"][listIdx]["Chunk"].value().asFunction());
                }
            }
            // Syntax plugins
            else if (ls["Plugins"][listIdx]["Type"].value().asString()=="lang") {
                if (ls["Plugins"][listIdx]["Chunk"].value().type()==LUA_TFUNCTION) {
                    currentSyntax->addUserChunk(ls["Plugins"][listIdx]["Chunk"].value().asFunction());
                }
            }
            listIdx++;
        }
    }  catch (Diluculum::LuaError err) {
        userScriptError=err.what();
        return false;
    }
    return true;
}

bool CodeGenerator::checkSpecialCmd()
{
    string noParseCmd="@highlight";
    size_t cmdPos = line.find ( noParseCmd );

    if ( cmdPos!=string::npos ) {
        *out<<line.substr ( noParseCmd.size() +cmdPos + 1 );

        // hide comment line from output
        token.clear();
        lineIndex=line.length();
        getInputChar();
        lineNumber--;
        // end hide

        return true; // do not parse line as comment
    }
    return false; //parse comment as usual
}

}
