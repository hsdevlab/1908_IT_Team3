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
  bool turnSignal[2]; // {R, L}
  bool doorOpen[4]; // {FL, FR, RL, RR}
  bool seatbeltOn;
  int accelator; // 0/1/2/3
  int brake; // 0/1/2/3
  GEAR_TYPE gear; // 0(P)/1(R)/2(N)/3(D)
};

struct EngineStatus{
  float velocity; // 엔진 속도
  float accel; // 가속도
  float fuel; // 잔여 연료량
};

struct TripInfo{
  float mileage; // 총 주행 거리
  int driveTime; // 총 주행 시간
  int fuelEconomy; // 차량 연비
  int instSpeed; // 순간 속도
  float avgSpeed; // 평균 속도
};
