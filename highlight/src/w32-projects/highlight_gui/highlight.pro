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

win32:CONFIG += static
CONFIG += precompile_header
PRECOMPILED_HEADER =../../gui-qt/precomp.h
DEFINES += O2 QT
win32:DESTDIR = ../../../
unix:DESTDIR = ../
SOURCES += ../../gui-qt/main.cpp \
    ../../gui-qt/mainwindow.cpp \
    ../../gui-qt/io_report.cpp \
    ../../gui-qt/showtextfile.cpp

win32:INCLUDEPATH += ../../include/
win32:INCLUDEPATH += ../../gui-qt/
win32:INCLUDEPATH += d:/devel/cpp/lua_bin_5.1.4
win32:INCLUDEPATH += d:/devel/cpp/boost_1_42_0

HEADERS += ../../gui-qt/mainwindow.h \
 #   ../../gui-qt/precomp.h \
    ../../gui-qt/io_report.h \
    ../../gui-qt/showtextfile.h

FORMS += ../../gui-qt/mainwindow.ui \
    ../../gui-qt/io_report.ui \
    ../../gui-qt/showtextfile.ui

RESOURCES = highlight-gui.qrc
TRANSLATIONS = ../../gui-qt/highlight_de_DE.ts ../../gui-qt/highlight_es_ES.ts ../../gui-qt/highlight_cs_CZ.ts

win32:RC_FILE = highlight-gui.rc

win32:LIBS += -L../.. -lhighlight
win32:LIBS += -Ld:/devel/cpp/lua_bin_5.1.4. -llua

unix:LIBS += -L.. -lhighlight
unix:LIBS += -L.. -llua5.1

win32:QMAKE_POST_LINK = $$quote(d:/devel/upx/upx.exe --best ../../../$${TARGET}.exe)

unix {
DEFINES += DATA_DIR=\\\"/usr/share/highlight\\\" \
    CONFIG_DIR=\\\"/etc/highlight\\\" \
    DOC_DIR=\\\"/usr/share/doc/highlight\\\"
    message("setting unix default paths")
}
