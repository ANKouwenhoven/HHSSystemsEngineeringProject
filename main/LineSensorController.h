#ifndef LINESENSORCONTROLLER_H
#define LINESENSORCONTROLLER_H

#include <Arduino.h>
#include <Zumo34U.h>

class LineSensorController {
  private:
    extern uint16_t lineSensorValues[5];
    Zumo34ULineSensors lineSensors;

  public:
    LineSensorController();
    ~LineSensorController();
    void init();
    void calibrateSensors();
    void readValues();
    void readLine();
}

#endif