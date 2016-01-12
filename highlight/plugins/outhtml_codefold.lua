
Description="Adds code folding for C style languages to HTML output (BETA)."

function syntaxUpdate(desc)
  
  function Set (list)
    local set = {}
    for _, l in ipairs(list) do set[l] = true end
    return set
  end
  
  local bracketLangs = Set { "C and C++", "C#", "Java", "Javascript", "ASCEND", 
    "Ceylon", "Crack", "Not eXactly C", "Rust","TTCN3", "Yang"}
  
  if not bracketLangs[desc] then
    return
  end
  
  if (HL_OUTPUT == HL_FORMAT_HTML or HL_OUTPUT == HL_FORMAT_XHTML) then
    pID=0      -- just a sequential counter to generate HTML IDs
    pCount=0   -- paranthesis counter to keep track of opening and closing pairs
    openPID={} -- save opening IDs as they are needed again for the close tag IDs
    currentLineNumber=0 -- remember the current line number
    
    HeaderInjection=[=[
  <script type="text/javascript">
  var beginOfBlock = [];
  var endOfBlock = {};

  function make_handler (elem) {
    return function (event) {
      hlToggleFold(elem)
    };
  }
  function hlAddEOB(openId, eob)  {
    endOfBlock[beginOfBlock[openId -1]] = eob;
  }
  function hlAddBtn(openId)  {
    elem = document.getElementById('line' + openId);    
    elem.className = "hl fld hl arrow_down";
    elem.addEventListener("click", make_handler(elem));
  }
  function hlToggleFold(sender){ 
    elem =    document.getElementById(sender.id);
    var isFolding = elem.className.indexOf ('down')>0;    
    elem.className = "hl fld hl arrow_" + (isFolding ? "right":"down");
    for (var i=parseInt(sender.id.substr(4))+1; i<=endOfBlock[sender.id.substr(4)]-1; i++){
      elem =    document.getElementById('line'+i);
      elem.style.display = (isFolding || elem.style.display=='block') ? 'none' : 'block';   
      if (isFolding && elem.className.indexOf ('right') > 0){
        elem.className = "hl fld hl arrow_down";
      }
      if (elem.nextSibling 
        && elem.nextSibling.nodeType==3 
        && !elem.nextSibling.data.match(/\S/) ) {
          elem.parentNode.removeChild(elem.nextSibling);
          if (elem.textContent.length==0) elem.textContent = " ";
        }
      }
    }
  </script>
]=]

FooterInjection=[[
  
  <script type="text/javascript">
    beginOfBlock.forEach(function (item) {
      hlAddBtn(item);
    });
  </script>  
  ]]
    end
 
  function getOpenParen(token)
     pID=pID+1
     pCount=pCount+1
     openPID[pCount] = pID     
     return '<script>beginOfBlock.push('..currentLineNumber..');</script>'..token
   end

   function getCloseParen(token)
     oID=openPID[pCount]
     pCount=pCount-1
     return '<script>hlAddEOB('..oID..', '.. currentLineNumber..');</script>'..token   
   end
   
  function Decorate(token, state)

    if (state ~= HL_OPERATOR or HL_OUTPUT ~= HL_FORMAT_HTML) then
      return
    end

    if token=='{' then
      return getOpenParen(token)
    end

    if token=='}' then
      return getCloseParen(token)
    end
    
  end
  
  function DecorateLineBegin(lineNumber)
    currentLineNumber = string.format("%d", lineNumber)
    return '<span id="line'..currentLineNumber..'" class="hl fld">' 
  end

  function DecorateLineEnd()
    return '</span>'
  end
  
end

function themeUpdate(desc)
  if (HL_OUTPUT == HL_FORMAT_HTML or HL_OUTPUT == HL_FORMAT_XHTML) then
    Injections[#Injections+1]=
  [[
    
.hl.fld { padding-left:25px; }
.hl.arrow_right:before {
  content: '+';
  color: ]]..Default.Colour..[[; 
  position: absolute;     
  left: 10px;   
}
.hl.arrow_down:before {
  content: '-';
  color: ]]..Default.Colour..[[; 
  position: absolute;     
  left: 10px;  
}
.hl.arrow_right, .hl.arrow_down  { 
  cursor: pointer;
} ]]
  end
end

Plugins={
  { Type="lang", Chunk=syntaxUpdate },
  { Type="theme", Chunk=themeUpdate },
}