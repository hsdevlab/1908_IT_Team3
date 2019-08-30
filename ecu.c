#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "ecu.h"
#define BUF_LEN 256
//메시지 송수신에 사용될 버퍼 크기를 선언

static struct DataContainer data_con; // Controller로부터 받아온 데이터 입력
static struct EngineStatus eng_stat; // ECU에서 계산된 엔진 상태정보
static struct TripInfo trip_info; // 차량 주행시 업데이트되는 주행정보(File로 작성)
static struct Media media;
static struct tm *date;

static char ecu_msg[BUF_LEN]; // ecu -> cluster로 보내는 메세지
static char buffer[BUF_LEN];
static char fval[BUF_LEN];

static FILE *log_file;
static FILE *trip_file;
static int tripFileLastLine = 0; // 주행정보 파일의 마지막 라인 넘버
static int server_fd, client_fd[3]; // 서버/클라이언트 소켓 번호
static char client_ip[20];

int setFvalMem(float val, int idx);

void *controllerThread(void *data)
{
  /*
   * 클러스터 스레드 함수
   * 클러스터 클라이언트 소켓으로 DataContainer + EngineStatus + TripInfo 메세지 전달
   */
    initStatus();
    log_file = fopen("log.txt","w");
    if(log_file == NULL){
        printf("Error : Can't open log file\n");
        exit(0);
    }
    while(1)
    {
        // Controller로부터 메세지 받아옴
        memset(buffer, 0x00, BUF_LEN);
        if(recv(*((int *)data), buffer, BUF_LEN, 0) <= 0) break;
        parseMsg();
        updateEngStat();
        updateTripInfo();
        printDataLog();
        printEngStat();
        writeTripInfo();
        setEcuMsg();
    }
    fclose(log_file);
    close(*((int *)data));
    printf("Server : %s client closed.\n", client_ip);
}

void *clusterThread(void *data)
{
  /*
   * 클러스터 스레드 함수
   * 클러스터 클라이언트 소켓으로 DataContainer + EngineStatus + TripInfo 메세지 전달
   */
    while(1)
    {
        usleep(50000);
        memset(ecu_msg, 0x00, BUF_LEN+1);
        setEcuMsg();
        ecu_msg[BUF_LEN] = '\n';
        if(send(*((int *)data), ecu_msg, BUF_LEN+1, 0) <= 0) {
          printf("send fail\n");
          break;
        }
        printf("send : %s\n\n", ecu_msg);
    }
    close(*((int *)data));
    printf("Server : %s client closed.\n", client_ip);
}

void *engineThread(void *data)
{
    while(1)
    {
        usleep(50000);
        if(send(*((int *)data), (struct EngineStatus *) &eng_stat, sizeof(eng_stat), 0) <= 0) {
          printf("send fail\n");
          break;
        }
    }
    close(*((int *)data));
    printf("Server : %s client closed.\n", client_ip);
}

int main(int argc, char *argv[])
{
    struct sockaddr_in server_addr, client_addr, cluster_addr;
    int len, msg_size;
    int tnum = 0;
    pthread_t thread_clu, thread_con, thread_eng; // 클러스터/컨트롤러 스레드
    int thread_id1, thread_id2, thread_id3;

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
        client_fd[tnum] = accept(server_fd, (struct sockaddr *)&client_addr, &len);
        if(client_fd[tnum] < 0)
        {
            printf("Server: accept failed.\n");
            exit(0);
        }
        inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, client_ip, sizeof(client_ip));
        printf("Server : %s client connected.\n", client_ip);

				if(!strcmp(client_ip, "192.168.100.47")) {
          // 클러스터 스레드
          thread_id1 = pthread_create(&thread_clu, NULL, clusterThread, (void *)&client_fd[tnum]);
          if(thread_id1 < 0){
            perror("cluster thread create error : ");
            exit(0);
          }
				} else if(!strcmp(client_ip, "127.0.0.1")){
          // 엔진 스레드
          thread_id2 = pthread_create(&thread_eng, NULL, engineThread, (void *)&client_fd[tnum]);
          if(thread_id2 < 0){
            perror("engine thread create error : ");
            exit(0);
          }
        } else {
          // 컨트롤러 스레드
          thread_id3 = pthread_create(&thread_con, NULL, controllerThread, (void *)&client_fd[tnum]);
          if(thread_id3 < 0){
            perror("controller thread create error : ");
            exit(0);
          }
				}
        tnum++;
        sleep(1);
    }

    close(server_fd);
    return 0;
}

int getStrSize(){
	int i = 0;
	while(fval[i] != '\0') i++;
	return i;
}

int setFvalMem(float val, int idx){
	memset(fval, 0x00, BUF_LEN);
	sprintf(fval, "%.2f", val);
	int str_len = getStrSize();
	for(int i=0; i<str_len; i++){
		ecu_msg[idx++] = fval[i];
	}
	return idx;
}

int setIvalMem(int val, int idx){
	memset(fval, 0x00, BUF_LEN);
	sprintf(fval, "%d", val);
	int str_len = getStrSize();
	for(int i=0; i<str_len; i++){
		ecu_msg[idx++] = fval[i];
	}
	return idx;
}

void setEcuMsg(){
  memset(ecu_msg, 0x00, BUF_LEN); // ECU 메세지 초기화
  int idx = 0;
  ecu_msg[idx++] = data_con.turnSignal[0] + '0';
	ecu_msg[idx++] = data_con.turnSignal[1] + '0';
	ecu_msg[idx++] = '|';
	ecu_msg[idx++] = data_con.doorOpen[0] + '0';
	ecu_msg[idx++] = data_con.doorOpen[1] + '0';
	ecu_msg[idx++] = data_con.doorOpen[2] + '0';
	ecu_msg[idx++] = data_con.doorOpen[3] + '0';
	ecu_msg[idx++] = '|';
	ecu_msg[idx++] = data_con.seatbeltOn + '0';
	ecu_msg[idx++] = '|';
	ecu_msg[idx++] = data_con.accelator + '0';
	ecu_msg[idx++] = '|';
	ecu_msg[idx++] = data_con.brake + '0';
	ecu_msg[idx++] = '|';
	ecu_msg[idx++] = data_con.gear + '0';
	ecu_msg[idx++] = '|';

	idx = setFvalMem(eng_stat.velocity, idx);
	ecu_msg[idx++] = '|';
	idx = setFvalMem(eng_stat.accel, idx);
	ecu_msg[idx++] = '|';
	idx = setFvalMem(eng_stat.fuel, idx);
	ecu_msg[idx++] = '|';
	idx = setFvalMem(trip_info.mileage, idx);
	ecu_msg[idx++] = '|';
	idx = setIvalMem(trip_info.driveTime, idx);
	ecu_msg[idx++] = '|';
	idx = setIvalMem(trip_info.fuelEconomy, idx);
	ecu_msg[idx++] = '|';
	idx = setIvalMem(trip_info.instSpeed, idx);
	ecu_msg[idx++] = '|';
	idx = setFvalMem(trip_info.avgSpeed, idx);

  ecu_msg[idx++] = '|';
  ecu_msg[idx++] = media.status + '0';
  ecu_msg[idx++] = '|';
  int slen = 0;
  for(int i=idx; i<BUF_LEN; i++){
    if(ecu_msg[i] != '|') { ecu_msg[i] = media.singerName[i-idx]; slen++; }
    else break;
  }
  ecu_msg[idx+slen] = '|';
  idx += slen + 1;
  slen = 0;
  for(int i=idx; i<BUF_LEN; i++){
    if(ecu_msg[i] != '|') { ecu_msg[i] = media.songName[i-idx]; slen++; }
    else break;
  }
  ecu_msg[idx+slen] = '|';
  idx += slen + 1;

}

void parseMsg(){
  data_con.turnSignal[0] = buffer[0] - '0';
  data_con.turnSignal[1] = buffer[1] - '0';
  data_con.doorOpen[0] = buffer[2] - '0';
  data_con.doorOpen[1] = buffer[3] - '0';
  data_con.doorOpen[2] = buffer[4] - '0';
  data_con.doorOpen[3] = buffer[5] - '0';
  data_con.seatbeltOn = buffer[6] - '0';
  data_con.accelator = buffer[7] - '0';
  data_con.brake = buffer[8] - '0';
  data_con.gear = buffer[9] - '0';
  media.status = buffer[10] - '0';

  memset(media.singerName, 0x00, 32);
  memset(media.songName, 0x00, 64);
  int hd = 12;
  int tl = getDeliIdx(hd);
  for(int i=hd; i<=tl; i++) media.singerName[i-hd] = buffer[i];
  media.singerName[tl+1] = '|';
  hd = tl + 2;
  tl = getDeliIdx(hd);
  for(int i=hd; i<=tl; i++) media.songName[i-hd] = buffer[i];
  media.songName[tl+1] = '|';

  printf("\n%d\n",media.status);
  printf("%s\n",media.singerName);
  printf("%s\n",media.songName);
}

int getDeliIdx(int hd){
  int tl;
  for(int i=hd; i<BUF_LEN; i++){
    if(buffer[i] == '|') {
      tl=i;
      break;
    }
  }
  return tl;
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
  trip_info.mileage = 0.0;
  trip_info.fuelEconomy = 10;
  trip_info.instSpeed = 0;
  trip_info.avgSpeed = 0;
  trip_info.driveTime = 0;

  // 미디어 초기화
  memset(media.singerName, 0x00, 32);
  memset(media.songName, 0x00, 64);
  media.status = 0;
  media.totalPlaytime = 0;
  media.currentPlaytime = 0;
  media.volume = 0;
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
  printf("Gear : [%s]\n\n", gr[data_con.gear]);
}

void printEngStat(){
  // 엔진상태 출력
  printf("=== Engine Status ===\n");
  printf("Velocity : %.2f\n", eng_stat.velocity);
  printf("Accelation : %.2f\n", eng_stat.accel);
  printf("Fuel Left : %.2f L\n\n", eng_stat.fuel);
}

void writeTripInfo(){
  // 주행 정보 파일 작성
  const time_t t = time(NULL);
  date = localtime(&t);
  fprintf(log_file, "%d/%02d/%02d %02d:%02d:%02d", date->tm_year+1900, date->tm_mon+1, date->tm_mday, date->tm_hour, date->tm_min, date->tm_sec);
  fprintf(log_file, "| Mileage:%.4fkm\t", trip_info.mileage);
  fprintf(log_file, "| FuelEco:%d\t", trip_info.fuelEconomy);
  fprintf(log_file, "| Speed:%dkm/h\t", trip_info.instSpeed);
  fprintf(log_file, "| AverageSpeed:%.2fkm/h\n", trip_info.avgSpeed);
}

void updateEngStat(){
  // 엔진 상태 업데이트

  /*
   * accelator : 0/1/2/3 , brake : 0/1/2/3
   * 가속도 : (0.25 * 엑셀) - (1.00 * 브레이크)
   * 속도 : 현재 속도 + 가속도
   * 엑셀과 브레이크가 모두 0일 경우, 속도는 매 초당 -3km/h로 감속
   * 최대속도(260km/h)에서 가속하거나 최저속도(0km/h)에서 감속할 경우 현상유지
   */
  if(data_con.accelator != 0 || data_con.brake != 0)
    eng_stat.accel = 0.25 * (data_con.accelator - (4 * data_con.brake));
  else
    eng_stat.accel = -0.25;

  if(eng_stat.velocity + eng_stat.accel > 260) eng_stat.velocity = 260;
  else if(eng_stat.velocity + eng_stat.accel < 0) eng_stat.velocity = 0;
  else eng_stat.velocity += eng_stat.accel;

  /*
   * 연료 : 잔존 연료량  - 주행거리 / 연비
   * 연비는 km/h로 계산되어 km/sec(1/20초)로 변환하기 위해 60 * 60 / 20으로 나눔
   */
  if(eng_stat.velocity == 0 || trip_info.fuelEconomy == 0) return;
  if(eng_stat.fuel - (eng_stat.velocity / (float)(3600 * trip_info.fuelEconomy)) < 0) {
    eng_stat.fuel = 0;
    // TODO: 연료 소진 알람 및 속도 제어로직 추가하기
  } else eng_stat.fuel -= (float)eng_stat.velocity / (float)(3600 * trip_info.fuelEconomy);
}

void updateTripInfo(){
  // 주행정보 업데이트
  trip_info.mileage += 0.05 * ((float)eng_stat.velocity / (float)3600);
  trip_info.instSpeed = eng_stat.velocity;
  trip_info.driveTime++;
  if(trip_info.driveTime != 0 && ((float)trip_info.driveTime / (float)180) != 0.0) trip_info.avgSpeed = trip_info.mileage / ((float)trip_info.driveTime / (float)72000);
}
