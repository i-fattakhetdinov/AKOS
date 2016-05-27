#include <stdio.h>    //printf(3)
#include <stdlib.h>   //exit(3)
#include <unistd.h>   //fork(3), chdir(3), sysconf(3)
#include <signal.h>   //signal(3)
#include <sys/stat.h> //umask(3)
#include <syslog.h>   //syslog(3), openlog(3), closelog(3)
#include <errno.h>

int daemonize()
{
    pid_t pid;
    
    // создаем потомка
    pid = fork();

    if (pid < 0) // если не удалось запустить потомка
    {
        // выведем на экран ошибку и её описание
        printf("Error: Start Daemon failed (%s)\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (pid > 0) { //родитель
        exit(EXIT_SUCCESS);
    }
    if( setsid() < 0 ) { //failed to become session leader
        perror("Failed Setsid\n");
        exit(EXIT_FAILURE);
    }
    
    close(0);
    close(1);
    close(2);
        
    signal(SIGCHLD,SIG_IGN);
    signal(SIGHUP,SIG_IGN);
    signal (SIGTERM, SIG_IGN);

        
    if ( (pid=fork()) < 0) { //ошибка
        fprintf(stderr,"error: failed fork\n");
        exit(EXIT_FAILURE);
    }
    if( pid > 0 ) { // родитель
        exit(EXIT_SUCCESS);
    }
        
    umask(0);
    // переходим в корень диска
    chdir("/");
       
    stdin=fopen("/dev/null","r");   //fd=0
    stdout=fopen("/dev/null","w+");  //fd=1
    stderr=fopen("/dev/null","w+");  //fd=2
      
    return 0;
}

int main()
{
    int res;
    int ttl=120;
    int delay=5;
    if ( (res = daemonize()) != 0 ) {
        fprintf(stderr,"error: daemonize failed\n");
        exit(EXIT_FAILURE);
    }
    while( ttl>0 ) {
        //daemon code here
        sleep(delay);
        ttl-=delay;
    }
    return(EXIT_SUCCESS);
}

