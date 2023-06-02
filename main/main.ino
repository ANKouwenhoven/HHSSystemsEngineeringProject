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
#define LINE_VALUE_GREEN 800
#define LINE_VALUE_GREY 600
#define LINE_VALUE_RED 400
#define LINE_VALUE_EMPTY 300

bool useEmitters = true;
bool sendInfo = false;
bool sendInfoVisual = false;
bool receiveCommands = true;
int lastError = 0;
const int maxSpeed = 200;

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
    delay(100);
  }

  if (sendInfoVisual) {
    printInfoAsVisual();
  }

  if (receiveCommands) {
    readAndProcessSerial();
  }

  int position = lineSensors.readLine(lineSensorValues);
  int error = position - 2000;
  int speedDifference = error / 4 + 6 * (error - lastError);
  lastError = error;

  int leftSpeed = (int)maxSpeed + speedDifference;
  int rightSpeed = (int)maxSpeed - speedDifference;

  leftSpeed = constrain(leftSpeed, 0, (int)maxSpeed);
  rightSpeed = constrain(rightSpeed, 0, (int)maxSpeed);

  motors.setSpeeds(leftSpeed, rightSpeed);
}

void determineLineColors() {
  String perceivedLineColors[5];

  for (int i = 0; i < NUMBER_OF_SENSORS; i++) {
    if (lineSensorValues[i] > LINE_VALUE_GREEN) {
      //Follow the black line
      perceivedLineColors[i] = "Black";
    } else if (lineSensorValues[i] > LINE_VALUE_GREY) {
      //Follow the green line
      perceivedLineColors[i] = "Green";
    } else if (lineSensorValues[i] > LINE_VALUE_RED) {
      //Briefly pause at the grey line
      perceivedLineColors[i] = "Grey";
    } else if (lineSensorValues[i] > LINE_VALUE_EMPTY) {
      //Ignore the red line
      perceivedLineColors[i] = "Red";
    } else {
      //Ignore the whitespace
      perceivedLineColors[i] = "Empty";
    }
  }

  static int lastReportTime = 0;
  if ((int)(millis() - lastReportTime) >= 100) {
    lastReportTime = millis();
    Serial1.print("Perceived colors per sensor ");

    for (int i = 0; i < NUMBER_OF_SENSORS; i++) {
      Serial1.print("| ");
      Serial1.println(perceivedLineColors[i]);
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
    }

    //Debug - ontvangen character wordt terug gestuurd over normale Serial
    Serial.print("I received a character: ");
    Serial.println((char) incomingByte);
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
      if (lineSensorValues[i] > LINE_VALUE_GREEN) {
        Serial1.print("#");
      } else if (lineSensorValues[i] > LINE_VALUE_GREY) {
        Serial1.print("=");
      } else if (lineSensorValues[i] > LINE_VALUE_RED) {
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
  for(uint16_t i = 0; i < 120; i++)
  {
    if (i > 30 && i <= 90)
    {
      motors.setSpeeds(-200, 200);
    }
    else
    {
      motors.setSpeeds(200, -200);
    }

    lineSensors.calibrate();
  }
  motors.setSpeeds(0, 0);
}
