#ifndef StepperController_h
#define StepperController_h

#include "Constants.h"
#include "Arduino.h"
#include "Stepper.h"
#include "Queue.h"
#include "MoveDesigner.h"

class StepperController{
  public:
    StepperController();
    void update();
    void attach(Stepper *stepper);
    void cancelMoves();
    void addMoveDesigner(MoveDesigner md);
    void addMotionMoveDesigner(MoveDesigner md);
    MoveDesigner popMoveDesigner();
    MoveDesigner peekMoveDesigner();
    bool repeat;
    void setSteppersToZero();

  private:
    void setFrequencyFrom(double moveDuration);
    bool _isExecutingMovement;
    bool _endlessRepeat;
    ui8 _numOfAttachedSteppers;
    ui32 _counter = 0;
    int32_t _maxSteps;
    double _maxSpeed;
    uint16_t _speeds[MAX_STEPPERS];
    Queue<MoveDesigner> _designerQueue = Queue<MoveDesigner>(MAX_DESIGNER_MOVES);
    Stepper *_steppers[MAX_STEPPERS];
    double _frequency;
};

extern StepperController StepperControl;

#endif