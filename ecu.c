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
struct TripInfo trip_info; // 차량 주행시 업데이트되는 주행정보(File로 작성)
struct tm *date;
FILE *log_file;

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
    {   // 소켓 생성
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
    {   //bind() 호출
        printf("Server : Can't bind local address.\n");
        exit(0);
    }

    if(listen(server_fd, 5) < 0)
    {   //소켓을 수동 대기모드로 설정
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

        initStatus();
        log_file = fopen("log.txt","w");
        if(log_file == NULL){
            printf("Error : Can't open log file\n");
            exit(0);
        }
        while(1)
        {
            // Controller로부터 메세지 받아옴
            if(recv(client_fd, (struct DataContainer *) &data_con, sizeof(data_con), 0) <= 0) break;
            updateEngStat();
            updateTripInfo();
            printDataLog();
            printEngStat();
            writeTripInfo();
        }

        close(client_fd);
        fclose(log_file);
        printf("Server : %s client closed.\n", temp);
    }

    close(server_fd);
    return 0;
}

void initStatus(){
  // 차량상태 초기화
  data_con.turnSignal[0] = data_con.turnSignal[1] = false;
  data_con.doorOpen[0] = data_con.doorOpen[1] =data_con.doorOpen[2] = data_con.doorOpen[3] = false;
  data_con.seatbeltOn = false;
  data_con.accelator = 0;
  data_con.brake = 0;
  data_con.gear = P;

  // 엔진상태 초기화
  eng_stat.velocity = 0;
  eng_stat.accel = 0;
  eng_stat.fuel = 65; // 연료 초기값은 65L로 지정

  // 주행정보 초기화
  trip_info.mileage = 0;
  trip_info.fuelEconomy = 10;
  trip_info.instSpeed = 0;
  trip_info.avgSpeed = 0;
  trip_info.driveTime = 0;
}

void printDataLog(){
  // 컨트롤러 데이터 로그 출력
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

void printEngStat(){
  // 엔진상태 출력
  printf("=== Engine Status ===\n");
  printf("Velocity : %d\n", eng_stat.velocity);
  printf("Accelation : %d\n", eng_stat.accel);
  printf("Fuel Left : %.2f L\n\n", eng_stat.fuel);
}

void writeTripInfo(){
  // 주행 정보 파일 작성
  const time_t t = time(NULL);
  date = localtime(&t);

  fprintf(log_file, "=== Trip Info ===\n");
  fprintf(log_file, "%d/%02d/%02d %02d:%02d:%02d\n", date->tm_year+1900, date->tm_mon+1, date->tm_mday, date->tm_hour, date->tm_min, date->tm_sec);
  fprintf(log_file, "Mileage : %d\n", trip_info.mileage);
  fprintf(log_file, "Fuel Economy : %d\n", trip_info.fuelEconomy);
  fprintf(log_file, "Instantaneous Car Speed : %d\n", trip_info.instSpeed);
  fprintf(log_file, "Average Car Speed : %d\n\n", trip_info.avgSpeed);
}

void updateEngStat(){
  // 엔진 상태 업데이트

  /*
   * accelator : 0/1/2/3 , brake : 0/1/2/3
   * 가속도 : (5 * 엑셀) - (5 * 브레이크)
   * 속도 : 현재 속도 + 가속도
   * 엑셀과 브레이크가 모두 0일 경우, 속도는 매 초당 -3km/h로 감속
   * 최대속도(260km/h)에서 가속하거나 최저속도(0km/h)에서 감속할 경우 현상유지
   */
  if(data_con.accelator != 0 && data_con.brake != 0)
    eng_stat.accel = 5 * (data_con.accelator - data_con.brake);
  else
    eng_stat.accel = -3;

  if(eng_stat.velocity + eng_stat.accel > 260) eng_stat.velocity = 260;
  else if(eng_stat.velocity + eng_stat.accel < 0) eng_stat.velocity = 0;
  else eng_stat.velocity += eng_stat.accel;

  /*
   * 연료 : 잔존 연료량  - 주행거리 / 연비
   * 연비는 km/h로 계산되어 km/sec로 변환하기 위해 60 * 60으로 나눔
   */
  if(eng_stat.velocity == 0 || trip_info.fuelEconomy == 0) return;
  if(eng_stat.fuel - (eng_stat.velocity / (3600 * trip_info.fuelEconomy)) < 0) {
    eng_stat.fuel = 0;
    // TODO: 연료 소진 알람 및 속도 제어로직 추가하기
  } else eng_stat.fuel -= eng_stat.velocity / (3600 * trip_info.fuelEconomy);
}

void updateTripInfo(){
  // 주행정보 업데이트
  trip_info.mileage += (eng_stat.velocity / 3600);
  trip_info.instSpeed = eng_stat.velocity;
  trip_info.avgSpeed = trip_info.mileage / (trip_info.driveTime / 3600);
  trip_info.driveTime++;
}
