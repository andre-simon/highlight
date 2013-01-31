
QT -= core gui
TARGET = highlight
TEMPLATE = lib
CONFIG += staticlib
CONFIG += exceptions
DEFINES -= UNICODE
DEFINES += NDEBUG

win32:DESTDIR = ..\\..

!contains( LINKTYPE, LUA52 ) {
win32:INCLUDEPATH += d:/devel/cpp/lua_bin_5.1.4
}
else {
win32:INCLUDEPATH += d:/devel/cpp/lua_bin_5.2.0
DEFINES += USE_LUA52
}

win32 { # For Windows
    # Leave an empty line because of the backslash at the end of the variable
    DIR_CORE = ..\\..\\core\\

    DIR_ASTYLE = ..\\..\\core\\astyle\\

    DIR_DILU = ..\\..\\core\\Diluculum\\

    INCLUDEPATH += ..\\..\\include
    INCLUDEPATH += d:/devel/cpp/boost_1_42_0
}
else { # For Linux
    DIR_CORE = ../../core/
    DIR_ASTYLE = ../../core/astyle/
    DIR_DILU = ../../core/Diluculum/
    HEADERS += ../../include/*.h
}

SOURCES += $$DIR_CORE*.cpp  $$DIR_ASTYLE*.cpp  $$DIR_DILU*.cpp
