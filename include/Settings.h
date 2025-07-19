#include <cstdint>
#ifndef SETTINGS_H
#define SETTINGS_H

struct Config
{
  const char *wifi_ssid;
  const char *wifi_password;
  const char *mqtt_server;
  uint16_t mqtt_port;
};

extern Config config;

#endif
