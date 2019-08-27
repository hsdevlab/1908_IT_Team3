#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "ecu.h"

#define BUF_LEN 128

void main(int argc, char *argv[])
{
    int n;
    char *haddr;
    struct sockaddr_in server_addr;
    int server_fd, client_fd;
    //struct sockaddr_in server_addr : 서버의 소켓주소 구조체
    char buf[BUF_LEN+1];

    if(argc < 3)
    {
            printf("usage : %s ip_Address port_Num\n", argv[0]);
            exit(0);
    }
    haddr = argv[1];

    if((server_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {//소켓 생성과 동시에 소켓 생성 유효검사
            printf("can't create socket\n");
            exit(0);
    }

    bzero((char *)&server_addr, sizeof(server_addr));
    //서버의 소켓주소 구조체 server_addr을 NULL로 초기화

    server_addr.sin_family = AF_INET; //주소 체계를 AF_INET 로 선택
    server_addr.sin_addr.s_addr = inet_addr(argv[1]); //32비트의 IP주소로 변환
    server_addr.sin_port = htons(atoi(argv[2])); // 서버 포트 번호

    if(connect(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {//서버로 연결요청
            printf("can't connect.\n");
            exit(0);
    }

    struct DataContainer dc;
    dc.turnSignal[0] = dc.turnSignal[1] = false;
    dc.doorOpen[0] = dc.doorOpen[1] = dc.doorOpen[2] = dc.doorOpen[3] = false;
    dc.seatbeltOn = true;
    dc.accelator = 0;
    dc.brake = 0;
    dc.gear = P;
    send(server_fd,(struct DataContainer *) &dc, sizeof(dc), 0);

    //char* tempMsg = "00 0000 1 0 0 0\0";
    //write(server_fd,tempMsg,strlen(tempMsg)+1);

    while((n = read(server_fd, buf, BUF_LEN)) > 0)
    {//서버가 보내오는 daytime 데이터의 수신
            buf[n] = NULL;
            printf("%s", buf);
    }

    close(server_fd);
    //사용이 완료된 소켓을 닫기
}
