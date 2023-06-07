#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <Arduino.h>
#include <Zumo34U.h>

class MotorController {
  private:
    Zumo34Motors motors;
    int maxSpeed;

  public:
    void driveOnLine(int[]);
    void driveStraight();
    void rotate(bool);
    void stop();
}

#endif