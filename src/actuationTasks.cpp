//
// Created by HP Envy on 02/03/2026.
//

#include "motorDrivers.h" // Motor drivers

#include "actuationTasks.h"

int turnDirection;



void driveForward() {
    motorRun(0, 255);
}

void driveBackward() {
    motorRun(1, 255);
}

void driveFlankRightFor() {
    motorRun(2, 255);
}

void driveFlankRightBac() {
    motorRun(3, 255);
}

void driveFlankLeftFor() {
    motorRun(4, 255);
}

void driveFlankLeftBac() {
    motorRun(5, 255);
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