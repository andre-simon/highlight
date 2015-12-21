# -------------------------------------------------
# Project created by QtCreator 2009-03-03T22:45:06
# -------------------------------------------------
TARGET = highlight-gui
TEMPLATE = app
INCLUDEPATH += . \
    ../include
        
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
    
win32:CONFIG += static

PRECOMPILED_HEADER = precomp.h
DEFINES += O2 QT
win32:DESTDIR = ../../
unix:DESTDIR = ../
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

win32:RC_FILE = highlight-gui.rc
win32:LIBS += -L../.. -lhighlight

# If Lua 5.2 is not default on your system yet you have to omit 5.1 here:
unix {
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

win32:QMAKE_POST_LINK = f:/upx/upx.exe --best ../../highlight-gui.exe

