
#include "FastIMU.h"
#include <Wire.h>
#include "pinOut.h"

#define IMU_ADDRESS 0x68
MPU6500 IMU;

calData calib = { 0 };  //Calibration data
GyroData gyroData;

void setupGyro()
{
    Wire.begin();
    Wire.setClock(400000); //400khz clock
    Serial.begin(115200);
    while (!Serial) {
        ;
    }

    int err = IMU.init(calib, IMU_ADDRESS);
    if (err != 0) {

        Serial.println("GYRO NOT FOUND!.");

    } else {

        Serial.println("Keep robot level.");
        delay(5000);
        IMU.calibrateAccelGyro(&calib);
        Serial.println("Calibration done!");
        IMU.setGyroRange(500);      //USE THESE TO SET THE RANGE, IF AN INVALID RANGE IS SET IT WILL RETURN -1
        IMU.setAccelRange(2);
    }
}

int currentRotation = 0;

int getGyroRotation(bool Zero){
    IMU.update();
    IMU.getGyro(&gyroData);

    if (Zero){
        currentRotation = 0;
    }

    currentRotation += round(gyroData.gyroZ / 11.11);

    return currentRotation;
}

int getGyroTemp(){
    if (IMU.hasTemperature())
    {
        return IMU.getTemp();
    }

    return 0;
}

int getVoltage(){
    return map(analogRead(PIN_VOLTAGE),0,1024,0,25);
}