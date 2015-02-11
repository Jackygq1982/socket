#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#define MAXPENDING 5  
#define BUFFSIZE 320
int main(int argc,char *argv[]) {
	int serversock, clientsock;  
	struct sockaddr_in echoserver, echoclient;  
	if (argc != 2) {  
		fprintf(stderr, "USAGE: socketServer <port>\n");  
		exit(1);  
	}
	if ((serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {  
		Die("Failed to create socket\n");  
	} 
	memset(&echoserver, 0, sizeof(echoserver));  
	echoserver.sin_family = AF_INET;  
	echoserver.sin_addr.s_addr = htonl(INADDR_ANY);  
	echoserver.sin_port = htons(atoi(argv[1]));  
	if (bind(serversock, (struct sockaddr *) &echoserver, sizeof(echoserver)) < 0) {  
		Die("Failed to bind the server socket\n");  
	}  
	if (listen(serversock, MAXPENDING) < 0) {  
		Die("Failed to listen on server socket\n");  
	}  

	FILE *f = fopen("./server.log","w");
	while (1) {  
		unsigned int clientlen = sizeof(echoclient);  
		if ((clientsock = accept(serversock, &echoclient, &clientlen)) < 0 ) {  
			Die("Failed to accept client connection\n");  
		}  
		printf("Client connected sock:%d\n", clientsock);
		//printf("Client connected:%s\n", inet_ntoa(echoclient.sin_addr));  
		int pid = fork();
		if(pid < 0) {
			fprintf(stderr,"create child process error!");
		} else if(pid == 0) {
			HandleClient(clientsock,f);  
		} else {
			continue;
		}
	} 
	fclose(f);
}

void HandleClient(int sock,FILE *f)  
{  
	char buffer[BUFFSIZE];  
	int received = -1;  
	if ((received = recv(sock, buffer, BUFFSIZE, 0)) < 0) {  
		Die("Failed to recevie inital bytes from client\n");  
	}  

	while (received > 0) {  
		/*
		buffer[received++] = ' ';
		buffer[received++] = 'r';
		buffer[received++] = 'e';
		buffer[received++] = 't';
		buffer[received++] = 'u';
		buffer[received++] = 'r';
		buffer[received++] = 'n';
		buffer[received++] = 'f';
		buffer[received++] = 'r';
		buffer[received++] = 'o';
		buffer[received++] = 'm';
		buffer[received++] = 's';
		buffer[received++] = 'e';
		buffer[received++] = 'r';
		buffer[received++] = 'v';
		buffer[received++] = 'e';
		buffer[received++] = 'r';
		if (send(sock, buffer, received+17, 0) != received+17) {  
			Die("Failed to send bytes to client\n");  
		}  
		if ((received = recv(sock, buffer, BUFFSIZE, 0)) < 0) {  
			//Die("Failed to receive additional bytes from client\n");  
		}  
		*/
		fwrite(buffer,received,1,f);
		fwrite("\n",sizeof(char),1,f);
		fflush(f);
		if (send(sock, buffer, received, 0) != received) {  
			Die("Failed to send bytes to client\n");  
		}  
		if ((received = recv(sock, buffer, BUFFSIZE, 0)) < 0) {  
			Die("Failed to receive additional bytes from client\n");  
		}  
	}  
	close(sock);  
}

void Die(char *mess)  
{  
	fprintf(stderr,mess);  
	exit(1);  
}
