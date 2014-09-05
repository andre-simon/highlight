--[[
Sample plugin file for highlight 3.9
]]

Description="Add qtproject.org reference links to HTML, LaTeX, RTF and ODT output of C++ code"

-- optional parameter: syntax description
function syntaxUpdate(desc)

  if desc~="C and C++" then
     return
  end
      
  function getURL(token)
     url='http://qt-project.org/doc/qt-5/'..string.lower(token).. '.html'
     
     if (HL_OUTPUT== HL_FORMAT_HTML or HL_OUTPUT == HL_FORMAT_XHTML) then
        return '<a class="hl" target="new" href="' .. url .. '">'.. token .. '</a>'
      elseif (HL_OUTPUT == HL_FORMAT_LATEX) then
	return '\\href{'..url..'}{'..token..'}'
      elseif (HL_OUTPUT == HL_FORMAT_RTF) then
	return '{{\\field{\\*\\fldinst HYPERLINK "'..url..'" }{\\fldrslt\\ul\\ulc0 '..token..'}}}'
      elseif (HL_OUTPUT == HL_FORMAT_ODT) then
	return '<text:a xlink:type="simple" xlink:href="'..url..'">'..token..'</text:a>'
     end
   end

  function Decorate(token, state)

    if (state ~= HL_STANDARD and state ~= HL_KEYWORD and state ~=HL_PREPROC) then
      return
    end

    if string.find(token, "Q%u%l")==1 then
      return getURL(token)
    end

  end
end


function themeUpdate(desc)
  if (HL_OUTPUT == HL_FORMAT_HTML or HL_OUTPUT == HL_FORMAT_XHTML) then
    Injections[#Injections+1]="a.hl, a.hl:visited {color:inherit;font-weight:inherit;}"
  elseif (HL_OUTPUT==HL_FORMAT_LATEX) then
    Injections[#Injections+1]="\\usepackage[colorlinks=false, pdfborderstyle={/S/U/W 1}]{hyperref}"
  end
end

--The Plugins array assigns code chunks to themes or language definitions.
--The chunks are interpreted after the theme or lang file were parsed,
--so you can refer to elements of these files

Plugins={

  { Type="lang", Chunk=syntaxUpdate },
  { Type="theme", Chunk=themeUpdate },

}
