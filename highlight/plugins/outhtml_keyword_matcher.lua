--[[
Sample plugin file for highlight 3.14

Assumes that CSS is enabled (ie Inline CSS is not set)
]]

Description="Shows matching keywords in HTML output. Edit plug-in to define the keyword group."

-- optional parameter: syntax description
function syntaxUpdate(desc)
  
  if (HL_OUTPUT == HL_FORMAT_HTML or HL_OUTPUT == HL_FORMAT_XHTML) then
    
    -- can use 1 to 4, depending on the syntax definition
    keywordGroup=1
    
    kwID = { }
    kwIDCnt=0
  
    HeaderInjection=[=[
<script type="text/javascript">
/* <![CDATA[ */
function showSameKW(sender){
  var myRegexp = /(kw_\d+)/;
  var kwID = myRegexp.exec(sender.id)[1]; 
  var elements=document.getElementsByTagName('span');
  for (var i = 0; i < elements.length; i++) { 
     if (elements[i].id.indexOf( kwID)==0 && myRegexp.exec(elements[i].id)[1] ==  kwID){
       elements[i].style.background= (elements[i].style.background=='') ? 'yellow': '';
     }
  }
}
/* ]]> */
</script>
]=]
    end
  
   --may be triggered twice per keyword  
   function OnStateChange(oldState, newState, token, kwgroup)
    if newState==HL_KEYWORD  and kwgroup==keywordGroup then
	
	if kwID[token] == nil then
	  kwIDCnt=kwIDCnt+1
	  kwID[token] = { }
	  kwID[token][0] = kwIDCnt
	  kwID[token][1] = 1
	else
	  kwID[token][1] = kwID[token][1] + 1
	end
	
    end
    return newState
  end
   
  function Decorate(token, state)
    if (state ~= HL_KEYWORD or kwID[token]==nil or HL_OUTPUT ~= HL_FORMAT_HTML) then
      return
    end
    return '<span class="hl box" id="kw_'..kwID[token][0]..'_'..kwID[token][1]..'" onclick="showSameKW(this);">'..token..'</span>'
  end
  
end


function themeUpdate(desc)
  if (HL_OUTPUT == HL_FORMAT_HTML or HL_OUTPUT == HL_FORMAT_XHTML) then
    Injections[#Injections+1]=".hl.box { border-width:1px;border-style:dotted;border-color:gray; cursor: pointer;}"
  end
end
--The Plugins array assigns code chunks to themes or language definitions.
--The chunks are interpreted after the theme or lang file were parsed,
--so you can refer to elements of these files

Plugins={

  { Type="lang", Chunk=syntaxUpdate },
  { Type="theme", Chunk=themeUpdate },

}
