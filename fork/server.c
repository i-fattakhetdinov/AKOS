#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define NUMSERVERS 10
#define BUF_SIZE 1024
int main()
{
    int sock, listener;
    struct sockaddr_in address;
    pid_t id = getpid();

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
    int i;
    for (i = 0; i < NUMSERVERS && id; i++) {
        id = fork();
        if (id == -1) { // Ошибка
            perror("Fork Error");
        }
    }

    printf("Server with pid %d started\n", getpid());

    while(1) {	
	sock = accept(listener, NULL, NULL); // NULL, NULL - адрес клиента не интересует, он принимает все
	if (sock < 0) {
	    perror("Accept Error");
            exit(1);
        }
        FILE *out = fopen("log.txt", "a");
	char s[BUF_SIZE];
        while(1) {
            if(recv(sock, s, BUF_SIZE, 0)) {
                fprintf(out, "%d : %s",getpid(), &s);
            } else break;
        }
        fprintf(out,"\n");
        fclose(out);
        close(sock);
    } 


    return 0;
}
