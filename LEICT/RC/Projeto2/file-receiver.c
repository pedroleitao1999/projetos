#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include "packet-format.h"


#define max(a,b) a > b ? a:b 
#define min(a,b) a < b ? a:b 

FILE *fp;
char *dataBuffer;
int buffSize = 1;
void handleData(char *data, uint32_t seq_num){
	seq_num = ntohl(seq_num);
	fprintf(stderr, "Sn = %d ds = %li\n", seq_num, strlen(data));
	if(seq_num > buffSize){

		dataBuffer = realloc(dataBuffer, seq_num*CHUNK_SIZE);
		if(!dataBuffer){
			exit(-1);
		}

	}
	buffSize = seq_num;
	strcpy(dataBuffer + (seq_num-1)*(CHUNK_SIZE), data);
	//fprintf(fp, "%s", data);
}

void doAck(int, data_pkt_t*, struct sockaddr *, size_t, ack_pkt_t*);

int main(int argc, char const *argv[]){
	fp = fopen(argv[1], "w");
	int sockfd, window_size;
	long port;
	struct sockaddr_in myAddr, senderAddr;
	dataBuffer = malloc(CHUNK_SIZE);
	memset(dataBuffer, 0, CHUNK_SIZE);
	port = strtol(argv[2], NULL, 10);
	window_size = strtol(argv[3], NULL, 10);

	senderAddr.sin_family = AF_INET;
	myAddr.sin_family = AF_INET; 
	myAddr.sin_addr.s_addr = INADDR_ANY; 
    myAddr.sin_port = htons(port); 

	if(argc != 4){
		return -1;
	}


	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("SOCKET FAIL");
		return -1;
	}

	if(bind(sockfd, (struct sockaddr*)&myAddr, sizeof(myAddr)) < 0){	
		perror("BIND FAIL");
		return -1;
	}
	


	data_pkt_t *buffer = malloc(sizeof(data_pkt_t));
	char test[1000];
	strcpy(test, "works");

	socklen_t sockLen = sizeof(myAddr);

	ack_pkt_t windowState = {1, 0b0};

	recvfrom(sockfd, buffer, sizeof(data_pkt_t), 0, (struct sockaddr*) &senderAddr, &sockLen);
	data_pkt_t *casted = (data_pkt_t *)buffer;
	fprintf(stderr,"Received %li bytes\n", strlen(casted->data));
	handleData(casted->data, casted->seq_num);
	fprintf(stderr, "Exited handleData\n");
	doAck(sockfd, buffer, (struct sockaddr*) &senderAddr, sizeof(senderAddr), &windowState);
			
	

		
	if(strlen(casted->data) != 999){
		fprintf(stderr, "Receiver: exiting...\n");
		for(int i = 0; i < buffSize; i++){
			fprintf(fp, "%s", dataBuffer + i* CHUNK_SIZE);
		}
		return 0;
	}
	long cont = 1;
	free(buffer);
	while(cont){
		//recvfrom(sockfd, buffer, sizeof(data_pkt_t), 0, (struct sockaddr *)&senderAddr, (socklen_t *)&len);
		struct sockaddr_in newAddr;
		newAddr.sin_family = AF_INET;
		sockLen = sizeof(myAddr);

		data_pkt_t *buff = malloc(sizeof(struct data_pkt_t));
		data_pkt_t received[window_size];
		memset(received, 0, sizeof(received));

		for(int i = 0; i < window_size; i++){			
			int r = recvfrom(sockfd, buff, sizeof(data_pkt_t), MSG_WAITALL,(struct sockaddr*) &newAddr, &sockLen);
			if(!r)
				continue;

			if(r == -1){
				perror("recv");
				exit(-1);
			}

			if(newAddr.sin_addr.s_addr != senderAddr.sin_addr.s_addr){
				fprintf(stderr, "%s\n", "diffsender");
				continue;
			}

			casted = (data_pkt_t *)buff;
			received[i] = *casted;
			handleData(casted->data, casted->seq_num);
			doAck(sockfd, buff, (struct sockaddr*) &newAddr, sizeof(senderAddr), &windowState);
			cont = (strlen(casted->data) == 999);
			if(strlen(casted->data) != 999){
				for(int i = 0; i < buffSize; i++){
					fprintf(fp, "%s", dataBuffer + i* (CHUNK_SIZE));
				}
				free(dataBuffer);
				fclose(fp);
				return 0;
			}
		}

		free(buff);
	}

	fprintf(stderr, "Receiver: exiting... buffSize = %d\n", buffSize);
	for(int i = 0; i < buffSize; i++){
		fprintf(fp, "%s", dataBuffer + i* (CHUNK_SIZE));
	}
	free(dataBuffer);
	fclose(fp);
	return 0;

}


void doAck(int sockfd, data_pkt_t *dataPacket, struct sockaddr *addr, size_t len, ack_pkt_t *windowState){
	// unpack
	uint32_t receivedSeqnum = ntohl(dataPacket->seq_num);
	char *data = dataPacket->data;

	ack_pkt_t localWS = (*windowState);

	fprintf(stderr, "RECEIVER: receivedSeqnum %d, local %d\n",receivedSeqnum, localWS.seq_num);
	// WS Logic
	if(receivedSeqnum == localWS.seq_num){
		fprintf(stderr, "Updating localWs\n");
		localWS.selective_acks >> 1;
		localWS.seq_num++;
	}else{
		uint32_t temp = 1;
		temp << (receivedSeqnum-localWS.seq_num-1);
		fprintf(stderr, "temp=%d\n", temp);
		localWS.selective_acks = localWS.selective_acks | temp;  
	}

	//update main var
	(*windowState) = localWS;
	// send data
	ack_pkt_t toSend = {htonl(localWS.seq_num), htonl(localWS.selective_acks)} ;
	sendto(sockfd, (void*)&toSend, sizeof(ack_pkt_t), 0, addr, len);

}