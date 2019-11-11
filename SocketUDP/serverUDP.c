#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DEFAULT_IP "127.0.0.1" // oppure "0.0.0.0"
#define MAX_STR 4096

int main(int argc, char* argv[]) {
	void errore(char*, int);
	
	if(argc!=3) {
		printf("USAGE:%s PORT MESSAGE\n", argv[0]);
		return -1;
	}
	int port = atoi(argv[1]);
	char* msg = argv[2];
	
	printf("opening a socket...\n");
	int socketId = socket(AF_INET, SOCK_DGRAM, 0);
	if (socketId== -1) errore("socket()", -2);
	printf("ok\n");
	
	printf("Binding socket on port %d...\n", port);
	
	struct sockaddr_in addrServer;
	addrServer.sin_family = AF_INET;
	inet_aton(DEFAULT_IP, &addrServer.sin_addr);
	addrServer.sin_port = htons(port);
	memset(addrServer.sin_zero, 0, 8);
	// for(int i=0; i<8; i++) addrServer.sin_zero[i] = 0;	
	int ret = bind(socketId,
								 (struct sockaddr*) &addrServer,
								 (socklen_t)sizeof(struct sockaddr_in));
	if (ret) errore("bind()", -3);
	printf("ok\n");
	
	printf("receiving request from the client...\n");
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
	
	printf("sending a message to the client...\n");
	ret = sendto(socketId,
				 msg,
				 strlen(msg)+1,
				 0,
				 (struct sockaddr*)&sender,
				 (socklen_t)sizeof(struct sockaddr_in));
	if(ret<=0) errore("sendto()", -5);
	printf("ok\n");			 
					 		 		 
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
