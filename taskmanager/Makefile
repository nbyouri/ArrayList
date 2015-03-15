CC=	clang -std=c99 -pedantic -Wall -lkvm -g
PREFIX=	/usr/pkg
SRC=	list.c tools.c entry.c examples.c
TARGET= test_list
EXSRC=	examples.c
EXTARGET=	test
LDFLAGS= -L${PREFIX}/lib -lList

all:
	${CC} ${CFLAGS} ${SRC} -o ${TARGET}

install:
	test -d ${DESTDIR}${PREFIX}/lib || mkdir -p ${DESTDIR}${PREFIX}/lib
	test -d ${DESTDIR}${PREFIX}/include/list || mkdir -p ${DESTDIR}${PREFIX}/include/list
	install -pm 755  ${TARGET} ${DESTDIR}${PREFIX}/lib/${TARGET}
	install -pm 755  list.h ${DESTDIR}${PREFIX}/include/list/list.h
	install -pm 755  entry.h ${DESTDIR}${PREFIX}/include/list/entry.h
	install -pm 755  tools.h ${DESTDIR}${PREFIX}/include/list/tools.h
	install -pm 755  queue.h ${DESTDIR}${PREFIX}/include/list/queue.h

deinstall:
	rm ${DESTDIR}${PREFIX}/lib/${TARGET}

example: 
	${CC} ${EXSRC} ${LDFLAGS} -o ${EXTARGET}


clean:
	rm ${TARGET}
