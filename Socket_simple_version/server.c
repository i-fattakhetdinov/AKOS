#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    int sock, listener;
    struct sockaddr_in address;
    char msg[1024];

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
	perror("Socket Error");
        exit(1);
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(5000);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listener, (struct sockaddr *)&address, sizeof(address)) < 0) {
	perror("Bind Error");
        exit(1);
    }
    listen(listener, 1);

    while(1) {	
	sock = accept(listener, 0, 0);
	if (sock < 0) {
	    perror("Accept Error");
            exit(1);
        }
	while(1) {
	    if(recv(sock, msg, 1024, 0)){
	        puts(msg);
	    } else break;
	}
        close(sock);
    }

    return 0;
}
