include Make.defines

PROGS =	requester battleship_server battleship_client
CUSTOMLIBS = requester.o

all:	${PROGS}
	
requester:	requester.o
		${CC} ${CFLAGS} -c requester.c ${LIBS}
	
battleship_server:	battleship_server.o
		${CC} ${CFLAGS} -o $@ battleship_server.o ${LIBS}
	
battleship_client:	battleship_client.o
		${CC} ${CFLAGS} -o $@ battleship_client.o ${CUSTOMLIBS} ${LIBS}

clean:
		rm -f ${PROGS} ${CLEANFILES}