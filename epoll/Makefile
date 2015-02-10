CC=gcc
MV=mv
RM=rm
all:server
lib:
server:lib
	${CC} -o selectServer server.c
	${MV} selectServer ${HOME}/socket/epoll/bin
clean:
	${RM} ${HOME}/socket/epoll/bin/*
