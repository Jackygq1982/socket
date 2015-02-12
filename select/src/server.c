#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#define LISTENQ 5
#define MAXLINE 1024
#define IPADDRESS "127.0.0.1"
int main(int argc,char *argv[]) {
	if (argc <= 1) {
                fprintf(stderr, "USAGE: selectServer <port>\n");
                exit(1);
        }
	int listenfd,connfd,sockfd;
	struct sockaddr_in chiaddr;
	listenfd = socket_bind(IPADDRESS,atoi(argv[1]));
	listen(listenfd,LISTENQ);
	fprintf(stdout,"server started for address [%s] and port [%s] \n",IPADDRESS,argv[1]);
	do_select(listenfd);
	return 0;
}
int socket_bind(char *ip,int port) {
	int listenfd = 0;
	struct sockaddr_in servaddr;
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	if(listenfd == -1) {
		perror("socket error!\n");
		exit(1);
	}
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	inet_pton(AF_INET,ip,&servaddr.sin_addr);
	servaddr.sin_port = htons(port);
	if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1) {
		perror("bind error \n");
		exit(1);
	}
	return listenfd;
}
void do_select(int listenfd) {
	int connfd,sockfd;
	struct sockaddr_in chiaddr;
	socklen_t chiaddrlen;
	fd_set rset,allset;
	int maxfd,maxi;
	int i;
	int clientfds[FD_SETSIZE];
	int nready;
	char ipaddress[15];
	memcpy(ipaddress,inet_ntoa(chiaddr.sin_addr),strlen(inet_ntoa(chiaddr.sin_addr)));
	for(i = 0 ; i < FD_SETSIZE ; i ++) 
		clientfds[i] = -1;
	maxi = -1;
	FD_ZERO(&allset);
	FD_SET(listenfd,&allset);
	maxfd = listenfd;

	for( ; ; ) {

		rset = allset;
		nready = select(maxfd+1,&rset,NULL,NULL,NULL);
		if(nready == -1) {
			perror("select error!\n");
			exit(1);
		}
		if(FD_ISSET(listenfd,&rset)) {
			chiaddrlen = sizeof(chiaddr);
			if((connfd = accept(listenfd,(struct sockaddr*)&chiaddr , &chiaddrlen)) == -1) {
				if(errno == EINTR)
					continue;
				else {
					perror("accept error\n");
					exit(1);
				}
			}
			fprintf(stdout,"accept a new client : %s:%d\n",ipaddress,chiaddr.sin_port);
			for(i = 0 ; i <FD_SETSIZE ; i++) {
				if(clientfds[i] < 0) {
					clientfds[i] = connfd;
					break;
				}

			}

			if( i == FD_SETSIZE) {
				fprintf(stderr,"too many clients.\n");
				exit(1);
			}
			FD_SET(connfd,&allset);
			maxfd = (connfd > maxfd ? connfd : maxfd);
			maxi = (i > maxi ? i : maxi);
			if(--nready <= 0)
				continue;
		}

		handle_connection(clientfds,maxi,&rset,&allset);

	}

}

void handle_connection(int *connfds,int num,fd_set *prest,fd_set *pallset) {
	int i,n;
	char buf[MAXLINE];
	memset(buf,0,MAXLINE);
	for(i = 0 ; i <= num ; i ++) {

		if(connfds[i] < 0) 
			continue;
		if(FD_ISSET(connfds[i],prest)) {
			n = read(connfds[i],buf,MAXLINE);
			if(n == 0) {
				close(connfds[i]);
				FD_CLR(connfds[i],pallset);
				connfds[i] = -1;
				continue;
			}
			write(STDOUT_FILENO,buf,n);
			fprintf(stdout,"\n");
			write(connfds[i],buf,n);

		}


	}

}
