/*
MAIN FILE - TO BE SPLIT
*/

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;

#define NUMBER_OF_SENSORS 5
uint16_t lineSensorValues[NUMBER_OF_SENSORS];

#define LINE_VALUE_BLACK 1000
#define LINE_VALUE_GREEN 200
#define LINE_VALUE_BROWN 330
#define LINE_VALUE_GREY 500
#define LINE_VALUE_EMPTY 80

//IR emitters
bool useEmitters = true;

//Zumo stuurt informatie over de sensorwaardes
bool sendInfo = false;

//Zumo stuurt informatie over de sensorwaardes in sneller te verwerken visuele mode
bool sendInfoVisual = false;

//Zumo stuurt informatie over de kleur die gedetecteerd wordt
bool sendColorInfo = false;

//Zumo verwerkt input die vanaf Serial1 verstuurd is
bool receiveCommands = true;

//In observermode zal Zumo niet rijden
bool observerMode = false;

int lastError = 0;
int maxSpeed = 400;
String consensusColor = "";

void setup() {
  lineSensors.initFiveSensors();
  Serial1.begin(9600);

  calibrateSensors();
  buttonA.waitForButton();
}

void loop() {
  lineSensors.readCalibrated(lineSensorValues, useEmitters ? QTR_EMITTERS_ON : QTR_EMITTERS_OFF);

  determineLineColors();

  if (sendInfo) {
    printToSerial();
  }

  if (sendInfoVisual) {
    printInfoAsVisual();
  }

  if (receiveCommands) {
    readAndProcessSerial();
  }

  if (observerMode == false) {
    int position = lineSensors.readLine(lineSensorValues);
    int error = position - 2000;

    // **Dit stuk code zou de Zumo beter maken in scherpe bochten, maar het maakte hem te instabiel**

    // int strongestpos = 2;
    // int currentMax = 0;
    // for (int i = 0; i < NUMBER_OF_SENSORS; i++) {
    //   if (lineSensorValues[i] > currentMax) {
    //     currentMax = lineSensorValues[i];
    //     strongestpos = i;
    //   }
    // }

    // if (strongestpos == 0) {
    //   motors.setSpeeds(-200, 300);
    // } else if (strongestpos == 4) {
    //   motors.setSpeeds(300, -200);
    // } else {

    int speedDifference = error * 10 + 50 * (error - lastError);
    lastError = error;

    int finalSpeed;
    if (consensusColor == "Black") {
      finalSpeed = maxSpeed;
    } else {
      finalSpeed = maxSpeed / 2;
    }
    int leftSpeed = (int)finalSpeed + speedDifference;
    int rightSpeed = (int)finalSpeed - speedDifference;

    leftSpeed = constrain(leftSpeed, 0, (int)finalSpeed);
    rightSpeed = constrain(rightSpeed, 0, (int)finalSpeed);

    motors.setSpeeds(leftSpeed, rightSpeed);
    // }
  } else {
    motors.setSpeeds(0, 0);
  }
}

/*
  Analyseert de linesensorValues en bepaalt welke kleur het meest waarschijnlijk gedetecteerd wordt
*/
void determineLineColors() {
  String perceivedLineColors[5];

  for (int i = 0; i < NUMBER_OF_SENSORS; i++) {
    if (lineSensorValues[i] > LINE_VALUE_GREY) {
      perceivedLineColors[i] = "Black";
    } else if (lineSensorValues[i] > LINE_VALUE_BROWN) {
      perceivedLineColors[i] = "Grey";
    } else if (lineSensorValues[i] > LINE_VALUE_GREEN) {
      perceivedLineColors[i] = "Brown / green";
    } else if (lineSensorValues[i] > LINE_VALUE_EMPTY) {
      perceivedLineColors[i] = "Green";
    } else {
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

  if (sendColorInfo) {
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
}

/*Verwerk de inkomende serial input;
  Input 'i': Toggle het weergeven van info
  Input 'e': Toggle de IR emitter
  Input 'o': Toggle observermode - in observer mode zal de zumo niet rijden
  Input 'v': Toggle het weergeven van info in symbol mode
  Input 'c': Toggle het weergeven van info in color mode
  Input '+': Verhoog de max speed met 50
  Input '-': Verlaag de max speed met 50
*/
void readAndProcessSerial() {
  int incomingByte = Serial1.read();
  if (incomingByte != -1) {
    switch ((char)incomingByte) {
      case 'i':
        sendInfo = !sendInfo;
        break;
      case 'e':
        useEmitters = !useEmitters;
        break;
      case 'o':
        observerMode = !observerMode;
        break;
      case 'v':
        sendInfoVisual = !sendInfoVisual;
        break;
      case 'c':
        sendColorInfo = !sendColorInfo;
        break;
      case '+':
        maxSpeed += 50;
        break;
      case '-':
        maxSpeed -= 50;
        break;
    }

    //Debug - ontvangen character wordt terug gestuurd over normale Serial
    //Serial.print("I received a character: ");
    //Serial.println((char) incomingByte);
  }
}

/*
  Neem alle 5 de sensorvalues, en print ze naar de serial via printInFormat
*/
void printToSerial() {
  for (int i = 0; i < 5; i++) {
    printInFormat(i + 1, lineSensorValues[i]);
  }
  Serial1.println(useEmitters ? 'E' : 'e');
}

/*
  Print de sensorinfo in symbol mode, waarbij er per sensor een symbool wordt weergegeven
  Symbolen zijn gebaseerd op signaalsterkte
  Hierdoor wordt er een estimate weergegeven onder welke sensor de lijn zich waarschijnlijk bevindt
*/
void printInfoAsVisual() {
  static int lastReportTime = 0;
  if ((int)(millis() - lastReportTime) >= 10) {
    lastReportTime = millis();

    Serial1.print("| ");
    for (int i = 0; i < NUMBER_OF_SENSORS; i++) {
      if (lineSensorValues[i] > LINE_VALUE_GREY) {
        Serial1.print("#");
      } else if (lineSensorValues[i] > LINE_VALUE_BROWN) {
        Serial1.print("=");
      } else if (lineSensorValues[i] > LINE_VALUE_GREEN) {
        Serial1.print("_");
      } else if (lineSensorValues[i] > LINE_VALUE_EMPTY) {
        Serial1.print(".");
      } else {
        Serial1.print(" ");
      }
      Serial1.print(" | ");
    }
    Serial1.println("");
  }
}

/*
  Neem de sensor en bijbehorende waarde en print ze op een nette manier
*/
void printInFormat(int sensorNumber, int value) {
  Serial1.print("Sensor ");
  Serial1.print(sensorNumber);
  Serial1.print(": ");
  Serial1.print(value);
  Serial1.print(" | ");
}

/*
  Draait om zijn as en calibreert de sensors
*/
void calibrateSensors() {
  delay(1000);
  for (uint16_t i = 0; i < 120; i++) {
    if (i > 30 && i <= 90) {
      motors.setSpeeds(-200, 200);
    } else {
      motors.setSpeeds(200, -200);
    }

    lineSensors.calibrate();
  }
  motors.setSpeeds(0, 0);
}
