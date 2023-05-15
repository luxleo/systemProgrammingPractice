#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sched.h>

#define MAX_CPU 2
static void print_cpuset(cpu_set_t *mask){
    /* 할당 가능한 cpu 목록 반환 */
    int i;
    for (i=0; i<MAX_CPU; i++){
        if(CPU_ISSET(i,mask)) printf("CPU%d ",i);
    }
    printf("\n");
}

int main(int argc, char **argv){
    int cur_cpu;
    cpu_set_t mask;
    if(sched_getaffinity(getpid(),sizeof(cpu_set_t), &mask)){
        perror("sched_getaffinity()\n");
        return -1;
    }
    print_cpuset(&mask);
    cur_cpu = sched_getcpu();
    printf("run on CPU%d\n",cur_cpu);
    
    CPU_ZERO(&mask); // cpu_mask 초기화
    CPU_SET(cur_cpu ^1,&mask); // 사용가능한 cpu 0,1두 가지라 가정하고 현재 사용되고있는 cpu외 cpu 마스크에 할당

    if(sched_setaffinity(getpid(),sizeof(cpu_set_t), &mask)){
        // cpu 재할당 문제 발생시 탈출
        perror("sched_setaffinity()\n");
        return -1;
    }
    cur_cpu = sched_getcpu(); // 새로 할당된 cpu
    printf("now, run on CPU%d",cur_cpu);

}