
QT -= core gui
TARGET = highlight
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app

win32 {

DESTDIR = ..
SOURCES += ..\\..\\cli\\main.cpp \
    ..\\..\\cli\\help.cpp \
    ..\\..\\cli\\cmdlineoptions.cpp \
    ..\\..\\cli\\arg_parser.cc
HEADERS += ..\\..\\cli\\main.h \
    ..\\..\\cli\\help.h \
    ..\\..\\cli\\cmdlineoptions.h \
    ..\\..\\cli\\arg_parser.h

INCLUDEPATH += d:\\devel\\cpp\\lua_bin_5.1.4
#INCLUDEPATH +=D:/Devel/cpp/LuaJIT-2.0.0/src

INCLUDEPATH += d:\\devel\\cpp\\boost_1_42_0
INCLUDEPATH += ..\\..\\include

}
else {
SOURCES += cli/main.cpp \
    cli/help.cpp \
    cli/cmdlineoptions.cpp \
    cli/arg_parser.cc
HEADERS += src/cli/main.h \
    cli/help.h \
    cli/cmdlineoptions.h \
    cli/arg_parser.h
}

win32:LIBS += -L. -lhighlight
unix:LIBS += -L.. -lhighlight

!contains( LINKTYPE, LUA52 ) {
win32:LIBS += -Ld:/devel/cpp/lua_bin_5.1.4. -llua
#win32:LIBS += -LD:/Devel/cpp/LuaJIT-2.0.0/src  -lluajit
unix:LIBS += -L.. -llua5.1
}
contains( LINKTYPE, LUA52 ) {
win32:LIBS += -Ld:/devel/cpp/lua_bin_5.2.0 -llua
unix:LIBS += -L.. -llua5.2
DEFINES += USE_LUA52
}

win32:QMAKE_POST_LINK = D:\Devel\upx308w\upx.exe --best ..\highlight.exe
