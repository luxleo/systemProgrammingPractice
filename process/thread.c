#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_TRY 100000000

int shared = 0;
pthread_mutex_t mutex;

void *child_thread_main(void *arg){
    /*
    mutex_lock ,unlock 없이 진행하면 논리적으로 최종 shared값이 0이 나와야 하지만
    그렇지 않게 되므로 mutex_lock,unlock으로 동기화 해주어야한다.
    */
    unsigned int i;
    printf("child thread id : %lu\n",pthread_self());

    //pthread_mutex_lock(&mutex);
    for (i=0; i<MAX_TRY; i++){
        pthread_mutex_lock(&mutex);
        shared++;
        pthread_mutex_unlock(&mutex);
    }
    for (i=0; i<MAX_TRY; i++){
        pthread_mutex_lock(&mutex);
        shared--;
        pthread_mutex_unlock(&mutex);
    }
    //pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(int argc, char **argv){
    int ret;
    pthread_t child_threads[2];

    if(pthread_mutex_init(&mutex,NULL)){
        printf("mutex init failed\n");
        return -1;
    }

    printf("main thread id : %lu\n",pthread_self());
    ret = pthread_create(&child_threads[0],NULL,child_thread_main,NULL);
    if (ret){
        printf("pthread_create() fail(%d)\n",ret);
    }
    ret = pthread_create(&child_threads[1],NULL,child_thread_main,NULL);
    if (ret){
        printf("pthread_create() fail(%d)\n",ret);
    }

    printf("execute joining threads [%lu]\n",child_threads[0]);
    if(pthread_join(child_threads[0],NULL)){
        printf("pthread_join(%lu) fail\n", child_threads[0]);
    }
    printf("execute joining threads [%lu]\n",child_threads[1]);
    if(pthread_join(child_threads[1],NULL)){
        printf("pthread_join(%lu) fail\n", child_threads[1]);
    }

    printf("value of shared : %d\n",shared);
    printf("main thread done\n");
}