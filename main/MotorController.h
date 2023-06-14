#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

#include <Arduino.h>
#include <Zumo32U4.h>

class MotorController {
  private:
    Zumo32U4Motors motors;
    int halfSpeed;
    int lastError;

  public:
    int maxSpeed;
    MotorController();
    MotorController(int);
    ~MotorController();
    void driveOnLine(int, String);
    void driveStraight(int);
    void rotate(bool);
    void stop();
};

#endif