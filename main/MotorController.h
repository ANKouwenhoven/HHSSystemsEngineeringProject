#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <Arduino.h>
#include <Zumo32U4.h>

class MotorController {
  private:
    Zumo32U4Motors motors;
    Zumo32U4ButtonA buttonA;
    int maxSpeed;
    int halfSpeed;
    int lastError;

  public:
    MotorController();
    MotorController(int);
    ~MotorController();
    void driveOnLine(int);
    void driveStraight(int);
    void rotate(bool);
    void stop();
};

#endif