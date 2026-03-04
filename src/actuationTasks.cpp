//
// Created by HP Envy on 02/03/2026.
//

#include <TaskManagerIO.h>
#include "motorDrivers.h" // Motor drivers and tests

#include "actuationTasks.h"

void motorRunTask(int distance, int speed, int actualSpeed) {
    taskManager.scheduleOnce(50, [speed] {
        motorRun(0, speed);
    });
    int timeOfStop = (distance/actualSpeed)*1000;
    taskManager.scheduleOnce(timeOfStop, [] {
        motorStop();
    });
}

void motorTurnTask(int angle, int firstQ, int turnSpeed) {
    int turningTime = (angle/turnSpeed)*1000;
    if (firstQ == 0) {
        taskManager.scheduleOnce(2000, [angle] {
            motorTurn(angle);
        });
        taskManager.scheduleOnce(2000 + turningTime, [] {
            motorStop();
        });
    }
    else {
        taskManager.scheduleOnce(10, [angle] {
            motorTurn(angle);
        });
        taskManager.scheduleOnce(10+turningTime, [] {
            motorStop();
        });
    }
}

void motorTestTask() {

}