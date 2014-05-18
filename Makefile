PROGS =	battleship_server battleship_client struct
#CUSTOMLIBS = struct.o
CC = gcc
CFLAGS = -g

all:	${PROGS}

struct:	struct.o
		${CC} ${CFLAGS} -c struct.c
	
battleship_server:	battleship_server.o
		${CC} ${CFLAGS} -o $@ battleship_server.c ${CUSTOMLIBS}
	
battleship_client:	battleship_client.o
		${CC} ${CFLAGS} -o $@ battleship_client.o ${CUSTOMLIBS}

clean:
		rm -f ${PROGS} *.o