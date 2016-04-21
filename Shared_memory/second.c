#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

#define SOCK_NAME "socket.soc"

#define SHMEM_PATH "/my_shmem"
// 64 KiB
#define SHMEM_SIZE (1 << 16)

void sendMsg()
{
  int sock;
  struct sockaddr address;

  sock = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (sock < 0) {
    perror("socket");
    exit(1);
  }

  address.sa_family = AF_UNIX;
  strcpy(address.sa_data, SOCK_NAME);
  sendto(sock, "done", 4, 0, &address,
    strlen(address.sa_data) + sizeof(address.sa_family));
  puts("send");
  return;
}


int main() {
  int res, shmem_fd;
  void *ptr;
  shmem_fd = shm_open(SHMEM_PATH, O_RDONLY, 0666); // Подключение объекта подразделяемой памяти POSIX для чтения 
  if (shmem_fd < 0) {
    perror("shm_open");
    exit(1);
  }
  
  ptr = mmap(NULL, SHMEM_SIZE,
             PROT_READ,
             MAP_SHARED, shmem_fd, 0); // Получили начальный адрес участка памяти
  if (!ptr) {
    perror("mmap");
    exit(1);
  }
 
  printf("%d\n", *(int*)ptr);
  sendMsg();

  
  return 0;
}
