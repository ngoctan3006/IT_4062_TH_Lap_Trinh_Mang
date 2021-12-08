TARGET_C = client
TARGET_S = server
CFLAGS = -c -Wall
CC = gcc
OBJS_C = client-socket.o
OBJS_S = server-socket.o

all: ${TARGET_C} ${TARGET_S}

${TARGET_C}: ${OBJS_C}
	${CC} ${OBJS_C} -o ${TARGET_C}

${TARGET_S}: ${OBJS_S}
	${CC} ${OBJS_S} -o ${TARGET_S} -lpthread

${OBJS_C}: client-socket.c
	${CC} ${CFLAGS} client-socket.c

${OBJS_S}: server-socket.c
	${CC} ${CFLAGS} server-socket.c

clean:
	rm -f ${OBJS_C} ${OBJS_S} ${TARGET_C} ${TARGET_S}