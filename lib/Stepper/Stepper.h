#ifndef Stepper_h
#define Stepper_h

#include "Constants.h"
#include "Arduino.h"

class Stepper{
  public:
    Stepper(ui8 stepPin, ui8 dirPin, ui8 limitPin, ui8 stepperID);
    void update();
    void manualUpdate();
    void setStep2Low();
    void setDesiredPosition(int32_t desiredPosition);
    void setManualDirection(bool manualDir);
    void setStepsToZero();
    int32_t currentPosition;
    int8_t id;
    int32_t lapse;
    int32_t _currentStepsToTake;
    int32_t _manualSteps;
  private:
    int32_t _desiredPosition;
    bool _lastState;
    ui8 _stepPin;
    ui8 _dirPin;
    ui8 _limitPin;
    ui8 _lastPulse;
    ui32 _lastChange;
    bool _isMovingCW;
};

#endif