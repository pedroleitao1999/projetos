//Example code: A simple server side code, which echos back the received message.
//Handle multiple socket connections with select and fd_set on Linux
#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros

#define BUFF_SIZE 8097
#define address struct sockaddr_in
#include <sys/socket.h>


void sendMessageToAllButI(int avoid, int *clientList, int listSize, char *message){
	for (int i = 0; i < listSize; ++i){
		if(i == avoid)
			continue;

		send(clientList[i], message, strlen(message), 0);

	}
}

int main(int argc , char *argv[]){
	if(argc != 2){
		fprintf(stderr, "DEBUG: ARGS NOT CORRECT\n");
		return -1;
	}

	int socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if(socketFD < 0){
		fprintf(stderr, "DEBUG: SOCKET CREATE FAIL %d\n", socketFD);
		return -1;
	}

	long PORT = strtol(argv[1], NULL, 10);
	address serverAddress;

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(PORT);


	int status = bind(socketFD, (struct sockaddr *)&serverAddress,  sizeof(serverAddress));
	if(status < 0){
		fprintf(stderr, "DEBUG: BIND FAIL\n");
		return -1;
	}

	status = listen(socketFD, 1000);

	if(status < 0){
		fprintf(stderr, "DEBUG: LISTEN FAIL\n");
		return -1;
	}

	fd_set fdList;
	int clientList[1000] = {0};
	char clientData[1000][100];
	int clientNum = 0;
	int fdlSize = socketFD+1;
	FD_ZERO(&fdList);
	for(;;){	
		FD_SET(socketFD, &fdList);
		for (int i = 0; i < clientNum; ++i){
			FD_SET(clientList[i], &fdList);

			if(clientList[i] + 1> fdlSize){
				fdlSize = clientList[i] + 1;
			}
		}


		if (select(fdlSize, &fdList, NULL, NULL, NULL) < 0){
			return -1;
		}


		if(FD_ISSET(socketFD, &fdList)){

			char newIp[100]; 
			struct sockaddr *clientAddr = malloc(sizeof(struct sockaddr));;
			int a =  sizeof( struct sockaddr );
			int newClient = accept(socketFD, clientAddr, &a);
			
			struct sockaddr_in *addr_in = (struct sockaddr_in*) clientAddr;
			
			char *s = inet_ntoa(addr_in->sin_addr);
			int port = htons(addr_in->sin_port);


			char portString[12];
			sprintf(portString, "%d", port);
			
			strcpy(clientData[clientNum], s);
			strcpy(clientData[clientNum] + strlen(clientData[clientNum]), ":");
			strcpy(clientData[clientNum] + strlen(clientData[clientNum]), portString);
			strcpy(clientData[clientNum] + strlen(clientData[clientNum]), " ");

			clientList[clientNum] = newClient;

			for(int j = 0; j < clientNum; j++){
				if(clientList[j]){
					char message[BUFF_SIZE];
					sprintf(message, "%shas connected\n",clientData[clientNum] );
					int ret = send(clientList[j], message, strlen(message), 0);

				}
			}
			clientNum++;
			FD_CLR(socketFD, &fdList);
		}

		int last;
		for (int i = 0; i < clientNum; ++i){
			last = 0;
			if(clientList[i] && FD_ISSET(clientList[i], &fdList)){
				char message[BUFF_SIZE];
				int r = recv(clientList[i], message, BUFF_SIZE, 0);
				char toSend[BUFF_SIZE];
				if(r == 0){
					clientList[i] = 0;
					sprintf(toSend, "%sleft\n",clientData[i] );
				}else{
					strcpy(toSend, clientData[i]);
					last = strlen(clientData[i]);

					strcpy(toSend + last, message);					
				}

				sendMessageToAllButI(i, clientList, clientNum, toSend);

				FD_CLR(clientList[i], &fdList);
				
			}

		}
		FD_ZERO(&fdList);
	}
	close(socketFD);
	return 0;
}