---
pagetitle:   Highligh.pp Example
author-meta: Tristano Ajmone
css: example.css
...

!comment( ~~~ USAGE INSTRUCTIONS ~~~ )
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
This markdown source file contains PP macros and needs to be preprocessed by PP before being fed to pandoc.

Ensure that pandoc, PP and Highlight are available on the system `%PATH%`, then execute the `build-example.bat` script.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Highlight PP-Macros Usage Example

# External File Examples

Through the `!raw(!HighlightFile)` macro, external source code files can be passed to Highlight and the HTML output injected into the final document.

## First Example

We'll import and syntax highlight the PureBASIC example file "`example.pb`" without passing any extra options to Highlight.

macro used:

!raw
``````````````````````````````````````````````````````````````````````````````
~~~~~~~~~~~~~~~~~~~~~~ { .macro }
!HighlightFile(example.pb)(purebasic)
~~~~~~~~~~~~~~~~~~~~~~
``````````````````````````````````````````````````````````````````````````````

output result:

!HighlightFile(example.pb)(purebasic)

## Second Example

Now we'll highlight the same external file, this time passing some extra options to Highlight in order to show line numbers.

macro used:

!raw
``````````````````````````````````````````````````````````````````````````````
~~~~~~~~~~~~~~~~~~~~~~ { .macro }
!HighlightFile(example.pb)(purebasic)(--line-numbers --line-number-length=2)
~~~~~~~~~~~~~~~~~~~~~~
``````````````````````````````````````````````````````````````````````````````

output result:

!HighlightFile(example.pb)(purebasic)(--line-numbers --line-number-length=2)

# Code Block Examples

With the `!raw(!Highlight)` macro it's possible to invoke Highlight on code blocks inside the document.

Let's apply the `!raw(!Highlight)` macro to a block of PureBASIC code, with some extra Highlight options for line numbering.

!define( example )
``````````````````````````````````````````````````````````````````````````````
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
``````````````````````````````````````````````````````````````````````````````

macro used:


`````````````````` { .macro }
!rawdef(example)
``````````````````

output result:

!example

# Adding A Color Theme

Now we'll import Highlight's `edit-purebasic` theme into the document.

macro used:

~~~~~~~~~~~~~~~~~~~~~~ { .macro }
!raw{!HighlightInlineTheme(edit-purebasic)}
~~~~~~~~~~~~~~~~~~~~~~


... which will inject the following code in the HTML document's source:

``` html
!HighlightInlineTheme(edit-purebasic)
```

!HighlightInlineTheme(edit-purebasic)

Enjoy...