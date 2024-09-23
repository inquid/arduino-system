/*
 *  ServoMotor.cpp - A comfortable wrapper library/class around Arduino's Servo library.
 *
 *  Isaac David Reyes Gonzalez
 */

#include <Arduino.h>
#include "ServoMotor.h"

//validaciones en los parametros, usar throws en el metodo?
ServoMotor::ServoMotor (unsigned char pin, unsigned char shift, unsigned char initialAngle) {
  (*this)._initialized = false;
  (*this)._pin = pin;
  (*this)._shift = shift;
  (*this)._angle = initialAngle;
}

ServoMotor::~ServoMotor () {
  (*this)._servo.detach();
}

void ServoMotor::init (void) {
  (*this)._servo.attach ((*this)._pin);
  (*this)._servo.write ((*this)._angle + (*this)._shift);
  (*this)._initialized = true;
}

unsigned char ServoMotor::getPin (void) {
  return (*this)._pin;
}

unsigned char ServoMotor::getShift (void) {
  return (*this)._shift;
}

unsigned char ServoMotor::getAngle (void) {
  return (*this)._angle;
}

boolean ServoMotor::isValidAngle (unsigned char angle) {
  return (0u <= angle && angle <= 180u);
}

//problema con valores mayores a 256 siendo aceptados
boolean ServoMotor::isValidSpeed (unsigned char speedo) {
  return (0u < speedo && speedo <= 180u);
}

boolean ServoMotor::setAngle (unsigned char angle, unsigned char speedo) {
  if (! (ServoMotor::isValidAngle(angle) && ServoMotor::isValidSpeed(speedo) && (*this)._initialized)) {
    return false;
  }
  while ((*this)._angle < angle) {
    ++(*this)._angle;
    (*this)._servo.write((*this)._angle + (*this)._shift);
    delay(1000u/speedo);
  }
  while ((*this)._angle > angle) {
    --(*this)._angle;
    (*this)._servo.write((*this)._angle + (*this)._shift);
    delay(1000u/speedo);
  }
  return true;
}

unsigned char ServoMotor::setAngleOrLess (unsigned char angle, unsigned char speedo) {
  if (! (ServoMotor::isValidSpeed(speedo) && (*this)._initialized)) {
    return (*this)._angle; 
  }
  while ((*this)._angle < angle && ServoMotor::isValidAngle((*this)._angle + 1)) {
    ++(*this)._angle;
    (*this)._servo.write((*this)._angle + (*this)._shift);
    delay(1000u/speedo);
  }
  while ((*this)._angle > angle && ServoMotor::isValidAngle((*this)._angle - 1)) {
    --ServoMotor::_angle;
    (*this)._servo.write((*this)._angle + (*this)._shift);
    delay(1000u/speedo);
  }
  return (*this)._angle;
}

boolean ServoMotor::rotate (signed short int angle, unsigned char speedo) {
  if (! (ServoMotor::isValidAngle(angle + (*this)._angle) && ServoMotor::isValidSpeed(speedo) && (*this)._initialized)) {
    return false;
  }
  while (angle > 0) {
    --angle;
    ++(*this)._angle;
    (*this)._servo.write((*this)._angle + (*this)._shift);
    delay(1000u/speedo);
  }
  while (angle < 0) {
    ++angle;
    --(*this)._angle;
    (*this)._servo.write((*this)._angle + (*this)._shift);
    delay(1000u/speedo);
  }
  return true;
}

unsigned char ServoMotor::rotateAtMost (signed short int angle, unsigned char speedo) {
  if (! (ServoMotor::isValidSpeed(speedo) && (*this)._initialized)) {
    return 0u;
  }
  signed short int angleBackup = angle;
  while (angle > 0 && ServoMotor::isValidAngle((*this)._angle + 1)) {
    --angle;
    ++(*this)._angle;
    (*this)._servo.write((*this)._angle + (*this)._shift);
    delay(1000u/speedo);
  }
  while (angle < 0 && ServoMotor::isValidAngle((*this)._angle - 1)) {
    ++angle;
    --(*this)._angle;
    (*this)._servo.write((*this)._angle + (*this)._shift);
    delay(1000u/speedo);
  }  
  return angleBackup - angle;
}

