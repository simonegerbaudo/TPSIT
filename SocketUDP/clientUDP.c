#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_STR 4096

int main(int argc, char* argv[]) {
	void errore(char*, int);
	
	if(argc!=4) {
		printf("USAGE:%s IP PORT MESSAGE\n", argv[0]);
		return -1;
	}
	char* ip = argv[1];
	int port = atoi(argv[2]);
	char* msg = argv[3];
	
	printf("opening a socket...\n");
	int socketId = socket(AF_INET, SOCK_DGRAM, 0);
	if (socketId == -1) errore("socket()", -2);
	printf("ok\n");
	
	struct sockaddr_in addrServer;
	addrServer.sin_family = AF_INET;
	inet_aton(ip, &addrServer.sin_addr);
	addrServer.sin_port = htons(port);
	for(int i=0; i<8; i++) addrServer.sin_zero[i] = 0;	
	
	printf("sending a message to the server...\n");
	int ret = sendto(socketId,
				 msg,
				 strlen(msg)+1,
				 0,
				 (struct sockaddr*)&addrServer,
				 (socklen_t)sizeof(struct sockaddr_in));
	if(ret<=0) errore("sendto()", -5);
	printf("ok\n");
				 
	printf("receiving response from the server...\n");
	char buffer[MAX_STR+1];
	struct sockaddr_in sender;
	int len=sizeof(struct sockaddr_in);

	ret = recvfrom (socketId, 
											buffer, 
											MAX_STR, 
											0, 
											(struct sockaddr*)&sender, 
											(socklen_t*)&len);  
	if(ret<=0) errore("recvfrom()", -9);
	buffer[ret] = '\0';
	printf("ok\n");

	char* ipSender = inet_ntoa(sender.sin_addr);
	int portSender = ntohs(sender.sin_port);
	printf ("[%s:%d]%s\n", ipSender, portSender, buffer);				 		 
				 				 
	printf("closing a socket...\n");
	ret = shutdown(socketId, SHUT_RDWR);
	if (ret == -1) errore("shutdown()", -10);
	printf("ok\n");
		
	return 0;
}

void errore(char* s, int n){
	printf("ERROR on %s (errno: %d - %s)\n", s, errno, strerror(errno));
	printf("exit with %d code\n", n);
	
	exit(n); 
}
