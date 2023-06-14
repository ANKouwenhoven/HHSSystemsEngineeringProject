#include "MotorController.h"

MotorController::MotorController() {}

MotorController::MotorController(int max): maxSpeed(max) {
  halfSpeed = max / 2;
  lastError = 0;
}

MotorController::~MotorController() {}

/**
  @brief Rijdt op basis van de lijnpositie.
  Deze functie regelt het rijden van de motoren op basis van de opgegeven lijnpositie.
  Het verschil tussen de huidige positie en de gewenste positie (2000) wordt berekend als de fout (error).
  Vervolgens wordt het snelheidsverschil (speedDifference) bepaald op basis van de fout en de vorige fout (lastError).
  De snelheden van de linker- en rechtermotor worden aangepast op basis van het maximale snelheidsverschil (maxSpeed) en het berekende snelheidsverschil.
  @param position De huidige positie van de lijn.
*/
void MotorController::driveOnLine(int position, String color) {
  int error = position - 2000;
  int speedDifference = error * 10 + 50 * (error - lastError);
  lastError = error;

  int finalSpeed;
  if (color == "Black") {
    finalSpeed = maxSpeed;
  } else {
    finalSpeed = maxSpeed / 2;
  }

  int leftSpeed = (int)finalSpeed + speedDifference;
  int rightSpeed = (int)finalSpeed - speedDifference;

  leftSpeed = constrain(leftSpeed, 0, (int)finalSpeed);
  rightSpeed = constrain(rightSpeed, 0, (int)finalSpeed);
  // Serial.print(leftSpeed);
  // Serial.println(rightSpeed);
  motors.setSpeeds(leftSpeed, rightSpeed);
}

/**
  Deze functie stelt de motoren in op dezelfde snelheid om rechtuit te rijden.
  @param speed De gewenste snelheid om rechtuit te rijden.
*/
void MotorController::driveStraight(int speed) {
  motors.setSpeeds(speed, speed);
}

/**
  Deze functie draait de robot in de aangegeven richting.
  @param clockwise Een bool-waarde die aangeeft of de draaibeweging met de klok mee moet zijn (true) of tegen de klok in (false).
*/
void MotorController::rotate(bool clockwise) {
  if (clockwise) {
    motors.setSpeeds(200, -200);
  } else {
    motors.setSpeeds(-200, 200);
  }
}

/**
  Stopt de Zumo.
*/
void MotorController::stop() {
  motors.setSpeeds(0, 0);
}