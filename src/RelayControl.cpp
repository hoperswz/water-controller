#include "RelayControl.h"
#include <esp32-hal-gpio.h>
#include <HardwareSerial.h>

#define RELAY_A 27 // 吸水泵针脚
#define RELAY_B 26 // 排水泵针脚

void setupRelay()
{
  pinMode(RELAY_A, OUTPUT);
  pinMode(RELAY_B, OUTPUT);
}

// 吸水
void pumpForward()
{
  Serial.println(">> 开始吸水");
  digitalWrite(RELAY_A, HIGH);
}

// 排水
void pumpReverse()
{
  Serial.println(">> 开始排水");
  digitalWrite(RELAY_B, HIGH);
}

// 停止排水
void pumpStopDrain()
{
  Serial.println(">> 停止排水");
  digitalWrite(RELAY_B, LOW);
}

// 停止吸水
void pumpStopFill()
{
  Serial.println(">> 停止吸水");
  digitalWrite(RELAY_A, LOW);
}