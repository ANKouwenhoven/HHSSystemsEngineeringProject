#include "MotorController.h"

MotorController(int max): maxSpeed(max) {
  halfSpeed = max / 2;
}

~MotorController() {}

void driveOnLine(int position) {
  int error = position - 2000;
  int speedDifference = error / 4 + 10 * (error - lastError);
   lastError = error;

  int leftSpeed = (int)maxSpeed + speedDifference;
  int rightSpeed = (int)maxSpeed - speedDifference;

  leftSpeed = constrain(leftSpeed, 0, (int)maxSpeed);
  rightSpeed = constrain(rightSpeed, 0, (int)maxSpeed);

  motors.setSpeeds(leftSpeed, rightSpeed);
}

void driveStraight(int speed) {
  motors.setSpeeds(speed, speed);
}

void rotate(bool clockwise) {
  if (clockwise) {
    motors.setSpeeds(200, -200);
  } else {
    motors.setSpeeds(-200, 200);
  }
}

void stop() {
  motors.setSpeeds(0, 0);
}