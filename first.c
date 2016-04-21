#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
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


void waitInfo()
{
  int sock, namelen;
  struct sockaddr address, rcvr;
  char msg[4];

  sock = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (sock < 0) {
    perror("socket");
    exit(1);
  }
  address.sa_family = AF_UNIX;
  strcpy(address.sa_data, SOCK_NAME);
 
  if (bind(sock, &address, strlen(address.sa_data) +
        sizeof(address.sa_family)) < 0) {
    perror("bind");
    exit(1);
  }


  while(1) {	
    if(recvfrom(sock, msg, 4, 0, &rcvr, &namelen)){
    if (msg[0] == 'd'){
        puts(msg);
        close(sock);
        unlink(SOCK_NAME);
        return;
      }
    }
  }

}

int main() {
  int res, shmem_fd;
  void *ptr;
  shmem_fd = shm_open(SHMEM_PATH, O_RDWR | O_CREAT, 0666); // Создание объекта подразделяемой памяти POSIX для чтения и записи 
  if (shmem_fd < 0) {
    perror("shm_open");
    exit(1);
  }
  res = ftruncate(shmem_fd, SHMEM_SIZE); // Установление размера файла
  if (res) {
    perror("ftruncate");
    exit(1);
  }
  ptr = mmap(NULL, SHMEM_SIZE,
             PROT_READ | PROT_WRITE,
             MAP_SHARED, shmem_fd, 0); // Получили начальный адрес участка памяти
  if (!ptr) {
    perror("mmap");
    exit(1);
  }
  *(int*)ptr = 0xDEADBEEF;
  waitInfo();
  res = munmap(ptr, SHMEM_SIZE);
  if (res) {
    perror("munmap");
    exit(1);
  }
  res = close(shmem_fd);
  if (res) {
    perror("close");
    exit(1);
  }
  res = shm_unlink(SHMEM_PATH);
  if (res) {
    perror("shm_unlink");
    exit(1);
  }
  return 0;
}

