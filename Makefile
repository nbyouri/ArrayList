CC=	gcc
PREFIX=	/usr/pkg
CFLAGS=	-Wall -DDEBUG -dynamiclib
SRC=	list.c tools.c entry.c
TARGET= libList.dylib
SHARED_LIB=	-install_name ${PREFIX}/lib/${TARGET}


all:
	${CC} ${CFLAGS} ${SRC} ${SHARED_LIB} -o ${TARGET}

install:
	test -d ${DESTDIR}${PREFIX}/lib || mkdir -p ${DESTDIR}${PREFIX}/lib
	install -pm 755  ${TARGET} ${DESTDIR}${PREFIX}/lib/${TARGET}

deinstall:
	rm ${DESTDIR}${PREFIX}/lib/${TARGET}

clean:
	rm ${TARGET}
