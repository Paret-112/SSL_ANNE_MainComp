//
// Created by HP Envy on 22/02/2026.
//

#ifndef SSL_ANNE_MAIN_PROJECTSECRETS_H
#define SSL_ANNE_MAIN_PROJECTSECRETS_H

#endif //SSL_ANNE_MAIN_PROJECTSECRETS_H

#define TESTSSID "Paret"
#define TESTPASS "Insurance"
#define LOCALPORT 1234

#define CLIENT_PORT 50123

#define ROBOTID 7

#define SERIAL_BAUD 115200

#define TURN_SPEED 100
#define ACTUAL_SPEED 100

// #define _CLIENT_MODE
// #define _UDP_MODE
#define _MQTT_MODE

#ifdef _MQTT_MODE
#define MQTT_SERVER "192.168.143.222"
#define MQTT_PORT 1883
#define MQTT_ROBOTREAD "robotAnne/Read"
#define MQTT_ROBOTWRITE "robotAnne/Write"
#endif


/***
 *  gameState 7 is the test state for configuring motor directions and other factors.
 *
 *  To run upload code onto Arduino Giga R1 Wi-fi
 *  using the nrg-Vision send command with gameState and other directives.
 *
 *  This whole thing is a mess in a beautiful way and I have no idea how we fare
 *  Next time I'm doing something like this, I'll start way earlier than two months
 *  - Kristjan
 *
 *  Project Lead/Manager:
 *  Andre Nurkma
 *
 *  Code Authors:
 *  Kristjan-Peeter Ivask (Paret-112)
 *  Mattias Mäits (MattNRG)
 *
 *  Engineers:
 *  Siim Neštšadim
 *  Mathias Järveoja
 *
 *
 *  # Notes for next time!!
 *  Double batteris, one for logic and second for motors
 *
 *
***/