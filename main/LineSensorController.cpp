#include "LineSensorController.h"

#define LINE_VALUE_BLACK 1000
#define LINE_VALUE_GREEN 200
#define LINE_VALUE_BROWN 330
#define LINE_VALUE_GREY 500
#define LINE_VALUE_EMPTY 80

#define NUMBER_OF_SENSORS 5

LineSensorController::LineSensorController(): useEmitters(true) {
  Zumo32U4LineSensors lineSensors();
}

LineSensorController::~LineSensorController() {}

void LineSensorController::init() {
  lineSensors.initFiveSensors();
}

/**
  @brief Kalibreert de lijnsensoren.
  Deze functie roept de calibrate-functie aan van het lineSensors-object om de lijnsensoren te kalibreren.
  De kalibratie wordt uitgevoerd voor de huidige omgevingsomstandigheden.
*/
void LineSensorController::calibrateSensors() {
  lineSensors.calibrate();
}

String LineSensorController::determineLineColor() {
  String perceivedLineColors[5];
  String consensusColor = "";

  for (int i = 0; i < NUMBER_OF_SENSORS; i++) {
    if (lineSensorValues[i] > LINE_VALUE_GREY) {
      perceivedLineColors[i] = "Black";
    } else if (lineSensorValues[i] > LINE_VALUE_BROWN) {
      perceivedLineColors[i] = "Grey";
    } else if (lineSensorValues[i] > LINE_VALUE_GREEN) {
      perceivedLineColors[i] = "Brown";
    } else if (lineSensorValues[i] > LINE_VALUE_EMPTY) {
      perceivedLineColors[i] = "Green";
    } else {
      //Ignore the whitespace
      perceivedLineColors[i] = "Empty";
    }
  }

  for (int i = 0; i < NUMBER_OF_SENSORS; i++) {
    if (lineSensorValues[i] > LINE_VALUE_EMPTY && lineSensorValues[i] < LINE_VALUE_GREY) {
      consensusColor = "Green";
    } else if (lineSensorValues[i] > LINE_VALUE_GREY) {
      consensusColor = "Black";
      break;
    }
  }

  return consensusColor;
}

/**
  @brief Leest de sensorwaarden van de lijnsensoren.
  Deze functie roept de readCalibrated-functie aan van het lineSensors-object om de sensorwaarden van de lijnsensoren te lezen.
  De gelezen waarden worden opgeslagen in de lineSensorValues-array.
  Het gebruik van de emitters wordt bepaald door de vlagvariabele useEmitters.
  @return Een pointer naar de lineSensorValues-array met de gelezen sensorwaarden.
*/
int* LineSensorController::readValues() {
  lineSensors.read(lineSensorValues, useEmitters ? QTR_EMITTERS_ON : QTR_EMITTERS_OFF);
  return lineSensorValues;
}

/**
  @brief Leest de lijnpositie op basis van de sensorwaarden.
  Deze functie roept de readLine-functie aan van het lineSensors-object om de lijnpositie te bepalen op basis van de sensorwaarden.
  De berekende lijnpositie wordt geretourneerd als een integer.
  @return De lijnpositie op basis van de sensorwaarden.
*/
int LineSensorController::readLine() {
  return lineSensors.readLine(lineSensorValues);
}