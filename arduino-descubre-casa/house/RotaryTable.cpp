#include "RotaryTable.h"

RotaryTable::RotaryTable (unsigned char leftServoMotorPin, unsigned char leftServoMotorShift, unsigned char leftServoMotorInitialAngle,
                                    unsigned char rightServoMotorPin, unsigned char rightServoMotorShift, unsigned char rightServoMotorInitialAngle,
                                    unsigned char lockUnlockSwivel,
                                    unsigned char stepperPulse, unsigned char stepperDirection, unsigned char stepperEnablement, unsigned char stepperGround, signed long int StepperInitialSteps)
                                   : _leftServoMotor (leftServoMotorPin, leftServoMotorShift, leftServoMotorInitialAngle),
                                     _rightServoMotor (rightServoMotorPin, rightServoMotorShift, rightServoMotorInitialAngle),
                                     _LOCK_UNLOCK_SWIVEL (lockUnlockSwivel),
                                     stepperMotor (stepperPulse, stepperDirection, stepperEnablement, stepperGround, StepperInitialSteps)
{
  (*this)._leftServoMotor.init();
  (*this)._rightServoMotor.init();
}

RotaryTable::~RotaryTable () {}

/*
 * can't simply return the logical conjunction because we must revert the
 * first motor's side-effect when the second fails.
 */
boolean RotaryTable::lock (void) {
  if ((*this)._leftServoMotor.setAngle(90u + (_LOCK_UNLOCK_SWIVEL / 2u), ServoMotor::speeds::FAST)) {
    if ((*this)._rightServoMotor.setAngle(90u - (_LOCK_UNLOCK_SWIVEL / 2u), ServoMotor::speeds::FAST)) {
      return true;
    } else {
      (*this)._leftServoMotor.setAngle(90u - (_LOCK_UNLOCK_SWIVEL / 2u), ServoMotor::speeds::FAST);
      return false;
    }
  }
  return false;
}

boolean RotaryTable::unlock (void) {
  if ((*this)._leftServoMotor.setAngle(90u - (_LOCK_UNLOCK_SWIVEL / 2u), ServoMotor::speeds::FAST)) {
    if ((*this)._rightServoMotor.setAngle(90u + (_LOCK_UNLOCK_SWIVEL / 2u), ServoMotor::speeds::FAST)) {
      return true;
    } else {
      (*this)._leftServoMotor.setAngle(90u + (_LOCK_UNLOCK_SWIVEL / 2u), ServoMotor::speeds::FAST);
      return false;
    }
  }
  return false;
}
