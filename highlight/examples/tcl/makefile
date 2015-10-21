# Simple Makefile for tclansifilter
# This file will compile the tclansifilter shared object.

CC=g++

# default lua lib
LUA_LIBS=$(shell pkg-config --libs lua)

CFLAGS= -c -Wall -O2 -fPIC -I ../../src/include

LDFLAGS= -shared -fPIC

SOURCES=stylecolour.cpp stringtools.cpp \
xhtmlgenerator.cpp latexgenerator.cpp texgenerator.cpp rtfgenerator.cpp \
htmlgenerator.cpp ansigenerator.cpp svggenerator.cpp codegenerator.cpp \
xterm256generator.cpp pangogenerator.cpp bbcodegenerator.cpp odtgenerator.cpp\
syntaxreader.cpp elementstyle.cpp themereader.cpp datadir.cpp preformatter.cpp platform_fs.cpp\
astyle/ASStreamIterator.cpp astyle/ASResource.cpp astyle/ASFormatter.cpp\
astyle/ASBeautifier.cpp astyle/ASEnhancer.cpp \
Diluculum/InternalUtils.cpp  Diluculum/LuaExceptions.cpp  Diluculum/LuaFunction.cpp\
Diluculum/LuaState.cpp Diluculum/LuaUserData.cpp  Diluculum/LuaUtils.cpp  Diluculum/LuaValue.cpp\
Diluculum/LuaVariable.cpp  Diluculum/LuaWrappers.cpp

OBJECTS=$(SOURCES:.cpp=.o) tclhighlight.o
BINARY=tclhighlight.so

all: $(OBJECTS) $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ ${LUA_LIBS}

%.o: ../../src/core/%.cpp
	$(CC) $(CFLAGS) $< -o $@

astyle/%.o: ../../src/core/astyle/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@

Diluculum/%.o: ../../src/core/Diluculum/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@

clean:
	@rm -f *.o
	@rm -f $(BINARY)
	@rm -rf astyle Diluculum
