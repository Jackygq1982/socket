CC=gcc
MV=mv
RM=rm
all:server client
lib:
client:lib
	${CC} -o clientServer client.c
	${MV} clientServer ${HOME}/socket/select/bin
server:lib
	${CC} -o selectServer server.c
	${MV} selectServer ${HOME}/socket/select/bin
clean:
	${RM} ${HOME}/socket/select/bin/*
