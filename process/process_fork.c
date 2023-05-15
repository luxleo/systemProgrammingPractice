#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

void test_func(){
    printf("this is test func from process %d\n",getpid());
}
int main(int argc,char **argv){
    int exit_status;
    pid_t pid;
    pid=fork();
    printf("origin process pid %d\n", getpid());
    if(pid == 0){
        /* child process*/
        printf("child process pid %d\n", getpid());
        if(execl("/bin/ls","ls","-al",NULL)==-1){
            // execl이 실패하는 경우
            printf("execl() failed");
            return -1;
        }
    }else if(pid>0){
        /*parent process*/
        /*parent process 는 child process 를 wait API를 통해서 처리를 해주어야한다.
        그렇지 않으면 좀비 프로세스(커널 프로세스 테이블에 남아있음)가 되어
        메모리 누수가 일어난다.*/ 
        printf("parent process pid %d, child process %d\n", getpid(),pid);
    }
    test_func();

    pid=wait(&exit_status); // child process 종료 전까지 기다림
    if(WIFEXITED(exit_status)){
        printf("child %d returns %d\n",pid,WEXITSTATUS(exit_status));
    }else{
        printf("child %d not exited\n",pid);
    }
    return 0;
}
