<!DOCTYPE html>
<html>
<head>
<title>Source file</title>
<style type="text/css">
body.hl { background-color:#1e1616; }
pre.hl  { color:#c4b1a8; background-color:#1e1616; font-size:11pt; font-family:'Noto Mono';}
.hl.num { color:#9c928c; }
.hl.esc { color:#837d83; }
.hl.str { color:#635d63; }
.hl.pps { color:#635d63; }
.hl.slc { color:#c4674a; }
.hl.com { color:#c4674a; }
.hl.ppc { color:#e7a731; }
.hl.opt { color:#7b94ad; }
.hl.ipl { color:#9c959c; }
.hl.lin { color:#9c928c; }
.hl.kwa { color:#49d4d6; }
.hl.kwb { color:#ba1919; }
.hl.kwc { color:#7b2421; }
.hl.kwd { color:#ad5139; }

/* Plug-in theme injections: */
.hl.arrow_fold:before {
  content: '+';
  color: #C4B1A8; 
  position: absolute;     
  left: 1em;   
}
.hl.arrow_fold:after {
  content: '\2026';
  color: #C4B1A8; 
  border-width:1px;border-style:dotted;border-color:#C4B1A8;     
  margin-left: 1em;
  padding:0px 2px 0px;
}
.hl.arrow_unfold:before {
  content: '-';
  color: #C4B1A8; 
  position: absolute;     
  left: 1em;  
}
.hl.arrow_fold, .hl.arrow_unfold  {
  cursor: pointer;
  /*background-color: #eee;*/
  width: 100%;
  display: inline-block;
}
.hl.arrow_fold {
  border-width: 1px;
  border-bottom-style: groove;
  border-color: #C4B1A8;
} 
</style>
</head>
<body class="hl">
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
  if (eob==beginOfBlock[openId -1] || eob - beginOfBlock[openId -1]< 1 ){
    delete beginOfBlock[openId -1];
  } else {
    endOfBlock[beginOfBlock[openId -1] ] = eob;
  }
}
function hlAddTitle(line, num, isFolding){
  elem.title="Click to "+(isFolding? "unfold ": "fold ") + num + " line"+(num>1?"s":"");
}
function hlAddBtn(openId)  {
  elem = document.getElementById('l_' + openId);    
  elem.className = "hl fld hl arrow_unfold";
  elem.addEventListener("click", make_handler(elem));
  hlAddTitle(elem, (endOfBlock[openId]-openId-1), false);
}
function hlToggleFold(sender){ 
  elem =    document.getElementById(sender.id);
  var num = parseInt(sender.id.substr(2));
  var isFolding = elem.className.indexOf ('unfold')>0;
  foldedLines[num] = isFolding ;
  elem.className = "hl fld hl arrow_" + (isFolding ? "fold":"unfold");
  hlAddTitle(elem, (endOfBlock[num]-num-1), isFolding);
  for (var i=num+1; i<=endOfBlock[num]-1; i++){
    if (!foldedLines[i]) foldedLines[i] = 0 ;
    foldedLines[i] = foldedLines[i] + (isFolding ? 1:-1);
    elem = document.getElementById('l_'+i);
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
<pre class="hl"><span id="l_1" class="hl fld"><span class="hl lin">01 </span><span class="hl kwa">program</span> HalloWelt<span class="hl opt">;</span></span>
<span id="l_2" class="hl fld"><span class="hl lin">02 </span>    <span class="hl kwa">procedure</span> foo<span class="hl opt">;</span></span>
<span id="l_3" class="hl fld"><span class="hl lin">03 </span>        <span class="hl kwa">procedure</span> bar<span class="hl opt">;</span></span>
<span id="l_4" class="hl fld"><span class="hl lin">04 </span>        <span class="hl kwa"><script>beginOfBlock.push(4);</script>begin</span></span>
<span id="l_5" class="hl fld"><span class="hl lin">05 </span>            <span class="hl kwd">writeln</span><span class="hl opt">(</span><span class="hl str">'Hallo Welt'</span><span class="hl opt">);</span></span>
<span id="l_6" class="hl fld"><span class="hl lin">06 </span>        <span class="hl kwa"><script>hlAddEOB(1, 6);</script>end</span><span class="hl opt">;</span></span>
<span id="l_7" class="hl fld"><span class="hl lin">07 </span>    <span class="hl kwa"><script>beginOfBlock.push(7);</script>begin</span></span>
<span id="l_8" class="hl fld"><span class="hl lin">08 </span>        bar</span>
<span id="l_9" class="hl fld"><span class="hl lin">09 </span>    <span class="hl kwa"><script>hlAddEOB(2, 9);</script>end</span><span class="hl opt">;</span></span>
<span id="l_10" class="hl fld"><span class="hl lin">10 </span><span class="hl kwa"><script>beginOfBlock.push(10);</script>begin</span> </span>
<span id="l_11" class="hl fld"><span class="hl lin">11 </span>    foo<span class="hl opt">;</span> <span class="hl slc">//kein Compilerfehler</span></span>
<span id="l_12" class="hl fld"><span class="hl lin">12 </span>    bar <span class="hl slc">//Compilerfehler, da bar nicht sichtbar</span></span>
<span id="l_13" class="hl fld"><span class="hl lin">13 </span><span class="hl kwa"><script>hlAddEOB(3, 13);</script>end</span><span class="hl opt">.</span></span>
</pre>
<script type="text/javascript">
/* <![CDATA[ */
beginOfBlock.forEach(function (item) {
  hlAddBtn(item);
});
var hlFoldElements=document.getElementsByClassName('hl fld');
for (var i=0; i<hlFoldElements.length; i++){
  hlFoldElements[i].style.setProperty('padding-left', '1.5em');   
}
/* ]]> */
</script>  

</body>
</html>
<!--HTML generated by highlight 3.28, http://www.andre-simon.de/-->
