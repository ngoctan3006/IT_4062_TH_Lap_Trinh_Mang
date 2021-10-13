TARGET = resolver
CFLAGS = -c -Wall
CC = gcc
OBJS = main.o

all: resolver

resolver: ${OBJS}
	${CC} ${OBJS} -o resolver

main.o: main.c
	${CC} ${CFLAGS} main.c

clean:
	rm -f ${OBJS} ${TARGET}