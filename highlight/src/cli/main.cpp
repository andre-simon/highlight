/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Die Apr 23 22:16:35 CEST 2002
    copyright            : (C) 2002-2016 by Andre Simon
    email                : andre.simon1@gmx.de

   Highlight is a universal source code to HTML converter. Syntax highlighting
   is formatted by Cascading Style Sheets. It's possible to easily enhance
   highlight's parsing database.

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
#include <algorithm>
#include <Diluculum/LuaState.hpp>

#include "main.h"
#include "../include/datadir.h"
#include "syntaxreader.h"

#define MAX_LINE__WIDTH       80

using namespace std;

void HLCmdLineApp::printVersionInfo()
{
    cout << "\n highlight version "
         << HIGHLIGHT_VERSION
         << "\n Copyright (C) 2002-2016 Andre Simon <andre.simon1 at gmx.de>"
         << "\n\n Argparser class"
         << "\n Copyright (C) 2006-2008 Antonio Diaz Diaz <ant_diaz at teleline.es>"
         << "\n\n Artistic Style Classes (2.05)"
         << "\n Copyright (C) 2006-2014 by Jim Pattee <jimp03 at email.com>"
         << "\n Copyright (C) 1998-2002 by Tal Davidson"
         << "\n\n Diluculum Lua wrapper (1.0)"
         << "\n Copyright (C) 2005-2013 by Leandro Motta Barros"
         << "\n\n xterm 256 color matching functions"
         << "\n Copyright (C) 2006 Wolfgang Frisch <wf at frexx.de>"

         << "\n\n This software is released under the terms of the GNU General "
         << "Public License."
         << "\n For more information about these matters, see the file named "
         << "COPYING.\n\n";
}

void HLCmdLineApp::printBadInstallationInfo()
{
    cerr << "highlight: Data directory not found ("<<DataDir::LSB_DATA_DIR<<")."
         " Bad installation or wrong "<< OPT_DATADIR << " parameter."
         << "\n\nCopy the highlight files into one of the directories listed "
         << "in INSTALL.\nYou may also set the data directory with "
         << OPT_DATADIR << ".\n";
}

int HLCmdLineApp::printInstalledFiles(const string& where, const string& wildcard, const string& what, const string&option)
{
    vector <string> filePaths;
    string searchDir = where + wildcard;

    bool directoryOK = Platform::getDirectoryEntries ( filePaths, searchDir, true );
    if ( !directoryOK ) {
        cerr << "highlight: Could not access directory "
             <<  searchDir
             << ", aborted.\n";
        return EXIT_FAILURE;
    }

    cout << "\nInstalled "<< what << " (located in " << where << "):\n\n";

    sort ( filePaths.begin(), filePaths.end() );
    string temp;

    for ( unsigned int i=0; i< filePaths.size(); i++ ) {
        temp = ( filePaths[i] ).substr ( where.length() );
        cout <<temp.substr ( 1, temp.length()- wildcard.length() ) << endl;
    }
    cout <<"\nUse name of the desired "<<what
         << " with the --" <<option<< " option." << endl;

    printConfigInfo();
    
    return EXIT_SUCCESS;
}


int HLCmdLineApp::printInstalledLanguages()
{
    vector <string> filePaths;
    string wildcard="*.lang";
    string directory=dataDir.getLangPath();
    string searchDir = directory + wildcard;

    bool directoryOK = Platform::getDirectoryEntries ( filePaths, searchDir, true );
    if ( !directoryOK ) {
        cerr << "highlight: Could not access directory "
             <<  searchDir
             << ", aborted.\n";
        return EXIT_FAILURE;
    }

    sort ( filePaths.begin(), filePaths.end() );
    string suffix, desc;
    cout << "\nInstalled language definitions"
         << " (located in " << directory << "):\n\n";

    for ( unsigned int i=0; i< filePaths.size(); i++ ) {
        Diluculum::LuaState ls;
        highlight::SyntaxReader::initLuaState(ls, filePaths[i],"");
        ls.doFile(filePaths[i]);
        desc = ls["Description"].value().asString();
        suffix = ( filePaths[i] ).substr ( directory.length() ) ;
        suffix = suffix.substr ( 1, suffix.length()- wildcard.length() );
        cout << setw ( 30 ) <<setiosflags ( ios::left ) <<desc<<": "<<suffix;
        int extCnt=0;
        for (StringMap::iterator it=extensions.begin(); it!=extensions.end(); it++) {
            if (it->second==suffix ) {
                cout << ((++extCnt==1)?" ( ":" ")<<it->first;
            }
        }
        cout << ((extCnt)?" )":"")<<endl;
    }
    cout <<"\nUse name of the desired language"
         << " with the --" OPT_SYNTAX " option." << endl;
         
    printConfigInfo();
         
    return EXIT_SUCCESS;
}

void HLCmdLineApp::printDebugInfo ( const highlight::SyntaxReader *lang,
                                    const string & langDefPath )
{
    if (!lang) return;
    cerr << "\nLoading language definition:\n" << langDefPath;
    cerr << "\n\nDescription: " << lang->getDescription();

    Diluculum::LuaState* luaState=lang->getLuaState();
    if (luaState) {
        cerr << "\n\nLUA GLOBALS:\n" ;
        Diluculum::LuaValueMap::iterator it;
        Diluculum::LuaValueMap glob =luaState->globals();
        for(it = glob.begin(); it != glob.end(); it++) {
            Diluculum::LuaValue first = it->first;
            Diluculum::LuaValue second = it->second;
            std::cerr << first.asString()<<": ";
            switch (second.type()) {
            case  LUA_TSTRING:
                cerr << "string [ "<<second.asString()<<" ]";
                break;
            case  LUA_TNUMBER:
                cerr << "number [ "<<second.asNumber()<<" ]";
                break;
            case  LUA_TBOOLEAN:
                cerr << "boolean [ "<<second.asBoolean()<<" ]";
                break;
            default:
                cerr << second.typeName();
            }
            cerr << endl;
        }

    }
    /*
    cerr << "\nREGEX:\n";
    highlight::RegexElement *re=NULL;
    for ( unsigned int i=0; i<lang->getRegexElements().size(); i++ )
    {
        re = lang->getRegexElements() [i];
        cerr << "State "<<re->open<<":\t"<<re->rex. <<"\n";
    }*/
    cerr << "\nKEYWORDS:\n";
    highlight::KeywordMap::iterator it;
    highlight::KeywordMap keys=lang->getKeywords();
    for ( it=keys.begin(); it!=keys.end(); it++ ) {
        cerr << " "<< it->first << "("<< it->second << ")";
    }
    cerr <<"\n\n";
}

void HLCmdLineApp::printConfigInfo ( )
{
    cout << "\nConfig file search directories:\n";
    dataDir.printConfigPaths();
    cout << "\nFiletype config file:\n"<<dataDir.getFiletypesConfPath ( "filetypes" ) <<"\n";
    cout << endl;
#ifdef HL_DATA_DIR
    cout << "Compiler directive HL_DATA_DIR = " <<HL_DATA_DIR<< "\n";
#endif
#ifdef HL_CONFIG_DIR
    cout << "Compiler directive HL_CONFIG_DIR = " <<HL_CONFIG_DIR<< "\n";
#endif
    cout << endl;
}

string HLCmdLineApp::getFileSuffix(const string& fileName)
{
    size_t ptPos=fileName.rfind(".");
    size_t psPos = fileName.rfind ( Platform::pathSeparator );
    if (ptPos == string::npos) {
        return  (psPos==string::npos) ? fileName : fileName.substr(psPos+1, fileName.length());
    }
    return (psPos!=string::npos && psPos>ptPos) ? "" : fileName.substr(ptPos+1, fileName.length());
}

bool HLCmdLineApp::loadFileTypeConfig ( const string& confName, StringMap* extMap, StringMap* shebangMap )
{
    if ( !extMap || !shebangMap ) return false;
    string confPath=dataDir.getFiletypesConfPath(confName);
    try {
        Diluculum::LuaState ls;
        Diluculum::LuaValueList ret= ls.doFile (confPath);

        int idx=1;
        string langName;
        Diluculum::LuaValue mapEntry;
        while ((mapEntry = ls["FileMapping"][idx].value()) !=Diluculum::Nil) {
            langName = mapEntry["Lang"].asString();
            if (mapEntry["Extensions"] !=Diluculum::Nil) {
                int extIdx=1;
                while (mapEntry["Extensions"][extIdx] !=Diluculum::Nil) {
                    extMap->insert ( make_pair ( mapEntry["Extensions"][extIdx].asString(),  langName ) );
                    extIdx++;
                }
            } else if (mapEntry["Shebang"] !=Diluculum::Nil) {
                shebangMap->insert ( make_pair ( mapEntry["Shebang"].asString(),  langName ) );
            }
            idx++;
        }

    } catch (Diluculum::LuaError err) {
        cerr <<err.what()<<"\n";
        return false;
    }
    return true;
}

int HLCmdLineApp::getNumDigits ( int i )
{
    int res=0;
    while ( i ) {
        i/=10;
        ++res;
    }
    return res;
}

void HLCmdLineApp::printProgressBar ( int total, int count )
{
    if ( !total ) return;
    int p=100*count / total;
    int numProgressItems=p/10;
    cout << "\r[";
    for ( int i=0; i<10; i++ ) {
        cout << ( ( i<numProgressItems ) ?"#":" " );
    }
    cout<< "] " <<setw ( 3 ) <<p<<"%, "<<count << " / " << total << "  " <<flush;
    if ( p==100 ) {
        cout << endl;
    }
}

void HLCmdLineApp::printCurrentAction ( const string&outfilePath,
                                        int total, int count, int countWidth )
{
    cout << "Writing file "
         << setw ( countWidth ) << count
         << " of "
         << total
         << ": "
         << outfilePath
         << "\n";
}

void HLCmdLineApp::printIOErrorReport ( unsigned int numberErrorFiles,
                                        vector<string> & fileList,
                                        const string &action )
{
    cerr << "highlight: Could not "
         << action
         << " file"
         << ( ( numberErrorFiles>1 ) ?"s":"" ) <<":\n";
    copy ( fileList.begin(), fileList.end(), ostream_iterator<string> ( cerr, "\n" ) );
    if ( fileList.size() < numberErrorFiles ) {
        cerr << "... ["
             << ( numberErrorFiles - fileList.size() )
             << " of "
             << numberErrorFiles
             << " failures not shown, use --"
             << OPT_VERBOSE
             << " switch to print all failures]\n";
    }
}

string HLCmdLineApp::analyzeFile ( const string& file )
{
    string firstLine;
    if ( !file.empty() ) {
        ifstream inFile ( file.c_str() );
        getline ( inFile, firstLine );
    } else {
        //  This copies all the data to a new buffer, uses the data to get the
        //  first line, then makes cin use the new buffer that underlies the
        //  stringstream instance
        cin_bufcopy << cin.rdbuf();
        getline ( cin_bufcopy, firstLine );
        cin_bufcopy.seekg ( 0, ios::beg );
        cin.rdbuf ( cin_bufcopy.rdbuf() );
    }
    StringMap::iterator it;
    boost::xpressive::sregex rex;
    boost::xpressive::smatch what;
    for ( it=scriptShebangs.begin(); it!=scriptShebangs.end(); it++ ) {
        rex = boost::xpressive::sregex::compile( it->first );
        if ( boost::xpressive::regex_search( firstLine, what, rex )  ) return it->second;
    }
    return "";
}

string HLCmdLineApp::guessFileType ( const string& suffix, const string &inputFile, bool useUserSuffix )
{
    string lcSuffix = StringTools::change_case(suffix);
    if (extensions.count(lcSuffix)) {
        return extensions[lcSuffix];
    }

    if (!useUserSuffix) {
        string shebang =  analyzeFile(inputFile);
        if (!shebang.empty()) return shebang;
    }
    return lcSuffix;
}

vector <string> HLCmdLineApp::collectPluginPaths(const vector<string>& plugins)
{
    vector<string> absolutePaths;
    for (unsigned int i=0; i<plugins.size(); i++) {
        if (Platform::fileExists(plugins[i])) {
            absolutePaths.push_back(plugins[i]);
        } else {
            absolutePaths.push_back(dataDir.getPluginPath(plugins[i]+".lua"));
        }
    }
    return absolutePaths;
}

int HLCmdLineApp::run ( const int argc, const char*argv[] )
{
    CmdLineOptions options ( argc, argv );

    // set data directory path, where /langDefs and /themes reside
    string dataDirPath = ( options.getDataDir().empty() ) ?  Platform::getAppPath() :options.getDataDir();

    if ( options.printVersion() ) {
        printVersionInfo();
        return EXIT_SUCCESS;
    }

    dataDir.initSearchDirectories ( dataDirPath );

    if ( options.printHelp() ) {
        Help::printHelp();
        return EXIT_SUCCESS;
    }

    if ( options.printConfigInfo() ) {
        printConfigInfo ( );
        return EXIT_SUCCESS;
    }

    if ( options.showThemes() ) {
        return printInstalledFiles(dataDir.getThemePath(), "*.theme", "themes", OPT_STYLE) ?EXIT_SUCCESS:EXIT_FAILURE;
    }
    if ( options.showPlugins() ) {
        return printInstalledFiles(dataDir.getPluginPath(), "*.lua", "plug-ins", OPT_PLUGIN) ?EXIT_SUCCESS:EXIT_FAILURE;
    }

    //call before printInstalledLanguages!
    loadFileTypeConfig ( "filetypes", &extensions, &scriptShebangs );

    if ( options.showLangdefs() ) {
        return printInstalledLanguages();
    }

    const vector <string> inFileList=options.getInputFileNames();

    if ( options.enableBatchMode() && inFileList[0].empty() ) {
        return EXIT_FAILURE;
    }

    string themePath=options.getAbsThemePath().empty() ? dataDir.getThemePath ( options.getThemeName() ): options.getAbsThemePath();

    unique_ptr<highlight::CodeGenerator> generator ( highlight::CodeGenerator::getInstance ( options.getOutputType() ) );

    generator->setHTMLAttachAnchors ( options.attachLineAnchors() );
    generator->setHTMLOrderedList ( options.orderedList() );
    generator->setHTMLInlineCSS ( options.inlineCSS() );
    generator->setHTMLEnclosePreTag ( options.enclosePreTag() );
    generator->setHTMLAnchorPrefix ( options.getAnchorPrefix() );
    generator->setHTMLClassName ( options.getClassName() );

    generator->setLATEXReplaceQuotes ( options.replaceQuotes() );
    generator->setLATEXNoShorthands ( options.disableBabelShorthands() );
    generator->setLATEXPrettySymbols ( options.prettySymbols() );

    generator->setRTFPageSize ( options.getPageSize() );
    generator->setRTFCharStyles ( options.includeCharStyles() );
    generator->setRTFPageColor ( options.includePageColor() );
    
    generator->setSVGSize ( options.getSVGWidth(),  options.getSVGHeight() );

    if (options.useCRDelimiter())
        generator->setEOLDelimiter('\r');

    generator->setValidateInput ( options.validateInput() );
    generator->setNumberWrappedLines ( options.numberWrappedLines() );

    generator->setStyleInputPath ( options.getStyleInFilename() );
    generator->setStyleOutputPath ( options.getStyleOutFilename() );
    generator->setIncludeStyle ( options.includeStyleDef() );
    generator->setPrintLineNumbers ( options.printLineNumbers(), options.getNumberStart() );
    generator->setPrintZeroes ( options.fillLineNrZeroes() );
    generator->setFragmentCode ( options.fragmentOutput() );
    generator->setKeepInjections ( options.keepInjections());
    generator->setPreformatting ( options.getWrappingStyle(),
                                  ( generator->getPrintLineNumbers() ) ?
                                  options.getLineLength() - options.getNumberWidth() : options.getLineLength(),
                                  options.getNumberSpaces() );

    generator->setEncoding ( options.getEncoding() );
    generator->setBaseFont ( options.getBaseFont() ) ;
    generator->setBaseFontSize ( options.getBaseFontSize() ) ;
    generator->setLineNumberWidth ( options.getNumberWidth() );
    generator->setStartingNestedLang( options.getStartNestedLang());
    generator->disableTrailingNL(options.disableTrailingNL());
    generator->setPluginReadFile(options.getPluginReadFilePath());

    bool styleFileWanted = !options.fragmentOutput() || options.styleOutPathDefined();

    const  vector <string> pluginFileList=collectPluginPaths( options.getPluginPaths());
    for (unsigned int i=0; i<pluginFileList.size(); i++) {
        if ( !generator->initPluginScript(pluginFileList[i]) ) {
            cerr << "highlight: "
                 << generator->getPluginScriptError()
                 << " in "
                 << pluginFileList[i]
                 <<"\n";
            return EXIT_FAILURE;
        }
    }

    if ( !generator->initTheme ( themePath ) ) {
        cerr << "highlight: "
             << generator->getThemeInitError()
             << "\n";
        return EXIT_FAILURE;
    }

    if ( options.printOnlyStyle() ) {
        if (!options.formatSupportsExtStyle()) {
            cerr << "highlight: output format supports no external styles.\n";
            return EXIT_FAILURE;
        }
        bool useStdout =  options.getStyleOutFilename() =="stdout";
        string cssOutFile=options.getOutDirectory()  + options.getStyleOutFilename();
        bool success=generator->printExternalStyle ( useStdout?"":cssOutFile );
        if ( !success ) {
            cerr << "highlight: Could not write " << cssOutFile <<".\n";
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    bool formattingEnabled = generator->initIndentationScheme ( options.getIndentScheme() );

    if ( !formattingEnabled && !options.getIndentScheme().empty() ) {
        cerr << "highlight: Undefined indentation scheme "
             << options.getIndentScheme()
             << ".\n";
        return EXIT_FAILURE;
    }

    string outDirectory = options.getOutDirectory();
#ifndef WIN32
    ifstream dirTest ( outDirectory.c_str() );
    if ( !outDirectory.empty() && !options.quietMode() && !dirTest ) {
        cerr << "highlight: Output directory \""
             << outDirectory
             << "\" does not exist.\n";
        return EXIT_FAILURE;
    }
    dirTest.close();
#endif

    bool initError=false, IOError=false;
    unsigned int fileCount=inFileList.size(),
                 fileCountWidth=getNumDigits ( fileCount ),
                 i=0,
                 numBadFormatting=0,
                 numBadInput=0,
                 numBadOutput=0;

    vector<string> badFormattedFiles, badInputFiles, badOutputFiles;
    std::set<string> usedFileNames;
    string inFileName, outFilePath;
    string suffix, lastSuffix;

    if ( options.syntaxGiven() ) { // user defined language definition, valid for all files
        suffix = guessFileType ( options.getSyntax(), "", true );
    }

    while ( i < fileCount && !initError ) {
        if ( !options.syntaxGiven() ) { // determine file type for each file
            suffix = guessFileType ( getFileSuffix ( inFileList[i] ), inFileList[i] );
        }
        if ( suffix.empty()  && options.forceOutput()) suffix="txt"; //avoid segfault
        if ( suffix.empty() ) {
            if ( !options.enableBatchMode() )
                cerr << "highlight: Undefined language definition. Use --"
                     << OPT_SYNTAX << " option.\n";
            if ( !options.forceOutput() ) {
                initError = true;
                break;
            }
        }

        if ( suffix != lastSuffix ) {
            string langDefPath=options.getAbsLangPath().empty() ? dataDir.getLangPath ( suffix+".lang" ) : options.getAbsLangPath();

            highlight::LoadResult loadRes= generator-> loadLanguage( langDefPath );

            if ( loadRes==highlight::LOAD_FAILED_REGEX ) {
                cerr << "highlight: Regex error ( "
                     << generator->getSyntaxRegexError()
                     << " ) in "<<suffix<<".lang\n";
                initError = true;
                break;
            } else if ( loadRes==highlight::LOAD_FAILED_LUA ) {
                cerr << "highlight: Lua error ( "
                     << generator->getSyntaxLuaError()
                     << " ) in "<<suffix<<".lang\n";
                initError = true;
                break;
            } else if ( loadRes==highlight::LOAD_FAILED ) {
                // do also ignore error msg if --syntax parameter should be skipped
                if ( ! (options.quietMode() || options.isSkippedExt ( suffix )) ) {
                    cerr << "highlight: Unknown source file extension \""
                         << suffix
                         << "\". Consider using the --skip option.\n";
                }
                if ( !options.forceOutput() ) {
                    initError = true;
                    break;
                }
            }
            if ( options.printDebugInfo() && loadRes==highlight::LOAD_OK ) {
                printDebugInfo ( generator->getSyntaxReader(), langDefPath );
            }
            lastSuffix = suffix;
        }

        string::size_type pos= ( inFileList[i] ).find_last_of ( Platform::pathSeparator );
        inFileName = inFileList[i].substr ( pos+1 );

        if ( options.enableBatchMode() ) {
            if (usedFileNames.count(inFileName)) {
                string prefix=inFileList[i].substr (0, pos+1 );
                replace (prefix.begin(), prefix.end(), Platform::pathSeparator, '_');
                inFileName.insert(0, prefix);
            } else {
                usedFileNames.insert(inFileName);
            }
            outFilePath = outDirectory;
            outFilePath += inFileName;
            outFilePath += options.getOutFileSuffix();

            if ( !options.quietMode() ) {
                if ( options.printProgress() ) {
                    printProgressBar ( fileCount, i+1 );
                } else {
                    printCurrentAction ( outFilePath, fileCount, i+1, fileCountWidth );
                }
            }
        } else {
            outFilePath = options.getSingleOutFilename();
            if ( outFilePath.size() && outFilePath==options.getSingleInFilename() ) {
                cerr 	<< "highlight: Output path equals input path: \""
                        << outFilePath << "\".\n";
                initError = true;
                break;
            }

        }

        if ( options.useFNamesAsAnchors() ) {
            generator->setHTMLAnchorPrefix ( inFileName );
        }

        generator->setTitle ( options.getDocumentTitle().empty() ?
                              inFileList[i]:options.getDocumentTitle() );

        generator->setKeyWordCase ( options.getKeywordCase() );

        highlight::ParseError error = generator->generateFile ( inFileList[i], outFilePath );

        if ( error==highlight::BAD_INPUT ) {
            if ( numBadInput++ < IO_ERROR_REPORT_LENGTH || options.printDebugInfo() ) {
                badInputFiles.push_back ( inFileList[i] );
            }
        } else if ( error==highlight::BAD_OUTPUT ) {
            if ( numBadOutput++ < IO_ERROR_REPORT_LENGTH || options.printDebugInfo() ) {
                badOutputFiles.push_back ( outFilePath );
            }
        }
        if ( formattingEnabled && !generator->formattingIsPossible() ) {
            if ( numBadFormatting++ < IO_ERROR_REPORT_LENGTH || options.printDebugInfo() ) {
                badFormattedFiles.push_back ( outFilePath );
            }
        }
        ++i;
    }

    if ( i  && !options.includeStyleDef()
            && styleFileWanted
            && options.formatSupportsExtStyle() ) {
        string cssOutFile=outDirectory  + options.getStyleOutFilename();
        bool success=generator->printExternalStyle ( cssOutFile );
        if ( !success ) {
            cerr << "highlight: Could not write " << cssOutFile <<".\n";
            IOError = true;
        }
    }

    if ( i && options.printIndexFile() ) {
        bool success=generator -> printIndexFile ( inFileList, outDirectory );
        if ( !success ) {
            cerr << "highlight: Could not write index file.\n";
            IOError = true;
        }
    }

    if ( numBadInput ) {
        printIOErrorReport ( numBadInput, badInputFiles, "read input" );
        IOError = true;
    }
    if ( numBadOutput ) {
        printIOErrorReport ( numBadOutput, badOutputFiles, "write output" );
        IOError = true;
    }
    if ( numBadFormatting ) {
        printIOErrorReport ( numBadFormatting, badFormattedFiles, "reformat" );
    }
    return ( initError || IOError ) ? EXIT_FAILURE : EXIT_SUCCESS;
}

int main ( const int argc, const char *argv[] )
{
    HLCmdLineApp app;
    return app.run ( argc, argv );
}
