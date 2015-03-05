CC=	gcc
CFLAGS=	-Wall -DDEBUG
SRC=	examples.c list.c strings.c entry.c
TARGET=	test-list

all:
	${CC} ${CFLAGS} ${SRC} -o ${TARGET}

test: all
	
	./${TARGET}

debug:
	lldb ${TARGET}
