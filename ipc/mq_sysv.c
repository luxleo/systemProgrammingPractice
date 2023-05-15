#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>

#define IPC_KEY_FILENAME "./ipc_sysV"
#define IPC_KEY_PROJNAME 'a'

struct msgbug {
    long mtype;
#define MSG_BUFSZ 64
    char buf[MSG_BUFSZ];
};

static void print_usage(const char *progname)
{
    printf("%s (send|recv) MTYPE\n",progname);
}
static int init_msgq(void)
{
    int msgq;
    key_t key;
    
    key = ftok(IPC_KEY_FILENAME, IPC_KEY_PROJNAME);
    if (key == -1){
        perror("ftok()");
        return -1;
    }
    msgq = msgget(key,0644 | IPC_CREAT);
    if (msgq == -1){
        perror("msgq()");
        return -1;
    }
    return msgq;
}
static int do_send(long mtype)
{   
    int msgq;
    struct msgbug mbuf;

    msgq = init_msgq();
    if (msgq == -1){
        perror("msgq()");
        return -1;
    }

    memset(&mbuf, 0, sizeof(mbuf)); // mbuf 초기화
    mbuf.mtype = mtype;
    
    //문자열을 mbuf.buf에 저장
    snprintf(mbuf.buf, sizeof(mbuf.buf),"hello world mtype is %ld",mtype); 
    if(msgsnd(msgq,&mbuf, sizeof(mbuf.buf),0)==-1){
        perror("msgsnd()");
        return -1;
    }
    return 0;
}
static int do_recv(long mtype)
{   
    int msgq,ret;
    struct msgbug mbuf;

    msgq = init_msgq();
    if (msgq == -1){
        perror("msgq()");
        return -1;
    }

    memset(&mbuf, 0, sizeof(mbuf)); // mbuf 초기화
    ret = msgrcv(msgq, &mbuf, sizeof(mbuf.buf),mtype,0);
    if(ret == -1){
        perror("msgrcv()");
        return -1;
    }
    printf("msgtype: %ld\nmsg content: [%s]\n",mbuf.mtype,mbuf.buf);
    return 0;
}
int main(int argc, char **argv)
{   
    int ret;
    long mtype;
    if(argc<3){
        print_usage(argv[0]);
        return -1;
    }
    mtype = strtol(argv[2],NULL,10); // 문자열을 long으로 전환

    if(!strcmp(argv[1],"send")){
        if(mtype <=0){
            print_usage(argv[0]);
            return -1;
        }
        ret = do_send(mtype);
    }else if(!strcmp(argv[1],"recv")){
        ret = do_recv(mtype);
    }else{
        print_usage(argv[0]);
        return -1;
    }
    return 0;
}