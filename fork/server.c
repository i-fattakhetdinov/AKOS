#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    int sock, listener, clientId = 0;
    struct sockaddr_in address;
    char msg[1024];
    FILE *out;
    
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
        exit(2);
    }
    listen(listener, 1);

    while(1) {	
	sock = accept(listener, 0, 0);
	if (sock < 0) {
	    perror("Accept Error");
            exit(3);
        }

	switch(fork()) {
	case -1: //Ошибка
	    perror("Fork Error");
	    break;

        case 0: //Процесс потомок
	    ;
            char filename[128];
            sprintf(filename, "client %d", clientId);
            out = fopen(filename, "w");
            close(listener);
            while(1) {
                if(recv(sock, msg, 1024, 0)) {
		    fprintf(out, "%s", &msg);
                } else break;
            }
            close(out);
            close(sock);
            exit(0);

	default:
            clientId++;
	    close(sock);
	} 

    }

    close(listener);

    return 0;
}
