default: client server

server: server.c
	gcc server.c -lpthread -o server

client: client.c
	gcc client.c -lpthread -o client

clean:
	rm -rf *.o server client
