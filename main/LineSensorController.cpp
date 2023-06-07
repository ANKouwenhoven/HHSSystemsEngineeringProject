#include "LineSensorController.h"

#define LINE_VALUE_BLACK 1000
#define LINE_VALUE_GREEN 200
#define LINE_VALUE_BROWN 330
#define LINE_VALUE_GREY 500
#define LINE_VALUE_EMPTY 80

#define NUMBER_OF_SENSORS 5

LineSensorController(): useEmitters(false) {}

~LineSensorController() {}

void calibrateSensors() {
  lineSensors.calibrate();
}

int[] readValues() {
  return lineSensors.readCalibrated(lineSensorValues, useEmitters ? QTR_EMITTERS_ON : QTR_EMITTERS_OFF);
}

int readLine() {
  return lineSensors.readLine(lineSensorValues);
}