#include "WaterSensor.h"
#include <esp32-hal-gpio.h>
#include <HardwareSerial.h>

const int waterSensorPin = 32; // 水位传感器针脚

void initWaterSensor()
{
  pinMode(waterSensorPin, INPUT);
}

int readWaterLevel()
{
  int val = analogRead(waterSensorPin);
  Serial.print("当前水位值: ");
  Serial.println(val);
  return val;
}
