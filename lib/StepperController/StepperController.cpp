#include "Constants.h"
#include "Arduino.h"
#include "Stepper.h"
#include "StepperController.h"
#include "Queue.h"
#include "State.h"
#include "Tools.h"

StepperController::StepperController(){
  _designerQueue.clear();
  repeat = false;
  _counter = 0;
  _numOfAttachedSteppers = 0;
  _isExecutingMovement = false;
  _maxSteps = 0;
  _maxSpeed = 0;
  for(ui8 i = 0; i < MAX_STEPPERS; i++){
    _steppers[i] = {0};
    _speeds[i] = 0;
  }
  PORT.println("Steppers Automatic Control Module initialized.");
}

void StepperController::attach(Stepper *stepper){
  if (stepper->id < MAX_STEPPERS){
    _steppers[stepper->id] = stepper;
    _numOfAttachedSteppers++;
  }
}

void StepperController::addMoveDesigner(MoveDesigner md){
  //PORT.println("pushed md");
  _designerQueue.push(md);
  _designerQueue.setResetPoint();
}

void StepperController::addMotionMoveDesigner(MoveDesigner md){
  _designerQueue.push(md);
}

MoveDesigner StepperController::popMoveDesigner(){
  return _designerQueue.pop();
}

MoveDesigner StepperController::peekMoveDesigner(){
  return _designerQueue.peek();
}

void StepperController::setFrequencyFrom(double moveDuration){
  _frequency = 0.0001 * M_PI / moveDuration;
}

void StepperController::cancelMoves(){
  _designerQueue.clear();
  stateled.Green();
  for(ui8 i = 0; i < _numOfAttachedSteppers; i++){
    if(_steppers[i]!=0){
      _steppers[i]->_currentStepsToTake = 0;
      _steppers[i]->setStep2Low();
    }
  }
}

void StepperController::setSteppersToZero(){
  for(ui8 i = 0; i < _numOfAttachedSteppers; i++){
    if(_steppers[i]!=0){
      _steppers[i]->currentPosition = 97;
    }
  }
}

void StepperController::update(){
  if(!_isExecutingMovement){
    if(_designerQueue.peek().isActive){
      int32_t bufvalue;
      int _counter;
      MoveDesigner md = _designerQueue.pop();
      //tool.PrintMoveDesigner(md, "_designerQueue POP");
      _maxSteps = 0;
      for(ui8 i = 0; i < _numOfAttachedSteppers; i++){
        bufvalue = abs(md.Designer[i].position - _steppers[i]->currentPosition);
        if(bufvalue == 0) _counter++;
        if( bufvalue > _maxSteps){
          _maxSteps = bufvalue;
        }
        //PORT.println((String) "mdsteps " + i + ": " + md.Designer[i].position );
        //PORT.println((String) "currsteps " + i + ": " + _steppers[i]->currentPosition );
        //PORT.println((String) "oper " + i + ": " + abs(md.Designer[i].position - _steppers[i]->currentPosition) );
        //PORT.println((String) "maxsteps " + i + ": " + _maxSteps );
      }
      if(_counter>=_numOfAttachedSteppers) return;
      _maxSpeed = (md.moveDuration * S2US) / _maxSteps;
      _maxSpeed = round(_maxSpeed);
      if(_maxSpeed < MAX_STEPPER_SPEED) _maxSpeed = MAX_STEPPER_SPEED;
      //PORT.println((String) "maxsteps: " + _maxSteps );
      //PORT.println((String) "md with maxSpeed: " + _maxSpeed );
      for(ui8 i = 0; i < _numOfAttachedSteppers; i++){
        if(md.Designer[i].isActive){
          _steppers[i]->setDesiredPosition(md.Designer[i].position);
          int32_t steps2move = abs(md.Designer[i].position - _steppers[i]->currentPosition);
          _steppers[i]->lapse = _maxSpeed * (_maxSteps / (steps2move != 0 ? steps2move : 1));
          //PORT.println((String) "lapse " + i + ": " + _steppers[i]->lapse);
        }
        else {
          _steppers[i]->setStepsToZero();
        }
      }
      stateled.Red();
      _isExecutingMovement = true;
    }
    else if(repeat){
      //PORT.println("Resetting _designerQueue");
      _designerQueue.reset();
    }
  } else {
    _counter=0;
    for(ui8 i = 0; i < MAX_STEPPERS; i++){
      if(_steppers[i] != 0){
        _steppers[i]->update();
        if(_steppers[i]->_currentStepsToTake == 0) _counter++;
      }
    }
    if(_counter >= _numOfAttachedSteppers){
      stateled.Green();
      _isExecutingMovement = false;
    }
  }
}

