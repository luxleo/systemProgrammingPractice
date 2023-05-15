#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv){
    int pipe_fds[2];
    pid_t pid;
    char buf[1024];
    int wstatus;

    memset(buf,0,sizeof(buf));
    printf("parent[%d]: function start!\n",getpid());

    if(pipe(pipe_fds)){
        /*파이프 오픈*/
        perror("pipe()\n");
        return -1;
    }
    
    pid = fork();
    if(pid == 0){
        /*child process*/
        close(pipe_fds[1]); // write-side close
        read(pipe_fds[0],buf,sizeof(buf));
        printf("child[%d]: msg from parent -> %s\n",getpid(),buf);

        close(pipe_fds[0]); // read-side close
    }else if(pid >0){
        /* parent process*/
        close(pipe_fds[0]); // read-side close
        strncpy(buf,"hello from parent",sizeof(buf)-1);
        write(pipe_fds[1],buf,strlen(buf));
        close(pipe_fds[1]);

        pid = wait(&wstatus);
    }else {
        /*error case*/
        perror("fork()");
        goto err; // 에러 발생시 파이프 닫기
    }
    return 0;
err:
    close(pipe_fds[0]);
    close(pipe_fds[1]);
}