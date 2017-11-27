QT -= core gui
TARGET = highlight
TEMPLATE = lib
CONFIG += staticlib
CONFIG += exceptions
DEFINES -= UNICODE
DEFINES += NDEBUG

DEFINES += WIN32
DESTDIR = ../..

SOURCES += E:\Devel\git\highlight\src\core\*.cpp
SOURCES += E:\Devel\git\highlight\src\core\astyle\*.cpp
SOURCES += E:\Devel\git\highlight\src\core\Diluculum\*.cpp

INCLUDEPATH += E:/devel/cpp/lua_bin_5.3.0
INCLUDEPATH += ../../include
INCLUDEPATH += E:/devel/cpp/boost_1_64_0
QMAKE_CXXFLAGS += -std=c++11
