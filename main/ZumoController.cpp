#include "ZumoController.h"

ZumoController(): runningMode(false) {
  IOController io(false, false, false, true, false);
  MotorController motors(100);
  LineSensorController lineSensors();
  Zumo34UButtonA buttonA();
}

~ZumoController() {}

void ZumoSetup() {
  io.init();
  calibrateSensors();
  buttonA.waitForButton();
}

void ZumoLoop() {
  io.printDebugMessage("Test: " + millis());
}

void flipRunMode() {
  runningMode = !runningMode;
}

void calibrateSensors() {
  usleep(1000000);

  for(uint16_t i = 0; i < 120; i++) {
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