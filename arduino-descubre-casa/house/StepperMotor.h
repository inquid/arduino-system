#include <Arduino.h>

#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

class StepperMotor {
  private:
    unsigned char _PINS[4];
    unsigned int _steps;
  public:
    enum pinOrder {PULSE, DIRECTION, ENABLEMENT, GROUND};
    StepperMotor (unsigned char pinPulse, unsigned char pinDirection, unsigned char pinEnablement, unsigned char pinGround, unsigned int initialSteps = 0);
    ~StepperMotor ();
    unsigned char (* getPins (void))[4];
    unsigned int getSteps (void);
    static boolean isValidSpeed (unsigned short int speedo);
    boolean rotate (signed int steps, unsigned short int speedo);
};

#endif
