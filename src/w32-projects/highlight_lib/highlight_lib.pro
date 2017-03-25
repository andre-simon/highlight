QT -= core gui
TARGET = highlight
TEMPLATE = lib
CONFIG += staticlib
CONFIG += exceptions
DEFINES -= UNICODE
DEFINES += NDEBUG

win32 {

    DEFINES += WIN32
    DESTDIR = ../..

    DIR_CORE = ../../core/
    DIR_ASTYLE = ../../core/astyle/
    DIR_DILU = ../../core/Diluculum/

    INCLUDEPATH += d:/devel/cpp/lua_bin_5.3.0
    INCLUDEPATH += ../../include
    INCLUDEPATH += d:/devel/cpp/boost_1_61_0
    QMAKE_CXXFLAGS += -std=c++11
}
else { # For Linux
    DIR_CORE = ../../core/
    DIR_ASTYLE = ../../core/astyle/
    DIR_DILU = ../../core/Diluculum/
    HEADERS += ../../include/*.h
}

SOURCES += $$DIR_CORE*.cpp  $$DIR_ASTYLE*.cpp  $$DIR_DILU*.cpp

