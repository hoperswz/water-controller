#include "WiFiHelper.h"
#include "MqttHelper.h"
#include "WaterSensor.h"
#include "RelayControl.h"
#include <HardwareSerial.h>

void setup()
{
  Serial.begin(115200);
  initWiFi();
  setupMqtt();
  initWaterSensor();
  setupRelay();
}

void loop()
{
  loopMqtt();

  int level = readWaterLevel();
  publishWaterLevel(level);

  delay(2000); // 每2秒检测一次
}
