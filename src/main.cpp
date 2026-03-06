#include <Arduino.h>

#define _TASK_SCHEDULING_OPTIONS

#include <WiFi.h>
#include "LightTaskScheduler.h"

#include "actuationTasks.h"
#include "pinOut.h" // Check and change before use!
#include "projectSettings.h" // Shh, secrets live here, make a new one or replace secrets in code for local tests

#include "connectionWifi.h" // Wifi connection module
#include "motorDrivers.h" // Motor drivers and tests

void gameStatus();
void networkCheck();

TaskScheduler runner;

Task tCheckNetwork(&networkCheck, 100, false, 7);
Task tGameStatus(&gameStatus, 200, false, 6);

Task tDriveForward(&driveForward, 10, true, 3);
Task tDriveBackward(&driveBackward, 10, true, 3);
Task tDriveFlankLeftFor(&driveFlankLeftFor, 10, true, 3);
Task tDriveFlankLeftBac(&driveFlankLeftBac, 10, true, 3);
Task tDriveFlankRightBac(&driveFlankRightBac, 10, true, 3);
Task tDriveFlankRightFor(&driveFlankRightFor, 10, true, 3);

Task tTurnLeft(&turnLeft, 10, true, 3);
Task tTurnRight(&turnLeft, 10, true, 3);

Task tAllStop(&allStop, 10, true, 7);

Task tPrepareKick(&prepareKick, 10, true, 4);
Task tKick(&kick, 10, true, 4);

Task tPumpActuateIn(&pumpActuateInCB, 10, true, 4);
Task tPumpActuateOut(&pumpActuateOutCB, 10, true, 4);
Task tPumpActuateStop(&pumpActuateStopCB, 10, true, 4);

int status = WL_IDLE_STATUS;

u_int8_t packetBuffer[256]; //buffer to hold incoming packet

int lastPacketID = 0;

int turnInitial;
int goDistance;
int turnLate;
int collectorOnQ;
int shootKickerAtEnd;
int gameState = 0;

bool motorBusy = false;
int robotState = 0;

unsigned long currentTime = millis();

CommandPacket packet;

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
  runner.addTask(&tGameStatus);
}

void loop() {
  currentTime = millis();
  // networkCheck();
  // gameStatus();
  runner.run();
}

void runStatus() {
  if (motorBusy == false) {
    if (turnInitial != 0 and robotState == 0) {
      if (turnInitial < 0) {

      }
    }
  }
}

void gameStatus() {
  Serial.println("Game status checking");
  if (gameState == 3) {
    tAllStop.run(currentTime);
    globalSpeedModSet(255);
    Serial.println("Game status: Game stopped");
    return;
  }
  Serial.println("Game status: Game going!");

}

void networkCheck() {
  Serial.println("Network check");
  currentTime = millis();
  // if there's data available, read a packet
  checkPackets(packetBuffer, currentTime, lastPacketID);
  memcpy(&packet, packetBuffer, sizeof(packet));
  if (packet.robot_id != ROBOTID) return;
  Serial.println("Robot ID checked");
  lastPacketID = packet.packetID;
  turnInitial = packet.angle1;
  goDistance = packet.distance;
  turnLate = packet.angle2;
  collectorOnQ = packet.collectorOnQ;
  shootKickerAtEnd = packet.shootKickerAtEnd;
  gameState = packet.gameState;
}
