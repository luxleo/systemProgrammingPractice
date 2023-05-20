#include <string.h>
#include <stdio.h>

static int create_file(void)
{
    FILE *fp;
    if(!(fp = fopen("basic_data","w"))){
        perror("fopen()");
        return -1;
    }
    printf("after fopen(), offset = %ld\n",ftell(fp));
    fputs("Hello from create file!\n",fp);
    fputs("Hello from create file!!\n",fp);
    fputs("Hello from create file!!\n",fp);

    printf("before fclose(), offset = %ld\n",ftell(fp));
    fclose(fp);
    return 0;
}

static int read_file(void)
{
    FILE *fp;
    char buf[1024];

    if (!(fp = fopen("basic_data","r"))){
        perror("fopen()");
        return -1;
    }
    printf("\nafter fopen(), offset = %ld\n",ftell(fp));
    memset(buf,0,sizeof(buf));
    fgets(buf,sizeof(buf),fp);
    printf("after fgets() 1, offset = %ld\n",ftell(fp));
    fgets(buf,sizeof(buf),fp);
    printf("after fgets() 2, offset = %ld\n",ftell(fp));

    fclose(fp);
    return 0;
}

static int add_file_fseek(void)
{
    FILE *fp;

    if (!(fp = fopen("basic_data","r+"))){
        perror("fopen()");
        return -1;
    }
    fseek(fp,0,SEEK_END); // 파일의 맨 끝으로 이동(파일의 끝을 기준으로 오프셋이 영이므로)
    fputs("final baby\n",fp);

    fclose(fp);
    return 0;
}

int main(int argc, char **argv)
{   
    create_file();
    read_file();
    add_file_fseek();
    return 0;
}