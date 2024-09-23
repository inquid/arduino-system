/*
 *  StepperMotorOld.h - para los drivers que no usamos
 *
 *  Isaac David Reyes Gonzalez
 */
 
#include <Arduino.h>
#include <Stepper.h>

#ifndef STEPPERMOTOROLD_H
#define STEPPERMOTOROLD_H

class StepperMotorOld {
  private:
    unsigned char _pins[4];
    signed long int _angle;
    float _stepsPerAngle;
    Stepper _stepper;
  public:
    static const unsigned char DEFAULT_SPEED = 60u;
    struct speeds {
      public:
        static const unsigned char SLOW = 6u;
        static const unsigned char MEDIUM = StepperMotorOld::DEFAULT_SPEED;
        static const unsigned char FAST = 180u;
    };
    StepperMotorOld (float anglesPerStep, unsigned char pin1, unsigned char pin2, signed long int initialAngle);
    StepperMotorOld (float anglesPerStep, unsigned char pin1, unsigned char pin2, unsigned char pin3, unsigned char pin4, signed long int initialAngle);
    ~StepperMotorOld ();
    unsigned char (* getPins (void))[4];
    signed long int getAngle (void);
    static boolean isValidSpeed (unsigned short int speedo);
    boolean setAngle (signed long int angle, unsigned short int speedo = (unsigned short int) StepperMotorOld::speeds::MEDIUM);
    boolean setCyclicAngle (signed long int angle, unsigned short int speedo = (unsigned short int) StepperMotorOld::speeds::MEDIUM);
    boolean rotate (signed long int angle, unsigned short int speedo = (unsigned short int) StepperMotorOld::speeds::MEDIUM);
};

#endif
