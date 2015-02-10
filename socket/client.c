#include <stdio.h>
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
	char buffer[BUFFSIZE];  
	unsigned int echolen;  
	int received = 0;  
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
	echolen = strlen(argv[2]);  
	if (send(sock, argv[2], echolen, 0) != echolen) {  
		Die("Mismatch in number of sent bytes\n");  
	}  
	fprintf(stdout, "Received: \n");  
	while (received < echolen) {  
		int bytes = 0;  
		if ((bytes = recv(sock, buffer, BUFFSIZE-1, 0)) < 1) {  
			Die("Failed to receive bytes from server\n");  
		}  
		received += bytes;  
		fprintf(stdout, buffer);  
	}  

	fprintf(stdout, "\n");  
	close(sock);  
	exit(0);  
}  
