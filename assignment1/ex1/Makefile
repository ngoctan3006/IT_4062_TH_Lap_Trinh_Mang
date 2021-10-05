TARGET = exercise1
CFLAGS = -c -Wall
CC = gcc
OBJS = main.o hazchem.o

all: exercise1

exercise1: ${OBJS}
	${CC} ${OBJS} -o exercise1

main.o: main.c
	${CC} ${CFLAGS} main.c

hazchem.o: hazchem.c
	${CC} ${CFLAGS} hazchem.c

clean:
	rm -f ${OBJS} ${TARGET}