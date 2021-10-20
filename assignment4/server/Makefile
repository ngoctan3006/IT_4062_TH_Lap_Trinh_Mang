TARGET = server
CFLAGS = -c -Wall
CC = gcc
OBJS = main.o

all: server

server: ${OBJS}
	${CC} ${OBJS} -o server

main.o: main.c
	${CC} ${CFLAGS} main.c

clean:
	rm -f ${OBJS} ${TARGET}