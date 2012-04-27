--[[
Sample plugin file for highlight 3.9
]]

Description="Add reference links to local C or C++ headers. Set base_url in the plug-in script if needed."

-- optional parameter: syntax description
function syntaxUpdate(desc)

    -- INSERT BASE URL HERE
  base_url=''
  
  if desc~="C and C++" then
     return
  end
 
  --see comment in themeUpdate
  table.insert( Keywords,
               { Id=6, Regex=[[\w+\.h[px]*]]
               } )

  function Decorate(token, state, docformat)

    if (docformat ~= HL_FORMAT_HTML and docformat ~= HL_FORMAT_XHTML) then
      return
    end

    if state==HL_PREPROC_STRING and string.find(token, "%w+.h[px]*")==1 then
      return '<a class="hl" href="' .. base_url.. string.lower(token) ..'.html">'.. token .. '</a>'
    end

  end
end


function themeUpdate(desc)
   -- no need to add a bogus style for the 6th keyword class defined in syntaxUpdate,
   -- the regex is just needed to get the complete token, but it will be recognized
   -- as string because string has higher priority
  Injection="a.hl, a.hl:visited {color:inherit;font-weight:inherit;}"
end

--The Plugins array assigns code chunks to themes or language definitions.
--The chunks are interpreted after the theme or lang file were parsed,
--so you can refer to elements of these files

Plugins={

  { Type="lang", Chunk=syntaxUpdate },
  { Type="theme", Chunk=themeUpdate },

}
