#include <stdio.h>
#include <string.h> // memset

int write_to_file(void)
{
    FILE *fp;
    fp = fopen("basic_data","w");
    if (fp == NULL){
        perror("fopen()");
        return -1;
    }
    fputs("hello from file_basic.c",fp);
    fclose(fp);
    return 0;
}
int read_file(void)
{
    FILE *fp;
    char buf[512];
    fp = fopen("basic_data","r");
    if(fp == NULL){
        perror("fopen()");
        return -1;
    }
    memset(buf,0,sizeof(buf)); // buf 초기화
    fgets(buf, sizeof(buf),fp);
    fclose(fp);

    printf("data : %s\n",buf);

    return 0;
}
int main(int argc, char **argv)
{   
    if(write_to_file()){
        perror("write to file()");
        return -1;
    }
    if(read_file()){
        perror("read_file()");
        return -1;
    }
    return 0;
}