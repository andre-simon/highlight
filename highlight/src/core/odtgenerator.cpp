/***************************************************************************
                    odtgenerator.cpp  -  description
                             -------------------
    begin                : Jul 19 2012
    copyright            : (C) 2012 by Andre Simon
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
#include "odtgenerator.h"

namespace highlight
{

ODTGenerator::ODTGenerator() : CodeGenerator ( ODTFLAT )
{
    newLineTag = "</text:p>\n<text:p text:style-name=\"Standard\">";
    spacer = "<text:s text:c=\"1\"/>"; // TODO insert number of spaces
    maskWs=true;
    if ( !preFormatter.getReplaceTabs()) { // TODO use <text:tab/>

        preFormatter.setReplaceTabs ( true );
        preFormatter.setNumberSpaces ( 4 );
    }
}

ODTGenerator::~ODTGenerator() {}

string ODTGenerator::getStyleDefinition()
{
    if ( styleDefinitionCache.empty() ) {
        ostringstream os;

        os  << "<office:styles>\n<style:style style:name=\"Standard\" style:family=\"paragraph\" style:class=\"text\">\n"
            << "  <style:text-properties style:font-name=\""<< getBaseFont()<<"\" fo:font-size=\""<<getBaseFontSize()<<"pt\"/>"
            << "</style:style>\n</office:styles>"
            << "<office:automatic-styles>\n";

        os << getAttributes ( STY_NAME_STD, docStyle.getDefaultStyle() )
           << getAttributes ( STY_NAME_NUM, docStyle.getNumberStyle() )
           << getAttributes ( STY_NAME_ESC, docStyle.getEscapeCharStyle() )
           << getAttributes ( STY_NAME_STR, docStyle.getStringStyle() )
           << getAttributes ( STY_NAME_DST, docStyle.getPreProcStringStyle() )
           << getAttributes ( STY_NAME_SLC, docStyle.getSingleLineCommentStyle() )
           << getAttributes ( STY_NAME_COM, docStyle.getCommentStyle() )
           << getAttributes ( STY_NAME_DIR, docStyle.getPreProcessorStyle() )
           << getAttributes ( STY_NAME_SYM, docStyle.getOperatorStyle() )
           << getAttributes ( STY_NAME_IPL, docStyle.getInterpolationStyle() )
           << getAttributes ( STY_NAME_LIN, docStyle.getLineStyle() );

        KeywordStyles styles = docStyle.getKeywordStyles();
        for ( KSIterator it=styles.begin(); it!=styles.end(); it++ ) {
            os << getAttributes (it->first, it->second );
        }

        os << "<style:page-layout style:name=\"pm1\">"
           << "<style:page-layout-properties fo:page-width=\"21.001cm\" fo:page-height=\"29.7cm\" style:num-format=\"1\""
           << " style:print-orientation=\"portrait\" fo:margin=\"2cm\" fo:margin-top=\"2cm\" fo:margin-bottom=\"2cm\" fo:margin-left=\"2cm\" fo:margin-right=\"2cm\" "
           << "fo:background-color=\"#"
           << ( docStyle.getBgColour().getRed ( HTML ) )
           << ( docStyle.getBgColour().getGreen ( HTML ) )
           << ( docStyle.getBgColour().getBlue ( HTML ) )
           << "\" style:writing-mode=\"lr-tb\" style:footnote-max-height=\"0cm\">\n"
           << " <style:background-image/>\n"
           << " <style:footnote-sep style:width=\"0.018cm\" style:distance-before-sep=\"0.101cm\" style:distance-after-sep=\"0.101cm\" style:line-style=\"solid\""
           << " style:adjustment=\"left\" style:rel-width=\"25%\" style:color=\"#000000\"/>"
           << "</style:page-layout-properties>\n"
           << "<style:header-style/>\n"
           << "<style:footer-style/>\n"
           << "</style:page-layout>\n"
           << "</office:automatic-styles>\n"
           << "<office:master-styles>\n"
           << " <style:master-page style:name=\"Standard\" style:page-layout-name=\"pm1\"/>\n"
           << "</office:master-styles>\n";

        styleDefinitionCache=os.str();
    }
    return styleDefinitionCache;
}

string ODTGenerator::getAttributes ( const string & elemName,
                                     const ElementStyle & elem )
{
    ostringstream s;
    s << "<style:style style:name=\""<<elemName<<"\" style:family=\"text\">\n"
      << "<style:text-properties fo:color=\"#"
      << ( elem.getColour().getRed ( HTML ) )
      << ( elem.getColour().getGreen ( HTML ) )
      << ( elem.getColour().getBlue ( HTML ) )
      << "\""
      << ( elem.isBold() ?     " fo:font-weight=\"bold\"" :"" )
      << ( elem.isItalic() ?   " fo:font-style=\"italic\"" :"" )
      << ( elem.isUnderline() ? " style:text-underline-style=\"solid\" style:text-underline-width=\"auto\" style:text-underline-color=\"font-color\"" :"" )
      <<"/>\n</style:style>\n" ;

    return s.str();
}

string ODTGenerator::getHeader()
{
    ostringstream header;
    header << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"\
           "<office:document xmlns:office=\"urn:oasis:names:tc:opendocument:xmlns:office:1.0\" xmlns:style=\"urn:oasis:names:tc:opendocument:xmlns:style:1.0\""\
           " xmlns:text=\"urn:oasis:names:tc:opendocument:xmlns:text:1.0\" xmlns:table=\"urn:oasis:names:tc:opendocument:xmlns:table:1.0\""\
           " xmlns:draw=\"urn:oasis:names:tc:opendocument:xmlns:drawing:1.0\" xmlns:fo=\"urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0\""\
           " xmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns:dc=\"http://purl.org/dc/elements/1.1/\" xmlns:meta=\"urn:oasis:names:tc:opendocument:xmlns:meta:1.0\""\
           " xmlns:number=\"urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0\" xmlns:svg=\"urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0\""\
           " xmlns:chart=\"urn:oasis:names:tc:opendocument:xmlns:chart:1.0\" xmlns:dr3d=\"urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0\""\
           " xmlns:math=\"http://www.w3.org/1998/Math/MathML\" xmlns:form=\"urn:oasis:names:tc:opendocument:xmlns:form:1.0\" xmlns:script=\"urn:oasis:names:tc:opendocument:xmlns:script:1.0\""\
           " xmlns:config=\"urn:oasis:names:tc:opendocument:xmlns:config:1.0\" xmlns:ooo=\"http://openoffice.org/2004/office\" xmlns:ooow=\"http://openoffice.org/2004/writer\""\
           " xmlns:oooc=\"http://openoffice.org/2004/calc\" xmlns:dom=\"http://www.w3.org/2001/xml-events\" xmlns:xforms=\"http://www.w3.org/2002/xforms\""\
           " xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:rpt=\"http://openoffice.org/2005/report\""\
           " xmlns:of=\"urn:oasis:names:tc:opendocument:xmlns:of:1.2\" xmlns:xhtml=\"http://www.w3.org/1999/xhtml\" xmlns:grddl=\"http://www.w3.org/2003/g/data-view#\""\
           " xmlns:tableooo=\"http://openoffice.org/2009/table\" xmlns:field=\"urn:openoffice:names:experimental:ooo-ms-interop:xmlns:field:1.0\""\
           " xmlns:formx=\"urn:openoffice:names:experimental:ooxml-odf-interop:xmlns:form:1.0\" xmlns:css3t=\"http://www.w3.org/TR/css3-text/\" office:version=\"1.2\""\
           " office:mimetype=\"application/vnd.oasis.opendocument.text\">\n"\
           " <office:font-face-decls>\n"\
           "  <style:font-face style:name=\""<< getBaseFont()<<"\" svg:font-family=\""<< getBaseFont()<<"\" style:font-family-generic=\"modern\" style:font-pitch=\"fixed\"/>\n"\
           " </office:font-face-decls>\n";

    header << getStyleDefinition();

    header  << " <office:body>\n"
            << "  <office:text>\n"
            << "<text:p text:style-name=\"Standard\">";

    return header.str();
}

void ODTGenerator::printBody()
{
    processRootState();
}

string ODTGenerator::getFooter()
{
    return "</text:p>\n  </office:text>\n </office:body>\n</office:document>";
}

string  ODTGenerator::getOpenTag ( const string& styleName )
{
    return "<text:span text:style-name=\""+styleName+"\">";
}

void ODTGenerator::initOutputTags ()
{
    openTags.push_back ( "" );
    openTags.push_back ( getOpenTag ( STY_NAME_STR ) );
    openTags.push_back ( getOpenTag ( STY_NAME_NUM ) );
    openTags.push_back ( getOpenTag ( STY_NAME_SLC ) );
    openTags.push_back ( getOpenTag ( STY_NAME_COM ) );
    openTags.push_back ( getOpenTag ( STY_NAME_ESC ) );
    openTags.push_back ( getOpenTag ( STY_NAME_DIR ) );
    openTags.push_back ( getOpenTag ( STY_NAME_DST ) );
    openTags.push_back ( getOpenTag ( STY_NAME_LIN ) );
    openTags.push_back ( getOpenTag ( STY_NAME_SYM ) );
    openTags.push_back ( getOpenTag ( STY_NAME_IPL ) );

    closeTags.push_back ( "" );
    for (unsigned int i=1; i<NUMBER_BUILTIN_STATES; i++ ) {
        closeTags.push_back ( "</text:span>" );
    }

}

string ODTGenerator::maskCharacter ( unsigned char c )
{
    switch ( c ) {
    case '<' :
        return "&lt;";
        break;
    case '>' :
        return "&gt;";
        break;
    case '&' :
        return "&amp;";
        break;
    case '\"' :
        return "&quot;";
        break;
    case ' ':
        return spacer;
        break;
    //	case '\t':
    //		return "<text:tab/>";
    //		break;
    default:
        return string ( 1, c );
    }
}

string ODTGenerator::getKeywordOpenTag ( unsigned int styleID )
{
    return getOpenTag ( currentSyntax->getKeywordClasses() [styleID] );
}

string ODTGenerator::getKeywordCloseTag ( unsigned int styleID )
{
    return "</text:span>";
}

}
