CC=gcc
MV=mv
RM=rm
all:server
lib:
server:lib
	${CC} -o selectServer server.c
	${MV} selectServer ${HOME}/socket/select/bin
clean:
	${RM} ${HOME}/socket/select/bin/*
