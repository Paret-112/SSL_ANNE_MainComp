#include <Arduino.h>

#include <WiFi.h>
#include <TaskScheduler.h>

#include "actuationTasks.h"
#include "pinOut.h" // Check and change before use!
#include "projectSettings.h" // Shh, secrets live here, make a new one or replace secrets in code for local tests

#include "connectionWifi.h" // Wifi connection module
#include "motorDrivers.h" // Motor drivers and tests

Task tDriveForward(0, 1,&driveForward);
Task tDriveBackward(0, 1, &driveBackward);
Task tDriveFlankLeftFor(0, 1, &driveFlankLeftFor);
Task tDriveFlankLeftBac(0, 1, &driveFlankLeftBac);
Task tDriveFlankRightBac(0, 1,&driveFlankRightBac);
Task tDriveFlankRightFor(0, 1,&driveFlankRightFor);
Task tDriveStop(0, 1,&driveStop);

Task tTurnLeft(0, 1,&turnLeft);
Task tTurnRight(0, 1,&turnRight);
Task tTurnStop(0, 1,&turnStop);

Task tPrepareKick(0, 1,&prepareKick);
Task tKick(0, 1,&kick);

Task tPumpActuateIn(0, 1,&pumpActuateIn);
Task tPumpActuateOut(0, 1,&pumpActuateOut);
Task tPumpStop(0, 1,&pumpActuateStop);

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

Scheduler mainScheduler;

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



}

void loop() {
  currentTime = millis();
  // if there's data available, read a packet
  checkPackets(packetBuffer, currentTime, lastPacketID);
  memcpy(&packet, packetBuffer, sizeof(packet));
  if (packet.robot_id == ROBOTID) {
    if (packet.packetID == lastPacketID) return;

  }
  // Set motor speed factor.
  analogWrite(PIN_MOTDR_L1_SPD, 128);
  analogWrite(PIN_MOTDR_L2_SPD, 128);
  analogWrite(PIN_MOTDR_R1_SPD, 128);
  analogWrite(PIN_MOTDR_R2_SPD, 128);
}



