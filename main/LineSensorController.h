#ifndef LINESENSORCONTROLLER_H
#define LINESENSORCONTROLLER_H

#include <Arduino.h>
#include <Zumo32U4.h>

class LineSensorController {
  private:
    int lineSensorValues[5];
    Zumo32U4LineSensors lineSensors;
    bool useEmitters;

  public:
    LineSensorController();
    ~LineSensorController();
    String determineLineColor();
    void calibrateSensors();
    int *readValues();
    int readLine();
    void init();
};

#endif