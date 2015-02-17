# -------------------------------------------------
# Project created by QtCreator 2009-03-03T22:45:06
# -------------------------------------------------
TARGET = highlight-gui
TEMPLATE = app
INCLUDEPATH += . \
    ../include
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
unix:LIBS += -L.. -lhighlight

# If Lua 5.2 is not default on your system yet you have to omit 5.1 here:
unix {
    LIBS += -llua
    CONFIG += link_pkgconfig
    PKGCONFIG += lua
}

win32:QMAKE_POST_LINK = f:/upx/upx.exe --best ../../highlight-gui.exe

