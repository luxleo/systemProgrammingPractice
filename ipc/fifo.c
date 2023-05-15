#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO_FILENAME "./testfifio"

static void print_usage(char *progname)
{
    printf("%s (w|r)\n",progname);
    return;
}
static int do_reader()
{
    int fd;
    char buf[128];

    fd = open(FIFO_FILENAME,O_RDONLY);
    if(fd<0){
        perror("open()");
        return -1;
    }
    read(fd,buf,sizeof(buf));
    printf("writer said -> %s\n",buf);
    close(fd);
    return 0;
}
static int do_writer()
{   
    int fd;
    char buf[128];

    unlink(FIFO_FILENAME); // 파일이므로 fifo 만들기전 언링크
    if(mkfifo(FIFO_FILENAME,0644)){
        perror("mkfifio()");
        return -1;
    }
    fd = open(FIFO_FILENAME,O_WRONLY);
    if(fd<0){
        perror("open()");
        return -1;
    }
    strncpy(buf,"Hello",sizeof(buf));
    write(fd,buf,strlen(buf));
    close(fd);
    return 0;
}
/*fifo(w|r)*/
int main(int argc, char **argv)
{
    if(argc<2){
        print_usage(argv[0]);
        return -1;
    }

    if(!strcmp(argv[1],"r")){
        /* reader*/
        do_reader();
    }else if(!strcmp(argv[1],"w")){
        /*writer*/
        do_writer();
    }else{
        print_usage(argv[0]);
        return -1;
    }
}