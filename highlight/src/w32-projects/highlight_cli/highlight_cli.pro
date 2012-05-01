# -------------------------------------------------
# Project created by QtCreator 2010-02-03T21:56:57
# -------------------------------------------------
QT -= core \
    gui
TARGET = highlight
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

#INCLUDEPATH += ../../ \
#    ../../src/core/

win32:INCLUDEPATH += d:/devel/cpp/lua_bin_5.1.4
win32:INCLUDEPATH += d:/devel/cpp/boost_1_42_0
win32:INCLUDEPATH += ../../include/

win32:DESTDIR = ../../../

SOURCES += ../../cli/main.cpp \
    ../../cli/help.cpp \
    ../../cli/cmdlineoptions.cpp \
    ../../cli/arg_parser.cc
HEADERS += ../../src/cli/main.h \
    ../../cli/help.h \
    ../../cli/cmdlineoptions.h \
    ../../cli/arg_parser.h

win32:LIBS += -L../.. -lhighlight
unix:LIBS += -L.. -lhighlight

!contains( LINKTYPE, LUA52 ) {
win32:LIBS += -Ld:/devel/cpp/lua_bin_5.1.4. -llua
unix:LIBS += -L.. -llua5.1
}
contains( LINKTYPE, LUA52 ) {
win32:LIBS += -Ld:/devel/cpp/lua_bin_5.2.0 -llua
unix:LIBS += -L.. -llua5.2
DEFINES += USE_LUA52
}

#win32:QMAKE_POST_LINK = d:/devel/upx/upx.exe --best ../../../highlight.exe
