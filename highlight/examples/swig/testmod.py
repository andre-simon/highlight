# -*- coding: utf-8 -*-
# More advanced SWIG module test script
#
# Import highlight.py, which is the interface for the _highlight.so module.
# See highlight.py for all available attributes and class members.
#
# Example: python2.7 testmod.py -Sperl -O odt testmod.pl testmod.pl.odt

import highlight
import sys
from optparse import OptionParser

formatList = { "html":  highlight.HTML,
       "xhtml": highlight.XHTML,
	   "latex": highlight.LATEX,
	   "rtf":   highlight.RTF,
	   "tex":   highlight.TEX,
	   "ansi":  highlight.ANSI,
	   "xterm256": highlight.XTERM256,
	   "odt":   highlight.ODTFLAT,
	   "bbcode":   highlight.BBCODE,
	   "svg":   highlight.SVG,
	   "pango": highlight.PANGO
	 }

HL_DIR="/usr/share/highlight/"

def highlightFile():

	parser = OptionParser("usage: %prog [options] input-file output-file")
	parser.add_option("-O", "--format", default="html",
	                choices=("html","xhtml","latex","tex","rtf","ansi","xterm256", "svg", "odt", "bbcode", "pango"),
			help="Output format (html, xhtml, latex, tex, rtf, ansi, xterm256, odt, bbcode, svg, pango)")
	parser.add_option("-d", "--doc-title", default="Source file",
			help="document title")
	parser.add_option("-f", "--fragment", action="store_true",
			help="omit file header and footer")
	parser.add_option("-F", "--reformat",
	        choices=('allman','gnu','java','kr','linux', 'banner','stroustrup','whitesmith', 'google', 'pico', 'lisp', 'vtk'),
			help="reformats and indents output in given style")
	parser.add_option("-l", "--linenumbers", action="store_true",
			help="print line numbers in output file")
	parser.add_option("-S", "--syntax",
			help="specify type of source code")
	parser.add_option("-s", "--theme", default="seashell",
			help="defines colour theme")
	parser.add_option("-u", "--encoding", default="ISO-8859-1",
			help="define output encoding which matches input file encoding")

	(options, args) = parser.parse_args(sys.argv[1:])
	if len(args)!=2:
		parser.print_help()
		return

	formatName=options.format.lower()
	outFormat = formatName in formatList and formatList[formatName] or highlight.HTML

	(infile, outfile) = args

	gen=highlight.CodeGenerator.getInstance(outFormat)
	datadir=highlight.DataDir()
	datadir.initSearchDirectories(HL_DIR)

	#initialize the generator with a colour theme and the language definition
	gen.initTheme(datadir.getThemePath("%s.theme" % options.theme))

	if options.reformat:
		gen.initIndentationScheme(options.reformat)

	if (options.syntax):
		syntax = options.syntax
	else:
		syntax = infile[infile.rindex(".")+1:]

	print datadir.getLangPath("%s.lang" % syntax)
	gen.loadLanguage(datadir.getLangPath("%s.lang" % syntax))

	gen.setIncludeStyle(1)
	gen.setTitle(options.doc_title)
	gen.setFragmentCode(options.fragment)
	gen.setPrintLineNumbers(options.linenumbers)
	gen.setEncoding(options.encoding)

	gen.generateFile(infile, outfile)

	# clear the instance
	highlight.CodeGenerator.deleteInstance(gen)

###############################################################################
if __name__ == "__main__":
	highlightFile()

