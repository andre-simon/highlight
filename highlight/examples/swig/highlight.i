%module highlight
%include stl.i
%include std_string.i
%include <boost_xpressive.i> 
%apply const std::string& { const string& };
%apply  std::string {  string };

%include "../../src/include/enums.h"
%include "../../src/include/syntaxreader.h"
%include "../../src/include/codegenerator.h"
