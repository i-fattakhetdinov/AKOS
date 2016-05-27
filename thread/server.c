#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct conn {
  int sock;
  int client_id;
};

void *func(struct conn *connection) {
  char filename[256];
  sprintf(filename, "client %d", connection->client_id);
  FILE *out = fopen(filename, "w");
  while (1) {
    char msg[1024];
    if(recv(connection->sock, msg, 1024, 0)){
      fputs(msg, out);
    } else break;
  }
  fclose(out);
  close(connection->sock);
  free(connection);
  return 0;
}

int main()
{
  int listener;
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

  if (bind(listener, (struct sockaddr *)&address, sizeof(address)) < 0) 
  {
    perror("Bind Error");
    exit(1);
  }
  if (listen(listener, 1) < 0) {
    perror("Listen Error");
    exit(1);
  }

  int client_id = 0;

  while(1) {	
    struct conn *connection = malloc(sizeof(struct conn));
    connection->sock = accept(listener, 0, 0);
    connection->client_id = client_id;
    if (connection->sock < 0) {
      perror("Accept Error");
      exit(1);
    }
    pthread_t thread;
    int status;
    int status_addr;
    status = pthread_create(&thread, 0, (void *)func, connection);
    if (status != 0) {
        perror("Pthread_create Error");
        exit(1);
    }
    status = pthread_join(thread, (void**)&status_addr);
    if (status != 0) {
        perror("Join Error");
        exit(1);
    }
    ++client_id;
  }

  return 0;
}
