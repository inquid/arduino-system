//probar velocidad mínima

#include <Arduino.h>
#include "Door.h"

Door::Door (unsigned char pinPulse, unsigned char pinDirection, unsigned char pinEnablement, unsigned char pinProblem, unsigned char interrupt, volatile boolean * const interrupted, unsigned long length, unsigned long initialSteps) : _INTERRUPT (interrupt), _INTERRUPTED (interrupted), _DOOR_LENGTH (length) {
  if (initialSteps > length) {
    return;
  } else {
    (*this)._steps = initialSteps;
  }
  (*this)._PINS[PULSE] = pinPulse;
  (*this)._PINS[DIRECTION] = pinDirection;
  (*this)._PINS[ENABLEMENT] = pinEnablement;
  (*this)._PINS[PROBLEM] = pinProblem;
  pinMode(pinPulse, OUTPUT);
  pinMode(pinDirection, OUTPUT);
  pinMode(pinEnablement, OUTPUT);
  pinMode(pinProblem, INPUT);
  digitalWrite(pinPulse, HIGH);
  digitalWrite(pinEnablement, LOW);
}

Door::~Door () {}

unsigned char (* Door::getPins (void))[4] {
  return &(*this)._PINS;
}

unsigned char Door::getInterrupt (void) {
  return (*this)._INTERRUPT;
}

unsigned long Door::getDoorLength (void) {
  return (*this)._DOOR_LENGTH;
}

unsigned long Door::getSteps (void) {
  Serial.print("pasos=");
  Serial.println((*this)._steps);
  return (*this)._steps;
}

boolean Door::isValidSpeed (unsigned int speedo) {
  return (6u <= speedo && speedo <= 3800u);
}

boolean Door::problem (void) {
  return digitalRead((*this)._PINS[PROBLEM]) == HIGH ? false : true;
}

boolean Door::rotate (signed long steps, unsigned int speedo) {
  analogWrite((*this)._PINS[ENABLEMENT], 0);
  analogWrite((*this)._PINS[PULSE], 0);
  if (Door::problem() || !Door::isValidSpeed(speedo) || (*this)._steps + steps < 0 || (*this)._steps + steps > _DOOR_LENGTH) {
    return false;
  }
  digitalWrite((*this)._PINS[DIRECTION], steps < 0 ? LOW : HIGH);
  delay(100);
  digitalWrite((*this)._PINS[ENABLEMENT], HIGH);
  /*
   * opening
   */
  while (steps > 0) {
    digitalWrite((*this)._PINS[PULSE], HIGH);
    delayMicroseconds(250);
    digitalWrite((*this)._PINS[PULSE], LOW);
    delayMicroseconds((1000000/speedo) - 250);
    --steps;
    ++(*this)._steps;
  }
  /*
   * closing
   */
  while (steps < 0) {
    if (*((*this)._INTERRUPTED)) {
      digitalWrite((*this)._PINS[ENABLEMENT], LOW);
      return false;
    }
    digitalWrite((*this)._PINS[PULSE], HIGH);
    delayMicroseconds(250);
    digitalWrite((*this)._PINS[PULSE], LOW);
    delayMicroseconds((1000000/speedo) - 250);
    ++steps;
    --(*this)._steps;
  }
  digitalWrite((*this)._PINS[ENABLEMENT], LOW);
  return true;
}

