#include <Arduino.h>

#include <WiFi.h>
#include "LightTaskScheduler.h"

#include "actuationTasks.h"
#include "pinOut.h" // Check and change before use!
#include "projectSettings.h" // Shh, secrets live here, make a new one or replace secrets in code for local tests

#include "connectionWifi.h" // Wifi connection module
#include "motorDrivers.h" // Motor drivers and tests

void gameStatus();
void networkCheck();
void runStatus();
void motorReset();

void resetCall(int, int);

TaskScheduler runner;

Task tCheckNetwork(&networkCheck, 100, false, 7);
// Task tGameStatus(&gameStatus, 200, false, 6);
// Task tRobotStatus(&runStatus, 50, false, 6);
// Task tMotorReset(&motorReset, 100, true, 6);

// Task tAllStop(&allStop, 10, true, 7);
//
// Task tPrepareKick(&prepareKick, 10, true, 4);
// Task tKick(&kick, 10, true, 4);
//
// Task tPumpActuateIn(&pumpActuateInCB, 10, true, 4);
// Task tPumpActuateOut(&pumpActuateOutCB, 10, true, 4);
// Task tPumpActuateStop(&pumpActuateStopCB, 10, true, 4);

int status = WL_IDLE_STATUS;

u_int8_t packetBuffer[256]; //buffer to hold incoming packet

bool motorBusy = false;
int robotState = 4;

unsigned long currentTime = millis();

#ifdef _UDP_MODE
CommandPacket packet;
CommandPacket currentPacket;
#endif

char ssid[] = TESTSSID;        // your network SSID (name)
char pass[] = TESTPASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key index number (needed only for WEP)

const unsigned int listeningPort = LOCALPORT;      // local port to listen on

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_MOTDR_L1_SPD, OUTPUT);
  pinMode(PIN_MOTDR_L1_FOR, OUTPUT);
  pinMode(PIN_MOTDR_L1_BAC, OUTPUT);
  pinMode(PIN_MOTDR_L2_SPD, OUTPUT);
  pinMode(PIN_MOTDR_L2_FOR, OUTPUT);
  pinMode(PIN_MOTDR_L2_BAC, OUTPUT);

  pinMode(PIN_MOTDR_R1_SPD, OUTPUT);
  pinMode(PIN_MOTDR_R1_FOR, OUTPUT);
  pinMode(PIN_MOTDR_R1_BAC, OUTPUT);
  pinMode(PIN_MOTDR_R2_SPD, OUTPUT);
  pinMode(PIN_MOTDR_R2_FOR, OUTPUT);
  pinMode(PIN_MOTDR_R2_BAC, OUTPUT);

  pinMode(PIN_MOTPUMP_SPD, OUTPUT);
  pinMode(PIN_MOTPUMP_IN, OUTPUT);
  pinMode(PIN_MOTPUMP_OUT, OUTPUT);

  pinMode(PIN_MOTKICK_SPD, OUTPUT);
  pinMode(PIN_MOTKICK_FOR, OUTPUT);
  pinMode(PIN_MOTKICK_BAC, OUTPUT);

  wifiInitialization(listeningPort, status, ssid, pass);

  // Main checking packages task
  runner.addTask(&tCheckNetwork);
  // runner.addTask(&tGameStatus);
  //
  // runner.addTask(&tRobotStatus);
  //
  // runner.addTask(&tAllStop);
  // runner.addTask(&tMotorReset);
}

void loop() {
  currentTime = millis();
  runner.run();
}


void networkCheck() {
  Serial.println("Network check");
  currentTime = millis();
  // if there's data available, read a packet
  checkPackets();

#ifdef _UDP_MODE
  memcpy(&packet, packetBuffer, sizeof(packet));
  if (packet.robot_id == ROBOTID and packet.packetID != currentPacket.packetID) {
    Serial.println("New packet!");
    memcpy(&currentPacket, &packet, sizeof(packet));
    robotState = 0;
  }
#endif

}