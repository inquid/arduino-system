/*
 *  StepperMotorOld.cpp
 *
 *  Isaac David Reyes Gonzalez
 */
 
#include <Arduino.h>
#include "StepperMotorOld.h"

StepperMotorOld::StepperMotorOld (float anglesPerStep, unsigned char pin1, unsigned char pin2, signed long int initialAngle) : _stepper((signed int) (360 / anglesPerStep), pin1, pin2) {
  (*this)._stepsPerAngle = 1 / anglesPerStep;
  (*this)._angle = initialAngle;
  (*this)._pins[0] = pin1;
  (*this)._pins[1] = pin2;
}

StepperMotorOld::StepperMotorOld (float anglesPerStep, unsigned char pin1, unsigned char pin2, unsigned char pin3, unsigned char pin4, signed long int initialAngle) : _stepper(360 / anglesPerStep, pin1, pin2, pin3, pin4) {
  (*this)._stepsPerAngle = 1 / anglesPerStep;
  (*this)._angle = initialAngle;
  (*this)._pins[0] = pin1;
  (*this)._pins[1] = pin2;
  (*this)._pins[2] = pin3;
  (*this)._pins[3] = pin4;
}

StepperMotorOld::~StepperMotorOld () {}

unsigned char (* StepperMotorOld::getPins (void))[4] {
  return &((*this)._pins);
}

signed long int StepperMotorOld::getAngle (void) {
  return (*this)._angle;
}

boolean StepperMotorOld::isValidSpeed (unsigned short int speedo) {
  return (6u <= speedo && speedo <= 360u);
}

boolean StepperMotorOld::setAngle (signed long int angle, unsigned short int speedo) {
  if (! StepperMotorOld::isValidSpeed(speedo)) {
    return false;
  }
  (*this)._stepper.setSpeed(speedo/6);
  (*this)._stepper.step((signed int) ((angle - (*this)._angle) * (*this)._stepsPerAngle));
  (*this)._angle = angle;
  return true;
}

boolean StepperMotorOld::setCyclicAngle (signed long int angle, unsigned short int speedo) {
  if (! (StepperMotorOld::isValidSpeed(speedo) && (0u < angle && angle <= 360u))) {
    return false;
  }
  return (*this).rotate((signed long int) (angle - ((*this)._angle) % 360), speedo);
}


boolean StepperMotorOld::rotate (signed long int angle, unsigned short int speedo) {
  if (! StepperMotorOld::isValidSpeed(speedo)) {
    return false;
  }
  (*this)._stepper.setSpeed(speedo/6);
  (*this)._stepper.step((signed int) (angle * (*this)._stepsPerAngle));
  (*this)._angle += angle;
  return true;
}

