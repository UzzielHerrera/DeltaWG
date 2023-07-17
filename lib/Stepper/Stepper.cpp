#include "esp32-hal-gpio.h"
#include "Arduino.h"
#include "Stepper.h"
#include "Constants.h"

Stepper::Stepper(ui8 stepPin, ui8 dirPin, ui8 limitPin, ui8 stepperID){
  currentPosition = 0;
  id = stepperID;
  _stepPin = stepPin;
  _dirPin = dirPin;
  _limitPin = limitPin;
  _isMovingCW = true;
  _desiredPosition = 0;
  _currentStepsToTake = 0;
  _lastPulse = 0;
  _lastChange = 0;
  _manualSteps = 0;
  lapse = 0;
  _lastState = false;
  pinMode(_stepPin, OUTPUT);
  pinMode(_dirPin, OUTPUT);
  pinMode(_limitPin, INPUT);
  digitalWrite(_stepPin, 0);
  digitalWrite(_dirPin, 0);
}

void Stepper::update(){
  if(_currentStepsToTake != 0){
    if((micros() - _lastChange) >= (lapse/2)){
      _lastChange = micros();
      if(_lastState){
        digitalWrite(_stepPin, 0);
        _currentStepsToTake--;
        currentPosition += _isMovingCW ? 1 : -1;
      } else {
        digitalWrite(_stepPin, 1);
      }
      _lastState = !_lastState;
    }
  }
}

void Stepper::manualUpdate(){
  if(_manualSteps != 0){
    if((micros() - _lastChange) >= (2000/2)){
      _lastChange = micros();
      if(_lastState){
        digitalWrite(_stepPin, 0);
        _manualSteps--;
        currentPosition += _isMovingCW ? 1 : -1;
        //PORT.println((String) "curPos:  " + currentPosition);
      } else {
        digitalWrite(_stepPin, 1);
      }
      _lastState = !_lastState;
    }
  }
}

void Stepper::setStep2Low(){
  if(_lastState){
    digitalWrite(_stepPin, 0);
  }
}

void Stepper::setManualDirection(bool manualDir){
  if(manualDir){
    digitalWrite(_dirPin, LOW);
    _isMovingCW = true;
  } else {
    digitalWrite(_dirPin, HIGH);
    _isMovingCW = false;
  }
}

void Stepper::setDesiredPosition(int32_t desiredPosition){
  _desiredPosition = desiredPosition;
  _currentStepsToTake = desiredPosition - currentPosition;
  if(_currentStepsToTake > 0){
    digitalWrite(_dirPin, LOW);
    _isMovingCW = true;
  } else {
    digitalWrite(_dirPin, HIGH);
    _isMovingCW = false;
  }
  _currentStepsToTake = abs(_currentStepsToTake);
  //PORT.println((String) "CSTT " + id + ": " + _currentStepsToTake);
}

void Stepper::setStepsToZero(){
  _currentStepsToTake = 0;
}
