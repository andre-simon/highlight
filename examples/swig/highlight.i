%module highlight
%include stl.i
%include std_string.i


%{
#include "../../src/include/datadir.h"
#include "../../src/include/enums.h"
#include "../../src/include/syntaxreader.h"
#include "../../src/include/codegenerator.h"
%}

%apply const std::string& { const string& };
%apply  std::string {  string };

%include "../../src/include/datadir.h"
%include "../../src/include/enums.h"
%include "../../src/include/syntaxreader.h"
%include "../../src/include/codegenerator.h"
