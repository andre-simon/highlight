
CXX=g++
CFLAGS=-g -O2 -fPIC

HL_INC=../../src/include/
HL_SRC=../../src/

LUA_CFLAGS=$(shell pkg-config --cflags lua)
LUA_LIBS=$(shell pkg-config --libs lua)
PYTHON_CFLAGS=$(shell pkg-config --cflags python-2.7)
PERL_INC=$(shell perl -MExtUtils::Embed -eccopts)
PHP_INC=$(shell php-config --includes)

lib-stamp:
	make -C ${HL_SRC} -f ./makefile clean
	PIC=1 make -C ${HL_SRC} -f ./makefile CFLAGS="${CFLAGS}" libhighlight.a
	touch $@

python: lib-stamp
	swig -c++ -python -o highlight_wrap.cpp  highlight.i
	${CXX} ${CFLAGS} -c highlight_wrap.cpp ${LUA_CFLAGS} ${PYTHON_CFLAGS} -I${HL_INC} 
	${CXX} -shared -s highlight_wrap.o  -L${HL_SRC} -lhighlight ${LUA_LIBS} -lpython2.7 -o _highlight.so

perl: lib-stamp
	swig -c++ -perl -o highlight_wrap.cpp  highlight.i
	${CXX} ${CFLAGS} -c highlight_wrap.cpp ${LUA_CFLAGS} ${PERL_INC} -I${HL_INC} 
	${CXX} -shared -s highlight_wrap.o  -L${HL_SRC} -lhighlight ${LUA_LIBS} -o highlight.so

php: lib-stamp
	swig -c++ -php -o highlight_wrap.cpp highlight.i
	${CXX} ${CFLAGS} -c highlight_wrap.cpp ${LUA_CFLAGS} ${PHP_INC} -I${HL_INC}
	${CXX} -shared -s highlight_wrap.o -L${HL_SRC} -lhighlight ${LUA_LIBS} -o highlight.so

clean: python-clean perl-clean php-clean
	rm -f *-stamp
	rm -f highlight.py

python-clean:
	rm -f _highlight.so *.pyc
	rm -rf __pycache__

perl-clean:
	rm -f highlight.so highlight_wrap.cpp highlight.pm *.o

php-clean:
	rm -f highlight.so highlight_wrap.cpp highlight.php *.o  php_highlight.h

.PHONY: python python-clean perl perl-clean php php-clean clean
