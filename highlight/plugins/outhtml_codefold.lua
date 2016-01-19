
Description="Adds code folding for C style languages to HTML output (not compatible with inline CSS or ordered list output)."

function syntaxUpdate(desc)
    
  function init()
    pID=0      -- just a sequential counter to generate HTML IDs
    pCount=0   -- paranthesis counter to keep track of opening and closing pairs
    openPID={} -- save opening IDs as they are needed again for the close tag IDs
    currentLineNumber=0 -- remember the current line number
    notEmbedded=false   -- disable plugin for nested code snippets (like JS in HTML)
  end
    
  init()  
  
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
  
  if (HL_OUTPUT ~= HL_FORMAT_HTML and HL_OUTPUT ~= HL_FORMAT_XHTML) then
    return
  end
   
    HeaderInjection=[=[
  <script type="text/javascript">
  /* <![CDATA[ */
  var beginOfBlock = [];
  var endOfBlock = {};
  var foldedLines = {};

  function make_handler (elem) {
    return function (event) {
      hlToggleFold(elem)
    };
  }
  function hlAddEOB(openId, eob)  {
    if (eob==beginOfBlock[openId -1]){
      delete beginOfBlock[openId -1];
    } else {
      endOfBlock[beginOfBlock[openId -1]] = eob;
    }
  }
  function hlAddBtn(openId)  {
    elem = document.getElementById('line' + openId);    
    elem.className = "hl fld hl arrow_unfold";
    elem.addEventListener("click", make_handler(elem));
  }
  function hlToggleFold(sender){ 
    elem =    document.getElementById(sender.id);
    var num = parseInt(sender.id.substr(4));
    var isFolding = elem.className.indexOf ('unfold')>0;
    foldedLines[num] = isFolding ;
    elem.className = "hl fld hl arrow_" + (isFolding ? "fold":"unfold");
    for (var i=num+1; i<=endOfBlock[num]-1; i++){
      if (!foldedLines[i]) foldedLines[i] = 0 ;
      foldedLines[i] = foldedLines[i] + (isFolding ? 1:-1);
      elem = document.getElementById('line'+i);	  
      if (    (isFolding || elem.style.display=='block')
           || (!isFolding && foldedLines[i]>=1 && elem.className.indexOf ('_fold') < 0) 
           || (!isFolding && foldedLines[i]>=2 && elem.className.indexOf ('_fold') > 0)) {
          elem.style.display = 'none';
      } else {
          elem.style.display = 'block';
      }
      if (elem.nextSibling 
        && elem.nextSibling.nodeType==3 
        && !elem.nextSibling.data.match(/\S/) ) {
          elem.parentNode.removeChild(elem.nextSibling);
          if (elem.textContent.length==0) elem.textContent = " ";
        }
      }
    }
   /* ]]> */
  </script>
]=]

  -- assign some CSS via JS to keep output sane for browsers with JS disabled
  FooterInjection=[=[

  <script type="text/javascript">
  /* <![CDATA[ */
    beginOfBlock.forEach(function (item) {
      hlAddBtn(item);
    });
    var hlElements=document.getElementsByClassName('hl');
    if (hlElements.length>1){
      var pre = hlElements[1];
      if (pre instanceof HTMLPreElement) {
        pre.style.setProperty('min-height', pre.clientHeight+'px');   
      }
    }
    hlElements=document.getElementsByClassName('hl fld');
    for (var i=0; i<hlElements.length; i++){
      hlElements[i].style.setProperty('padding-left', '1.5em');   
    }
    /* ]]> */
  </script>  
  ]=]
 
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

    if (state ~= HL_OPERATOR or notEmbedded==false) then
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
    
    --TODO we need an initialization hook:
    if lineNumber==1 then
        init()
        notEmbedded=true
    end 
    -- the line number does not increase for wrapped lines (--wrap, --wrap-simple)
    if (tonumber(currentLineNumber)==lineNumber or notEmbedded==false) then
      return
    end
    currentLineNumber = string.format("%d", lineNumber)
    return '<span id="line'..currentLineNumber..'" class="hl fld">' 
  end

  function DecorateLineEnd(lineNumber)
    
    if (tonumber(currentLineNumber)==lineNumber or notEmbedded==false) then
      return
    end
    return '</span>'
  end
  
end

function themeUpdate(desc)
  if (HL_OUTPUT == HL_FORMAT_HTML or HL_OUTPUT == HL_FORMAT_XHTML) then
    Injections[#Injections+1]=[[
.hl.arrow_fold:before {
  content: '+';
  color: ]]..Default.Colour..[[; 
  position: absolute;     
  left: 1em;   
}
.hl.arrow_fold:after {
  content: '\2026';
  color: ]]..Default.Colour..[[; 
  border-width:1px;border-style:dotted;border-color:]]..Default.Colour..[[;     
  margin-left: 1em;
  padding:0px 2px 0px;
}
.hl.arrow_unfold:before {
  content: '-';
  color: ]]..Default.Colour..[[; 
  position: absolute;     
  left: 1em;  
}
.hl.arrow_fold, .hl.arrow_unfold  { 
  cursor: pointer;
} ]]
  end
end

Plugins={
  { Type="lang", Chunk=syntaxUpdate },
  { Type="theme", Chunk=themeUpdate },
}