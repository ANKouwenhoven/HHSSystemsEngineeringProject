#ifndef ZUMO_CONTROLLER_H
#define ZUMO_CONTROLLER_H

#include <Wire.h>
#include <Zumo32U4.h>

extern Zumo32U4LineSensors lineSensors;
extern Zumo32U4Motors motors;
extern Zumo32U4Buzzer buzzer;
extern Zumo32U4ButtonA buttonA;

#define NUMBER_OF_SENSORS 5
extern uint16_t lineSensorValues[NUMBER_OF_SENSORS];

#define LINE_VALUE_BLACK 1000
#define LINE_VALUE_GREEN 800
#define LINE_VALUE_GREY 600
#define LINE_VALUE_RED 400
#define LINE_VALUE_EMPTY 300

extern bool useEmitters;
extern bool sendInfo;
extern bool sendInfoVisual;
extern bool sendColorInfo;
extern bool receiveCommands;
extern bool observerMode;

extern int lastError;
extern int maxSpeed;

void setup();
void loop();

void determineLineColors();
void readAndProcessSerial();
void printToSerial();
void printInfoAsVisual();
void printInFormat(int sensorNumber, int value);
void calibrateSensors();

#endif // ZUMO_CONTROLLER_H
