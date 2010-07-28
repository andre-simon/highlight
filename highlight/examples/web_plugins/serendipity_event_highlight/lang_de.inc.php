<?php 

@define('PLUGIN_EVENT_HIGHLIGHT_NAME',     'Markup: Highlight');
@define('PLUGIN_EVENT_HIGHLIGHT_DESC',     'Farbige Syntax-Hervorhebung mit dem highlight tool. Einbindung mit dem Tag: [highlight lang=langName ] code [/highlight]');
@define('PLUGIN_EVENT_HIGHLIGHT_TRANSFORM', 'Benutzen Sie <b>[highlight lang=langName][/highlight]</b> Tags, um Sourcecode farbig hervorzuheben.');
@define('PLUGIN_EVENT_HIGHLIGHT_VERSION','01');

@define('PLUGIN_EVENT_HIGHLIGHT_HLBINDIR','Highlight Pfad');
@define('PLUGIN_EVENT_HIGHLIGHT_HLBINDIR_DESC','Pfad der highlight Programdatei');
@define('PLUGIN_EVENT_HIGHLIGHT_HLDATADIR','Highlight Datenverzeichnis');
@define('PLUGIN_EVENT_HIGHLIGHT_HLDATADIR_DESC','Dieses Verzeichnis enthält die Programmdaten, z.B. die Unterverzeichnisse langDefs, themes etc');
@define('PLUGIN_EVENT_HIGHLIGHT_LINENUMBERS', 'Ausgabe Zeilennummern');
@define('PLUGIN_EVENT_HIGHLIGHT_LINENUMBERS_DESC','Sollen Zeilennummern ausgegeben werden?');
@define('PLUGIN_EVENT_HIGHLIGHT_LINENUMBERZEROES', 'Zeilennummern füllen');
@define('PLUGIN_EVENT_HIGHLIGHT_LINENUMBERZEROES_DESC','Sollen Zeilennummern mit Nullen aufgefüllt werden?');
@define('PLUGIN_EVENT_HIGHLIGHT_LINENUMBERSTART', 'Beginn der Nummerierung');
@define('PLUGIN_EVENT_HIGHLIGHT_LINENUMBERSTART_DESC','Wo soll die Nummerierung beginnen?');
@define('PLUGIN_EVENT_HIGHLIGHT_LINENUMBERLEN', 'Zeilennnummernbreite');
@define('PLUGIN_EVENT_HIGHLIGHT_LINENUMBERLEN_DESC','Mit wievielen Stellen soll die Nummerierung ausgegeben werden?');
@define('PLUGIN_EVENT_HIGHLIGHT_THEMES', 'Farbschema');
@define('PLUGIN_EVENT_HIGHLIGHT_THEMES_DESC','Wähle ein Farbschema aus, das zu deinem Blog-Style passt.');
@define('PLUGIN_EVENT_HIGHLIGHT_FORMAT', 'Neuformatierung und Einrückung');
@define('PLUGIN_EVENT_HIGHLIGHT_FORMAT_DESC','Wähle ein Schema aus, um C, C++, C# und Java Code einheitlich zu formatieren.');

@define('PLUGIN_EVENT_HIGHLIGHT_WRAP', 'Zeilenumbruch');
@define('PLUGIN_EVENT_HIGHLIGHT_WRAP_DESC','Bestimme, ob Zeilen umgebrochen werden sollen. WRAP INTELLIGENT bedeutet, dass Funktionsparameter und Zuweisungen nach dem Umbruch korrekt eingerückt werden.');
@define('PLUGIN_EVENT_HIGHLIGHT_WRAPLEN', 'Zeilenlänge vor Umbruch');
@define('PLUGIN_EVENT_HIGHLIGHT_WRAPLEN_DESC','Gib die maximale Zeilenlänge an (nur wirksam wenn obige Option Zeilenumbruch nicht deaktiviert ist).');
@define('PLUGIN_EVENT_HIGHLIGHT_TABLEN', 'Tabulatorbreite');
@define('PLUGIN_EVENT_HIGHLIGHT_TABLEN_DESC','Bestimme die Anzahl von Leerzeichen, die ein Tab ersetzen.');

?>
