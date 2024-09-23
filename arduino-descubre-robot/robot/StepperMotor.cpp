//probar velocidad mínima

#include <Arduino.h>
#include "StepperMotor.h"

StepperMotor::StepperMotor (unsigned char pinPulse, unsigned char pinDirection, unsigned char pinEnablement, unsigned char pinGround, unsigned int initialSteps) {
  (*this)._steps = initialSteps;
  (*this)._PINS[PULSE] = pinPulse;
  (*this)._PINS[DIRECTION] = pinDirection;
  (*this)._PINS[ENABLEMENT] = pinEnablement;
  (*this)._PINS[GROUND] = pinGround;  
  pinMode(pinPulse, OUTPUT);
  pinMode(pinDirection, OUTPUT);
  pinMode(pinEnablement, OUTPUT);
  pinMode(pinGround, OUTPUT);
  digitalWrite(pinEnablement, LOW);
  digitalWrite(pinPulse, HIGH);
  digitalWrite(pinGround, LOW);
}

StepperMotor::~StepperMotor () {}

unsigned char (* StepperMotor::getPins (void))[4] {
  return &(*this)._PINS;
}

unsigned int StepperMotor::getSteps (void) {
  return (*this)._steps;
}

boolean StepperMotor::rotate (signed int steps) {
  digitalWrite((*this)._PINS[ENABLEMENT], LOW);
  digitalWrite((*this)._PINS[DIRECTION], steps < 0 ? LOW : HIGH);
  delay(100);
  /*
   * opening
   */
  while (steps > 0) {
    digitalWrite((*this)._PINS[PULSE], LOW);
    delayMicroseconds(3000);
    digitalWrite((*this)._PINS[PULSE], HIGH);
    delayMicroseconds(1000);
    --steps;
    ++(*this)._steps;
  }
  /*
   * closing
   */
  while (steps < 0) {
    digitalWrite((*this)._PINS[PULSE], LOW);
    delayMicroseconds(3000);
    digitalWrite((*this)._PINS[PULSE], HIGH);
    delayMicroseconds(1000);
    ++steps;
    --(*this)._steps;
  }
  digitalWrite((*this)._PINS[ENABLEMENT], LOW);
  return true;
}


