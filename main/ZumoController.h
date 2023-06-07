#ifndef ZUMOCONTROLLER_H
#define ZUMOCONTROLLER_H

#include <Arduino.h>
#include <Zumo34U.h>
#include MotorController.h
#include LineSensorController.h
#include IOController.h

class ZumoController {
  private:
    MotorController motors;
    LineSensorController lineSensor;
    IOController io;
    Zumo34UButtonA buttonA;
    bool runningMode;

  public:
    ZumoController();
    ~ZumoController();
    void zumoSetup();
    void zumoLoop();
    void flipRunMode();
    void calibrateSensors();
}

#endif