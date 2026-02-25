//
// Created by HP Envy on 22/02/2026.
//

#include "motorDrivers.h"

#include <arduino.h>

#include "pinOut.h"

void motorTest() {
    digitalWrite(PIN_MOTDR_L1_FOR, HIGH);
    digitalWrite(PIN_MOTDR_L2_FOR, HIGH);

    digitalWrite(PIN_MOTDR_R1_FOR, HIGH);
    digitalWrite(PIN_MOTDR_R2_FOR, HIGH);

    delay(3000);

    digitalWrite(PIN_MOTDR_L1_FOR, LOW);
    digitalWrite(PIN_MOTDR_L2_FOR, LOW);

    digitalWrite(PIN_MOTDR_R1_FOR, LOW);
    digitalWrite(PIN_MOTDR_R2_FOR, LOW);

    digitalWrite(PIN_MOTDR_L1_BAC, HIGH);
    digitalWrite(PIN_MOTDR_L2_BAC, HIGH);

    digitalWrite(PIN_MOTDR_R1_BAC, HIGH);
    digitalWrite(PIN_MOTDR_R2_BAC, HIGH);

    delay(3000);

    digitalWrite(PIN_MOTDR_L1_BAC, LOW);
    digitalWrite(PIN_MOTDR_L2_BAC, LOW);

    digitalWrite(PIN_MOTDR_R1_BAC, LOW);
    digitalWrite(PIN_MOTDR_R2_BAC, LOW);
}

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

void motorRun(int direction) {
    if (direction == 0) { // FORWARDS!!
        analogWrite(PIN_MOTDR_L1_SPD, 255);
        analogWrite(PIN_MOTDR_L2_SPD, 255);
        analogWrite(PIN_MOTDR_R1_SPD, 255);
        analogWrite(PIN_MOTDR_R2_SPD, 255);

        digitalWrite(PIN_MOTDR_R1_FOR, HIGH);
        digitalWrite(PIN_MOTDR_R2_FOR, HIGH);
        digitalWrite(PIN_MOTDR_L1_FOR, HIGH);
        digitalWrite(PIN_MOTDR_L2_FOR, HIGH);
    }
    if (direction == 1) { // RETREAT!!
        analogWrite(PIN_MOTDR_L1_SPD, 255);
        analogWrite(PIN_MOTDR_L2_SPD, 255);
        analogWrite(PIN_MOTDR_R1_SPD, 255);
        analogWrite(PIN_MOTDR_R2_SPD, 255);

        digitalWrite(PIN_MOTDR_R1_BAC, HIGH);
        digitalWrite(PIN_MOTDR_R2_BAC, HIGH);
        digitalWrite(PIN_MOTDR_L1_BAC, HIGH);
        digitalWrite(PIN_MOTDR_L2_BAC, HIGH);
    }
    if (direction == 2) { // FLANK RIGHT
        analogWrite(PIN_MOTDR_L1_SPD, 255);
        analogWrite(PIN_MOTDR_R2_SPD, 255);

        digitalWrite(PIN_MOTDR_R1_FOR, HIGH);
        digitalWrite(PIN_MOTDR_L2_FOR, HIGH);
    }
    if (direction == 3) { // BACK RIGHT
        analogWrite(PIN_MOTDR_L2_SPD, 255);
        analogWrite(PIN_MOTDR_R1_SPD, 255);

        digitalWrite(PIN_MOTDR_R2_BAC, HIGH);
        digitalWrite(PIN_MOTDR_L1_BAC, HIGH);
    }
    if (direction == 4) { // FLANK LEFT
        analogWrite(PIN_MOTDR_L2_SPD, 255);
        analogWrite(PIN_MOTDR_R1_SPD, 255);

        digitalWrite(PIN_MOTDR_R2_FOR, HIGH);
        digitalWrite(PIN_MOTDR_L1_FOR, HIGH);
    }
    if (direction == 1) { // BACK LEFT
        analogWrite(PIN_MOTDR_L1_SPD, 255);
        analogWrite(PIN_MOTDR_R2_SPD, 255);

        digitalWrite(PIN_MOTDR_R1_BAC, HIGH);
        digitalWrite(PIN_MOTDR_L2_BAC, HIGH);
    }
    else {
        motorStop();
    }
}