# -------------------------------------------------
# Project created by QtCreator 2010-02-03T21:26:53
# -------------------------------------------------
QT -= core \
    gui
TARGET = highlight
TEMPLATE = lib
CONFIG += staticlib
CONFIG += exceptions
DEFINES -= UNICODE
DEFINES += NDEBUG

win32:DESTDIR = ../../
win32:INCLUDEPATH += ../../include/
!contains( LINKTYPE, LUA52 ) {
win32:INCLUDEPATH += d:/devel/cpp/lua_bin_5.1.4
}
contains( LINKTYPE, LUA52 ) {
win32:INCLUDEPATH += d:/devel/cpp/lua_bin_5.2.0
DEFINES += USE_LUA52
}
win32:INCLUDEPATH += d:/devel/cpp/boost_1_42_0
SOURCES += ../../core/*.cpp \
    ../../core/astyle/*.cpp \
    ../../core/re/*.cpp \
    ../../core/Diluculum/*.cpp
HEADERS += ../../include/*.h
