//
// Created by HP Envy on 02/03/2026.
//

#ifndef SSL_ANNE_MAINCOMP_ACTUATIONTASKS_H
#define SSL_ANNE_MAINCOMP_ACTUATIONTASKS_H

void driveForward();
void driveBackward();
void driveFlankLeftFor();
void driveFlankLeftBac();
void driveFlankRightBac();
void driveFlankRightFor();

void turnLeft();
void turnRight();

void allStop();

void prepareKick();
void kick();

void pumpActuateInCB();
void pumpActuateOutCB();
void pumpActuateStopCB();

void globalSpeedModSet(int);

#endif //SSL_ANNE_MAINCOMP_ACTUATIONTASKS_H