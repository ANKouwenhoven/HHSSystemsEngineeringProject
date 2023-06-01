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

bool useEmitters = true;
bool sendInfo = true;
bool receiveCommands = true;
int lastError = 0;
const int maxSpeed = 200;

void setup() {
  lineSensors.initFiveSensors();  
  Serial1.begin(9600);
  buttonA.waitForButton();
}

void loop() {
  lineSensors.readCalibrated(lineSensorValues, useEmitters ? QTR_EMITTERS_ON : QTR_EMITTERS_OFF);

  if (sendInfo) {
    printToSerial();
    delay(100);
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

  motor.setSpeeds(leftSpeed, rightSpeed);
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
