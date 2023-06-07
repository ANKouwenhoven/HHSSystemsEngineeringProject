#ifndef LINESENSORCONTROLLER_H
#define LINESENSORCONTROLLER_H

#include <Zumo34U.h>

class LineSensorController {
  private:
    Zumo34ULineSensors lineSensors;

  public:
    void init();
    void calibrateSensors();
    void readValues();
    void readLine();
}

#endif