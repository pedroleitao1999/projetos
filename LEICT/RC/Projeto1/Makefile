all: server client


test: 
	zip project1 chat-server.c chat-client.c Makefile
	./test-submission.sh project1.zip

server:
	gcc chat-server.c -o chat-server
client:
	gcc chat-client.c -o chat-client


clean:
	rm *.o
	rm chat-server
	rm chat-client
	rm project1.zip
