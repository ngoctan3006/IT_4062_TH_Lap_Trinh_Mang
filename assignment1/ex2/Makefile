TARGET = exercise2
CFLAGS = -c -Wall
CC = gcc
OBJS = main.o studentmanager.o

all: exercise2

exercise2: ${OBJS}
	${CC} ${OBJS} -o exercise2

main.o: main.c
	${CC} ${CFLAGS} main.c

studentmanager.o: studentmanager.c
	${CC} ${CFLAGS} studentmanager.c

clean:
	rm -f ${OBJS} ${TARGET}