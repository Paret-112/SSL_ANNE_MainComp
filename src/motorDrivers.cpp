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
