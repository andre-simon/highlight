
CXX=g++
CFLAGS=-g -O2 -fPIC

HL_INC=../../src/include/
HL_SRC=../../src/

LUA_CFLAGS=$(shell pkg-config --cflags lua)
LUA_LIBS=$(shell pkg-config --libs lua)
PYTHON_CFLAGS=$(shell pkg-config --cflags python-2.7)
PERL_INC=$(shell perl -MExtUtils::Embed -eccopts)

lib-stamp:
	make -C ${HL_SRC} -f ./makefile clean
	PIC=1 make -C ${HL_SRC} -f ./makefile CFLAGS="${CFLAGS}" libhighlight.a
	touch $@

python: lib-stamp
	swig -c++ -python -o highlight_wrap.cpp  highlight.i
	${CXX} ${CFLAGS} -c highlight_wrap.cpp ${LUA_CFLAGS} ${PYTHON_CFLAGS} -I${HL_INC} 
	${CXX} -shared -s highlight_wrap.o  -L${HL_SRC} -lhighlight ${LUA_LIBS} -o _highlight.so

perl: lib-stamp
	swig -c++ -perl -o highlight_wrap.cpp  highlight.i
	${CXX} ${CFLAGS} -c highlight_wrap.cpp ${LUA_CFLAGS} ${PERL_INC} -I${HL_INC} 
	${CXX} -shared -s highlight_wrap.o  -L${HL_SRC} -lhighlight ${LUA_LIBS} -o highlight.so


clean: python-clean perl-clean
	rm -f *-stamp
	rm -f highlight.py

perl-clean:
	rm -f highlight.so highlight_wrap.cpp highlight.pm *.o


.PHONY: python python-clean perl perl-clean clean
