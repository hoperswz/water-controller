#include "MqttHelper.h"
#include <PubSubClient.h>
#include <WiFi.h>
#include "RelayControl.h" // 控制继电器模块
#include "Settings.h"

WiFiClient espClient;
PubSubClient client(espClient);

// 订阅回调函数
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("收到控制消息：");
  String message;
  for (unsigned int i = 0; i < length; i++)
  {
    message += (char)payload[i];
  }
  Serial.println(message);
  Serial.println(topic);

  if (String(topic) == "/esp32/control-drain")
  {
    if (message == "on")
    {
      pumpReverse();
      Serial.println("开始排水 ");
      publishPumpState("/esp32/control-drain/state", true);
    }
    else
    {
      pumpStopDrain();
      publishPumpState("/esp32/control-drain/state", false);
      Serial.println("停止 ");
    }
  }

  if (String(topic) == "/esp32/control-fill")
  {
    if (message == "on")
    {
      pumpForward();
      publishPumpState("/esp32/control-fill/state", true);
      Serial.println("开始吸水 ");
    }
    else
    {
      pumpStopFill();
      publishPumpState("/esp32/control-fill/state", false);
      Serial.println("停止 ");
    }
  }
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("连接 MQTT 中...");
    if (client.connect("esp32-water"))
    {
      Serial.print("等待订阅...");

      delay(2000);

      Serial.println("开始订阅...");

      client.subscribe("/esp32/control");
      client.subscribe("/esp32/control-drain");
      client.subscribe("/esp32/control-fill");

      const char *water_config = R"rawliteral({
        "name": "水位传感器",
        "state_topic": "/esp32/waterLevel",
        "unit_of_measurement": "cm",
        "device_class": "distance",
                 "device": {
          "identifiers": ["esp32_water_system"],
          "name": "ESP32自动换水系统",
          "manufacturer": "DIY",
          "model": "ESP32-WaterV1"
        },
        "unique_id": "esp32_water_sensor_v1"
      })rawliteral";


      // 水位传感器注册
      bool o = client.publish("homeassistant/sensor/esp32_water_level/config", water_config, true);

      Serial.println(o ? "水位传感器注册成功" : "水位传感器注册失败");

      // 水泵排水
      bool o1 = client.publish("homeassistant/switch/esp32_control_drain/config", R"rawliteral({
        "name": "鱼缸水泵-排水",
        "command_topic": "/esp32/control-drain",
        "payload_on": "on",
        "payload_off": "off",
        "state_topic": "/esp32/control-drain/state",
                 "device": {
          "identifiers": ["esp32_water_system"],
          "name": "ESP32自动换水系统",
          "manufacturer": "DIY",
          "model": "ESP32-WaterV1"
        },
        "unique_id": "esp32_pump_switch_drain_v3"
      })rawliteral",
                               true);

      Serial.println(o1 ? "水泵排水注册成功" : "水泵排水注册失败");

      // 水泵吸水
      bool o2 = client.publish("homeassistant/switch/esp32_control_fill/config", R"rawliteral({
        "name": "鱼缸水泵-吸水",
        "command_topic": "/esp32/control-fill",
        "payload_on": "on",
        "payload_off": "off",
        "state_topic": "/esp32/control-fill/state",
         "device": {
          "identifiers": ["esp32_water_system"],
          "name": "ESP32自动换水系统",
          "manufacturer": "DIY",
          "model": "ESP32-WaterV1"
        },
        "unique_id": "esp32_pump_switch_fill_v3"
      })rawliteral",
                               true);
      Serial.println(o2 ? "水泵吸水注册成功" : "水泵吸水注册失败");
    }
    else
    {
      delay(3000);
    }
  }
}

void setupMqtt()
{
  client.setServer(config.mqtt_server, config.mqtt_port);
  client.setCallback(callback); // 设置消息回调
  client.setBufferSize(1024); // 设置消息体大小, 必须设置
}

void loopMqtt()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}

void publishWaterLevel(int cm)
{
  if (!client.connected())
    reconnect();
  client.loop();
  char buf[10];
  sprintf(buf, "%d", cm);
  client.publish("/esp32/waterLevel", buf);
}

void publishPumpState(const char *topic, bool on)
{
  if (!client.connected())
    reconnect();
  client.loop();
  client.publish(topic, on ? "on" : "off", true);
}
