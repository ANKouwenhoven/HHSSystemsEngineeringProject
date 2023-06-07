#include "ZumoController.h"

ZumoController(): runningMode(false) {
  IOController io(false, false, false, true, false);
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