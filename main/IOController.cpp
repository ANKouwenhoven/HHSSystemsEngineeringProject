#include "IOController.h"

IOController() {}

IOController(bool o1, bool o2, bool o3, bool o4, bool o5): sendRawInfo(o1), sendVisualInfo(o2), sendColorInfo(o3), receiveCommands(o4), runningMode(o5) {
  init();
}

~IOController() {}

void init() {
  Serial1.begin(9600);
}

void printToSerial(int[] lineSensorValues) {
  for (int i = 0; i < 5; i++) {
    printInFormat(i + 1, lineSensorValues[i]);
  }
}

void printAsVisual(int[] lineSensorValues, int bG, int bB, int bGr, int bE) {
  static int lastReportTime = 0;
  if ((int)(millis() - lastReportTime) >= 10) {
    lastReportTime = millis();

    Serial1.print("| ");
    for (int i = 0; i < 5; i++) {
      if (lineSensorValues[i] > bG) {
        Serial1.print("#");
      } else if (lineSensorValues[i] > bB) {
        Serial1.print("=");
      } else if (lineSensorValues[i] > bGr) {
        Serial1.print("_");
      } else if (lineSensorValues[i] > bE) {
        Serial1.print(".");
      } else {
        Serial1.print(" ");
      }
      Serial1.print(" | ");
    }
    Serial1.println("");
  }
}

void printPerceivedColors(int[] lineSensorValues, int bG, int bB, int bGr, int bE) {
  String perceivedLineColors[5];

  for (int i = 0; i < 5; i++) {
    if (lineSensorValues[i] > bG) {
      perceivedLineColors[i] = "Black";
    } else if (lineSensorValues[i] > bB) {
      perceivedLineColors[i] = "Grey";
    } else if (lineSensorValues[i] > bGr) {
      perceivedLineColors[i] = "Brown";
    } else if (lineSensorValues[i] > bE) {
      perceivedLineColors[i] = "Green";
    } else {
      //Ignore the whitespace (or red)
      perceivedLineColors[i] = "Empty";
    }
  }

  static int lastReportTime = 0;
  if ((int)(millis() - lastReportTime) >= 100) {
    lastReportTime = millis();
    Serial1.print("Perceived colors per sensor ");

    for (int i = 0; i < NUMBER_OF_SENSORS; i++) {
      Serial1.print(" | ");
      Serial1.print(perceivedLineColors[i]);
    }
    Serial1.println(" |");
  }
}

void printDebugMessage(string message) {
  Serial1.print(message);
}