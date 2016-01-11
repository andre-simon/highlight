Description="Adds code folding to HTML output (BETA status)."

function syntaxUpdate(desc)
  
  if (HL_OUTPUT == HL_FORMAT_HTML or HL_OUTPUT == HL_FORMAT_XHTML) then
    pID=0      -- just a sequential counter to generate HTML IDs
    pCount=0   -- paranthesis counter to keep track of opening and closing pairs
    openPID={} -- save opening IDs as they are needed again for the close tag IDs
    currentLineNumber=0 -- remember the current line number
    
    HeaderInjection=[[
<script type="text/javascript">
 var endOfBlock = {};
 function showCFBegin(sender, beginOfBlock){ 
        if (sender.style.background==''){
                sender.style.background= 'yellow';
                sender.innerHTML = "{ &hellip;";
        } else {
                sender.style.background= '';
                sender.innerHTML = "{";
        }
        for (var i=1+beginOfBlock; i<=endOfBlock[sender.id.substr(3)] -1 ; i++){
          hideLine('hlr_'+i);
    }
  }
function addEOB(openId, eob)  {
   endOfBlock[openId] = eob;
}
function hideLine(id) {
    elem = document.getElementById(id);
        elem.style.display= (elem.style.display=='none') ?  'block' : 'none';           
    if(elem.nextSibling 
       && elem.nextSibling.nodeType==3 
       && !elem.nextSibling.data.match(/\S/) )
    {
       elem.parentNode.removeChild(elem.nextSibling);
           if (elem.textContent.length==0)
                elem.textContent = " ";
    }
}
</script>
]]
    end
 
  function getOpenParen(token)
     pID=pID+1
     pCount=pCount+1
     openPID[pCount] = pID     
     return '<span class="hl box" id="ob_'..pID..
            '" onclick="showCFBegin(this,'..
            string.format("%d", currentLineNumber)..');">'..token..'</span>'
   end

   function getCloseParen(token)
     oID=openPID[pCount]
     pCount=pCount-1
     return '<script>addEOB('..oID..', '..string.format("%d", currentLineNumber)..
            ');</script>'..token   
   end
   
  function Decorate(token, state)

    if (state ~= HL_OPERATOR or HL_OUTPUT ~= HL_FORMAT_HTML) then
      return
    end

    if string.find(token, "{")==1 then
      return getOpenParen(token)
    end

    if string.find(token, "}")==1 then
      return getCloseParen(token)
    end
    
  end
  
  function DecorateLineBegin(lineNumber)
    currentLineNumber = lineNumber
    return '<span id="hlr_'..string.format("%d", lineNumber)..'">'
  end

  function DecorateLineEnd()
    return '</span>'
  end
  
end

function themeUpdate(desc)
  if (HL_OUTPUT == HL_FORMAT_HTML or HL_OUTPUT == HL_FORMAT_XHTML) then
    Injections[#Injections+1]=".hl.box { border-width:1px;border-style:dotted;border-color:gray; cursor: pointer;}"
  end
end

Plugins={
  { Type="lang", Chunk=syntaxUpdate },
  { Type="theme", Chunk=themeUpdate },
}