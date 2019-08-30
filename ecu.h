#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum _GEAR_TYPE{
  P = 0,
  R,
  N,
  D
}GEAR_TYPE;

struct DataContainer{
  bool turnSignal[2];   //[0] {R, L}
  bool doorOpen[4];     //[1] {FL, FR, RL, RR}
  bool seatbeltOn;      //[2]
  int accelator;        //[3] 0/1/2/3
  int brake;            //[4] 0/1/2/3
  GEAR_TYPE gear;       //[5] 0(P)/1(R)/2(N)/3(D)
};

struct EngineStatus{
  float velocity;       //[6] 엔진 속도
  float accel;          //[7] 가속도
  float fuel;           //[8] 잔여 연료량
};

struct TripInfo{
  float mileage;        //[9] 주행 거리
  int driveTime;        //[10] 총 주행 시간
  int fuelEconomy;      //[11] 차량 연비
  int instSpeed;        //[12] 순간 속도
  float avgSpeed;       //[13] 평균 속도
};

struct Media{
  char singerName[32];  //[14] 가수 이름
  char songName[64];    //[15] 노래 이름
  int status;           //[16] 0:Pause 1:Play
  int totalPlaytime;    //[17] 해당 곡의 플레이 시간
  int currentPlaytime;  //[18] 현재 플레이 시간
  int volume;           //[19] 음량
};
