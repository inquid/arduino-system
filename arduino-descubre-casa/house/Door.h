#include <Arduino.h>

#ifndef DOOR_H
#define DOOR_H

class Door {
  private:
    unsigned char _PINS[4];
    const unsigned char _INTERRUPT;
    volatile boolean * const _INTERRUPTED;
    const unsigned long _DOOR_LENGTH;
    unsigned long _steps;
  public:
    enum ordenPines {PULSE, DIRECTION, ENABLEMENT, PROBLEM};
    Door (unsigned char pinPulse, unsigned char pinDirection, unsigned char pinEnablement, unsigned char pinProblem, unsigned char interrupt, volatile boolean * const interrupted, unsigned long length, unsigned long initialSteps = 0);
    ~Door ();
    unsigned char (* getPins (void))[4];
    unsigned char getInterrupt (void);
    unsigned long getDoorLength (void);
    unsigned long getSteps (void);
    static boolean isValidSpeed (unsigned int speedo);
    boolean problem (void);
    boolean rotate (signed long steps, unsigned int speedo = 720u);
};

#endif
