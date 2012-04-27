--[[
Sample plugin file for highlight 3.9
]]

Description="Add developer.gnome.org reference links to HTML output of C++ GTK code"

-- optional parameter: syntax description
function syntaxUpdate(desc)

  if desc~="C and C++" then
     return
  end

  url_start='<a class="hl" target="new" href="http://developer.gnome.org/gtk/2.24/'

  function Decorate(token, state, docformat)

    if (docformat ~= HL_FORMAT_HTML and docformat ~= HL_FORMAT_XHTML) then
      return
    end

    if (state ~= HL_STANDARD and state ~= HL_KEYWORD) then
      return
    end

    if string.find(token, "Gtk%u%l")==1 then
      return url_start.. token ..'.html">'.. token .. '</a>'
    end

  end
end


function themeUpdate(desc)
   -- inherit formatting of enclosing span tags
   Injection="a.hl, a.hl:visited {color:inherit;font-weight:inherit;}"
end

--The Plugins array assigns code chunks to themes or language definitions.
--The chunks are interpreted after the theme or lang file were parsed,
--so you can refer to elements of these files

Plugins={

  { Type="lang", Chunk=syntaxUpdate },
  { Type="theme", Chunk=themeUpdate },

}
