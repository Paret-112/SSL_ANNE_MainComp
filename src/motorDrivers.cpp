//
// Created by HP Envy on 22/02/2026.
//

#include "motorDrivers.h"

#include <arduino.h>

#include "pinOut.h"

void motorStop() {
    analogWrite(PIN_MOTDR_L1_SPD, 0);
    analogWrite(PIN_MOTDR_L2_SPD, 0);
    analogWrite(PIN_MOTDR_R1_SPD, 0);
    analogWrite(PIN_MOTDR_R2_SPD, 0);

    digitalWrite(PIN_MOTDR_R1_FOR, LOW);
    digitalWrite(PIN_MOTDR_R2_FOR, LOW);
    digitalWrite(PIN_MOTDR_L1_FOR, LOW);
    digitalWrite(PIN_MOTDR_L2_FOR, LOW);
}

void motorRun(int direction, unsigned int speed) {
    if (direction == 0) { // FORWARDS!!
        analogWrite(PIN_MOTDR_L1_SPD, speed);
        analogWrite(PIN_MOTDR_L2_SPD, speed);
        analogWrite(PIN_MOTDR_R1_SPD, speed);
        analogWrite(PIN_MOTDR_R2_SPD, speed);

        digitalWrite(PIN_MOTDR_R1_FOR, HIGH);
        digitalWrite(PIN_MOTDR_R2_FOR, HIGH);
        digitalWrite(PIN_MOTDR_L1_FOR, HIGH);
        digitalWrite(PIN_MOTDR_L2_FOR, HIGH);
        return;
    }
    if (direction == 1) { // RETREAT!!
        analogWrite(PIN_MOTDR_L1_SPD, speed);
        analogWrite(PIN_MOTDR_L2_SPD, speed);
        analogWrite(PIN_MOTDR_R1_SPD, speed);
        analogWrite(PIN_MOTDR_R2_SPD, speed);

        digitalWrite(PIN_MOTDR_R1_BAC, HIGH);
        digitalWrite(PIN_MOTDR_R2_BAC, HIGH);
        digitalWrite(PIN_MOTDR_L1_BAC, HIGH);
        digitalWrite(PIN_MOTDR_L2_BAC, HIGH);
        return;
    }
    if (direction == 2) { // FLANK RIGHT
        analogWrite(PIN_MOTDR_L1_SPD, speed);
        analogWrite(PIN_MOTDR_R2_SPD, speed);

        digitalWrite(PIN_MOTDR_R1_FOR, HIGH);
        digitalWrite(PIN_MOTDR_L2_FOR, HIGH);
        return;
    }
    if (direction == 3) { // BACK RIGHT
        analogWrite(PIN_MOTDR_L2_SPD, speed);
        analogWrite(PIN_MOTDR_R1_SPD, speed);

        digitalWrite(PIN_MOTDR_R2_BAC, HIGH);
        digitalWrite(PIN_MOTDR_L1_BAC, HIGH);
        return;
    }
    if (direction == 4) { // FLANK LEFT
        analogWrite(PIN_MOTDR_L2_SPD, speed);
        analogWrite(PIN_MOTDR_R1_SPD, speed);

        digitalWrite(PIN_MOTDR_R2_FOR, HIGH);
        digitalWrite(PIN_MOTDR_L1_FOR, HIGH);
        return;
    }
    if (direction == 5) { // BACK LEFT
        analogWrite(PIN_MOTDR_L1_SPD, speed);
        analogWrite(PIN_MOTDR_R2_SPD, speed);

        digitalWrite(PIN_MOTDR_R1_BAC, HIGH);
        digitalWrite(PIN_MOTDR_L2_BAC, HIGH);
    }
    else {
        motorStop();
    }
}

void motorTurn(int degrees) {
    analogWrite(PIN_MOTDR_L1_SPD, 128);
    analogWrite(PIN_MOTDR_L2_SPD, 128);
    analogWrite(PIN_MOTDR_R1_SPD, 128);
    analogWrite(PIN_MOTDR_R2_SPD, 128);

    if (degrees > 0) {
        digitalWrite(PIN_MOTDR_L1_FOR, HIGH);
        digitalWrite(PIN_MOTDR_L2_FOR, HIGH);
        digitalWrite(PIN_MOTDR_R1_BAC, HIGH);
        digitalWrite(PIN_MOTDR_R2_BAC, HIGH);
    }
    if (degrees < 0) {
        digitalWrite(PIN_MOTDR_L1_BAC, HIGH);
        digitalWrite(PIN_MOTDR_L2_BAC, HIGH);
        digitalWrite(PIN_MOTDR_R1_FOR, HIGH);
        digitalWrite(PIN_MOTDR_R2_FOR, HIGH);
    }
}

void motorKick() {

}

void motorKickPrepare() {

}

void pumpActuateIn() {

}

void pumpActuateOut() {

}

void pumpActuateStop() {

}