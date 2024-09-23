#include "StepperMotor.h"
#include "ServoMotor.h"

#ifndef RotaryTable_H
#define RotaryTable_H

class RotaryTable {
  private:
    const unsigned char _LOCK_UNLOCK_SWIVEL;
    ServoMotor _leftServoMotor, _rightServoMotor;
  public:
    StepperMotor stepperMotor;
    RotaryTable (unsigned char leftServoMotorPin, unsigned char leftServoMotorShift, unsigned char leftServoMotorInitialAngle,
                 unsigned char rightServoMotorPin, unsigned char rightServoMotorShift, unsigned char rightServoMotorInitialAngle,
                 unsigned char lockUnlockSwivel,
                 unsigned char stepperPulse, unsigned char stepperDirection, unsigned char stepperEnablement, unsigned char stepperGround, signed long int StepperInitialSteps = 0);
    ~RotaryTable ();
    boolean lock (void);
    boolean unlock (void);
};

#endif
