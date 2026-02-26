//
// Created by HP Envy on 22/02/2026.
//

#ifndef SSL_ANNE_MAIN_CONNECTIONWIFI_H
#define SSL_ANNE_MAIN_CONNECTIONWIFI_H

struct packedInstructions {
    int direction;
    bool turnToQ;
    int distance;
    bool collectorOnQ;
    bool shootKickerAtEnd;
    int gameState;
};

void printWifiStatus();
void wifiInitialization(unsigned int, int, char[], char[]);
bool checkPackets(char[], char[]);
packedInstructions repackageInstructions(char[]);

#endif //SSL_ANNE_MAIN_CONNECTIONWIFI_H