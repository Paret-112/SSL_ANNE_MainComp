//
// Created by HP Envy on 02/03/2026.
//

#include <TaskManagerIO.h>
#include "motorDrivers.h" // Motor drivers and tests

#include "actuationTasks.h"

void motorRunTasks(int distance, int speed, int actualSpeed) {
    taskManager.scheduleOnce(50, [speed] {
        motorRun(0, speed);
    });
    actualSpeed = 100; // = 100 mm/s, must be changed!!! TODO:Remove and add to definitions in settings!
    int timeOfStop = (distance/actualSpeed)*1000;
    taskManager.scheduleOnce(timeOfStop, [] {
        motorStop();
    });
}

void motorTurnTask(int angle, int firstQ, int turnSpeed) {
    turnSpeed = 180; // 180 degrees/s, must be changed!! TODO:Remove and add to definitions in settings!
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