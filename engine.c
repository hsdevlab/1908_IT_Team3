#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "ecu.h"

#define BUF_LEN 128

struct EngineStatus eng_stat;

void printStatus(){
  printf("     ____\n");
  printf("  __/  |_\\_\n");
  printf(" |  _     _``-.  현재속력 %.2f km/h\n", eng_stat.velocity);
  printf(" '-(_)---(_)--'  잔여연료 %.2f L\n\n", eng_stat.fuel);
}

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
    {
        printf("can't create socket\n");
        exit(0);
    }

    bzero((char *)&server_addr, sizeof(server_addr));
    //서버의 소켓주소 구조체 server_addr을 NULL로 초기화

    server_addr.sin_family = AF_INET; //주소 체계를 AF_INET 로 선택
    server_addr.sin_addr.s_addr = inet_addr(argv[1]); //32비트의 IP주소로 변환
    server_addr.sin_port = htons(atoi(argv[2])); // 서버 포트 번호

    if(connect(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("can't connect.\n");
        exit(0);
    }

    struct DataContainer dc;
    while(1)
    {
      usleep(50000);
      printf("=== Engine Status ===\n");
      recv(server_fd, (struct EngineStatus *) &eng_stat, sizeof(eng_stat), 0);

      printStatus();
      // TODO: 컨트롤러단 주기적 신호 발생시키기
    }

    close(server_fd);
}
