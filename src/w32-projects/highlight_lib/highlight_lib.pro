QT -= core gui
TARGET = highlight
TEMPLATE = lib
CONFIG += staticlib
CONFIG += exceptions
DEFINES -= UNICODE
DEFINES += NDEBUG


    DEFINES += WIN32
    DESTDIR = ../..

    SOURCES += D:\Devel\cpp\highlight\src\core\*.cpp
    SOURCES += D:\Devel\cpp\highlight\src\core\astyle\*.cpp
    SOURCES += D:\Devel\cpp\highlight\src\core\Diluculum\*.cpp

    INCLUDEPATH += d:/devel/cpp/lua_bin_5.3.0
    INCLUDEPATH += ../../include
    INCLUDEPATH += d:/devel/cpp/boost_1_61_0
    QMAKE_CXXFLAGS += -std=c++11



