//
// Created by HP Envy on 02/03/2026.
//

#include <TaskScheduler.h>
#include "motorDrivers.h" // Motor drivers and tests

#include "actuationTasks.h"

void motorRunTask(int distance, int speed, int actualSpeed) {

}

void motorTurnTask(int angle, int firstQ, int turnSpeed) {
    int turningTime = (angle/turnSpeed)*1000;

}

void motorTestTask() {

}