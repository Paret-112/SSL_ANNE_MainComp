//
// Created by HP Envy on 22/02/2026.
//

#include <Arduino.h>

#ifndef SSL_ANNE_MAIN_CONNECTIONWIFI_H
#define SSL_ANNE_MAIN_CONNECTIONWIFI_H

#define MSG_DISCOVER 1
#define MSG_DISCOVER_REPLY 2
#define MSG_COMMAND 3


void printWifiStatus();
void wifiInitialization(unsigned int, int, char[], char[]);

struct __attribute__((packed)) CommandPacket {
    uint8_t msg_type;
    uint8_t robot_id;
    int angle1;
    int distance;
    int angle2;
    int collectorOnQ;
    int shootKickerAtEnd;
    int gameState;
    int packetID;
};

int checkPackets(uint8_t, unsigned long);

void checkPackets();
void connectClient();

void mqttClientPoll(int);
void mqttClientPublish();

void interpreter(char, String);


#endif //SSL_ANNE_MAIN_CONNECTIONWIFI_H