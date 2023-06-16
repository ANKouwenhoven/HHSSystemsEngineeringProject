#include "IOController.h"

IOController::IOController() {}

IOController::IOController(bool o1, bool o2, bool o3, bool o4, bool o5): sendRawInfo(o1), sendInfoVisual(o2), sendColorInfo(o3), receiveCommands(o4), runningMode(o5) {
  init();
}

IOController::~IOController() {}

/**
  Initialiseert de Serial poorten.
*/
void IOController::init() {
  Serial1.begin(9600);
  Serial.begin(9600);
}

/**
  @brief Drukt de sensorwaarden af op de Serial1-poort.
  Deze functie drukt de sensorwaarden af op de Serial1-poort door gebruik te maken van de printInFormat-functie.
  @param lineSensorValues Een array met de sensorwaarden van de lijnsensoren.
*/
void IOController::printToSerial(int lineSensorValues[]) {
  if (!sendRawInfo) return;
  for (int i = 0; i < 5; i++) {
    printInFormat(i + 1, lineSensorValues[i]);
  }
}

/**
  @brief Drukt de sensorwaarde af in een specifiek formaat.
  Deze functie drukt de opgegeven sensorwaarde af in een specifiek formaat op de Serial1-poort.
  Het formaat bestaat uit de tekst "Sensor <sensorNumber>: <value> | ".
  @param sensorNumber Het nummer van de sensor.
  @param value De waarde van de sensor.
*/
void IOController::printInFormat(int sensorNumber, int value) {
  Serial1.print("Sensor ");
  Serial1.print(sensorNumber);
  Serial1.print(": ");
  Serial1.print(value);
  Serial1.println(" | ");
}

/**
  @brief Print de lineSensorValues als visuele representatie.
  Deze functie print de lineSensorValues-array als een visuele representatie op de Serial1-poort.
  De visuele representatie bestaat uit een reeks symbolen, waarbij elk symbool een sensorwaarde voorstelt.
  De symbolen worden afgedrukt op basis van de drempelwaarden tG, tB, tGr en tE.
  @param lineSensorValues Een array met de sensorwaarden van de lijnsensoren.
  @param tG Drempelwaarde voor het symbool "#" (hoogste waarde).
  @param tB Drempelwaarde voor het symbool "=".
  @param tGr Drempelwaarde voor het symbool "_".
  @param tE Drempelwaarde voor het symbool "." (laagste waarde).
*/
void IOController::printAsVisual(int lineSensorValues[], int tG, int tB, int tGr, int tE) {
  if (!sendInfoVisual) return;
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

/**
  @brief Print de waargenomen kleuren op basis van de lineSensorValues.
  Deze functie bepaalt de waargenomen kleuren op basis van de sensorwaarden van de lijnsensoren (lineSensorValues).
  De kleur van elke sensor wordt bepaald aan de hand van drempelwaarden (tG, tB, tGr, tE).
  De waargenomen kleuren worden afgedrukt op de Serial1-poort.
  @param lineSensorValues Een array met de sensorwaarden van de lijnsensoren.
  @param tG Drempelwaarde voor de kleur "Black" (hoogste waarde).
  @param tB Drempelwaarde voor de kleur "Grey".
  @param tGr Drempelwaarde voor de kleur "Brown".
  @param tE Drempelwaarde voor de kleur "Green" (laagste waarde).
*/
void IOController::printPerceivedColors(int lineSensorValues[], int tG, int tB, int tGr, int tE) {
  if (!sendColorInfo) return;
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

/**
  Simpele serial print voor debugging doeleinden
  @param message de inhoud van het bericht
*/
void IOController::printDebugMessage(String message) {
  Serial1.println(message);
}

/**
  @brief Leest en verwerkt de invoer van Serial1.
  Deze functie leest een enkele byte van Serial1 en verwerkt het volgens de ontvangen waarde.
  De verwerking omvat het aanpassen van verschillende vlagvariabelen op basis van de ontvangen waarde.
  Mogelijke ontvangen waarden en hun betekenis:
  'i': Wisselt de status van de vlagvariabele sendRawInfo (sturen van ruwe informatie).
  'r': Wisselt de status van runningMode in de ZumoController
  'v': Wisselt de status van sendInfoVisual (sturen van visuele informatie).
  'c': Wisselt de status van sendColorInfo (sturen van kleurinformatie).
*/
void IOController::readAndProcessInput() {
  int incomingByte = Serial1.read();
  if (incomingByte != -1) {
    switch ((char) incomingByte) {
      case 'i':
        sendRawInfo = !sendRawInfo;
        break;
      case 'r':
        //ZumoController::flipRunMode();
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