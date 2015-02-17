# -------------------------------------------------
# Project created by QtCreator 2009-03-03T22:45:06
# -------------------------------------------------

TEMPLATE = app

contains( LINKTYPE, SHARED ) {
TARGET = highlight-gui-shared
}
!contains( LINKTYPE, SHARED ) {
TARGET = highlight-gui
}

greaterThan(QT_MAJOR_VERSION, 4): QT += core gui widgets
DEFINES += O2 QT

win32:CONFIG += static

win32:DESTDIR = ..\\..\\..
unix:DESTDIR = ../..

win32:INCLUDEPATH += include/
win32:INCLUDEPATH += gui-qt/

LIBS += -L../.. -lhighlight

win32:LIBS += -Ld:/devel/cpp/lua_bin_5.3.0 -llua
win32:INCLUDEPATH += d:/devel/cpp/lua_bin_5.3.0

!contains( LINKTYPE, LUA52 ) {
unix:LIBS += -L.. -llua5.1
}
contains( LINKTYPE, LUA52 ) {
unix:LIBS += -L.. -llua5.2
}

win32{
INCLUDEPATH += .
INCLUDEPATH += ..\\..\\include
INCLUDEPATH += d:/devel/cpp/boost_1_42_0
SOURCES += ..\\..\\gui-qt\\main.cpp ..\\..\\gui-qt\\mainwindow.cpp ..\\..\\gui-qt\\io_report.cpp  ..\\..\\gui-qt\\showtextfile.cpp
HEADERS += ..\\..\\gui-qt\\mainwindow.h ..\\..\\gui-qt\\io_report.h  ..\\..\\gui-qt\\showtextfile.h
FORMS += ..\\..\\gui-qt\\mainwindow.ui  ..\\..\\gui-qt\\io_report.ui ..\\..\\gui-qt\\showtextfile.ui

}
else {
INCLUDEPATH += ../../include
SOURCES += ../../gui-qt/main.cpp  ../../gui-qt/mainwindow.cpp ../../gui-qt/io_report.cpp  ../../gui-qt/showtextfile.cpp
HEADERS += ../../gui-qt/mainwindow.h ../../gui-qt/io_report.h  ../../gui-qt/showtextfile.h
FORMS += ../../gui-qt/mainwindow.ui  ../../gui-qt/io_report.ui ../../gui-qt/showtextfile.ui
}

RESOURCES = highlight-gui.qrc
TRANSLATIONS = gui-qt/highlight_de_DE.ts gui-qt/highlight_es_ES.ts gui-qt/highlight_cs_CZ.ts gui-qt/highlight_zh_CN.ts

win32:RC_FILE = highlight-gui.rc

win32:QMAKE_POST_LINK = $$quote(D:\Devel\upx308w\upx.exe --best D:\Devel\cpp\syntaxhighlight-code\highlight\highlight-gui.exe)

unix {
DEFINES += DATA_DIR=\\\"/usr/share/highlight/\\\" \
    CONFIG_DIR=\\\"/etc/highlight/\\\" \
    HL_DOC_DIR=\\\"/usr/share/doc/highlight/\\\"
    message("setting unix default paths")
}

HEADERS += \
    ../../gui-qt/debuginfo.h

SOURCES += \
    ../../gui-qt/debuginfo.cpp

FORMS += \
    ../../gui-qt/debuginfo.ui
