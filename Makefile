CC=	gcc
CFLAGS=	-Wall -DDEBUG -ferror-limit=1000 -DQUEUE_MACRO_DEBUG
SRC=	examples.c list.c tools.c entry.c
TARGET=	test-list

all:
	${CC} ${CFLAGS} ${SRC} -o ${TARGET}

test: all
	
	./${TARGET}

debug:
	lldb ${TARGET}
