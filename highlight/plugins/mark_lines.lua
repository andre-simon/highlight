Description="Marks the lines defined as comma separated list in the plug-in parameter (BETA status)."

function syntaxUpdate(desc)
  
  if (HL_OUTPUT ~= HL_FORMAT_HTML and HL_OUTPUT ~= HL_FORMAT_XHTML) then
    return
   end
  
  -- explode(seperator, string)
  function explode(d,p)
     local t, ll
     t={}
     ll=0
     if(#p == 1) then return {p} end
     while true do
       l=string.find(p,d,ll,true) -- find the next d in the string
       if l~=nil then -- if "not not" found then..
       --table.insert(t, string.sub(p,ll,l-1)) -- Save it in our array.
       t[tonumber(string.sub(p,ll,l-1))] = 1
       ll=l+1 -- save just after where we found it for searching next time.
     else
       --table.insert(t, string.sub(p,ll)) -- Save what's left in our array.
       t[tonumber(string.sub(p,ll))] = 1
       break -- Break at end, as it should be, according to the lua manual.
     end
   end
   return t
  end

  linesToMark=explode(',', HL_PLUGIN_PARAM)
  currentLineNumber=0

  function DecorateLineBegin(lineNumber)
    if (HL_OUTPUT ~= HL_FORMAT_HTML and HL_OUTPUT ~= HL_FORMAT_XHTML) then
      return
    end
    currentLineNumber = lineNumber
    if (linesToMark[currentLineNumber]) then
      return '<span class="hl mark">'
    end
  end

  function DecorateLineEnd()
    if (HL_OUTPUT ~= HL_FORMAT_HTML and HL_OUTPUT ~= HL_FORMAT_XHTML) then
      return
    end
    if (linesToMark[currentLineNumber]) then
      return '</span>'
    end
  end
  
end

function themeUpdate(desc)
  
  function lighten(colour)
    if string.match(colour, "#%x+")==nil then
      return "rgba(0,0,0,0)"
    end
    
    base_rr = ("0x"..string.match(colour, "%x%x", 2))
    base_gg = ("0x"..string.match(colour, "%x%x", 4))
    base_bb = ("0x"..string.match(colour, "%x%x", 6))
    
    min_bright=math.min(base_rr, base_gg, base_bb)
    max_bright=math.max(base_rr, base_gg, base_bb)
    brightness = (min_bright + max_bright) / (255*2.0)
    
    if (brightness < 0.1) then
      return "rgba(50,50,50, 0.5)"
    elseif (brightness < 0.5) then
      percent = 100
    elseif (brightness > 0.95) then
      percent = -10
    else
      percent = 80
    end
    
    rr = math.floor(base_rr * (100 + percent) / 100 )
    gg = math.floor(base_gg * (100 + percent) / 100 )
    bb = math.floor(base_bb * (100 + percent) / 100 )
    
    if (rr>255) then rr = 255 end
    if (gg>255) then gg = 255 end
    if (bb>255) then bb = 255 end
    return string.format("rgba(%d,%d,%d,0.25)", rr, gg, bb)
  end
  
  if (HL_OUTPUT == HL_FORMAT_HTML or HL_OUTPUT == HL_FORMAT_XHTML) then
    Injections[#Injections+1]=".hl.mark { background-color:"..lighten(Canvas.Colour).."; width:100%;float:left;}"
  end
end

Plugins={
  { Type="lang", Chunk=syntaxUpdate },
  { Type="theme", Chunk=themeUpdate },
}