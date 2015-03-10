CC=	gcc
PREFIX=	/usr/pkg
CFLAGS=	-Wall -DDEBUG -dynamiclib
SRC=	list.c tools.c entry.c
TARGET= libList.dylib
EXSRC=	examples.c
EXTARGET=	test
LDFLAGS= -L${PREFIX}/lib -lList
SHARED_LIB=	-install_name ${PREFIX}/lib/${TARGET}

all:
	${CC} ${CFLAGS} ${SRC} ${SHARED_LIB} -o ${TARGET}

install:
	test -d ${DESTDIR}${PREFIX}/lib || mkdir -p ${DESTDIR}${PREFIX}/lib
	test -d ${DESTDIR}${PREFIX}/include/list || mkdir -p ${DESTDIR}${PREFIX}/include/list
	install -pm 755  ${TARGET} ${DESTDIR}${PREFIX}/lib/${TARGET}
	install -pm 755  ${TARGET} ${DESTDIR}${PREFIX}/include/list/list.h
	install -pm 755  ${TARGET} ${DESTDIR}${PREFIX}/include/list/entry.h
	install -pm 755  ${TARGET} ${DESTDIR}${PREFIX}/include/list/tools.h

deinstall:
	rm ${DESTDIR}${PREFIX}/lib/${TARGET}

example: 
	${CC} ${EXSRC} ${LDFLAGS} -o ${EXTARGET}


clean:
	rm ${TARGET}
