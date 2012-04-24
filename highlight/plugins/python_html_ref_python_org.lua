--[[
Sample plugin file for highlight 3.9
]]

Description="Add python.org reference links to HTML output of Python code"

-- optional parameter: syntax description
function syntaxUpdate(desc)

  if desc~="Python" then
     return
  end

  function Set (list)
    local set = {}
    for _, l in ipairs(list) do set[l] = true end
      return set
  end

   simple_items = Set {"assert","pass","del","print","return","yield","raise","break","continue","import","global","exec" }
   compound_items = Set {"if","while", "for","try", "with" }
   url_start='<a class="hl" target="new" href="http://docs.python.org/reference/'

  function Decorate(token, state, docformat)

    if (docformat ~= HL_FORMAT_HTML and docformat ~= HL_FORMAT_XHTML) then
      return
    end

    if (state ~= HL_STANDARD and state ~= HL_KEYWORD) then
      return
    end

    if simple_items[token] then
      return url_start.."simple_stmts.html#the-".. token .."-statement\">".. token .. "</a>"
    elseif compound_items[token] then
      return url_start.."compound_stmts.html#the-".. token .."-statement\">".. token .. "</a>"

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
