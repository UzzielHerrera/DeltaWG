#ifndef ManualController_h
#define ManualController_h

#include "Constants.h"
#include "Arduino.h"
#include "Stepper.h"
#include "Queue.h"
#include "MoveDesigner.h"
#include "State.h"

class ManualController{
  public:
    ManualController();
    void addMoveDesigner(MoveDesigner md);
    void cancelMoves();
    void update();
    void attach(Stepper *stepper);
    bool _isMoving;
  private:
    ui8 _counter;
    ui8 _numOfAttachedSteppers;
    Stepper *_steppers[MAX_STEPPERS];
    Queue<MoveDesigner> _manualQueue = Queue<MoveDesigner>(MAX_DESIGNER_MOVES);
};

extern ManualController manualControl;

#endif