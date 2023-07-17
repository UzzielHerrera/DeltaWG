#include "ManualController.h"


ManualController::ManualController(){
  _counter = 0;
  _isMoving = false;
  _numOfAttachedSteppers = 0;
  for(ui8 i = 0; i < MAX_STEPPERS; i++){
    _steppers[i] = {0};
  }
  PORT.println("Steppers Manual Control Module initialized.");
}

void ManualController::cancelMoves(){
  _manualQueue.clear();
  stateled.Green();
  for(ui8 i = 0; i < _numOfAttachedSteppers; i++){
    if(_steppers[i]!=0){
      _steppers[i]->_manualSteps = 0;
      _steppers[i]->setStep2Low();
    }
  }
}

void ManualController::attach(Stepper *stepper){
  if (stepper->id < MAX_STEPPERS){
    _steppers[stepper->id] = stepper;
    _numOfAttachedSteppers++;
  }
}

void ManualController::addMoveDesigner(MoveDesigner md){
  _manualQueue.push(md);
}

void ManualController::update(){
  if(!_isMoving){
    if(_manualQueue.peek().isActive && _manualQueue.count() > 0){
      //PORT.println("pop manual md");
      MoveDesigner md = _manualQueue.pop();
      for(ui8 i = 0; i < _numOfAttachedSteppers; i++){
        if(md.Designer[i].isActive){
          bool dir = md.Designer[i].position > 0 ? true : false;
          _steppers[i]->_manualSteps += abs(md.Designer[i].position);
          _steppers[i]->setManualDirection(dir);
          //PORT.println(md.Designer[i].position);
          //PORT.println(( String) "CurrPos: " + _steppers[i]->currentPosition);
        }
      }
      stateled.Red();
      _isMoving = true;
    }
  } else {
    _counter = 0;
    for(ui8 i = 0; i < _numOfAttachedSteppers; i++){
      if(_steppers[i]!=0){
        _steppers[i]->manualUpdate();
        if(_steppers[i]->_manualSteps == 0) _counter++;        
      }
    }
    if(_counter >= _numOfAttachedSteppers){
      //PORT.println((String) "Counter: " + _counter );
      stateled.Green();
      _isMoving = false;
    }
  }
}
