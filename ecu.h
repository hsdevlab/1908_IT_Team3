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
  int velocity; // 엔진 속도
  int accel; // 가속도
  int fuel; // 잔여 연료량
};
