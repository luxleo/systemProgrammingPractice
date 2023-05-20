# 홧 아이 런
## file_processing
```
1. fopen
- FILE *fopen(const char *pathname, const char *mode);
    parameter:
        -pathname:파일 경로
        - mode: options
    return:
        성공시: 열린 파일 포인터 (stream)
        실패시: NULL
- fopen options:
    "r" : readOnly, start at head, work only file exist
    "r+" : read and write, start at head,  work only file exist
    "w" : writeOnly, start at head, if file exist delete it and recreate
    "w+" : read and write, start at head, if file exist delete it and recreate
    - a means append
    "a" : writeOnly, start at bottom, if file exist read, does'n create
    "a+" : read and write, start at bottom, if file exist than read, if doen't create
2. fclose
- int fclose(FILE *stream)
    parameter: stream: fopen으로 연 파일 포인터
    return : 성공시 0, 실패시 EOF(End Of File)
- fclose option:
3. ftell : 현재 파일 오프셋 가져오기(GUI의 경우 커서의 위치) (단위는BYTE)
 - long ftell(FILE *stream)
    return : 성공시 파일 포지션(오프셋값) , 실패시 -1
4. fseek : 오프셋 설정(단위는 BYTE)
- int fseek(FILE *stream, long offset, int whence)
- parameter:
    whence:오프셋의 기준
        - SEEK_SET: 파일의 시작 위치 기준
        - SEEK_END : 파일의 끝 위치 기준
        - SEEK_CUR : 현재 오프셋 기준
- return: 성공시 파일 포지션(오프셋 값), 실패시 -1
5. 파일 읽고 쓰기
-write mode: 메모장에 쓰는 것과 같이 저장
    10 저장시 -> 1:0x31, 0x30 저장
- binary mode: 데이터 그대로 저장 (자원 소모가 훨씬 적다)
    10 저장시 -> 0x0a 저장
    - fprinf
    int fprintf(FILE *stream, const char *format, ...)
    parameter:
        format: "%d "...
    return: 성공시: 쓰여진 데이터 크기, 실패시: 음수
    - fscanf: int fscanf(FILE *stream, const char *format, ...)
    return : 성공시: 입력 받은 데이터 갯수, 실패시 :EOF


```
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
### IPC(pipe)
```
pipe: related process (부모-자식,자식-자식)에 사용할 수 있는 IPC 도구
API:
1. int pipe(int pipefd[2]); -> pipe 생성
- param: pipefd[2]:생성될 pipefd를 저장할 버퍼
- pipefd[0]: reader-side buffer, pipefd[1]: writer-side buffer
- return : 0: 성공, -1: 실패
2. pipe I/O:
2-1 blocking 일어날 조건:
- pipe full 일때 write() -> blocking
- pipe empty 일때 read() -> blocking
- write size가 PIPE_BUF 보다 크면(초과 해서 적으면) interleaved(중간 데이터 손실 )발생
```

### IPC(Named pipe)
```
named pipe: pipe와 같이 unidirectional-byte stream이나 
파일 경로가 ID이므로 unrelated process 간 통신 가능하다.
read-side와 write-side가 모두 open()을 실행해야 동작한다.

API:
int mkfifo(const char *pathname, mode_t mode); named pipe 생성
params:
- pathname: 생성할 named pipe 파일 경로
- mode: permission
returns:
- 성공시 0, 실패시 -1
```
### IPC(Message Queue-SysV)
```
Message 기반 Communication
- byte stream 과 달리 일부만 읽을수 없다.
- 즉 하나의 메시지는 하나의 덩어리(단위)이다.
- unrelated process 간에도 사용 가능하다.
- POSIX 기반과 달리 SysV에서는 message Type 별로 구독 가능하다.
-- sender와 receiver가 1:N의 관계일때 지정한 message type 만 수신 가능하다.
- IPC-key 기반 identifier(not fd)

SysV MQ APIS
- 데이터 송수신:
-- int msgsnd(int msgid, const void *msgp, size_t msgsz, int msgflg)
    메시지 전송
    파라미터:
        msgid:msgget()함수를 대입
        msgp: 전송할 메시지 버퍼로 아래와 같은 형식의 구조체 대입
            struct msgbuf{
                long mtype; // 메시지 타입
                char mtext[size]; // 메시지 본문
            }
        msgsz: 전송 메시지 사이즈 msgbuf의 mtext길이(byte)
        msgflg: IPC_NOWAIT(non blocking I/O)
    반환값:
        성공시:0
        실패시:-1
-- size_t msgrcv(int msqid,void *msgp,size_t msgsz, long msgtyp, int msgflg)
    메시지 수신
    파라미터:
        msgtyp: attach한 MQ에서 수신할 타입
            0:메시지큐의 첫번째
            양수: 해당 수와 일치하는 타입의 첫번째 메시지
            음수: 지정된 절댓값과 같거나 작은 타입의 메시지 중 첫번째
        msgp: msgsnd와 동일
        msgsz: msgp의 mtext 수신할 사이즈 (byte)
        msgflg:
            IPC_NOWAIT: non-blocking으로 수신
            MSG_COPY: 메시지를 이동하는 것이 아니라 복사하여 가져옴(중복해서 수신 가능)
                반드시 IPC_NOWAIT와 함께 사용한다.
            MSG_EXCEPT: msgtyp 과 일치하지 않는 타입의 메시지 수신
            MSG_NOERROR: msgsz보다 큰 메시지는 truncate 시킨다.
    반환값:
        성공시: 수신한 메시지 길이
        실패시 :-1
- 기타:
-- int msgctl(int msqid, int cmd, struct msqid_ds *buf)
    메시지 큐 제어
    파라미터:
        msqid:
        cmd:    
            IPC_STAT:kernel의 msqid_ds의 정보 조회
            IPC_SET: kernel의 msqid_ds에 설정
            IPC_RMID: MQ 제거
-- int msgget(key_t key, int msgflg)
    Message Queue ID를 구한다.
    옵션(msgflg)에 따라 생성가능
    파라미터:
        key: IPC object or IPC_PRIVATE(IPC_PRIVATE 지정시 새로운 MQ ID생성)
        msgflg: permission+mask
            IPC_CREAT: key에 매치되는 MQ ID 없으면 생성
            IPC_EXCL: key에 매치되는 MQ ID 있으면 에러 발생
    반환값:
        성공시: 생성된 MQ ID
        실패시: -1
-- key_t ftok(const char *pathname, int proj_id)
    두 파라미터를 조합하여 MQ ID반환, 두 값에 따라 unique한 키를 반환하나 항상 유니크하다고 보장할수 없다.
    파라미터:
        pathname: 조합할 파일경로, 이때 readable한 파일이어야함
        proj_id: 임의의 아이디 ex -> 'a'
    반환값:
        성공시: IPC key
        실패시: -1
    용례:
        위의 msgget의 key 파라미터 자리에 대입: msgget(ftok(pathname,'a'),msgflg)
IPC TOOL TERMINAL CMD
ipcs -q: 활성화된 메시지 큐 출력
ipcrm -q [message queue id]
```