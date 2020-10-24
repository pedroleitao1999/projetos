#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> 
#include <arpa/inet.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> 
#include <netdb.h>
#include "packet-format.h"
#include <sys/stat.h>


#define FILENAME argv[1]
#define HOSTNAME argv[2]
#define PORTARG  argv[3]
#define WINDOW_SIZE argv[4]

#define max(a,b) a > b ? a:b 
#define min(a,b) a < b ? a:b 

int getFileSize(const char *fn){
	struct stat st;
	stat(fn, &st);
	return st.st_size;

}
/*
void generatePackets(int totalNumPackets, data_pkt_t *myPackets, FILE *fp){
	for(int i = 0; i < totalNumPackets; i++){
		fgets(myPackets[i].data, 1000, fp);
		myPackets[i].seq_num = htonl(i + 1);	
	}

}*/

void generateChunks(int totalNumPackets, data_pkt_t *myPackets, FILE *fp){
	for(int i = 0; i < totalNumPackets; i++){
		memset(myPackets[i].data, 0, 1000);
		fread(&(myPackets[i].data), 999, 1, fp);
		myPackets[i].seq_num = htonl(i + 1);
		fprintf(stderr, "SENDER: chunk %d generated out of %d with size %li\n", i, totalNumPackets, strlen(myPackets[i].data));
	}
	
}

uint32_t sendChunks(data_pkt_t *myPackets, int begin, int end,int sock, int currentAcks, struct sockaddr *servaddr){
	int ret = currentAcks;
	for(int i = begin; i < end; i++){
		if(1 & currentAcks){
			fprintf(stderr, "Sender: not sending %d\n", i);
			continue;
		}
		currentAcks > 1;
		fprintf(stderr, "SENDER: sent %li bytes with seqnum %d\n", strlen(myPackets[i].data), ntohl(myPackets[i].seq_num));
		sendto(sock, (void *) &myPackets[i], sizeof(data_pkt_t), 0, servaddr, sizeof(struct sockaddr));	
	}
	return ret;
}

int main(int argc, char const *argv[]){

	FILE *fp;
	int sockfd, recvSockFd;
	struct hostent *host = gethostbyname(HOSTNAME);
	if(!host){
		perror("gethostbyname");
	}

	const char *fn = argv[1];
	long port = strtol(PORTARG, NULL, 10);
	int window_size = strtol(WINDOW_SIZE, NULL, 10);
	struct sockaddr_in servaddr;

	

    HOSTNAME = inet_ntoa(*((struct in_addr*) 
                           host->h_addr_list[0]));
	


	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr(HOSTNAME); 
    servaddr.sin_port = htons(port); 

	if(argc != 5){
		fprintf(stderr, "SENDER: %s\n", "INVALID ARGUMENTS");
		return -1;
	}

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
		perror("SENDER: SOCKET FAIL");
		return -1;
	}

	struct timeval timeout;
	timeout.tv_sec = TIMEOUT/1000;
	timeout.tv_usec = 0;
	
	setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(struct timeval));
	
	int sockLen = sizeof(servaddr);
	

	fp = fopen(fn, "r");


	int seqNum = 1;
	int fileSize = getFileSize(FILENAME);
	fprintf(stderr, "SENDER: Filesize is %d\n",fileSize );
	int numPackets = fileSize / (CHUNK_SIZE-1);
	
	int totalNumPackets = ++numPackets;
	int toAdd = 0;
	if(fileSize % (CHUNK_SIZE-1) == 0){
		toAdd = 1; 
	}
	
	data_pkt_t *myPackets = malloc(sizeof(struct data_pkt_t) * totalNumPackets);

	fprintf(stderr, "SENDER: Generating %d packets\n", totalNumPackets);
	//generatePackets(totalNumPackets, myPackets, fp);
	generateChunks(totalNumPackets, myPackets, fp);
	if(toAdd){
		data_pkt_t temp = { htonl(totalNumPackets), ""};
		myPackets = realloc(myPackets, (totalNumPackets)*sizeof(myPackets[0]));
		myPackets[totalNumPackets-1] = temp;
		fprintf(stderr, "SENDER: Added empty packet to tail\n");
	}




	ack_pkt_t currentWindowState = {1, 0b0};





	ack_pkt_t ack;

	unsigned int len = sizeof(servaddr);
	int r = 0;

	int failCounter = 0;
	uint32_t acks = 0;
	int begin = 0;
	while(1){
		fprintf(stderr, "SENDER: Begin is %d, totalNumPackets is %d\n", begin, totalNumPackets);
		if(begin == totalNumPackets){
			fprintf(stderr, "SENDER: exiting...\n");
			return 0;
		}

		if(failCounter > MAX_RETRIES){
			fprintf(stderr, "SENDER: Failed too many times, exiting\n");
			return -1;
		}

		int end = min(begin + window_size, totalNumPackets);
		
		uint32_t sent = sendChunks(myPackets, begin, end, sockfd, acks, (struct sockaddr *)&servaddr);
		
		for(int i = 0; i < window_size; i++){
			r = recvfrom(sockfd, &ack, sizeof(struct ack_pkt_t), 0, (struct sockaddr *)&servaddr, &len);
			if(1 & sent){
				continue;
			}
			sent > 1;
			
			if(r == -1 || r == 0){
				// Timeout, probably
				failCounter++;
				continue;
			}
			acks = ntohl(ack.selective_acks);
			begin = ntohl(ack.seq_num) - 1;
		}


	}

	return 0;

}