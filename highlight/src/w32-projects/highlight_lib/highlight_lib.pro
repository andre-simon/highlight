# -------------------------------------------------
# Project created by QtCreator 2010-02-03T21:26:53
# -------------------------------------------------
QT -= core \
    gui
TARGET = highlight
TEMPLATE = lib
CONFIG += staticlib
DEFINES -= UNICODE
DEFINES += NDEBUG

# win32:LIBS += -Lf:/cpp/lua_bin_5.1.4. -llua
win32:DESTDIR = ../../
win32:INCLUDEPATH += ../../include/
win32:INCLUDEPATH += f:/cpp/lua_bin_5.1.4
win32:INCLUDEPATH += f:/cpp/boost_1_42_0
SOURCES += ../../core/*.cpp \
    ../../core/astyle/*.cpp \
    ../../core/re/*.cpp \
    ../../core/Diluculum/*.cpp
HEADERS += ../../include/*.h
