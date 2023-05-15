## process
```
1. child process -> parent process: sigchild API를 통하여 
- child process exit()시 signum을 부모 프로세스에 보내 자식 프로세스를 처리하여
- 좀비 프로세스로 남지 않도록 한다. 이때 부모 프로세스는 wait API를 통하여 
- 자식 프로세스가 종료될때까지 기다린다.

2. fork() -> exec(): 새로운 프로세스 실행
- 포크이후 exec()으로 새로운 프로세스 실행시 새로 프로세스를 만드는 것보다 오버헤드가 적고
- parent process가 직접 관리하기에 더 수월하다.
- execl(str path, args,NULL);
- wait(int *wstatus): parent process가 child process의 exit()호출을 기다린후
- wstatus에 exit code를 넣어서 반환한다.
- WIFEXITED(wstatus),WEXITSTATUS(wstatus) .. 등등의 매크로를 이용하여
- child process 종료시의 다양한 정보를 얻을 수 있다.

3. processor affinity
3-1 processor 계층: processor -> core -> (hyper thread) core 논리 분할
3-2 :
- 코어와 하이퍼 코어가 공유: L1,L2
- 같은 프로세서 내의 코어만 공유 가능: L3
- 최악의 경우: 다른 프로세서 내의 캐시에 접근 하거나, 다른 프로세서에 연결된 메모리에 접근
3-3 software 처리 processor 조회,수정:
- sched_getaffinity(): 조회
- sched_setaffinity(): 처리하는 프로세서 변경

4. thread
4-1: int pthread_create(), void pthread_exit()으로 각각 스레드 생성 종료
4-2: int pthread_join(): 프로세스의 wait 처럼 호출한 스레드가 자식 스레드 자원 회수
4-3: int pthread_detach(): 호출된 스레드 종료시 스스로 자원회수
4-4: int pthread_mutex_init() 스레드 동기화: 스레드끼리 상호배제(공유 자원 보호) 생성
- mutex -> mutual exclusive(상호 배제)
- int pthread_mutex_destroy(): 생성된 mutex 파괴
- int pthread_mutex_lock(): 상호배제 실시하고 unlock 만날때 까지 코드 실행
- int pthread_mutex_trylock(): lock()과 달리 이미 lock 상태이면 바로 에러 발생
- int pthread_mutex_unlock():

```