#include "WiFiHelper.h"
#include <WiFi.h>
#include "Settings.h"

void initWiFi()
{
  WiFi.begin(config.wifi_ssid, config.wifi_password);
  Serial.print("连接 WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi 已连接，IP地址: " + WiFi.localIP().toString());
}
