TARGET_C = client
TARGET_S = server
CFLAGS = -c -Wall
CC = gcc
OBJS_C = tcp_client.o
OBJS_S = select_server.o

all: ${TARGET_C} ${TARGET_S}

${TARGET_C}: ${OBJS_C}
	${CC} ${OBJS_C} -o ${TARGET_C}

${TARGET_S}: ${OBJS_S}
	${CC} ${OBJS_S} -o ${TARGET_S} -lpthread

${OBJS_C}: tcp_client.c
	${CC} ${CFLAGS} tcp_client.c

${OBJS_S}: select_server.c
	${CC} ${CFLAGS} select_server.c

clean:
	rm -f ${OBJS_C} ${OBJS_S} ${TARGET_C} ${TARGET_S}