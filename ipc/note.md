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