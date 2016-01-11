Description="Marks the lines defined as plug-in parameter (BETA status)."

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
    currentLineNumber = lineNumber
    if (linesToMark[currentLineNumber]) then
      return '<span class="hl mark"'..string.format("%d", lineNumber)..'">'
    end
  end

  function DecorateLineEnd()
    if (linesToMark[currentLineNumber]) then
      return '</span>'
    end
  end
  
end

function themeUpdate(desc)
  if (HL_OUTPUT == HL_FORMAT_HTML or HL_OUTPUT == HL_FORMAT_XHTML) then
    Injections[#Injections+1]=".hl.mark { background-color:yellow; width:100%;float:left;}"
  end
end

Plugins={
  { Type="lang", Chunk=syntaxUpdate },
  { Type="theme", Chunk=themeUpdate },
}