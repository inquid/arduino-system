/*
 *  ServoMotor.h
 *
 *  Isaac David Reyes Gonzalez
 */

#include <Arduino.h>
#include <Servo.h>

#ifndef SERVOMOTOR_H
#define SERVOMOTOR_H

class ServoMotor {
  private:
    boolean _initialized;
    unsigned char _pin, _angle, _shift;
    Servo _servo;
  public:
    static const unsigned char DEFAULT_SPEED = 60u;
    struct speeds {
      public:
        static const unsigned char SLOW = 6u;
        static const unsigned char MEDIUM = ServoMotor::DEFAULT_SPEED;
        static const unsigned char FAST = 180u;
    };
    ServoMotor (unsigned char pin, unsigned char shift, unsigned char initialAngle = 0u);
    ~ServoMotor ();
    void init (void);
    unsigned char getPin (void);
    unsigned char getShift (void);
    unsigned char getAngle (void);
    static boolean isValidAngle (unsigned char angle);
    static boolean isValidSpeed (unsigned char speedo);
    boolean setAngle (unsigned char angle, unsigned char speedo = ServoMotor::speeds::MEDIUM);
    unsigned char setAngleOrLess (unsigned char angle, unsigned char speedo = ServoMotor::speeds::MEDIUM);
    boolean rotate (signed short int angle, unsigned char speedo = ServoMotor::speeds::MEDIUM);
    unsigned char rotateAtMost (signed short int angle, unsigned char speedo = ServoMotor::speeds::MEDIUM);
};

#endif
