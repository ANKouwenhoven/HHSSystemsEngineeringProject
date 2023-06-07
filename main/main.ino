/*
MAIN FILE - TO BE SPLIT
*/

#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;
Zumo32U4Buzzer buzzer;
Zumo32U4ButtonA buttonA;

#define NUMBER_OF_SENSORS 5
uint16_t lineSensorValues[NUMBER_OF_SENSORS];

#define LINE_VALUE_BLACK 1000
#define LINE_VALUE_GREEN 200
#define LINE_VALUE_BROWN 330
#define LINE_VALUE_GREY 500
#define LINE_VALUE_EMPTY 80

bool useEmitters = true;
bool sendInfo = false;
bool sendInfoVisual = false;
bool sendColorInfo = true;
bool receiveCommands = true;
bool observerMode = true;

int lastError = 0;
int maxSpeed = 100;

void setup() {
  lineSensors.initFiveSensors();  
  Serial1.begin(9600);

  calibrateSensors();
  buttonA.waitForButton();
}

void loop() {
  lineSensors.readCalibrated(lineSensorValues, useEmitters ? QTR_EMITTERS_ON : QTR_EMITTERS_OFF);
  lineSensorValues[2] -= 50;
  if (lineSensorValues[2] < 0) {
    lineSensorValues[2] = 0;
  }
  lineSensorValues[0] += 50;
  if (lineSensorValues[0] > 1000) {
    lineSensorValues[0] = 1000;
  }

  determineLineColors();

  if (sendInfo) {
    printToSerial();
    delay(100);
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
    int speedDifference = error / 4 + 10 * (error - lastError);
    lastError = error;

    int leftSpeed = (int)maxSpeed + speedDifference;
    int rightSpeed = (int)maxSpeed - speedDifference;

    leftSpeed = constrain(leftSpeed, 0, (int)maxSpeed);
    rightSpeed = constrain(rightSpeed, 0, (int)maxSpeed);

    motors.setSpeeds(leftSpeed, rightSpeed);
  } else {
    motors.setSpeeds(0, 0);
  }
}

void determineLineColors() {
  String perceivedLineColors[5];

  for (int i = 0; i < NUMBER_OF_SENSORS; i++) {
    if (lineSensorValues[i] > LINE_VALUE_GREY) {
      perceivedLineColors[i] = "Black";
    } else if (lineSensorValues[i] > LINE_VALUE_BROWN) {
      perceivedLineColors[i] = "Grey";
    } else if (lineSensorValues[i] > LINE_VALUE_GREEN) {
      perceivedLineColors[i] = "Brown";
    } else if (lineSensorValues[i] > LINE_VALUE_EMPTY) {
      perceivedLineColors[i] = "Green";
    } else {
      //Ignore the whitespace
      perceivedLineColors[i] = "Empty";
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
  Input 'i': Toggle het verwerken van info aan/uit
  Input 'e': Toggle de IR emitter aan/uit
*/
void readAndProcessSerial() {
  int incomingByte = Serial1.read();
  if (incomingByte != -1) {
    switch ((char) incomingByte) {
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
        sendColorInfo != sendColorInfo;
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

//Neem alle 5 de sensorvalues, en print ze naar de serial via printInFormat
void printToSerial() {
  for (int i = 0; i < 5; i++) {
    printInFormat(i + 1, lineSensorValues[i]);
  }
  Serial1.println(useEmitters ? 'E' : 'e');
}

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

//Neem de sensor en bijbehorende waarde en print ze op een nette manier
void printInFormat(int sensorNumber, int value) {
  Serial1.print("Sensor ");
  Serial1.print(sensorNumber);
  Serial1.print(": ");
  Serial1.print(value);
  Serial1.print(" | ");
}

void calibrateSensors() {
  delay(1000);
  for(uint16_t i = 0; i < 120; i++) {
    if (i > 30 && i <= 90) {
      motors.setSpeeds(-200, 200);
    }
    else {
      motors.setSpeeds(200, -200);
    }

    lineSensors.calibrate();
  }
  motors.setSpeeds(0, 0);
}
