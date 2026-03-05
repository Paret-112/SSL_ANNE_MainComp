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
void driveStop();

void turnLeft();
void turnRight();
void turnStop();

void prepareKick();
void kick();

Task tDriveForward(0, 1, &driveForward);
Task tDriveBackward(0, 1, &driveBackward);
Task tDriveFlankLeftFor(0, 1, &driveFlankLeftFor);
Task tDriveFlankLeftBac(0, 1, &driveFlankLeftBac);
Task tDriveFlankRightBac(0, 1, &driveFlankRightBac);
Task tDriveFlankRightFor(0, 1, &driveFlankRightFor);

Task tDriveStop(0, 1, &driveStop);
Task tTurnLeft(0, 1, &turnLeft);
Task tTurnRight(0, 1, &turnRight);
Task tTurnStop(0, 1, &turnStop);

Task tPrepareKick(0, 1, &prepareKick);
Task tKick(0, 1, &kick);

#endif //SSL_ANNE_MAINCOMP_ACTUATIONTASKS_H