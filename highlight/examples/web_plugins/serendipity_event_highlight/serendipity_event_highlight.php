<?php
/**

serendipity_event_highlight.php

=== Highlight plugin ===

This plugin uses highlight (http://www.andre-simon.de/) to add syntax highlighting to serendipity.

== Description ==

The highlight utility converts source code of 120 programming languages to HTML
with syntax highlighting. This plugin pipes the content of [highlight] tags associated
with a lang parameter to highlight, and returns the output code which is included
in the Serendipity blog entry.
The plugin configuration menu offers the most important options of the highlight utility.
The highlighted code is formatted with inline CSS to avoid complicated plugin setup.

Usage:

Paste the following in the code section of the blog editing form:

[highlight lang=c]#include <stdio.h>

int main (void){
  printf("This is some random code");
  return 0;
} [/highlight]

Normally the file extension can be used for the lang parameter (like pl, py, java, pas etc).
Execute 'highlight --list-langs' to get a list of all supported programming langauges.

== Installation ==

1. Install the highlight utility on your host (www.andre-simon.de)
2. Copy the serendipity_event_highlight dir into the serendipity plugin dir
3. Activate and configure the plugin in the serendipity administration menu

   IMPORTANT:
   To avoid insertion of superfluous <br> Tags by the nl2br plugin, add the
   string "highlight" to the  "list of HTML-tags where no breaks shall be
   converted" in the nl2br configuration menu.

 */


if (IN_serendipity !== true) {
    die ("Don't hack!");
}

// Probe for a language include with constants. Still include defines later on, if some constants were missing
$probelang = dirname(__FILE__) . '/' . $serendipity['charset'] . 'lang_' . $serendipity['lang'] . '.inc.php';
if (file_exists($probelang)) {
    include $probelang;
}

include dirname(__FILE__) . '/lang_en.inc.php';

class serendipity_event_highlight extends serendipity_event
{
    var $title = PLUGIN_EVENT_HIGHLIGHT_NAME;
    // Top Level Configuration, requires name of the Plugin, description text, and configuration information in an array..
    function introspect(&$propbag)
    {
        global $serendipity;

        $propbag->add('name',          PLUGIN_EVENT_HIGHLIGHT_NAME);
        $propbag->add('description',   PLUGIN_EVENT_HIGHLIGHT_DESC);
        $propbag->add('stackable',     false);
        $propbag->add('author',        'Andre Simon');
        $propbag->add('requirements',  array(
            'serendipity' => '0.9',
            'highlight'   => '3.23'
        ));
        $propbag->add('version',       '0.2');
        $propbag->add('event_hooks', array('frontend_display' => true, 'frontend_comment' => true));
        $propbag->add('groups', array('MARKUP'));

        $this->markup_elements = array(
            array(
              'name'     => 'ENTRY_BODY',
              'element'  => 'body',
            ),
            array(
              'name'     => 'EXTENDED_BODY',
              'element'  => 'extended',
            ),
            array(
              'name'     => 'COMMENT',
              'element'  => 'comment',
            ),
            array(
              'name'     => 'HTML_NUGGET',
              'element'  => 'html_nugget',
            )
        );

	#Colour themes of a highlight default installation
        $this->themes = array(
	"acid", "aiseered", "andes", "anotherdark", "autumn", "baycomb",
	"bclear", "biogoo", "bipolar", "blacknblue", "bluegreen", "breeze",
	"bright", "camo", "candy", "clarity", "dante", "darkblue",
	"darkbone", "darkness", "darkslategray", "darkspectrum", "denim",
	"dusk", "earendel", "easter", "edit-anjuta", "edit-eclipse",
	"edit-emacs", "edit-flashdevelop", "edit-gedit", "edit-jedit",
	"edit-kwrite", "edit-matlab", "edit-msvs2008", "edit-nedit",
	"edit-vim-dark", "edit-vim", "edit-xcode", "ekvoli", "fine_blue",
	"freya", "fruit", "golden", "greenlcd", "kellys", "leo",
	"lucretia", "manxome", "maroloccio", "matrix", "moe", "molokai",
	"moria", "navajo-night", "navy", "neon", "night", "nightshimmer",
	"nuvola", "olive", "orion", "oxygenated", "pablo", "peaksea",
	"print", "rand01", "rdark", "relaxedgreen", "rootwater",
	"seashell", "solarized-dark", "solarized-light", "tabula",
	"tcsoft", "the", "vampire", "whitengrey", "xoria256", "zellner",
	"zenburn", "zmrok"
        );

	#Reformatting schemes of a highlight default installation
        $this->reformatschemes = array ("disabled","allman", "ansi", "banner",
        "gnu", "google", "horstmann", "java", "kr", "linux", "lisp",
        "otbs", "pico", "stroustrup", "vtk", "whitesmith");

	#highlight output options
        $conf_array = array('hl_bin_dir','hl_data_dir','hl_linenumbers','hl_linenumberstart','hl_linenumberzeroes','hl_linenumberlen',
                            'hl_wrap','hl_wrap_len','hl_tab_len', 'hl_theme','hl_format');

        foreach($this->markup_elements as $element) {
            $conf_array[] = $element['name'];
        }
        $propbag->add('configuration', $conf_array);
    }

    function generate_content(&$title) {
        $title = $this->title;
    }


    function introspect_config_item($name, &$propbag) {
        switch ($name) {
            case 'hl_bin_dir' :
                $propbag->add('name',        PLUGIN_EVENT_HIGHLIGHT_HLBINDIR);
                $propbag->add('type', 'string');
                $propbag->add('description', PLUGIN_EVENT_HIGHLIGHT_HLBINDIR_DESC);
                $propbag->add('default', 'highlight');
                break;
            case 'hl_data_dir' :
                $propbag->add('name',        PLUGIN_EVENT_HIGHLIGHT_HLDATADIR);
                $propbag->add('type', 'string');
                $propbag->add('description', PLUGIN_EVENT_HIGHLIGHT_HLDATADIR_DESC);
                $propbag->add('default', '/usr/share/highlight/');
                break;
            case 'hl_linenumbers' :
                $propbag->add('name',        PLUGIN_EVENT_HIGHLIGHT_LINENUMBERS);
                $propbag->add('type', 'boolean');
                $propbag->add('description', PLUGIN_EVENT_HIGHLIGHT_LINENUMBERS_DESC);
                $propbag->add('default', 'false');
                break;
            case 'hl_linenumberzeroes' :
                $propbag->add('name',        PLUGIN_EVENT_HIGHLIGHT_LINENUMBERZEROES);
                $propbag->add('type', 'boolean');
                $propbag->add('description', PLUGIN_EVENT_HIGHLIGHT_LINENUMBERZEROES_DESC);
                $propbag->add('default', 'false');
                break;
            case 'hl_linenumberstart' :
                $propbag->add('name',        PLUGIN_EVENT_HIGHLIGHT_LINENUMBERSTART);
                $propbag->add('type', 'string');
                $propbag->add('description', PLUGIN_EVENT_HIGHLIGHT_LINENUMBERSTART_DESC);
                $propbag->add('default', '1');
 		$propbag->add('validate', 'number');
                break;

            case 'hl_linenumberlen' :
                $propbag->add('name',        PLUGIN_EVENT_HIGHLIGHT_LINENUMBERLEN);
                $propbag->add('type', 'string');
                $propbag->add('description', PLUGIN_EVENT_HIGHLIGHT_LINENUMBERLEN_DESC);
                $propbag->add('default', '2');
				$propbag->add('validate', 'number');
                break;

            case 'hl_theme' :
                $propbag->add('type', 'select');
                $propbag->add('name', PLUGIN_EVENT_HIGHLIGHT_THEMES);
                $propbag->add('description', PLUGIN_EVENT_HIGHLIGHT_THEMES_DESC);
                $propbag->add('select_values', $this->themes);
                $propbag->add('default', '1');
                break;

            case 'hl_format' :
                $propbag->add('type', 'select');
                $propbag->add('name', PLUGIN_EVENT_HIGHLIGHT_FORMAT);
                $propbag->add('description', PLUGIN_EVENT_HIGHLIGHT_FORMAT_DESC);
                $propbag->add('select_values', $this->reformatschemes);
                $propbag->add('default', '0');
                break;

            case 'hl_wrap' :
                $wrapstyles=array ('No wrap', 'Simple wrap','Intelligent wrap');
                $propbag->add('type', 'select');
                $propbag->add('name', PLUGIN_EVENT_HIGHLIGHT_WRAP);
                $propbag->add('description', PLUGIN_EVENT_HIGHLIGHT_WRAP_DESC);
                $propbag->add('select_values', $wrapstyles);
                $propbag->add('default', '0');
                break;
            case 'hl_wrap_len' :
                $propbag->add('name',        PLUGIN_EVENT_HIGHLIGHT_WRAPLEN);
                $propbag->add('type', 'string');
                $propbag->add('description', PLUGIN_EVENT_HIGHLIGHT_WRAPLEN_DESC);
                $propbag->add('default', '60');
				$propbag->add('validate', 'number');
                break;
            case 'hl_tab_len' :
                $propbag->add('name',        PLUGIN_EVENT_HIGHLIGHT_TABLEN);
                $propbag->add('type', 'string');
                $propbag->add('description', PLUGIN_EVENT_HIGHLIGHT_TABLEN_DESC);
                $propbag->add('default', '4');
				$propbag->add('validate', 'number');
                break;
            default :
                $propbag->add('name',        constant($name));
                $propbag->add('type',        'boolean');
                $propbag->add('default',     'true');
                $propbag->add('description', sprintf(APPLY_MARKUP_TO, constant($name)));
        }
        return true;
    }

    function geshi($input) {
        $input = preg_replace_callback('/\[highlight(?:\s)*lang=([A-Za-z0-9_\-]+)(?:\s)*()?\](.*?)\[\/highlight\]/si', array(&$this, 'highlightcallback'), $input);
        return $input;
    }

    function event_hook($event, &$bag, &$eventData) {
        global $serendipity;

        $hooks = &$bag->get('event_hooks');

        if (isset($hooks[$event])) {
            switch($event) {
                case 'frontend_display':
                    foreach ($this->markup_elements as $temp) {
                        if (serendipity_db_bool($this->get_config($temp['name'], true)) && isset($eventData[$temp['element']]) &&
                            !$eventData['properties']['ep_disable_markup_' . $this->instance] &&
                            !isset($serendipity['POST']['properties']['disable_markup_' . $this->instance])) {
                            $element = $temp['element'];
                            $eventData[$element] = $this->geshi($eventData[$element]);
                        }
                    }
                    return true;
                    break;

                case 'frontend_comment':
                    if (serendipity_db_bool($this->get_config('COMMENT', true))) {
                        echo '<div class="serendipity_commentDirection serendipity_comment_geshi">' . PLUGIN_EVENT_HIGHLIGHT_TRANSFORM . '</div>';
                    }
                    return true;
                    break;

                default:
                  return false;
            }
        } else {
            return false;
        }
    }

    function highlightcallback($matches) {

	$search = array("&amp;","&quot;", "&lt;", "&gt;","&#92;","&#39;","&nbsp;");
	$replace = array("&","\"", "<", ">","\\","\'", " ");
	$input_code = str_replace($search, $replace, $matches[3]);

	$descriptorspec = array(
	0 => array("pipe", "r"),  // stdin is a pipe that the child will read from
	1 => array("pipe", "w")  // stdout is a pipe that the child will write to
	);

	$hl_cmd_str =  $this->get_config('hl_bin_dir');
	$hl_cmd_str .= ' --inline-css -f ';

	if ( $this->get_config('hl_linenumbers')){
		$hl_cmd_str .= " -l -m ";
		$hl_cmd_str .= $this->get_config('hl_linenumberstart');
		if ($this->get_config('hl_linenumberzeroes')){
			$hl_cmd_str .= " -z ";
		}
		$hl_cmd_str .= ' -j ';
		$hl_cmd_str .= $this->get_config('hl_linenumberlen');
	}

	if ($this->get_config('hl_tab_len')) {
		$hl_cmd_str .= " -t ";
		$hl_cmd_str .= $this->get_config('hl_tab_len');
	}

	if ($this->get_config('hl_wrap')>0){
		$hl_cmd_str .= ($this->get_config('hl_wrap') == 1)? ' -V ':' -W ';
		$hl_cmd_str .= " -J ";
		$hl_cmd_str .= $this->get_config('hl_wrap_len');
	}

	if ($this->get_config('hl_format')>1){
		$hl_cmd_str .= " -F ";
		$hl_cmd_str .= $this->reformatschemes[$this->get_config('hl_format')];
	}


	$hl_cmd_str .= " -s ";
	$hl_cmd_str .= $this->themes[$this->get_config('hl_theme')];


	$lang = strtolower($matches[1]);
	$hl_cmd_str .= " -S $lang ";

	$process = proc_open($hl_cmd_str, $descriptorspec, $pipes);
	if (is_resource($process)) {

		fwrite($pipes[0], $matches[3]);
		fclose($pipes[0]);

		$output = stream_get_contents($pipes[1]);
		fclose($pipes[1]);

		// It is important that you close any pipes before calling
		// proc_close in order to avoid a deadlock
		proc_close($process);
	}
	$result='';
	if (!strlen($output)) {
		$result .= "<small>ERROR: Execution of highlight ($hl_cmd_str) failed or missing input. Check binary  path.</small>";
		$result .= "<pre style=\"font-size:9pt;\">";
		$result .= $input_code;
		$result .= "</pre>";

	} else {
		$result .= "<pre style=\"font-size:9pt;\">";
		$result .= $output;
		$result .= "</pre>";
	}
	return $result;
    }
}

?>
