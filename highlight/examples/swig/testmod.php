<?php

/* Set enable_dl = On in your php.ini and copy highlight.so in /usr/lib/php/modules/ */

if (!extension_loaded('highlight')){
      if (!dl('highlight.so')) {
	  exit;
      }
}

/**
 * return value options
 *
 * @param $options array $options
 * @param $short_opts string key short_opts
 * @param $long_opts string key short_opts
 * @return string value of short_opts or long_opts or false if not set
 */
function valueOptions($options,$short_opts,$long_opts) {
    if (isset($options[$short_opts])) {
        if ($options[$short_opts]==false) {
            return true;
        } else {
            return $options[$short_opts];
        }

    } else if (isset($options[$long_opts])) {
        if ($options[$long_opts]==false) {
            return true;
        } else {
            return $options[$long_opts];
        }

    } else {
        return false;
    }
}
$help = <<<HELP
General Options:

-i, --input <file>          name of single input file
-S, --syntax <type>         specify type of source code

Output formatting options:

-O, --outformat <format>    output file in given format
                            <format>=[HTML,XHTML,LATEX,TEX
                            ODT,RTF,ANSI,XTERM256,BBCODE,SVG]
-f, --fragment              omit document header and footer
-F, --reformat <style>      reformats and indents output in given style
                            <style>=[allman, banner, gnu,horstmann,java
                            kr,linux,otb,sstroustrup, whitesmith]
    --inlinecss             output CSS within each tag (verbose output)
-I, --includestyle          include style definition
-l, --linenumbers           print line numbers in output file
-m, --linenumberstart <cnt> start line numbering with cnt (assumes -l)
-s, --style <style>         set colour style
HELP;

$short_opts = '';
$short_opts .= 'f';
$short_opts .= 'F:';
$short_opts .= 'i:';
$short_opts .= 'I';
$short_opts .= 'l';
$short_opts .= 'm:';
$short_opts .= 'O:';
$short_opts .= 's:';
$short_opts .= 'S:';

$long_opts = array(
    'fragment',
    'inlinecss',
    'input:',
    'includestyle',
    'linenumbers',
    'linenumberstart:',
    'outputformat:',
    'reformat:',
    'style:',
    'syntax:'
);
$options = getopt($short_opts,$long_opts);

if (empty($options)) {
    echo $help."\n";
    exit;
}

$outformat = valueOptions($options,'O','outformat');
$outformat = strtolower($outformat);
$out = 0; // default, HTML


if($outformat) {
    switch ($outformat) {
        case 'xhtml': $out = 1; break;
        case 'tex': $out = 2; break;
        case 'latex': $out = 3; break;
        case 'rtf': $out = 4; break;
        case 'ansi': $out = 5; break;
        case 'xterm256': $out = 6; break;
        case 'html32': $out = 7; break;
        case 'svg': $out = 8; break;
        case 'bbcode': $out = 9; break;
        case 'pango': $out = 10; break;
        case 'odt': $out = 11; break;
        default: $out = 0;
    }
}

$gen = codegenerator_getinstance($out);

// Initialize DataDir instance
$data_dir = new_datadir();
$highlight_data = datadir_lsb_data_dir_get();

// -F, --reformat <style>
$reformat = valueOptions($options,'F','reformat');
if ($reformat) {
    codegenerator_initindentationscheme($gen,$reformat);
}

// --inlinecss
$inlinecss = valueOptions($options,'inlinecss','inlinecss');
if($inlinecss) {
    codegenerator_sethtmlinlinecss($gen,true);
}

// -s, --style <style>
$style = valueOptions($options,'s','style');
if ($style) {
    $theme_path = datadir_getthemepath($data_dir,$style.".theme");

} else {
    $theme_path = datadir_getthemepath($data_dir,"seashell.theme");
}
codegenerator_inittheme($gen,$highlight_data.$theme_path);

// -i, --input <file>
$input = valueOptions($options,'i','input');
$handle = fopen($input,"r");
$filename = fread($handle,filesize($input));

// -S, --syntax <type>
$syntax = valueOptions($options,'S','syntax');
if (!$syntax) {
    $ext_temp = explode('.',$input);
    $syntax = end($ext_temp);
}
$lang_path = datadir_getlangpath($data_dir,$syntax.".lang");
codegenerator_loadlanguage($gen,$highlight_data.$lang_path);

// -I, --includestyle
$includestyle = valueOptions($options,'I','includestyle');
if($includestyle) {
    codegenerator_setincludestyle($gen,true);
}

// -f, --fragment
$fragment = valueOptions($options,'f','fragment');
if ($fragment) {
    codegenerator_setfragmentcode($gen,true);
}

// -l, --linenumbers
$linenumbers = valueOptions($options,'l','linenumbers');
if($linenumbers) {
    codegenerator_setprintlinenumbers($gen,true);
}

// -m, --linenumberstart <cnt>
$linenumberstart = valueOptions($options,'m','linenumberstart');
if ($linenumberstart) {
    $linenumberstart = (int) $linenumberstart; // make sure it's integer
    codegenerator_setprintlinenumbers($gen,true,$linenumberstart);
}



//print!!!
echo codegenerator_generatestring($gen,$filename);

