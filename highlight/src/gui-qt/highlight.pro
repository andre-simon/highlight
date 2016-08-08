
TARGET = highlight-gui

TEMPLATE = app

INCLUDEPATH += .  ../include

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

PRECOMPILED_HEADER = precomp.h
DEFINES += O2 QT

SOURCES += main.cpp \
    mainwindow.cpp \
    io_report.cpp \
    showtextfile.cpp

HEADERS += mainwindow.h \
    precomp.h \
    io_report.h \
    showtextfile.h

FORMS += mainwindow.ui \
    io_report.ui \
    showtextfile.ui

RESOURCES = highlight-gui.qrc
TRANSLATIONS = highlight_de_DE.ts highlight_es_ES.ts highlight_cs_CZ.ts  highlight_zh_CN.ts

QMAKE_CXXFLAGS += -std=c++11

# If Lua 5.2 is not default on your system yet you have to omit 5.1 here:
unix {
	DESTDIR = ../
    LIBS += -L.. -lhighlight
    LIBS += -llua
    CONFIG += link_pkgconfig
    PKGCONFIG += lua

    # to make it run within Qt Creator
    !contains(DEFINES, DATA_DIR) {
        DEFINES+=DATA_DIR=\\\"/usr/share/highlight/\\\"
    }
    !contains(DEFINES, CONFIG_DIR) {
        DEFINES+=CONFIG_DIR=\\\"/etc/highlight/\\\"
    }
    !contains(DEFINES, DOC_DIR) {
        DEFINES+=DOC_DIR=\\\"/usr/share/doc/highlight/\\\"
    }
}

win32 {
    DESTDIR = ..\\..\\..

    INCLUDEPATH += d:/devel/cpp/lua_bin_5.3.0
    INCLUDEPATH += ..\\..\\include
    INCLUDEPATH += d:/devel/cpp/boost_1_61_0

    DEFINES += WIN32
    CONFIG += static
    
    LIBS += -LD:\Devel\cpp\syntaxhighlight-code\highlight\src -lhighlight
    LIBS += -LD:\Devel\cpp\lua_bin_5.3.0 -llua

    RC_FILE = highlight-gui.rc
    QMAKE_POST_LINK = $$quote(D:\Devel\upx391w\upx.exe --best D:\Devel\cpp\syntaxhighlight-code\highlight\highlight-gui.exe)
}
