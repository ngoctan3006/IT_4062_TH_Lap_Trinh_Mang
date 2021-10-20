TARGET = client
CFLAGS = -c -Wall
CC = gcc
OBJS = main.o

all: client

client: ${OBJS}
	${CC} ${OBJS} -o client

main.o: main.c
	${CC} ${CFLAGS} main.c

clean:
	rm -f ${OBJS} ${TARGET}