!comment(USAGE INSTRUCTIONS)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
This markdown source file contains PP macros and needs to be preprocessed by PP before being fed to pandoc.

Ensure that pandoc, PP and Highlight are available on the system `%PATH%`, then execute the `build-example.bat` script.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# Highlight PP-Macros Usage Example

# First Example

We import the PureBASIC example file "`example.pb`" without passing any extra options to Highlight:

!HighlightFile(example.pb)(purebasic)

# Second Example

Now we import the same file, this time passing some extra options to Highlight in order to show line numbers:

!HighlightFile(example.pb)(purebasic)(--line-numbers --line-length=1)

# Add Color Theme

Now we import Highlight's `edit-purebasic` theme into the document:

!HighlightInlineTheme(edit-purebasic)
