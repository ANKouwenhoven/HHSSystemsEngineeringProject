#ifndef ZUMOCONTROLLER_H
#define ZUMOCONTROLLER_H

#include "IOController.h"
#include <Arduino.h>
#include <Zumo32U4.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "MotorController.h"
#include "LineSensorController.h"

class ZumoController {
  private:
    IOController io;
    Zumo32U4ButtonA buttonA;
    MotorController motors;
    LineSensorController lineSensors;
    bool runningMode;

  public:
    ZumoController();
    ~ZumoController();
    void zumoSetup();
    void zumoLoop();
    void flipRunMode();
    void calibrateSensors();
};

#endif