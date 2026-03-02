#include <Arduino.h>

#include <WiFi.h>
#include <TaskManagerIO.h>

#include "pinOut.h" // Check and change before use!
#include "projectSettings.h" // Shh, secrets live here, make a new one or replace secrets in code for local tests

#include "connectionWifi.h" // Wifi connection module
#include "motorDrivers.h" // Motor drivers and tests

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

  wifiInitialization(listeningPort, status, ssid, pass);

  // Create a task that's scheduled every second
  taskManager.schedule(repeatMillis(100), [] {
    checkPackets(packetBuffer, currentTime);
  });
  taskManager.schedule(repeatSeconds(1), [] {

  });

}

void loop() {
  currentTime = millis();
  taskManager.runLoop();
  // if there's data available, read a packet


  // Set motor speed factor.
  analogWrite(PIN_MOTDR_L1_SPD, 128);
  analogWrite(PIN_MOTDR_L2_SPD, 128);
  analogWrite(PIN_MOTDR_R1_SPD, 128);
  analogWrite(PIN_MOTDR_R2_SPD, 128);
}



