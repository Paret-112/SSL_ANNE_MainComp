//
// Created by HP Envy on 02/03/2026.
//

#include "motorDrivers.h" // Motor drivers

#include "actuationTasks.h"

#include <Arduino.h>

int globalSpeedMod = 0;

void globalSpeedModSet(int set) {
    globalSpeedMod = set;
}

void driveForward() {
    motorRun(0, 255 - globalSpeedMod);
    Serial.println("Forward!!");
}

void driveBackward() {
    motorRun(1, 255 - globalSpeedMod);
    Serial.println("BACK, BACK!!!");
}

void driveFlankRightFor() {
    motorRun(2, 255 - globalSpeedMod);
}

void driveFlankRightBac() {
    motorRun(3, 255 - globalSpeedMod);
}

void driveFlankLeftFor() {
    motorRun(4, 255 - globalSpeedMod);
}

void driveFlankLeftBac() {
    motorRun(5, 255 - globalSpeedMod);
}

void allStop() {
    motorStop();
    Serial.println("Stoping");
}

void turnRight() {
    motorTurn(1, 128 - globalSpeedMod);
    Serial.println("Turning Right");
}
void turnLeft() {
    motorTurn(-1, 128 - globalSpeedMod);
    Serial.println("Turning Left");
}

void kick() {
    motorKick();
    pumpActuateOut();
    delay(100);
    pumpActuateStop();
    Serial.println("SCOOOOOORRREEEE!!");
}

void prepareKick() {
    motorKickPrepare();
    Serial.println("And he readies his foot!");
}

void pumpActuateInCB() {
    pumpActuateIn();
}
void pumpActuateOutCB() {
    pumpActuateOut();
}
void pumpActuateStopCB() {
    pumpActuateStop();
}