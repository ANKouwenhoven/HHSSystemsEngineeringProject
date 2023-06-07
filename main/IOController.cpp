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

void printAsVisual(int[] lineSensorValues, int tG, int tB, int tGr, int tE) {
  static int lastReportTime = 0;
  if ((int)(millis() - lastReportTime) >= 10) {
    lastReportTime = millis();

    Serial1.print("| ");
    for (int i = 0; i < 5; i++) {
      if (lineSensorValues[i] > tG) {
        Serial1.print("#");
      } else if (lineSensorValues[i] > tB) {
        Serial1.print("=");
      } else if (lineSensorValues[i] > tGr) {
        Serial1.print("_");
      } else if (lineSensorValues[i] > tE) {
        Serial1.print(".");
      } else {
        Serial1.print(" ");
      }
      Serial1.print(" | ");
    }
    Serial1.println("");
  }
}

void printPerceivedColors(int[] lineSensorValues, int tG, int tB, int tGr, int tE) {
  String perceivedLineColors[5];

  for (int i = 0; i < 5; i++) {
    if (lineSensorValues[i] > tG) {
      perceivedLineColors[i] = "Black";
    } else if (lineSensorValues[i] > tB) {
      perceivedLineColors[i] = "Grey";
    } else if (lineSensorValues[i] > tGr) {
      perceivedLineColors[i] = "Brown";
    } else if (lineSensorValues[i] > tE) {
      perceivedLineColors[i] = "Green";
    } else {
      //Ignore the whitespace (or red)
      perceivedLineColors[i] = "Empty";
    }
  }

  static int lastReportTime = 0;
  if ((int)(millis() - lastReportTime) >= 50) {
    lastReportTime = millis();
    Serial1.print("Perceived colors per sensor ");

    for (int i = 0; i < 5; i++) {
      Serial1.print(" | ");
      Serial1.print(perceivedLineColors[i]);
    }
    Serial1.println(" |");
  }
}

void printDebugMessage(string message) {
  Serial1.print(message);
}

void readAndProcessInput() {
  int incomingByte = Serial1.read();
  if (incomingByte != -1) {
    switch ((char) incomingByte) {
      case 'i':
        sendInfo = !sendInfo;
        break;
      case 'r':
        ZumoController::flipRunMode();
        break;
      case 'v':
        sendInfoVisual = !sendInfoVisual;
        break;
      case 'c':
        sendColorInfo = !sendColorInfo;
        break;
    }

    //Debug - ontvangen character wordt terug gestuurd over normale Serial
    //Serial.print("I received a character: ");
    //Serial.println((char) incomingByte);
  }
}