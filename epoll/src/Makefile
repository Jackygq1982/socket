CC=gcc
MV=mv
RM=rm
all:server
lib:
server:lib
	${CC} -o epollServer server.c
	${MV} epollServer ${HOME}/socket/epoll/bin
clean:
	${RM} ${HOME}/socket/epoll/bin/*
