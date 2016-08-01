/***************************************************************************
                          cmdlineoptions.cpp  -  description
                             -------------------
    begin                : Sun Nov 25 2001
    copyright            : (C) 2001-2016 by Andre Simon
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

#include "cmdlineoptions.h"
#include "platform_fs.h"
#include "datadir.h"
#include <sstream>
#include <cstdio>

#include "arg_parser.h"

using namespace std;

CmdLineOptions::CmdLineOptions ( const int argc, const char *argv[] ) :
    numberSpaces ( 0 ),
    lineNrWidth ( 5 ),
    lineLength ( 80 ),
    lineNrStart ( 1 ),
    wrappingStyle ( highlight::WRAP_DISABLED ),
    outputType ( highlight::HTML ),
    keywordCase ( StringTools::CASE_UNCHANGED ),
    baseFontSize("10"),
    className ( "hl" ),
    opt_syntax ( false ),
    opt_include_style ( false ),
    opt_help ( false ),
    opt_version ( false ),
    opt_verbose ( false ),
    opt_print_config ( false ),
    opt_linenumbers ( false ),
    opt_style ( false ),
    opt_batch_mode ( false ),
    opt_fragment ( false ) ,
    opt_attach_line_anchors ( false ),
    opt_show_themes ( false ),
    opt_show_langdefs ( false ),
    opt_show_plugins (false),
    opt_printindex ( false ),
    opt_quiet ( false ),
    opt_replacequotes ( false ),
    opt_babel ( false ),
    opt_print_progress ( false ),
    opt_fill_zeroes ( false ),
    opt_stylepath_explicit ( false ),
    opt_force_output ( false ),
    opt_ordered_list ( false ),
    opt_fnames_as_anchors ( false ),
    opt_validate ( false ),
    opt_number_wrapped_lines ( true ),
    opt_inline_css ( false ),
    opt_enclose_pre ( false ),
    opt_char_styles ( false ),
    opt_page_color(false),
    opt_pretty_symbols ( false ),
    opt_delim_CR (false),
    opt_print_style(false),
    opt_no_trailing_nl(false),
    opt_keep_injections(false),
    anchorPrefix ( "l" ),
    helpLang ( "en" ),
    encodingName ( "ISO-8859-1" )
{
    enum Optcode {
        S_OPT_ENCLOSE_PRE = 256, S_OPT_FORCE_OUTPUT,
        S_OPT_INLINE_CSS, S_OPT_KW_CASE,
        S_OPT_PRINT_CONFIG, S_OPT_TEST_INPUT, S_OPT_NO_NUMBER_WL,
        S_OPT_SVG_WIDTH, S_OPT_SVG_HEIGHT, S_OPT_CLASSNAME,
        S_OPT_RTF_CHAR_STYLES, S_OPT_RTF_PAGE_COLOR,
        S_OPT_SKIP_UNKNOWN,
        S_OPT_COMPAT_DOC, S_OPT_COMPAT_NODOC, S_OPT_COMPAT_TAB, S_OPT_COMPAT_CSS,
        S_OPT_COMPAT_OUTDIR, S_OPT_COMPAT_FAILSAFE,
        S_OPT_COMPAT_SRCLANG, S_OPT_COMPAT_LINENUM, S_OPT_COMPAT_LINEREF,
        S_OPT_PRETTY_SYMBOLS, S_OPT_EOL_DELIM_CR, S_OPT_START_NESTED,
        S_OPT_PRINT_STYLE, S_OPT_NO_TRAILING_NL, S_OPT_PLUGIN, S_OPT_ABS_CFG_PATH,
        S_OPT_PLUGIN_READFILE, S_OPT_PLUGIN_PARAMETER, S_LIST_SCRIPTS,
        S_OPT_KEEP_INJECTIONS
    };

    const Arg_parser::Option options[] = {
        { 'a', OPT_ANCHORS,        Arg_parser::no  },
        { 'b', OPT_BABEL,          Arg_parser::no  },
        { 'B', OPT_BATCHREC,       Arg_parser::yes },
        { 'c', OPT_STYLE_OUT,      Arg_parser::yes },
        { 'C', OPT_INDEXFILE,      Arg_parser::no  },
        { 'd', OPT_OUTDIR,         Arg_parser::yes },
        { 'D', OPT_DATADIR,        Arg_parser::yes },
        { 'e', OPT_STYLE_IN,       Arg_parser::yes },
        { 'f', OPT_FRAGMENT,       Arg_parser::no  },
        { 'F', OPT_FORMAT,         Arg_parser::yes },

        { 'h', OPT_HELP,           Arg_parser::no  },
        { 'i', OPT_IN,             Arg_parser::yes },
        { 'I', OPT_INC_STYLE,      Arg_parser::no  },
        { 'j', OPT_LNR_LEN,        Arg_parser::yes },
        { 'J', OPT_LINE_LEN,       Arg_parser::yes },
        { 'k', OPT_BASE_FONT,      Arg_parser::yes },
        { 'K', OPT_BASE_FONT_SIZE, Arg_parser::yes },
        { 'l', OPT_LINENO,         Arg_parser::no  },
        { 'm', OPT_LNR_START,      Arg_parser::yes },
        { 'n', OPT_ORDERED_LIST,   Arg_parser::no  },
        { 'N', OPT_ANCHOR_FN,      Arg_parser::no  },
        { 'o', OPT_OUT,            Arg_parser::yes },
        { 'O', OPT_OUTFORMAT, 	   Arg_parser::yes },
        { 'p', OPT_LISTLANGS,      Arg_parser::no  },
        { 'P', OPT_PROGRESSBAR,    Arg_parser::no  },
        { 'q', OPT_QUIET,          Arg_parser::no  },
        { 'Q', OPT_VERSION,        Arg_parser::no  },
        { 'r', OPT_REPLACE_QUOTES, Arg_parser::no  },
        { 's', OPT_STYLE,          Arg_parser::yes },
        { 'S', OPT_SYNTAX,         Arg_parser::yes },
        { 't', OPT_DELTABS,        Arg_parser::yes },
        { 'T', OPT_DOC_TITLE,      Arg_parser::yes },
        { 'u', OPT_ENCODING,       Arg_parser::yes },
        { 'v', OPT_VERBOSE,        Arg_parser::no  },
        { 'V', OPT_WRAPSIMPLE,     Arg_parser::no  },
        { 'w', OPT_LISTTHEMES,     Arg_parser::no  },
        { 'W', OPT_WRAP,           Arg_parser::no  },
        { 'x', OPT_RTF_PAGE_SIZE,  Arg_parser::yes },
        { 'y', OPT_ANCHOR_PFX,     Arg_parser::yes },
        { 'z', OPT_FILLZEROES,     Arg_parser::no  },

        { S_OPT_CLASSNAME,        OPT_CLASSNAME,    Arg_parser::yes },
        { S_OPT_SVG_WIDTH,        OPT_SVG_WIDTH,    Arg_parser::yes  },
        { S_OPT_SVG_HEIGHT,       OPT_SVG_HEIGHT,   Arg_parser::yes  },
        { S_OPT_ENCLOSE_PRE,      OPT_ENCLOSE_PRE,  Arg_parser::no  },
        { S_OPT_FORCE_OUTPUT,     OPT_FORCE_OUTPUT, Arg_parser::no  },
        { S_OPT_INLINE_CSS,       OPT_INLINE_CSS,   Arg_parser::no  },
        { S_OPT_KW_CASE,          OPT_KW_CASE,      Arg_parser::yes },
        { S_OPT_PRINT_CONFIG,     OPT_PRINT_CONFIG, Arg_parser::no  },
        { S_OPT_TEST_INPUT,       OPT_TEST_INPUT,   Arg_parser::no  },
        { S_OPT_NO_NUMBER_WL,     OPT_NO_NUMBER_WL, Arg_parser::no  },
        { S_OPT_RTF_CHAR_STYLES,  OPT_RTF_CHAR_STYLES, Arg_parser::no  },
        { S_OPT_RTF_PAGE_COLOR,   OPT_RTF_PAGE_COLOR, Arg_parser::no  },
        { S_OPT_SKIP_UNKNOWN,     OPT_SKIP_UNKNOWN, Arg_parser::yes },
        { S_OPT_START_NESTED,     OPT_START_NESTED,   Arg_parser::yes },
        { S_OPT_COMPAT_DOC,       OPT_COMPAT_DOC, Arg_parser::no },
        { S_OPT_COMPAT_NODOC,     OPT_COMPAT_NODOC, Arg_parser::no },
        { S_OPT_COMPAT_TAB,       OPT_COMPAT_TAB, Arg_parser::yes },
        { S_OPT_COMPAT_CSS,       OPT_COMPAT_CSS, Arg_parser::yes },
        { S_OPT_COMPAT_OUTDIR,    OPT_COMPAT_OUTDIR, Arg_parser::yes },
        { S_OPT_COMPAT_FAILSAFE,  OPT_COMPAT_FAILSAFE, Arg_parser::no },
        { S_OPT_COMPAT_SRCLANG,   OPT_COMPAT_SRCLANG, Arg_parser::yes },
        { S_OPT_COMPAT_LINENUM,   OPT_COMPAT_LINENUM, Arg_parser::maybe },
        { S_OPT_COMPAT_LINEREF,   OPT_COMPAT_LINEREF, Arg_parser::maybe },
        { S_OPT_PRETTY_SYMBOLS,   OPT_PRETTY_SYMBOLS, Arg_parser::no },
        { S_OPT_EOL_DELIM_CR,     OPT_EOL_DELIM_CR,   Arg_parser::no },
        { S_OPT_PRINT_STYLE,      OPT_PRINT_STYLE, Arg_parser::no },
        { S_OPT_NO_TRAILING_NL,   OPT_NO_TRAILING_NL, Arg_parser::no },
        { S_OPT_KEEP_INJECTIONS,  OPT_KEEP_INJECTIONS, Arg_parser::no },
        { S_OPT_PLUGIN,           OPT_PLUGIN, Arg_parser::yes },
        { S_OPT_PLUGIN_READFILE,  OPT_PLUGIN_READFILE, Arg_parser::yes },
        { S_OPT_PLUGIN_PARAMETER, OPT_PLUGIN_PARAMETER, Arg_parser::yes },
        { S_OPT_ABS_CFG_PATH,     OPT_ABS_CFG_PATH,  Arg_parser::yes},
        { S_LIST_SCRIPTS,         OPT_LIST_SCRIPTS,  Arg_parser::yes},

        { 0, 0, Arg_parser::no }
    };


    Arg_parser parser ( argc, argv, options );
    if ( parser.error().size() ) { // bad option
        cerr << "highlight: "<< parser.error() <<"\n";
        cerr << "Try `highlight --help' for more information.\n";
        exit ( 1 );
    }
    
    int argind = 0;
    for ( ; argind < parser.arguments(); ++argind ) {
        const int code = parser.code ( argind );
        const std::string & arg = parser.argument ( argind );
        if ( !code ) break; // no more options
        switch ( code ) {
        case 'O': {
            const string tmp = StringTools::change_case ( arg );
            if ( tmp == "xhtml" )
                outputType = highlight::XHTML;
            else if ( tmp == "tex" )
                outputType = highlight::TEX;
            else if ( tmp == "latex" )
                outputType = highlight::LATEX;
            else if ( tmp == "rtf" )
                outputType = highlight::RTF;
            else if ( tmp == "ansi" || tmp == "esc" ) // gnu source-highlight esc parameter
                outputType = highlight::ANSI;
            else if ( tmp == "xterm256" )
                outputType = highlight::XTERM256;
            else if ( tmp == "svg" )
                outputType = highlight::SVG;
            else if ( tmp == "bbcode" )
                outputType = highlight::BBCODE;
            else if ( tmp == "pango" )
                outputType = highlight::PANGO;
            else if ( tmp == "odt" )
                outputType = highlight::ODTFLAT;
            else
                outputType = highlight::HTML;
        }
        break;
        case 'a':
            opt_attach_line_anchors = true;
            break;
        case 'b':
            opt_babel=true;
            break;
        case 'B':
            opt_batch_mode = true;
            readDirectory ( arg );
            break;
        case 'c':
        case S_OPT_COMPAT_CSS:
            styleOutFilename = arg;
            opt_stylepath_explicit=true;
            break;
        case 'C':
            opt_printindex=true;
            break;
        case 'T':
            docTitle = arg;
            break;
        case 'D':
            dataDir=validateDirPath ( arg );
            break;
        case 'e':
            styleInFilename = arg;
            break;
        case 'f':
        case S_OPT_COMPAT_NODOC:
            opt_fragment = true;
            break;
        case 'F':
            indentScheme = arg;
            break;
        case S_OPT_CLASSNAME:
            className = arg;
            break;
        case 'h':
            opt_help = true;
            break;
        case 'i':
            inputFileNames.push_back ( arg );
            break;
        case 'I':
            opt_include_style = true;
            break;
        case 'j':
            StringTools::str2num<int> ( lineNrWidth, arg, std::dec );
            break;
        case 'J':
            StringTools::str2num<int> ( lineLength, arg, std::dec );
            break;
        case 'k':
            baseFont = arg;
            break;
        case 'K':
            baseFontSize = arg;
            break;
        case S_OPT_COMPAT_LINENUM:
            if ( arg=="0" ) opt_fill_zeroes=true;
        case 'l':
            opt_linenumbers = true;
            break;
        case 'm':
            StringTools::str2num<int> ( lineNrStart, arg, std::dec );
            break;
        case 'M':
            outputType=highlight::XTERM256;
            break;
        case 'n':
            opt_ordered_list = opt_linenumbers = true;
            break;
        case 'N':
            opt_fnames_as_anchors=true;
            break;
        case 'o':
            outFilename = arg;
            break;
        case 'd':
        case S_OPT_COMPAT_OUTDIR:
            outDirectory = validateDirPath ( arg );
            break;
        case 'p':
            opt_show_langdefs = true;
            break;
        case 'P':
            opt_print_progress=true;
            break;
        case 'q':
            opt_quiet = true;
            break;
        case 'Q':
            opt_version = true;
            break;
        case 'r':
            opt_replacequotes=true;
            break;
        case 's':
            styleName = arg;
            opt_style = true;
            break;
        case 'S':
        case S_OPT_COMPAT_SRCLANG:
            syntax = arg;
            opt_syntax = true;
            break;
        case 't':
        case S_OPT_COMPAT_TAB:
            StringTools::str2num<int> ( numberSpaces, arg, std::dec );
            break;
        case 'u':
            encodingName = arg;
            break;
        case 'v':
            opt_verbose = true;
            break;
        case 'V':
            wrappingStyle = highlight::WRAP_SIMPLE;
            break;
        case 'w':
            opt_show_themes = true;
            break;
        case 'W':
            wrappingStyle = highlight::WRAP_DEFAULT;
            break;
        case 'x':
            pageSize = arg;
            break;
        case 'y':
            anchorPrefix = arg;
            break;
        case 'z':
            opt_fill_zeroes=true;
            break;
        case S_OPT_SVG_WIDTH:
            svg_width = arg;
            break;
        case S_OPT_SVG_HEIGHT:
            svg_height = arg;
            break;
        case S_OPT_ENCLOSE_PRE:
            opt_enclose_pre=true;
            break;
        case S_OPT_FORCE_OUTPUT:
        case S_OPT_COMPAT_FAILSAFE:
            opt_force_output = true;
            break;
        case S_OPT_INLINE_CSS:
            opt_inline_css=true;
            break;
        case S_OPT_KW_CASE: {
            const string tmp = StringTools::change_case ( arg );
            if ( tmp == "upper" )
                keywordCase = StringTools::CASE_UPPER;
            else if ( tmp == "lower" )
                keywordCase = StringTools::CASE_LOWER;
            else if ( tmp == "capitalize" )
                keywordCase = StringTools::CASE_CAPITALIZE;
        }
        break;

        case S_OPT_PRINT_CONFIG:
            opt_print_config = true;
            break;
        case S_OPT_TEST_INPUT:
            opt_validate=true;
            break;
        case S_OPT_NO_NUMBER_WL:
            opt_number_wrapped_lines=false;
            break;
        case S_OPT_RTF_CHAR_STYLES:
            opt_char_styles=true;
            break;
        case S_OPT_RTF_PAGE_COLOR:
             opt_page_color=true;
            break;
          
        case S_OPT_SKIP_UNKNOWN:
            skipArg=arg;
            break;
        case S_OPT_PLUGIN:
            userPlugins.push_back(arg);
            break;

        case S_OPT_PLUGIN_PARAMETER:
        case S_OPT_PLUGIN_READFILE:
            pluginParameter=arg;
            break;

        case S_OPT_PRETTY_SYMBOLS:
            opt_pretty_symbols = true;
            break;
        case S_OPT_COMPAT_DOC:
            opt_fragment = false;
            break;
        case S_OPT_COMPAT_LINEREF:
            opt_linenumbers = true;
            opt_attach_line_anchors = true;
            anchorPrefix = ( arg.empty() ) ? "line":arg;
            break;
        case S_OPT_EOL_DELIM_CR:
            opt_delim_CR = true;
            break;
        case S_OPT_START_NESTED:
            startNestedLang=arg;
            break;
        case S_OPT_PRINT_STYLE:
            opt_print_style = true;
            break;
        case S_OPT_NO_TRAILING_NL:
            opt_no_trailing_nl = true;
            break;
        case S_OPT_KEEP_INJECTIONS:
            opt_keep_injections = true;
            break;
        case S_OPT_ABS_CFG_PATH:
            if (arg.find(".lang")!=string::npos) {
                absLangPath=arg;
                syntax = arg.substr(0, arg.find_last_of('.'));
                opt_syntax = true;
            } else if (arg.find(".theme")!=string::npos) absThemePath=arg;
            else cerr << "highlight: unknown config file type" << endl;
            break;
        case S_LIST_SCRIPTS:
            opt_show_themes=(arg=="themes");
            opt_show_plugins=(arg=="plugins");
            opt_show_langdefs=!(opt_show_themes&&opt_show_plugins);
            break;

        default:
            cerr << "highlight: option parsing failed" << endl;
        }
    }

    if ( argind < parser.arguments() ) { //still args left
        if ( inputFileNames.empty() ) {
            while ( argind < parser.arguments() ) {
                inputFileNames.push_back ( parser.argument ( argind++ ) );
            }
        }
    } else if ( inputFileNames.empty() ) {
        inputFileNames.push_back ( "" );
    }

    if ( skipArg.size() ) {
        istringstream valueStream;
        string elem;
        string wildcard;
        valueStream.str ( StringTools::change_case ( skipArg,StringTools::CASE_LOWER ) );

        while ( getline ( valueStream, elem, ';' ) ) {
            ignoredFileTypes.insert ( elem );
        }
        for ( vector<string>::iterator file=inputFileNames.begin(); file!=inputFileNames.end(); file++ ) {
            for ( set<string>::iterator ext=ignoredFileTypes.begin(); ext!=ignoredFileTypes.end(); ext++ ) {
                wildcard="*."+*ext;
                if ( Platform::wildcmp ( wildcard.c_str(), ( *file ).c_str() ) ) {
                    file = inputFileNames.erase ( file );
                    break;
                }
            }
        }
    }
}

CmdLineOptions::~CmdLineOptions() {}

const string &CmdLineOptions::getSingleOutFilename()
{
    if ( !inputFileNames.empty() && !outDirectory.empty() ) {
        if ( outFilename.empty() ) {
            outFilename = outDirectory;
            int delim = getSingleInFilename().find_last_of ( Platform::pathSeparator ) +1;
            outFilename += getSingleInFilename().substr ( ( delim>-1 ) ?delim:0 )
                           + getOutFileSuffix();
        }
    }
    return outFilename;
}

const string &CmdLineOptions::getSingleInFilename()  const
{
    return inputFileNames[0];
}

const string &CmdLineOptions::getOutDirectory()
{
    if ( !outFilename.empty() && !enableBatchMode() ) {
        outDirectory=getDirName ( outFilename );
    }
    return outDirectory;
}

const string CmdLineOptions::getStyleOutFilename() const
{
    if ( !styleOutFilename.empty() ) return styleOutFilename;

    if ( outputType==highlight::TEX || outputType==highlight::LATEX ) {
        return "highlight.sty";
    } else {
        return "highlight.css";
    }
}
const string &CmdLineOptions::getStyleInFilename() const
{
    return styleInFilename;
}
const string& CmdLineOptions::getSVGWidth() const
{
    return svg_width;
}
const string& CmdLineOptions::getSVGHeight() const
{
    return svg_height;
}
int CmdLineOptions::getNumberSpaces() const
{
    return numberSpaces;
}
bool CmdLineOptions::printVersion() const
{
    return opt_version;
}
bool CmdLineOptions::printHelp() const
{
    return opt_help;
}
bool CmdLineOptions::printDebugInfo() const
{
    return opt_verbose;
}
bool CmdLineOptions::printConfigInfo() const
{
    return opt_print_config;
}
bool CmdLineOptions::quietMode() const
{
    return opt_quiet;
}
bool CmdLineOptions::includeStyleDef() const
{
    return opt_include_style;
}
bool CmdLineOptions::useFNamesAsAnchors() const
{
    return opt_fnames_as_anchors;
}

bool CmdLineOptions::formatSupportsExtStyle()
{
    return outputType==highlight::HTML ||
           outputType==highlight::XHTML ||
           outputType==highlight::LATEX ||
           outputType==highlight::TEX ||
           outputType==highlight::SVG;
}

bool CmdLineOptions::printLineNumbers() const
{
    return opt_linenumbers;
}

string CmdLineOptions::getThemeName() const
{
    return ( ( opt_style ) ? styleName+".theme" : (outputType==highlight::XTERM256)?"edit-vim-dark.theme":"edit-kwrite.theme" );
}
bool CmdLineOptions::enableBatchMode() const
{
    return inputFileNames.size() >1 || opt_batch_mode;
}
bool CmdLineOptions::fragmentOutput() const
{
    return opt_fragment;
}
string CmdLineOptions::getOutFileSuffix() const
{
    switch ( outputType ) {
    case highlight::XHTML:
        return ".xhtml";
    case highlight::RTF:
        return ".rtf";
    case highlight::TEX:
    case highlight::LATEX:
        return ".tex";
    case highlight::SVG:
        return ".svg";
    case highlight::ANSI:
        return ".ansi";
    case highlight::XTERM256:
        return ".xterm";
    case highlight::BBCODE:
        return ".bbcode";
    case highlight::ODTFLAT:
        return ".fodt";
    default:
        return ".html";
    }
}
string CmdLineOptions::getDirName ( const string & path )
{
    size_t dirNameLength=path.rfind ( Platform::pathSeparator );
    return ( dirNameLength==string::npos ) ?string() :path.substr ( 0, dirNameLength+1 );
}
bool CmdLineOptions::attachLineAnchors() const
{
    return opt_attach_line_anchors;
}
bool CmdLineOptions::showThemes() const
{
    return opt_show_themes;
}
bool CmdLineOptions::showLangdefs() const
{
    return opt_show_langdefs;
}
bool CmdLineOptions::showPlugins() const
{
    return opt_show_plugins;
}
bool CmdLineOptions::outDirGiven() const
{
    return !outFilename.empty();
}
bool CmdLineOptions::replaceQuotes() const
{
    return opt_replacequotes;
}
bool CmdLineOptions::disableBabelShorthands() const
{
    return opt_babel;
}
bool CmdLineOptions::prettySymbols() const
{
    return opt_pretty_symbols;
}
bool CmdLineOptions::getFlag ( const string& paramVal )
{
    return StringTools::change_case ( paramVal ) =="true";
}
bool CmdLineOptions::orderedList() const
{
    return opt_ordered_list;
}
bool CmdLineOptions::useCRDelimiter() const
{
    return opt_delim_CR;
}
const string &CmdLineOptions::getDataDir() const
{
    return dataDir;
}
const vector<string> &CmdLineOptions::getPluginPaths() const
{
    return userPlugins;
}
bool CmdLineOptions::printOnlyStyle() const
{
    return opt_print_style;
}

string CmdLineOptions::getIndentScheme() const
{
    return StringTools::change_case ( indentScheme );
}

const string &CmdLineOptions::getSyntax() const
{
    return syntax;
}
const string&CmdLineOptions::getEncoding() const
{
    return encodingName;
}

const string& CmdLineOptions::getAnchorPrefix() const
{
    return anchorPrefix;
}

const string &CmdLineOptions::getPageSize() const
{
    return pageSize;
}

bool CmdLineOptions::printIndexFile() const
{
    return opt_printindex && ( outputType==highlight::HTML ||
                               outputType==highlight::XHTML );
}
bool CmdLineOptions::printProgress() const
{
    return opt_print_progress;
}
bool CmdLineOptions::fillLineNrZeroes() const
{
    return opt_fill_zeroes;
}
bool CmdLineOptions::syntaxGiven() const
{
    return opt_syntax;
}
bool CmdLineOptions::omitEncoding() const
{
    return StringTools::change_case ( encodingName ) =="none";
}
bool CmdLineOptions::forceOutput() const
{
    return opt_force_output;
}
bool CmdLineOptions::validateInput() const
{
    return opt_validate;
}
bool CmdLineOptions::numberWrappedLines() const
{
    return opt_number_wrapped_lines;
}
bool CmdLineOptions::inlineCSS() const
{
    return opt_inline_css;
}
bool CmdLineOptions::enclosePreTag() const
{
    return opt_enclose_pre;
}
bool CmdLineOptions::includeCharStyles() const
{
    return opt_char_styles;
}
bool CmdLineOptions::includePageColor() const
{
  return opt_page_color;
}
bool CmdLineOptions::disableTrailingNL() const
{
    return opt_no_trailing_nl;
}
bool CmdLineOptions::keepInjections() const
{
  return opt_keep_injections;
}
const string& CmdLineOptions::getDocumentTitle() const
{
    return docTitle;
}
highlight::WrapMode CmdLineOptions::getWrappingStyle() const
{
    return wrappingStyle;
}
const vector <string> & CmdLineOptions::getInputFileNames() const
{
    return inputFileNames;
}

void CmdLineOptions::readDirectory ( const string & wildcard )
{
    // get matching files, use  recursive search
    bool directoryOK=Platform::getDirectoryEntries ( inputFileNames, wildcard, true );
    if ( !directoryOK ) {
        cerr << "highlight: No files matched the pattern \""
             << wildcard << "\"."<< endl;
    }
}

string CmdLineOptions::validateDirPath ( const string & path )
{
    return ( path[path.length()-1] !=Platform::pathSeparator ) ?
           path+Platform::pathSeparator : path;
}

highlight::OutputType CmdLineOptions::getOutputType() const
{
    return outputType;
}

StringTools::KeywordCase CmdLineOptions::getKeywordCase() const
{
    return keywordCase;
}

bool CmdLineOptions::hasBaseFont() const
{
    return ( ! baseFont.empty() ) ;
}

const string& CmdLineOptions::getBaseFont() const
{
    return baseFont ;
}

const string& CmdLineOptions::getBaseFontSize() const
{
    return baseFontSize ;
}

const string& CmdLineOptions::getClassName() const
{
    return className ;
}

const string& CmdLineOptions::getStartNestedLang() const
{
    return startNestedLang;
}
const string& CmdLineOptions::getAbsThemePath() const
{
    return absThemePath;
}

const string& CmdLineOptions::getAbsLangPath() const
{
    return absLangPath;
}
const string& CmdLineOptions::getPluginReadFilePath() const
{
    return pluginParameter;
}
int CmdLineOptions::getNumberWidth()
{
    return lineNrWidth;
}

int CmdLineOptions::getLineLength()
{
    return lineLength;
}

int CmdLineOptions::getNumberStart()
{
    return lineNrStart;
}
