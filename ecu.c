#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "ecu.h"
#define BUF_LEN 128
//메시지 송수신에 사용될 버퍼 크기를 선언

struct DataContainer data_con; // Controller로부터 받아온 데이터 입력
struct EngineStatus eng_stat; // ECU에서 계산된 엔진 상태정보

int main(int argc, char *argv[])
{
    char buffer[BUF_LEN];
    struct sockaddr_in server_addr, client_addr;
    char temp[20];
    int server_fd, client_fd; // 서버/클라이언트 소켓 번호
    int len, msg_size;

    if(argc != 2)
    {
        printf("usage : %s [port]\n", argv[0]);
        exit(0);
    }

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {// 소켓 생성
        printf("Server : Can't open stream socket\n");
        exit(0);
    }
    memset(&server_addr, 0x00, sizeof(server_addr));
    //server_Addr 을 NULL로 초기화

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));
    //server_addr 셋팅

    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <0)
    {//bind() 호출
        printf("Server : Can't bind local address.\n");
        exit(0);
    }

    if(listen(server_fd, 5) < 0)
    {//소켓을 수동 대기모드로 설정
        printf("Server : Can't listening connect.\n");
        exit(0);
    }

    memset(buffer, 0x00, sizeof(buffer));
    printf("Server : wating connection request.\n");
    len = sizeof(client_addr);
    while(1)
    {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &len);
        if(client_fd < 0)
        {
            printf("Server: accept failed.\n");
            exit(0);
        }
        inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, temp, sizeof(temp));
        printf("Server : %s client connected.\n", temp);

        //msg_size = read(client_fd, buffer, 1024);
        recv(client_fd, (struct DataContainer *) &data_con, sizeof(data_con), 0);
        printf("%d",data_con.seatbeltOn);
        testPrintData();
        // Controller로부터 버퍼에 메세지 받아옴

        //write(client_fd, buffer, msg_size);
        close(client_fd);
        printf("Server : %s client closed.\n", temp);
    }
    close(server_fd);
    return 0;
}

void testPrintData(){
  char* sig[2] = {"off", "on"};
  char* gr[4] = {"P", "R", "N", "D"};

  printf("=== Data Container ===\n");
  printf("Turn signal : R(%s) / L(%s)\n",sig[data_con.turnSignal[0]], sig[data_con.turnSignal[1]]);
  printf("Door open : FL(%s) / FR(%s) / RL(%s) / RR(%s)\n",
          sig[data_con.doorOpen[0]], sig[data_con.doorOpen[1]], sig[data_con.doorOpen[2]], sig[data_con.doorOpen[3]]);
  printf("Seatbelt : %s\n", sig[data_con.seatbeltOn]);
  printf("Accelator : %d\n", data_con.accelator);
  printf("Brake : %d\n", data_con.brake);
  printf("Gear : [%s]\n", gr[data_con.gear]);
}
