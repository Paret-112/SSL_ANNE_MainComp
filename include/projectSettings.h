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
#define MQTT_ROBOTREAD "robotMaria/Read"
#define MQTT_ROBOTWRITE "robotMaria/Write"
#endif


/*** Welcome to this hole of a repo, introducing the team and some quotes.
 *
 *  To run upload code onto Arduino Giga R1 Wi-Fi, define the mode in which
 *  instructions will be given, and pray to any god you know of.
 *
 *
 *  Project lead/manager:
 *  Andre Nurkma
 *
 *  Project socialmedia Gal
 *  Anita
 *
 *  Code team:
 *  Mattias Mäits (MattNRG)
 *  Kristjan-Peeter Ivask (Paret-112)
 *  Aleksandr Hmelnitski
 *
 *  Assembly team:
 *  Siim Neštšadim
 *  Mathias Järveoja
 *
 *  Team N.R.G mentor:
 *  Kenari Koorik
 *
 *  # Notes for next time!!
 *  Double batteris, one for logic and second for motors
 *
 *  This whole thing is a mess in a beautiful way and I have no idea how we fare
 *  Next time I'm doing something like this, I'll start way earlier than a month.
 *  - Kristjan
 *
 *  We had 1 month to design it, 1 month to build it, and one month to debug it.
 *  And that month was February.
 *  - Probably Siim
 *
 *
***/