#ifndef LINESENSORCONTROLLER_H
#define LINESENSORCONTROLLER_H

#include <Arduino.h>
#include <Zumo34U.h>

class LineSensorController {
  private:
    extern uint16_t lineSensorValues[5];
    Zumo34ULineSensors lineSensors;
    bool useEmitters;

  public:
    LineSensorController();
    ~LineSensorController();
    void calibrateSensors();
    int[] readValues();
    int readLine();
}

#endif