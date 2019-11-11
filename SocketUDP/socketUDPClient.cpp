#include <arpa/inet.h>
#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define MAX_BUFFER_SIZE 4096

void errore(char*, int);

int main(int argc, char *argv[]){

	if(argc!=4){
		printf("USAGE: %s IP PORT MESSAGE\n", argv[0]);
		return -1;
	}

	char* ip = argv[1];
	int port = atoi(argv[2]);
	char* message = argv[3];
	
	//aprto socket
	printf("opening a socket...\n");

	int socket_id = socket(AF_INET, SOCK_DGRAM, 0);   //restituisce un file descriptor dello stream (valore non negativo) (id del sochet) (se negativo = errore)
	
	if(socket_id == -1){errore("socket()", -2);}

	printf("ok\n");

	printf("Sending a message to the server...\n");

	struct sockaddr_in addr_server;
	addr_server.sin_family = AF_INET;
	inet_aton(ip, &addr_server.sin_addr);
	addr_server.sin_port = htons(port);
	for(int i=0 ; i<8 ; i++) addr_server.sin_zero[i]=0;


	sendto(socket_id,
		message,
		strlen(message),
		0,
		(struct sockaddr*) &addr_server,
		(socklen_t) sizeof(struct sockaddr_in));

	//da aggiungere codice sul quaderno

	printf("Reciving form server...\n");

	char bufferone [MAX_BUFFER_SIZE +1];
	struct sockaddr_in sender;
	int size_of_sender=sizeof(struct sockaddr_in);

	int ret= recvfrom(socket_id, bufferone, MAX_BUFFER_SIZE, 0,(struct sockaddr*) &sender,
			  (socklen_t *)&size_of_sender);

	if(ret<=0){
		errore("recvfrom()", -9);
	}
	bufferone[ret]='\0';

	printf("Ok\n");

	char * sender_ip = inet_ntoa(sender.sin_addr);
	int sender_port = ntohs(sender.sin_port);
	printf("[%s:%d]:%s\n", sender_ip, sender_port, bufferone);
	




	//chiudo socket
	printf("Closing a socket...\n");

	ret = shutdown(socket_id, SHUT_RDWR);
	
	if(ret == -1){errore("shutdown()", -10);}

	printf("ok\n");
	

	return 0;
}


void errore(char* s, int n){

	printf("ERROR on %s ( errno: %d - %s)\n", s , errno , strerror(errno));
	printf("exit with return code %d\n", n);

	exit (n);

}
