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
Task tGameStatus(&gameStatus, 200, false, 6);
Task tRobotStatus(&runStatus, 50, false, 6);
Task tMotorReset(&motorReset, 100, true, 6);

Task tAllStop(&allStop, 10, true, 7);

Task tPrepareKick(&prepareKick, 10, true, 4);
Task tKick(&kick, 10, true, 4);

Task tPumpActuateIn(&pumpActuateInCB, 10, true, 4);
Task tPumpActuateOut(&pumpActuateOutCB, 10, true, 4);
Task tPumpActuateStop(&pumpActuateStopCB, 10, true, 4);

int status = WL_IDLE_STATUS;

u_int8_t packetBuffer[256]; //buffer to hold incoming packet

bool motorBusy = false;
int robotState = 4;

unsigned long currentTime = millis();

CommandPacket packet;
CommandPacket currentPacket;

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

  runner.addTask(&tRobotStatus);

  runner.addTask(&tAllStop);
  runner.addTask(&tMotorReset);
}

void loop() {
  currentTime = millis();
  runner.run();
}

void runStatus() {
  Serial.println("runStatus checking");
  Serial.print("motor Status: ");
  Serial.println(motorBusy);
  Serial.print("task line status: ");
  Serial.println(robotState);
  if (robotState == 4) return;
  if (currentPacket.angle1 == 0 and !motorBusy) robotState = 1;
  else {
    motorBusy = true;
    robotState = 1;
    if (currentPacket.angle1 < 0) {
      turnRight();
    }
    else {
      turnLeft();
    }
    Serial.println("runStatus: turning!");
    resetCall(TURN_SPEED, currentPacket.angle1);
  }

  if (currentPacket.distance == 0 and robotState == 1 and !motorBusy) robotState = 2;
  else {
    motorBusy = true;
    robotState = 2;
    if (currentPacket.distance < 0) {
      driveForward();
    }
    else {
      driveBackward();
    }
    resetCall(ACTUAL_SPEED, currentPacket.distance);
    Serial.println("runStatus: moving!");
  }

  if (currentPacket.angle2 == 0 and robotState == 2 and !motorBusy) robotState = 3;
  else {
    motorBusy = true;
    robotState = 3;
    if (currentPacket.angle2 < 0) {
      turnRight();
    }
    else {
      turnLeft();
    }
    resetCall(TURN_SPEED, currentPacket.angle2);
    Serial.println("runStatus: turning2!");
  }
  if (currentPacket.collectorOnQ != 0) {
    pumpActuateInCB();
  }
  if (currentPacket.collectorOnQ == 0 and robotState != 1) {
    pumpActuateStop();
  }
  if (currentPacket.shootKickerAtEnd == 1 and robotState == 3) {
    kick();
    robotState = 4;
  }
  if (currentPacket.shootKickerAtEnd != 1 and robotState == 3) {
    robotState = 4;
  }

}

void gameStatus() {
  Serial.println("Game status checking");
  // Serial.println(currentPacket.packetID);
  // Serial.println(packet.packetID);
  if (currentPacket.gameState == 7) {
    prepareKick();
    pumpActuateIn();
    // driveForward();
    // delay(3000);
    // allStop();
    // turnRight();
    // delay(3000);
    // allStop();
    // turnLeft();
    // delay(6000);
    // allStop();
    // driveBackward();
    // delay(3000);
    // allStop();
    delay(10000);
    pumpActuateStop();
    pumpActuateOut();
  }

  if (currentPacket.gameState == 3) {
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
  checkPackets(packetBuffer, currentTime);
  memcpy(&packet, packetBuffer, sizeof(packet));
  if (packet.robot_id == ROBOTID and packet.packetID != currentPacket.packetID) {
    Serial.println("New packet!");
    memcpy(&currentPacket, &packet, sizeof(packet));
    robotState = 0;
  }
}

void resetCall(int speed, int type) {
  tAllStop.reset(currentTime);
  tAllStop.setInterval(1000 * type/speed);
  tAllStop.resume();
  tMotorReset.reset(currentTime);
  tMotorReset.setInterval(1000 * (25 + type)/speed);
  tMotorReset.resume();
  Serial.println("resetCall: scheduled!");
}

void motorReset() {
  motorBusy = false;
  Serial.println("motorReset concluded");
  tAllStop.pause();
  tMotorReset.pause();
}
