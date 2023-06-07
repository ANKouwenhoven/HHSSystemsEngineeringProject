#include "ZumoController.h"

ZumoController(): runningMode(false) {
  IOController io(false, false, false, true, false);
  MotorController motors(100);
}

~ZumoController() {}

void ZumoSetup() {
  io.init();
}

void ZumoLoop() {
  io.printDebugMessage("Test: " + millis());
}

void flipRunMode() {
  runningMode = !runningMode;
}