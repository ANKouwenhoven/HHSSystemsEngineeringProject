#ifndef ZUMOCONTROLLER_H
#define ZUMOCONTROLLER_H

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

  public:
    void zumoSetup();
    void zumoLoop();
}

#endif