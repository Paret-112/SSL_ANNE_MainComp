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

    digitalWrite(PIN_MOTDR_R1_BAC, LOW);
    digitalWrite(PIN_MOTDR_R2_BAC, LOW);
    digitalWrite(PIN_MOTDR_L1_BAC, LOW);
    digitalWrite(PIN_MOTDR_L2_BAC, LOW);
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

void motorTurn(int direction) {
    analogWrite(PIN_MOTDR_L1_SPD, 128);
    analogWrite(PIN_MOTDR_L2_SPD, 128);
    analogWrite(PIN_MOTDR_R1_SPD, 128);
    analogWrite(PIN_MOTDR_R2_SPD, 128);

    if (direction == -1) {
        digitalWrite(PIN_MOTDR_L1_FOR, HIGH);
        digitalWrite(PIN_MOTDR_L2_FOR, HIGH);
        digitalWrite(PIN_MOTDR_R1_BAC, HIGH);
        digitalWrite(PIN_MOTDR_R2_BAC, HIGH);
    }
    if (direction == 1) {
        digitalWrite(PIN_MOTDR_L1_BAC, HIGH);
        digitalWrite(PIN_MOTDR_L2_BAC, HIGH);
        digitalWrite(PIN_MOTDR_R1_FOR, HIGH);
        digitalWrite(PIN_MOTDR_R2_FOR, HIGH);
    }
}

void motorKick() {
    digitalWrite(PIN_MOTKICK_FOR, HIGH);
    analogWrite(PIN_MOTKICK_SPD, 255);
    digitalWrite(PIN_MOTKICK_BAC, HIGH);
}

void motorKickPrepare() {
    analogWrite(PIN_MOTKICK_SPD, 255);
    digitalWrite(PIN_MOTKICK_FOR, HIGH);
    digitalWrite(PIN_MOTKICK_BAC, LOW);

}

void pumpActuateIn() {
    digitalWrite(PIN_MOTPUMP_IN, HIGH);
    analogWrite(PIN_MOTPUMP_SPD, 255);
    digitalWrite(PIN_MOTPUMP_OUT, LOW);
}

void pumpActuateOut() {
    digitalWrite(PIN_MOTPUMP_OUT, HIGH);
    analogWrite(PIN_MOTPUMP_SPD, 255);
    digitalWrite(PIN_MOTPUMP_IN, LOW);
}

void pumpActuateStop() {
    digitalWrite(PIN_MOTPUMP_OUT, LOW);
    digitalWrite(PIN_MOTPUMP_IN, LOW);
    analogWrite(PIN_MOTPUMP_SPD, 0);
}