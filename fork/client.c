#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    int sock;
    struct sockaddr_in address;
    char message[1024];
    scanf("%s", message);
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
	perror("Socket Error");
	exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(5000);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Connect Error");
	exit(2);
    }
    
    send(sock, message, sizeof(message), 0);
    close(sock);
    
    return 0;
}
