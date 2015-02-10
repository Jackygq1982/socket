CC=gcc -g
MV=mv
RM=rm
all:clean server clinet
lib:
server:lib
	${CC} -o socketServer server.c
	${MV} socketServer ${HOME}/socket/socket/bin
clinet:lib
	${CC} -o socketClient client.c
	${MV} socketClient ${HOME}/socket/socket/bin
clean:
	${RM} ${HOME}/socket/socket/bin/*
