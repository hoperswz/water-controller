#ifndef MQTT_HELPER_H
#define MQTT_HELPER_H

void setupMqtt();
void loopMqtt();
void publishWaterLevel(int cm);
// void publishPumpState(const char *topic, bool on);
void publishPumpState(const char *topic, bool on);

#endif
