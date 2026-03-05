#include <Arduino.h>

#define _TASK_SCHEDULING_OPTIONS

#include <WiFi.h>
#include <TaskScheduler.h>

#include "actuationTasks.h"
#include "pinOut.h" // Check and change before use!
#include "projectSettings.h" // Shh, secrets live here, make a new one or replace secrets in code for local tests

#include "connectionWifi.h" // Wifi connection module
#include "motorDrivers.h" // Motor drivers and tests

void gameStatus();
void networkStatus();

Scheduler runner;

// Task tNetworkCheck(100, -1,&networkCheck, &runner, true);
// Task tGameChecker(100, -1, &gameStatus, &runner, true);

Task tTurnLeftFirst(100, 1, &turnLeft, &runner);
Task tTurnRightFirst(100, 1, &turnRight, &runner);

Task tDriveForward(10, 1,&driveForward, &runner);
Task tDriveBackward(10, 1, &driveBackward, &runner);
Task tDriveFlankLeftFor(10, 1, &driveFlankLeftFor, &runner);
Task tDriveFlankLeftBac(10, 1, &driveFlankLeftBac, &runner);
Task tDriveFlankRightBac(10, 1,&driveFlankRightBac, &runner);
Task tDriveFlankRightFor(10, 1,&driveFlankRightFor, &runner);
Task tDriveStop(10, 1,&driveStop, &runner);

Task tTurnLeftLast(10, 1,&turnLeft, &runner);
Task tTurnRightLast(10, 1,&turnRight, &runner);
Task tTurnStop(10, 1,&turnStop, &runner);

Task tPrepareKick(10, 1,&prepareKick, &runner);
Task tKick(10, 1,&kick, &runner);

Task tPumpActuateIn(10, 1,&pumpActuateIn, &runner);
Task tPumpActuateOut(10, 1,&pumpActuateOut, &runner);
Task tPumpStop(10, 1,&pumpActuateStop, &runner);

int status = WL_IDLE_STATUS;

u_int8_t packetBuffer[256]; //buffer to hold incoming packet

int lastPacketID = 0;

int turnInitial;
int goDistance;
int turnLate;
int collectorOnQ;
int shootKickerAtEnd;
int gameState = 0;

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
  runner.init();

  // runner.addTask(tNetworkCheck);
  // runner.addTask(tGameChecker);
  //
  // tGameChecker.setSchedulingOption(TASK_SCHEDULE);
  // tNetworkCheck.setSchedulingOption(TASK_SCHEDULE);

  runner.addTask(tTurnLeftFirst);
  runner.addTask(tTurnRightFirst);

  runner.addTask(tDriveForward);
  runner.addTask(tDriveBackward);
  runner.addTask(tDriveFlankLeftFor);
  runner.addTask(tDriveFlankLeftBac);
  runner.addTask(tDriveFlankRightFor);
  runner.addTask(tDriveFlankRightBac);
  runner.addTask(tDriveStop);

  runner.addTask(tTurnLeftLast);
  runner.addTask(tTurnRightLast);
  runner.addTask(tTurnStop);

  runner.addTask(tPrepareKick);
  runner.addTask(tKick);

  runner.addTask(tPumpActuateIn);
  runner.addTask(tPumpActuateOut);
  runner.addTask(tPumpStop);

  // tNetworkCheck.enable();
  // tGameChecker.enable();
}

void loop() {
  networkCheck();
  gameStatus();
  runner.execute();
}

void gameStatus() {
  Serial.println("Game status checking");
  if (gameState == 3) {
    runner.disableAll();
    globalSpeedModSet(255);
    tDriveStop.enable();
    return;
  }
  Serial.println("Game state checked");

  if (turnInitial) {
    Serial.println("Turn found!");
    if (!tDriveStop.isEnabled() and !tTurnStop.isEnabled()) {
      Serial.println("No other movement tasks found");
      if (turnInitial > 0) {
        tTurnRightFirst.enable();
        Serial.println("Turning 1");
      } else {
        tTurnLeftFirst.enable();
        Serial.println("Turning 2");
      }
      tTurnStop.enableDelayed(turnInitial/(TURN_SPEED/1000));
      Serial.println("I might be the problem");
      //tTurnStop.enableDelayed(turnInitial/(TURN_SPEED/1000));
      //Serial.println("No other movement tasks found");
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
        tTurnRightLast.enable();
      } else {
        tTurnLeftLast.enable();
      }
      tTurnStop.enableDelayed(turnInitial/(TURN_SPEED/1000));
    }
  }
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
