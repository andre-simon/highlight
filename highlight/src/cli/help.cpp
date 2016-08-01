/***************************************************************************
                          help.cpp  -  description
                             -------------------
    begin                : Die Apr 23 2002
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

#include <iostream>
#include "help.h"

using namespace std;

namespace Help
{
void printHelp()
{
    cout<<"USAGE: highlight [OPTIONS]... [FILES]...\n";
    cout<<"\n";
    cout<<"General options:\n";
    cout<<"\n";
    cout<<" -B, --batch-recursive=<wc>     convert all matching files, searches subdirs\n";
    cout<<"                                  (Example: -B '*.cpp')\n";
    cout<<" -D, --data-dir=<directory>     set path to data directory (deprecated)\n";
    cout<<"     --config-file=<file>       set path to a lang or theme file\n";
    cout<<" -d, --outdir=<directory>       name of output directory\n";
    cout<<" -h, --help                     print this help\n";
    cout<<" -i, --input=<file>             name of single input file\n";
    cout<<" -o, --output=<file>            name of single output file\n";
    cout<<" -P, --progress                 print progress bar in batch mode\n";
    cout<<" -q, --quiet                    supress progress info in batch mode\n";
    cout<<" -S, --syntax=<type>            specify type of source code\n";
    cout<<" -v, --verbose                  print debug info\n";
    cout<<"     --force                    generate output if input syntax is unknown\n";
    cout<<"     --list-scripts=<type>      list installed scripts\n";
    cout<<"                                  <type> = [langs, themes, plugins]\n";
    cout<<"     --plug-in=<script>         execute Lua plug-in script; repeat option to\n";
    cout<<"                                  execute multiple plug-ins\n";
    cout<<"     --plug-in-param=<value>    set plug-in input parameter\n";
    cout<<"     --print-config             print path configuration\n";
    cout<<"     --print-style              print stylesheet only (see --style-outfile)\n";
    cout<<"     --skip=<list>              ignore listed unknown file types\n";
    cout<<"                                  (Example: --skip='bak;c~;h~')\n";
    cout<<"     --start-nested=<lang>      define nested language which starts input\n";
    cout<<"                                  without opening delimiter\n";
    cout<<"     --validate-input           test if input is text, remove Unicode BOM\n";
    cout<<"     --version                  print version and copyright information\n";
    cout<<"\n";
    cout<<"\n";
    cout<<"Output formatting options:\n";
    cout<<"\n";
    cout<<" -O, --out-format=<format>      output file in given format\n";
    cout<<"                                  <format>=[html, xhtml, latex, tex, odt, rtf,\n";
    cout<<"                                  ansi, xterm256, bbcode, pango, svg]\n";
    cout<<" -c, --style-outfile=<file>     name of style file or print to stdout, if\n";
    cout<<"                                  'stdout' is given as file argument\n";

    cout<<" -e, --style-infile=<file>      to be included in style-outfile (deprecated)\n";
    cout<<"                                  use a plug-in file instead\n";

    cout<<" -f, --fragment                 omit document header and footer\n";
    cout<<" -F, --reformat=<style>         reformats and indents output in given style\n";
    cout<<"                                  <style> = [allman, banner, gnu,\n";
    cout<<"                                  horstmann, java, kr, linux, otbs, vtk,\n";
    cout<<"                                  stroustrup, whitesmith, google, pico, lisp]\n";
    cout<<" -I, --include-style            include style definition in output file\n";
    cout<<" -J, --line-length=<num>        line length before wrapping (see -V, -W)\n";
    cout<<" -j, --line-number-length=<num> line number width incl. left padding (default: 5)\n";
    cout<<" -k, --font=<font>              set font (specific to output format)\n";
    cout<<" -K, --font-size=<num?>         set font size (specific to output format)\n";
    cout<<" -l, --line-numbers             print line numbers in output file\n";
    cout<<" -m, --line-number-start=<cnt>  start line numbering with cnt (assumes -l)\n";
    cout<<" -s, --style=<style>            set colour style (theme)\n";
    cout<<" -t, --replace-tabs=<num>       replace tabs by <num> spaces\n";
    cout<<" -T, --doc-title=<title>        document title\n";
    cout<<" -u, --encoding=<enc>           set output encoding which matches input file\n";
    cout<<"                                  encoding; omit encoding info if set to NONE\n";
    cout<<" -V, --wrap-simple              wrap lines after 80 (default) characters w/o\n";
    cout<<"                                  indenting function parameters and statements\n";
    cout<<" -W, --wrap                     wrap lines after 80 (default) characters\n";
    cout<<"     --wrap-no-numbers          omit line numbers of wrapped lines\n";
    cout<<"                                  (assumes -l)\n";
    cout<<" -z, --zeroes                   pad line numbers with 0's\n";
    cout<<"     --delim-cr                 set CR as end-of-line delimiter (MacOS 9)\n";
    cout<<"     --keep-injections          output plug-in injections in spite of -f\n";
    cout<<"     --kw-case=<case>           change case of case insensitive keywords\n";
    cout<<"                                  <case> =  [upper, lower, capitalize]\n";
    cout<<"     --no-trailing-nl           omit trailing newline\n";
    cout<<"\n\n";
    cout<<"(X)HTML output options:\n";
    cout<<"\n";
    cout<<" -a, --anchors                  attach anchor to line numbers\n";
    cout<<" -y, --anchor-prefix=<str>      set anchor name prefix\n";
    cout<<" -N, --anchor-filename          use input file name as anchor prefix\n";
    cout<<" -C, --print-index              print index with hyperlinks to output files\n";
    cout<<" -n, --ordered-list             print lines as ordered list items\n";
    cout<<"     --class-name=<name>        set CSS class name prefix;\n";
    cout<<"                                  omit class name if set to NONE\n";
    cout<<"     --inline-css               output CSS within each tag (verbose output)\n";
    cout<<"     --enclose-pre              enclose fragmented output with pre tag \n";
    cout<<"                                  (assumes -f)\n";
    cout<<"\n\n";
    cout<<"LaTeX output options:\n";
    cout<<"\n";
    cout<<" -b, --babel                    disable Babel package shorthands\n";
    cout<<" -r, --replace-quotes           replace double quotes by \\dq{}\n";
    cout<<"     --pretty-symbols           improve appearance of brackets and other symbols\n";
    cout<<"\n";
    cout<<"\n";
    cout<<"RTF output options:\n";
    cout<<"\n";
    cout<<"     --page-color               include page color attributes\n";
    cout<<" -x, --page-size=<ps>           set page size \n";
    cout<<"                                  <ps> = [a3, a4, a5, b4, b5, b6, letter]\n";
    cout<<"     --char-styles              include character stylesheets\n";
    cout<<"\n";
    cout<<"\n";
    cout<<"SVG output options:\n";
    cout<<"\n";
    cout<<"     --height                   set image height (units allowed)\n";
    cout<<"     --width                    set image width (see --height)\n";
    cout<<"\n";
    cout<<"\n";
    cout<<"GNU source-highlight compatibility options:\n";
    cout<<"\n";
    cout<<"     --doc                      create stand alone document\n";
    cout<<"     --no-doc                   cancel the --doc option\n";
    cout<<"     --css=filename             the external style sheet filename\n";
    cout<<"     --src-lang=STRING          source language\n";
    cout<<" -t, --tab=INT                  specify tab length\n";
    cout<<" -n, --line-number[=0]          number all output lines, optional padding\n";
    cout<<"     --line-number-ref[=p]      number all output lines and generate an anchor,\n";
    cout<<"                                  made of the specified prefix p + the line\n";
    cout<<"                                  number  (default='line')\n";
    cout<<"     --output-dir=path          output directory\n";
    cout<<"     --failsafe                 if no language definition is found for the\n";
    cout<<"                                  input, it is simply copied to the output\n";
    cout<<"\n";
    cout<<"\n";
    cout<<"If no in- or output files are specified, stdin and stdout will be used.\n";
    cout<<"HTML will be generated unless an other output format is given. Style definitions\n";
    cout<<"are stored in highlight.css (HTML, XHTML, SVG) or highlight.sty (LaTeX, TeX)\n";
    cout<<"if neither -c nor -I is given.\n";
    cout<<"Reformatting code (-F) will only work with C, C++, C# and Java input files.\n";
    cout<<"Wrapping lines with -V or -W will cause faulty highlighting of long single\n";
    cout<<"line comments and directives. Use with caution.\n";
    cout<<"See README files how to apply plug-ins to customize the output.\n";
    cout<<"\n";
    cout<<"Updates and information: http://www.andre-simon.de/\n";
}

}
