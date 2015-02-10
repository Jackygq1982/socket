#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#define BUFFSIZE 32  
#define FP fprintf  
void Die(char *mess)  
{  
	fprintf(stderr,mess);  
	exit(1);  
}  
int main(int argc, char *argv[])  
{  
	int sock;  
	struct sockaddr_in echoserver;  
	if (argc != 4) {  
		fprintf(stderr, "USAGE: TCP echo <server_ip> <word> <port>\n");  
		exit(1);  
	}  
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {  
		Die("Failed to create socket\n");  
	}  
	memset(&echoserver, 0, sizeof(echoserver));  
	echoserver.sin_family = AF_INET;  
	echoserver.sin_addr.s_addr = inet_addr(argv[1]);  
	echoserver.sin_port = htons(atoi(argv[3]));  
	if (connect(sock, (struct sockaddr *) &echoserver, sizeof(echoserver)) < 0) {  
		Die("Failed to connect with server\n");  
	}  
	int i = 0;
	for(i = 0 ; i < 10000 ; i ++) {
		char msg[100];
		memset(msg,0x00,sizeof(msg));
		sprintf(msg,"%s%d",argv[2],i);
		send_msg(sock,msg);
		sleep(10);
	}
	close(sock);  

	exit(0);  
}  

send_msg(int fd,char * msg) {
	int received = 0;  
	char buffer[BUFFSIZE];  
	unsigned int echolen = strlen(msg);  
	if (send(fd, msg, echolen, 0) != echolen) {  
		Die("Mismatch in number of sent bytes\n");  
	}  
	fprintf(stdout, "Received: \n");  
	while (received < echolen) {  
		int bytes = 0;  
		if ((bytes = recv(fd, buffer, BUFFSIZE-1, 0)) < 1) {  
			Die("Failed to receive bytes from server\n");  
		}  
		received += bytes;  
		fprintf(stdout, buffer);  
	}  

	fprintf(stdout, "\n");  
}
