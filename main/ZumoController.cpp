#include "ZumoController.h"

ZumoController::ZumoController() {
  IOController io(false, false, false, true, false);
  runningMode = true;
  MotorController motors(100);
  LineSensorController lineSensors();
  Zumo32U4ButtonA buttonA();
}

ZumoController::~ZumoController() {}

/**
  @brief Initialiseert de Zumo-controller.
  Deze functie initialiseert de Zumo-controller door de IOController te initialiseren,
  de sensoren te kalibreren en te wachten tot er op de A knop wordt gedrukt.
*/
void ZumoController::zumoSetup() {
  io.init();
  calibrateSensors();
  buttonA.waitForButton();
}

/**
  Hoofdloop van de Zumo
*/
void ZumoController::zumoLoop() {
  delay(100);
  int position = lineSensors.readLine();
  motors.driveOnLine(position);
  io.printDebugMessage((String)position);
}

/**
  Schakelt het rijden van de Zumo aan/uit.
*/
void ZumoController::flipRunMode() {
  runningMode = !runningMode;
}

/**
  @brief Kalibreert de sensoren van de Zumo-controller.
  Deze functie kalibreert de sensoren van de Zumo-controller door een kalibratieproces uit te voeren.
  Draait clockwise en anticlockwise terwijl hij de sensoren kalibreert.
*/
void ZumoController::calibrateSensors() {
  delay(1000);

  for(uint16_t i = 0; i < 120; i++) {
    delay(10);
    if (i > 30 && i <= 90) {
      motors.rotate(true);
    }
    else {
      motors.rotate(false);
    }

    lineSensors.calibrateSensors();
  }
  motors.stop();
}