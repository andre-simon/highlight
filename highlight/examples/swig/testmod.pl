# Perl SWIG module test script
#
# Import highlight.pm, which is the interface for the highlight.so module.
# See highlight.pm for all available attributes and class members.

use highlight;

#get a generator instance (for HTML output)
my $gen = highlight::CodeGenerator::getInstance($highlight::CodeGenerator::HTML);
 
my $dir = new highlight::DataDir();

#$dir->searchDataDir("");
$dir->initSearchDirectories("");


my $themepath=$dir->getThemePath("seashell.theme");
my $langpath=$dir->getLangPath("c.lang");

#initialize the generator with a colour theme and the language definition
$gen->initTheme($themepath);
$gen->loadLanguage($langpath);

#set some parameters
$gen->setIncludeStyle(1);
$gen->setEncoding("ISO-8859-1");

#get output string
my $output=$gen->generateString("int main(int argc, char **argv) {\n".
                           " HighlightApp app;\n".
                           " return app.run(argc, argv);\n".
                           "}\n");
print $output;

# clear the instance
highlight::CodeGenerator::deleteInstance($gen);
