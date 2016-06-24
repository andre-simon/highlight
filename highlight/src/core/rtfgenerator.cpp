/***************************************************************************
                          rtfcode.cpp  -  description
                             -------------------
    begin                : Die Jul 9 2002
    copyright            : (C) 2002-2016 by Andre Simon
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


#include <sstream>
#include <string>

#include "charcodes.h"
#include "version.h"
#include "rtfgenerator.h"

namespace highlight
{

RtfGenerator::RtfGenerator()
    : CodeGenerator ( RTF ),
      pageSize ( "a4" ), // Default: DIN A4
      addCharStyles ( false ),
      addPageColor(false),
      isUtf8(false),
      utf16Char(0),
      utf8SeqLen(0)
{
    newLineTag = "}\\par\\pard\n\\cbpat1{";
    spacer = " ";
   
    // Page dimensions
    psMap["a3"] = PageSize ( 16837,23811 );
    psMap["a4"] = PageSize ( 11905,16837 );
    psMap["a5"] = PageSize ( 8390,11905 );

    psMap["b4"] = PageSize ( 14173,20012 );
    psMap["b5"] = PageSize ( 9977,14173 );
    psMap["b6"] = PageSize ( 7086,9977 );

    psMap["letter"] = PageSize ( 12240,15840 );
    psMap["legal"] = PageSize ( 12240,20163 );
}

RtfGenerator::~RtfGenerator()
{}

string RtfGenerator::getHeader()
{
    return string();
}

string RtfGenerator::getAttributes ( const ElementStyle & col )
{
    stringstream s;
    s  << "\\red"<< col.getColour().getRed ( RTF )
       << "\\green"<<col.getColour().getGreen ( RTF )
       << "\\blue"<<col.getColour().getBlue ( RTF )
       << ";";
    return s.str();
}

string  RtfGenerator::getOpenTag ( int styleNumber,const ElementStyle & elem )
{
    ostringstream s;
    s << "{";
    if ( addCharStyles ) {
        s<<"\\*\\cs"<< ( styleNumber+2 );
    }
    s << "\\cf"<< ( styleNumber+2 ) <<"{";

    if ( elem.isBold() ) s << "\\b ";
    if ( elem.isItalic() ) s << "\\i ";
    if ( elem.isUnderline() ) s << "\\ul ";
    return  s.str();
}


string  RtfGenerator::getCloseTag ( const ElementStyle &elem )
{
    ostringstream s;
    if ( elem.isBold() ) s << "\\b0 ";
    if ( elem.isItalic() ) s << "\\i0 ";
    if ( elem.isUnderline() ) s << "\\ul0 ";
    s << "}}";
    return  s.str();
}

string RtfGenerator::getCharStyle ( int styleNumber,const ElementStyle &elem,
                                    const string&styleName )
{
    ostringstream s;
    s << "{\\*\\cs"<< ( styleNumber+2 ) <<"\\additive\\cf"<< ( styleNumber+2 )
      << "\\f1\\fs";
    int fontSize=0;
    StringTools::str2num<int> ( fontSize, this->getBaseFontSize(), std::dec );
    s << ( ( fontSize ) ? fontSize*2: 20 );  // RTF needs double amount
    if ( elem.isBold() ) s << "\\b";
    if ( elem.isItalic() ) s << "\\i";
    if ( elem.isUnderline() ) s << "\\ul";
    s << "\\sbasedon222\\snext0 "<< styleName << ";}\n";
    return  s.str();
}
// {\*\cs2\additive\cf2\f1\fs20\sbasedon222\snext0 HL Default;}


void RtfGenerator::printBody()
{
  isUtf8 = StringTools::change_case ( encoding ) == "utf-8";
  
  *out << "{\\rtf1\\ansi \\deff1" // drop \\uc0 because of unicode output
         << "{\\fonttbl{\\f1\\fmodern\\fprq1\\fcharset0 " ;
    *out << this->getBaseFont() ;
    *out << ";}}"
         << "{\\colortbl;";

    *out << "\\red"    << ( docStyle.getBgColour().getRed ( RTF ) );
    *out << "\\green"    << ( docStyle.getBgColour().getGreen ( RTF ) );
    *out << "\\blue"    << ( docStyle.getBgColour().getBlue ( RTF ) );
    *out << ";";

    *out << getAttributes ( docStyle.getDefaultStyle() );

    *out << getAttributes ( docStyle.getStringStyle() );
    *out << getAttributes ( docStyle.getNumberStyle() );
    *out << getAttributes ( docStyle.getSingleLineCommentStyle() );

    *out << getAttributes ( docStyle.getCommentStyle() );
    *out << getAttributes ( docStyle.getEscapeCharStyle() );
    *out << getAttributes ( docStyle.getPreProcessorStyle() );

    *out << getAttributes ( docStyle.getPreProcStringStyle() );
    *out << getAttributes ( docStyle.getLineStyle() );
    *out << getAttributes ( docStyle.getOperatorStyle() );
    *out << getAttributes ( docStyle.getInterpolationStyle() );

    /* For output formats which can refer to external styles it is more safe
       to use the colour theme's keyword class names, since the language
       definitions (which may change during a batch conversion) do not have to define
       all keyword classes, that are needed to highlight all input files correctly.
       It is ok for RTF to use the language definition's class names, because RTF
       does not refer to external styles.
       We cannot use the theme's class names, because KSIterator returns an
       alphabetically ordered list, which is not good because RTF is dependent
       on the order. We access the keyword style with an ID, which is calculated
       ignoring the alphabetic order.
    */
    vector<string>  keywordClasses = currentSyntax->getKeywordClasses();
    for ( unsigned int i=0; i<keywordClasses.size(); i++ ) {
        *out << getAttributes ( docStyle.getKeywordStyle ( keywordClasses[i] ) );
    }

    *out << "}\n";

    if ( addCharStyles ) {
        *out << "{\\stylesheet{\n";
        *out << getCharStyle ( STANDARD, docStyle.getDefaultStyle(), "HL Default" );
        *out << getCharStyle ( STRING, docStyle.getStringStyle(), "HL String" );
        *out << getCharStyle ( NUMBER, docStyle.getNumberStyle(), "HL Number" );
        *out << getCharStyle ( SL_COMMENT, docStyle.getSingleLineCommentStyle(), "HL SL Comment" );
        *out << getCharStyle ( ML_COMMENT, docStyle.getCommentStyle(), "HL ML Comment" );
        *out << getCharStyle ( ESC_CHAR, docStyle.getEscapeCharStyle(), "HL Escape Character" );
        *out << getCharStyle ( DIRECTIVE, docStyle.getPreProcessorStyle(), "HL Directive" );
        *out << getCharStyle ( DIRECTIVE_STRING, docStyle.getPreProcStringStyle(), "HL Directive String" );
        *out << getCharStyle ( LINENUMBER, docStyle.getLineStyle(), "HL Line" );
        *out << getCharStyle ( SYMBOL, docStyle.getOperatorStyle(), "HL Operator" );
        *out << getCharStyle ( STRING_INTERPOLATION, docStyle.getInterpolationStyle(), "HL Interpolation" );
        char styleName[20];
        for ( unsigned int i=0; i<keywordClasses.size(); i++ ) {
            sprintf ( styleName, "HL Keyword %c", 'A'+i ); //maybe better simple numbering
            *out << getCharStyle ( KEYWORD+i, docStyle.getKeywordStyle ( keywordClasses[i] ), string ( styleName ) );
        }
        *out << "}}\n";
    }
    
    if (addPageColor) {
      long svVal =  docStyle.getBgColour().getRed() +  
                    docStyle.getBgColour().getGreen() * 256+ 
                    docStyle.getBgColour().getBlue() * 256 * 256;
                    *out<<"\\viewbksp1\\ilfomacatclnup0{\\*\\background{\\shp{{\\sp{\\sn fillColor}{\\sv "<<svVal<<"}}}}}\n";
    }

    *out  << "\\paperw"<< psMap[pageSize].width <<"\\paperh"<< psMap[pageSize].height
          << "\\margl1134\\margr1134\\margt1134\\margb1134\\sectd" // page margins
          << "\\plain\\f1\\fs" ;  // Font formatting
    int fontSize=0;
    StringTools::str2num<int> ( fontSize, this->getBaseFontSize(), std::dec );
    *out << ( ( fontSize ) ? fontSize*2: 20 );  // RTF needs double amount
    *out << "\n\\pard \\cbpat1{";

    processRootState();

    *out << "}}"<<endl;
}

string RtfGenerator::getFooter()
{
    return string();
}

void RtfGenerator::initOutputTags ( )
{
    openTags.push_back ( getOpenTag ( STANDARD, docStyle.getDefaultStyle() ) );
    openTags.push_back ( getOpenTag ( STRING, docStyle.getStringStyle() ) );
    openTags.push_back ( getOpenTag ( NUMBER, docStyle.getNumberStyle() ) );
    openTags.push_back ( getOpenTag ( SL_COMMENT, docStyle.getSingleLineCommentStyle() ) );
    openTags.push_back ( getOpenTag ( ML_COMMENT,docStyle.getCommentStyle() ) );
    openTags.push_back ( getOpenTag ( ESC_CHAR, docStyle.getEscapeCharStyle() ) );
    openTags.push_back ( getOpenTag ( DIRECTIVE, docStyle.getPreProcessorStyle() ) );
    openTags.push_back ( getOpenTag ( DIRECTIVE_STRING, docStyle.getPreProcStringStyle() ) );
    openTags.push_back ( getOpenTag ( LINENUMBER, docStyle.getLineStyle() ) );
    openTags.push_back ( getOpenTag ( SYMBOL, docStyle.getOperatorStyle() ) );
    openTags.push_back ( getOpenTag ( STRING_INTERPOLATION, docStyle.getInterpolationStyle()) );

    closeTags.push_back ( getCloseTag ( docStyle.getDefaultStyle() ) );
    closeTags.push_back ( getCloseTag ( docStyle.getStringStyle() ) );
    closeTags.push_back ( getCloseTag ( docStyle.getNumberStyle() ) );
    closeTags.push_back ( getCloseTag ( docStyle.getSingleLineCommentStyle() ) );
    closeTags.push_back ( getCloseTag ( docStyle.getCommentStyle() ) );
    closeTags.push_back ( getCloseTag ( docStyle.getEscapeCharStyle() ) );
    closeTags.push_back ( getCloseTag ( docStyle.getPreProcessorStyle() ) );
    closeTags.push_back ( getCloseTag ( docStyle.getPreProcStringStyle() ) );
    closeTags.push_back ( getCloseTag ( docStyle.getLineStyle() ) );
    closeTags.push_back ( getCloseTag ( docStyle.getOperatorStyle() ) );
    closeTags.push_back ( getCloseTag ( docStyle.getInterpolationStyle() ) );
}

string RtfGenerator::maskCharacter ( unsigned char c )
{  
  if (isUtf8 && c > 0x7f  && utf8SeqLen==0){
    
    //http://stackoverflow.com/questions/7153935/how-to-convert-utf-8-stdstring-to-utf-16-stdwstring
    
    if (c <= 0xDF)
    {
      utf16Char = c&0x1F;
      utf8SeqLen = 1;
    }
    else if (c <= 0xEF)
    {
      utf16Char = c&0x0F;
      utf8SeqLen = 2;
    }
    else if (c <= 0xF7)
    {
      utf16Char = c&0x07;
      utf8SeqLen = 3;
    } else {
      utf8SeqLen = 0;
    }
    return "";
  }
  
  if (utf8SeqLen) {
      utf16Char <<= 6;
      utf16Char += c & 0x3f;   
      --utf8SeqLen;
    
      if (!utf8SeqLen){
        string m ( "\\u" );
        m += to_string(utf16Char);
        m += '?';
        utf16Char=0L;
        return m;
      } else {
        return "";
      }
  }
  
  switch ( c ) {
    case '}' :
    case '{' :
    case '\\' : {
          string m ( "\\" );
          return m += c;
        }
        break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
          string m ( 1, '{' );
          m += c;
          m += '}';
          return m;
        }
        break;

    case AUML_LC:
        return "\\'e4";
        break;
    case OUML_LC:
        return "\\'f6";
        break;
    case UUML_LC:
        return "\\'fc";
        break;
    case AUML_UC:
        return "\\'c4";
        break;
    case OUML_UC:
        return "\\'d6";
        break;
    case UUML_UC:
        return "\\'dc";
        break;

    case AACUTE_LC:
        return "\\'e1";
        break;
    case EACUTE_LC:
        return "\\'e9";
        break;
    case OACUTE_LC:
        return "\\'f3";
        break;
    case UACUTE_LC:
        return "\\'fa";
        break;

    case AGRAVE_LC:
        return "\\'e0";
        break;
    case EGRAVE_LC:
        return "\\'e8";
        break;
    case OGRAVE_LC:
        return "\\'f2";
        break;
    case UGRAVE_LC:
        return "\\'f9";
        break;

    case AACUTE_UC:
        return "\\'c1";
        break;
    case EACUTE_UC:
        return "\\'c9";
        break;
    case OACUTE_UC:
        return "\\'d3";
        break;
    case UACUTE_UC:
        return "\\'da";
        break;
    case AGRAVE_UC:
        return "\\'c0";
        break;
    case EGRAVE_UC:
        return "\\'c8";
        break;
    case OGRAVE_UC:
        return "\\'d2";
        break;
    case UGRAVE_UC:
        return "\\'d9";
        break;

    case SZLIG:
        return "\\'df";
        break;

    default :
        return string ( 1, c );
    }
}

string RtfGenerator::getKeywordOpenTag ( unsigned int styleID )
{
    return getOpenTag ( KEYWORD+styleID,
                        docStyle.getKeywordStyle ( currentSyntax->getKeywordClasses() [styleID] ) );
}

string RtfGenerator::getKeywordCloseTag ( unsigned int styleID )
{
    return getCloseTag ( docStyle.getKeywordStyle ( currentSyntax->getKeywordClasses() [styleID] ) );
}

void RtfGenerator::setRTFPageSize ( const string & ps )
{
    if ( psMap.count ( ps ) ) pageSize = ps;
}

void RtfGenerator::setRTFCharStyles ( bool cs )
{
    addCharStyles = cs;
}

void RtfGenerator::setRTFPageColor ( bool pc )
{
  addPageColor = pc;
}


}
