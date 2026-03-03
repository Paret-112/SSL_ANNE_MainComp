//
// Created by HP Envy on 22/02/2026.
//

#include <Arduino.h>

#ifndef SSL_ANNE_MAIN_CONNECTIONWIFI_H
#define SSL_ANNE_MAIN_CONNECTIONWIFI_H

#define MSG_DISCOVER 1
#define MSG_DISCOVER_REPLY 2
#define MSG_COMMAND 3

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

    void reset() {
        uint8_t msg_type = 0;
        uint8_t robot_id = 0;
        angle1 = 0;
        distance = 0;
        angle2 = 0;
        collectorOnQ = 0;
    }

};

void printWifiStatus();
void wifiInitialization(unsigned int, int, char[], char[]);
void checkPackets(u_int8_t[], unsigned long, int);


#endif //SSL_ANNE_MAIN_CONNECTIONWIFI_H