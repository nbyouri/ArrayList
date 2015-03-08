CC=	gcc
CFLAGS=	-Wall -DDEBUG -ferror-limit=1000
SRC=	examples.c list.c tools.c entry.c
TARGET=	test-list

all:
	${CC} ${CFLAGS} ${SRC} -o ${TARGET}

test: all
	
	./${TARGET}

debug:
	lldb ${TARGET}
