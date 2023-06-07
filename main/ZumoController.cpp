#include "ZumoController.h"

ZumoController() {
  IOController io(false, false, false, true, false);
}

~ZumoController() {}

void ZumoSetup() {
  io.init();
}

void ZumoLoop() {
  io.printDebugMessage("Test: " + millis());
}