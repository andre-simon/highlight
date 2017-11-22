---
pagetitle:   Highligh.pp Example
author-meta: Tristano Ajmone
css: example.css
...



# Highlight PP-Macros Usage Example

# External File Examples

Through the `!HighlightFile` macro, external source code files can be passed to Highlight and the HTML output injected into the final document.

## First Example

We'll import and syntax highlight the PureBASIC example file "`example.pb`" without passing any extra options to Highlight.

macro used:

~~~~~~~~~~~~~~~~~~~~~~ { .macro }
!HighlightFile(example.pb)(purebasic)
~~~~~~~~~~~~~~~~~~~~~~


output result:

<pre class="hl purebasic"><code class="purebasic"><span class="hl slc">; ==============================================================================</span>
<span class="hl slc">;                             PureBASIC Example File                            </span>
<span class="hl slc">; ==============================================================================</span>
<span class="hl slc">; &quot;example.pb&quot; | PureBASIC 5.61</span>

<span class="hl kwa">For</span> i<span class="hl opt">=</span><span class="hl num">5</span> <span class="hl kwa">To</span> <span class="hl num">1</span> <span class="hl kwa">Step</span> <span class="hl opt">-</span><span class="hl num">1</span>
  TEXT$ <span class="hl opt">=</span> <span class="hl str">&quot;Iteration number: &quot;</span> <span class="hl opt">+</span> <span class="hl kwc">Str</span>(i) <span class="hl opt">+</span> <span class="hl str">~&quot;</span><span class="hl esc">\n\n</span><span class="hl str">Do you wish to continue?&quot;</span>
  UserChoice <span class="hl opt">=</span> <span class="hl kwc">MessageRequester</span>(<span class="hl str">&quot;Countdown Dialog&quot;</span>, TEXT$, <span class="hl kwb">#PB_MessageRequester_YesNo</span> <span class="hl opt">|</span> <span class="hl kwb">#PB_MessageRequester_Info</span>)
  <span class="hl kwa">If</span> UserChoice <span class="hl opt">=</span> <span class="hl kwb">#PB_MessageRequester_No</span>
    <span class="hl kwa">Break</span>
  <span class="hl kwa">EndIf</span>
<span class="hl kwa">Next</span></code></pre>


## Second Example

Now we'll highlight the same external file, this time passing some extra options to Highlight in order to show line numbers.

macro used:

~~~~~~~~~~~~~~~~~~~~~~ { .macro }
!HighlightFile(example.pb)(purebasic)(--line-numbers --line-number-length=2)
~~~~~~~~~~~~~~~~~~~~~~


output result:

<pre class="hl purebasic"><code class="purebasic"><span class="hl lin"> 1 </span><span class="hl slc">; ==============================================================================</span>
<span class="hl lin"> 2 </span><span class="hl slc">;                             PureBASIC Example File                            </span>
<span class="hl lin"> 3 </span><span class="hl slc">; ==============================================================================</span>
<span class="hl lin"> 4 </span><span class="hl slc">; &quot;example.pb&quot; | PureBASIC 5.61</span>
<span class="hl lin"> 5 </span>
<span class="hl lin"> 6 </span><span class="hl kwa">For</span> i<span class="hl opt">=</span><span class="hl num">5</span> <span class="hl kwa">To</span> <span class="hl num">1</span> <span class="hl kwa">Step</span> <span class="hl opt">-</span><span class="hl num">1</span>
<span class="hl lin"> 7 </span>  TEXT$ <span class="hl opt">=</span> <span class="hl str">&quot;Iteration number: &quot;</span> <span class="hl opt">+</span> <span class="hl kwc">Str</span>(i) <span class="hl opt">+</span> <span class="hl str">~&quot;</span><span class="hl esc">\n\n</span><span class="hl str">Do you wish to continue?&quot;</span>
<span class="hl lin"> 8 </span>  UserChoice <span class="hl opt">=</span> <span class="hl kwc">MessageRequester</span>(<span class="hl str">&quot;Countdown Dialog&quot;</span>, TEXT$, <span class="hl kwb">#PB_MessageRequester_YesNo</span> <span class="hl opt">|</span> <span class="hl kwb">#PB_MessageRequester_Info</span>)
<span class="hl lin"> 9 </span>  <span class="hl kwa">If</span> UserChoice <span class="hl opt">=</span> <span class="hl kwb">#PB_MessageRequester_No</span>
<span class="hl lin">10 </span>    <span class="hl kwa">Break</span>
<span class="hl lin">11 </span>  <span class="hl kwa">EndIf</span>
<span class="hl lin">12 </span><span class="hl kwa">Next</span></code></pre>


# Code Block Examples

With the `!Highlight` macro it's possible to invoke Highlight on code blocks inside the document.

Let's apply the `!Highlight` macro to a block of PureBASIC code, with some extra Highlight options for line numbering.



macro used:


`````````````````` { .macro }
!Highlight(purebasic)(-l -j 2)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
; PureBASIC 5.61 Example (inside markdown document)

Truth$ = "André Simon's Highlight is 100% cool!" ; Undeniable!

; Let's build an Ascii frame for this truthful statement:
; =======================================================

FrameCenter$ = "| " + Truth$ + ~" |\n"
FrameBorder$ = LSet("+", Len(FrameCenter$)-2, "-") + "+"
FramedTruth$ = FrameBorder$ + #LF$ + FrameCenter$ + FrameBorder$

; Now by printing FramedTruth$ you'll get:
; +---------------------------------------+
; | André Simon's Highlight is 100% cool! |
; +---------------------------------------+

Debug FramedTruth$
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

``````````````````

output result:

<pre class="hl purebasic"><code class="purebasic"><span class="hl lin"> 1 </span><span class="hl slc">; PureBASIC 5.61 Example (inside markdown document)</span>
<span class="hl lin"> 2 </span>
<span class="hl lin"> 3 </span>Truth$ <span class="hl opt">=</span> <span class="hl str">&quot;André Simon&apos;s Highlight is 100% cool!&quot;</span> <span class="hl slc">; Undeniable!</span>
<span class="hl lin"> 4 </span>
<span class="hl lin"> 5 </span><span class="hl slc">; Let&apos;s build an Ascii frame for this truthful statement:</span>
<span class="hl lin"> 6 </span><span class="hl slc">; =======================================================</span>
<span class="hl lin"> 7 </span>
<span class="hl lin"> 8 </span>FrameCenter$ <span class="hl opt">=</span> <span class="hl str">&quot;| &quot;</span> <span class="hl opt">+</span> Truth$ <span class="hl opt">+</span> <span class="hl str">~&quot; |</span><span class="hl esc">\n</span><span class="hl str">&quot;</span>
<span class="hl lin"> 9 </span>FrameBorder$ <span class="hl opt">=</span> <span class="hl kwc">LSet</span>(<span class="hl str">&quot;+&quot;</span>, <span class="hl kwc">Len</span>(FrameCenter$)<span class="hl opt">-</span><span class="hl num">2</span>, <span class="hl str">&quot;-&quot;</span>) <span class="hl opt">+</span> <span class="hl str">&quot;+&quot;</span>
<span class="hl lin">10 </span>FramedTruth$ <span class="hl opt">=</span> FrameBorder$ <span class="hl opt">+</span> <span class="hl kwb">#LF$</span> <span class="hl opt">+</span> FrameCenter$ <span class="hl opt">+</span> FrameBorder$
<span class="hl lin">11 </span>
<span class="hl lin">12 </span><span class="hl slc">; Now by printing FramedTruth$ you&apos;ll get:</span>
<span class="hl lin">13 </span><span class="hl slc">; +---------------------------------------+</span>
<span class="hl lin">14 </span><span class="hl slc">; | André Simon&apos;s Highlight is 100% cool! |</span>
<span class="hl lin">15 </span><span class="hl slc">; +---------------------------------------+</span>
<span class="hl lin">16 </span>
<span class="hl lin">17 </span><span class="hl kwa">Debug</span> FramedTruth$</code></pre>



# Adding A Color Theme

Now we'll import Highlight's `edit-purebasic` theme into the document.

macro used:

~~~~~~~~~~~~~~~~~~~~~~ { .macro }
!HighlightInlineTheme(edit-purebasic)
~~~~~~~~~~~~~~~~~~~~~~


... which will inject the following code in the HTML document's source:

``` html
<style type="text/css">
/* Style definition file generated by highlight 3.39, http://www.andre-simon.de/ */
/* highlight theme: PureBASIC */
body.hl	{ background-color:#ffffdf; }
pre.hl	{ color:#000000; background-color:#ffffdf; font-size:10pt; font-family:'Courier New',monospace;}
.hl.num { color:#000000; }
.hl.esc { color:#0080ff; }
.hl.str { color:#0080ff; }
.hl.pps { color:#0080ff; }
.hl.slc { color:#00aaaa; }
.hl.com { color:#00aaaa; }
.hl.ppc { color:#924b72; }
.hl.opt { color:#000000; }
.hl.ipl { color:#0080ff; }
.hl.lin { color:#808080; }
.hl.kwa { color:#006666; font-weight:bold; }
.hl.kwb { color:#924b72; }
.hl.kwc { color:#006666; }
.hl.kwd { color:#0080ff; }


</style>

```

<style type="text/css">
/* Style definition file generated by highlight 3.39, http://www.andre-simon.de/ */
/* highlight theme: PureBASIC */
body.hl	{ background-color:#ffffdf; }
pre.hl	{ color:#000000; background-color:#ffffdf; font-size:10pt; font-family:'Courier New',monospace;}
.hl.num { color:#000000; }
.hl.esc { color:#0080ff; }
.hl.str { color:#0080ff; }
.hl.pps { color:#0080ff; }
.hl.slc { color:#00aaaa; }
.hl.com { color:#00aaaa; }
.hl.ppc { color:#924b72; }
.hl.opt { color:#000000; }
.hl.ipl { color:#0080ff; }
.hl.lin { color:#808080; }
.hl.kwa { color:#006666; font-weight:bold; }
.hl.kwb { color:#924b72; }
.hl.kwc { color:#006666; }
.hl.kwd { color:#0080ff; }


</style>


Enjoy...