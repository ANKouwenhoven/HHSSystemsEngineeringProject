#include "LineSensorController.h"

#define LINE_VALUE_BLACK 1000
#define LINE_VALUE_GREEN 200
#define LINE_VALUE_BROWN 330
#define LINE_VALUE_GREY 500
#define LINE_VALUE_EMPTY 80

#define NUMBER_OF_SENSORS 5

LineSensorController::LineSensorController() {
  Zumo32U4LineSensors lineSensors();
}

LineSensorController::~LineSensorController() {}

void LineSensorController::calibrateSensors() {
  lineSensors.calibrate();
}

int* LineSensorController::readValues() {
  lineSensors.readCalibrated(lineSensorValues, useEmitters ? QTR_EMITTERS_ON : QTR_EMITTERS_OFF);
  return lineSensorValues;
}

int LineSensorController::readLine() {
  return lineSensors.readLine(lineSensorValues);
}