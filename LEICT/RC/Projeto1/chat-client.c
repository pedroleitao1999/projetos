#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <netdb.h>
#define BUFF_SIZE 8097
#define address struct sockaddr_in
#include <sys/socket.h>

int socketFD;
void handleStdin(char *buff){
	fprintf(stdout, "%s", buff);
}
int isValidMessage(char *buff){
	for(int i = 0; i < strlen(buff); i++){
		if(buff[i] == '\n')
			return 1;
	}
	return 0;
}


int handleFd(int fdNum, fd_set *fdList){

	char buffer[BUFF_SIZE] = {0};

	if(FD_ISSET(fdNum, fdList)){
		int num = recv(fdNum, buffer, BUFF_SIZE-1,0);
		if(!num)
			return 1;
	
		if(fdNum != 1){
			handleStdin(buffer);
		}else{
			send(socketFD, buffer, BUFF_SIZE,0 );
		}
	

		FD_CLR(fdNum, fdList);
	}

	return 0;
}



int main(int argc, char const *argv[]){

	// Arg validation
	if(argc != 3){
		return -1;
	}

	
	// Socket is created
	socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if(socketFD < 0){
		fprintf(stderr, "DEBUG: SOCKET CREATE FAIL %d\n", socketFD);
		return -1;
	}

	long PORT = strtol(argv[2], NULL, 10);

	address serverAddress;
	struct hostent *h = gethostbyname(argv[1]);
	if(!h)
		return -1;

	argv[1] = inet_ntoa(*((struct in_addr*) 
                           h->h_addr_list[0]));
	
	if(argv[1])
		puts(argv[1]);
	else
		fprintf(stderr, "DEBUG: COULDN'T GET HOST NAME\n");


	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(argv[1]);
	serverAddress.sin_port = htons(PORT);


	int status = connect(socketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
	if(status < 0){
		fprintf(stderr, "DEBUG: CONNECT FAIL\n");
		return -1;
	}else{
		fprintf(stderr, "DEBUG: CONNECT SUCCESS\n");
	}

	fd_set fdList;

	for(;;){
		FD_ZERO(&fdList);
		FD_SET(1, &fdList);
		FD_SET(socketFD, &fdList);


		int rcvFD = select(socketFD+1, &fdList, NULL, NULL, NULL);

		if(rcvFD <= 0){
			fprintf(stderr, "DEBUG: RECV FAIL\n");
			return -1;
		}

		if(handleFd(1, &fdList))
			break;


		if(handleFd(socketFD, &fdList))
			break;


		FD_ZERO(&fdList);
	}

	close(socketFD);
	return 0;
}
