#include <Arduino.h>

#include <WiFi.h>
#include <TaskScheduler.h>

#include "actuationTasks.h"
#include "pinOut.h" // Check and change before use!
#include "projectSettings.h" // Shh, secrets live here, make a new one or replace secrets in code for local tests

#include "connectionWifi.h" // Wifi connection module
#include "motorDrivers.h" // Motor drivers and tests

Scheduler mainScheduler;

Task tDriveForward(0, 1,&driveForward, &mainScheduler);
Task tDriveBackward(0, 1, &driveBackward, &mainScheduler);
Task tDriveFlankLeftFor(0, 1, &driveFlankLeftFor, &mainScheduler);
Task tDriveFlankLeftBac(0, 1, &driveFlankLeftBac, &mainScheduler);
Task tDriveFlankRightBac(0, 1,&driveFlankRightBac, &mainScheduler);
Task tDriveFlankRightFor(0, 1,&driveFlankRightFor, &mainScheduler);
Task tDriveStop(0, 1,&driveStop, &mainScheduler);

Task tTurnLeft(0, 1,&turnLeft, &mainScheduler);
Task tTurnRight(0, 1,&turnRight, &mainScheduler);
Task tTurnStop(0, 1,&turnStop, &mainScheduler);

Task tPrepareKick(0, 1,&prepareKick, &mainScheduler);
Task tKick(0, 1,&kick, &mainScheduler);

Task tPumpActuateIn(0, 1,&pumpActuateIn, &mainScheduler);
Task tPumpActuateOut(0, 1,&pumpActuateOut, &mainScheduler);
Task tPumpStop(0, 1,&pumpActuateStop, &mainScheduler);

int status = WL_IDLE_STATUS;

char ssid[] = TESTSSID;        // your network SSID (name)
char pass[] = TESTPASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key index number (needed only for WEP)

const unsigned int listeningPort = LOCALPORT;      // local port to listen on

u_int8_t packetBuffer[256]; //buffer to hold incoming packet
char  replyBuffer[] = "acknowledged";       // a string to send back
char testPacket[256] = "motorTest";

unsigned long previousTime = millis();
unsigned long currentTime = millis();

int lastPacketID = 0;

int turnInitial;
int goDistance;
int turnLate;
int collectorOnQ;
int shootKickerAtEnd;
int gameState;

CommandPacket packet;

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
  mainScheduler.init();
  mainScheduler.addTask(tDriveForward);
  mainScheduler.addTask(tDriveBackward);
  mainScheduler.addTask(tDriveFlankLeftFor);
  mainScheduler.addTask(tDriveFlankLeftBac);
  mainScheduler.addTask(tDriveFlankRightFor);
  mainScheduler.addTask(tDriveFlankRightBac);
  mainScheduler.addTask(tDriveStop);

  mainScheduler.addTask(tTurnLeft);
  mainScheduler.addTask(tTurnRight);
  mainScheduler.addTask(tTurnStop);

  mainScheduler.addTask(tPrepareKick);
  mainScheduler.addTask(tKick);

  mainScheduler.addTask(tPumpActuateIn);
  mainScheduler.addTask(tPumpActuateOut);
  mainScheduler.addTask(tPumpStop);
}

void loop() {
  currentTime = millis();
  // if there's data available, read a packet
  checkPackets(packetBuffer, currentTime, lastPacketID);
  memcpy(&packet, packetBuffer, sizeof(packet));
  if (packet.robot_id != ROBOTID) return;
  if (packet.packetID == lastPacketID) return;
  lastPacketID = packet.packetID;
  turnInitial = packet.angle1;
  goDistance = packet.distance;
  turnLate = packet.angle2;
  collectorOnQ = packet.collectorOnQ;
  shootKickerAtEnd = packet.shootKickerAtEnd;
  gameState = packet.gameState;

  if (gameState == 3) {
    mainScheduler.disableAll();
    tDriveStop.enable();
    return;
  }

  if (turnInitial) {
    if (!tDriveStop.isEnabled() and !tTurnStop.isEnabled()) {
      if (turnInitial > 0) {
        tTurnRight.enable();
      } else {
        tTurnLeft.enable();
      }
    tTurnStop.enableDelayed(turnInitial/(TURN_SPEED/1000));
    }
  }

  if (goDistance) {
    if (!tDriveStop.isEnabled() and !tTurnStop.isEnabled()) {
      if (goDistance > 0) {
        tDriveForward.enable();
      } else {
        tDriveBackward.enable();
      }
      tDriveStop.enableDelayed(goDistance/(ACTUAL_SPEED/1000));
    }
  }

  if (turnLate) {
    if (!tDriveStop.isEnabled() and !tTurnStop.isEnabled()) {
      if (turnInitial > 0) {
        tTurnRight.enable();
      } else {
        tTurnLeft.enable();
      }
      tTurnStop.enableDelayed(turnInitial/(TURN_SPEED/1000));
    }
  }

}



