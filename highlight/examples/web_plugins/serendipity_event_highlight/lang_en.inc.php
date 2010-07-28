<?php 

@define('PLUGIN_EVENT_HIGHLIGHT_NAME',     'Markup: Highlight');
@define('PLUGIN_EVENT_HIGHLIGHT_DESC',     'Coloured Syntax Highlighting using the highlight utility. Tag usage: [highlight lang=langName ] code [/highlight]');
@define('PLUGIN_EVENT_HIGHLIGHT_TRANSFORM', 'You can use <b>[highlight lang=langName][/highlight]</b> tags to embed source code snippets');
@define('PLUGIN_EVENT_HIGHLIGHT_VERSION','01');

@define('PLUGIN_EVENT_HIGHLIGHT_HLBINDIR','Path of highlight binary');
@define('PLUGIN_EVENT_HIGHLIGHT_HLBINDIR_DESC','Path of the highlight binary');
@define('PLUGIN_EVENT_HIGHLIGHT_HLDATADIR','Path of highlight data directory');
@define('PLUGIN_EVENT_HIGHLIGHT_HLDATADIR_DESC','This directory contains the data files located in the subdirectories langDefs, themes etc');
@define('PLUGIN_EVENT_HIGHLIGHT_LINENUMBERS', 'Line number printout');
@define('PLUGIN_EVENT_HIGHLIGHT_LINENUMBERS_DESC','Define if the line numbers should be shown.');
@define('PLUGIN_EVENT_HIGHLIGHT_LINENUMBERZEROES', 'Line number padding');
@define('PLUGIN_EVENT_HIGHLIGHT_LINENUMBERZEROES_DESC','Define if the line numbers should be padded with zeroes.');
@define('PLUGIN_EVENT_HIGHLIGHT_LINENUMBERSTART', 'Line number start');
@define('PLUGIN_EVENT_HIGHLIGHT_LINENUMBERSTART_DESC','Define where the line numbering should start.');
@define('PLUGIN_EVENT_HIGHLIGHT_LINENUMBERLEN', 'Line number width');
@define('PLUGIN_EVENT_HIGHLIGHT_LINENUMBERLEN_DESC','Define the line number width.');
@define('PLUGIN_EVENT_HIGHLIGHT_THEMES', 'Color theme');
@define('PLUGIN_EVENT_HIGHLIGHT_THEMES_DESC','Choose a color theme which matches your blog.');
@define('PLUGIN_EVENT_HIGHLIGHT_FORMAT', 'Code reformatting');
@define('PLUGIN_EVENT_HIGHLIGHT_FORMAT_DESC','Choose a indentation and reformatting scheme to reformat C, C++, C# and Java code.');

@define('PLUGIN_EVENT_HIGHLIGHT_WRAP', 'Line wrapping');
@define('PLUGIN_EVENT_HIGHLIGHT_WRAP_DESC','Define if long lines should be wrapped. WRAP INTELLIGENT means that function parameters and assignments are intended after wrapping.');
@define('PLUGIN_EVENT_HIGHLIGHT_WRAPLEN', 'Line length before wrapping');
@define('PLUGIN_EVENT_HIGHLIGHT_WRAPLEN_DESC','Set the maximum line length (only available if line wrapping is not disabled above).');
@define('PLUGIN_EVENT_HIGHLIGHT_TABLEN', 'Tab width');
@define('PLUGIN_EVENT_HIGHLIGHT_TABLEN_DESC','Define how many spaces should replace a tab.');

?>
