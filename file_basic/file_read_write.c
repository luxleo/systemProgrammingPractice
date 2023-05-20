#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
fwrite, fread 는 바이너리 형식으로 읽고 쓴다.
바이너리 커맨드로 읽을 때는 cat아닌 hexdump로
*/

typedef struct person {
    int age;
    char name[20];
} Person;

static int write_to_file(void)
{
    FILE *fp=NULL;
    Person kim = {
        .age = 20,
        .name = "kim"
    };
    Person lee = {
        .age = 30,
        .name = "lee"
    };

    if(!(fp = fopen("persons","w"))){
        perror("fopen()");
        return -1;
    }

    if(fwrite(&kim, sizeof(Person),1,fp)!=1){
        goto err;
    }
    if(fwrite(&lee, sizeof(Person),1,fp)!= 1){
        goto err;
    }

    fclose(fp);
    return 0;
err:
    if(fp){
        fclose(fp);
    }
    return -1;
}

static int read_to_file(void)
{
    FILE *fp;
    Person persons[2];
    int i;
    if(!(fp = fopen("persons","r"))){
        perror("fopen()");
        return -1;
    }
    if(fread(persons,sizeof(Person),2,fp) !=2){
        fclose(fp);
        return -1;
    }

    for(i = 0; i<2; i++){
        printf("name : %s\tage : %d\n",persons[i].name,persons[i].age);
    }

    fclose(fp);
    return 0;
}

int main(int argc, char **argv)
{   
    write_to_file();
    read_to_file();
    return 0;
}