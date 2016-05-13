/*
 * tclhighlight.c -- a minimal Tcl wrapper for highlight
 * based on the ansifilter extension of Martin Heinrich
 *
 * See README_TCL for compilation and usage
 *
 * Edit theme and syntax below
 */

#include <tcl.h>
#include <memory>
#include "../../src/include/codegenerator.h"

// Tcl namespace
#define NS "highlight"

// highlight requires C++ compiler
extern "C" {

static int
Execute_Escape_Cmd(highlight::OutputType type, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    int i;
    // XXX this creates and destroys new instances for every call!
    unique_ptr<highlight::CodeGenerator> generator(highlight::CodeGenerator::getInstance(type));

    generator->initTheme ( "/usr/share/highlight/themes/edit-kwrite.theme" ); //EDIT theme
    generator->loadLanguage ( "/usr/share/highlight/langDefs/c.lang" );       //EDIT language definition
    generator->setFragmentCode(1);  // -f
    generator->disableTrailingNL(1);

    for (i = 1; i < objc; i++) {
        Tcl_AppendResult(interp, generator->generateString(Tcl_GetString(objv[i])).c_str(), NULL);
    }
    return TCL_OK;
}

static int
LaTeXEscape_Cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return Execute_Escape_Cmd(highlight::LATEX, interp, objc, objv);
}

static int
TeXEscape_Cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return Execute_Escape_Cmd(highlight::TEX, interp, objc, objv);
}

static int
HtmlEscape_Cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return Execute_Escape_Cmd(highlight::HTML, interp, objc, objv);
}

static int
BBCodeEscape_Cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return Execute_Escape_Cmd(highlight::BBCODE, interp, objc, objv);
}

static int
PangoEscape_Cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return Execute_Escape_Cmd(highlight::PANGO, interp, objc, objv);
}

static int
AnsiEscape_Cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return Execute_Escape_Cmd(highlight::ANSI, interp, objc, objv);
}

static int
Xterm256Escape_Cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *const objv[])
{
    return Execute_Escape_Cmd(highlight::XTERM256, interp, objc, objv);
}
/*
 * Tclhighlight_Init -- Called when Tcl loads your extension.
 */
int DLLEXPORT
Tclhighlight_Init(Tcl_Interp *interp)
{
    if (Tcl_InitStubs(interp, TCL_VERSION, 0) == NULL) {
        return TCL_ERROR;
    }
    // provide package
    if (Tcl_PkgProvide(interp, "highlight", "0.1") == TCL_ERROR) {
        return TCL_ERROR;
    }
    // create command
    Tcl_CreateObjCommand(interp, NS "::tex", TeXEscape_Cmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, NS "::latex", LaTeXEscape_Cmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, NS "::html", HtmlEscape_Cmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, NS "::bbcode", BBCodeEscape_Cmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, NS "::pango", PangoEscape_Cmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, NS "::ansi", AnsiEscape_Cmd, NULL, NULL);
    Tcl_CreateObjCommand(interp, NS "::xterm256", Xterm256Escape_Cmd, NULL, NULL);
    return TCL_OK;
}

} // extern "C"
