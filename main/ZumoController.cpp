#include "ZumoController.h"

ZumoController::ZumoController() {
  IOController io(false, false, false, true, false);
  runningMode = true;
  MotorController motors(100);
  LineSensorController lineSensors();
  Zumo32U4ButtonA buttonA();
}

ZumoController::~ZumoController() {}

void ZumoController::zumoSetup() {
  io.init();
  calibrateSensors();
  buttonA.waitForButton();
}

void ZumoController::zumoLoop() {
  //io.printDebugMessage("Test: ");
  delay(100);
  int position = lineSensors.readLine();
  motors.driveOnLine(position);
  io.printDebugMessage((String)position);
}

void ZumoController::flipRunMode() {
  runningMode = !runningMode;
}

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