//
// Created by HP Envy on 02/03/2026.
//

#include "motorDrivers.h" // Motor drivers

#include "actuationTasks.h"

int turnDirection;
int globalSpeedMod;



void driveForward() {
    motorRun(0, 255/globalSpeedMod);
}

void driveBackward() {
    motorRun(1, 255/globalSpeedMod);
}

void driveFlankRightFor() {
    motorRun(2, 255/globalSpeedMod);
}

void driveFlankRightBac() {
    motorRun(3, 255/globalSpeedMod);
}

void driveFlankLeftFor() {
    motorRun(4, 255/globalSpeedMod);
}

void driveFlankLeftBac() {
    motorRun(5, 255/globalSpeedMod);
}

void driveStop() {
    motorStop();
}

void turnRight() {
    motorTurn(1);
}
void turnLeft() {
    motorTurn(-1);
}

void turnStop() {
    motorStop();
}

void kick() {
    motorKick();
}

void prepareKick() {
    motorKickPrepare();
}