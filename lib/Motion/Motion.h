#ifndef Motion_h
#define Motion_h

#include "Constants.h"
#include "Arduino.h"
#include "Tools.h"
#include "MoveDesigner.h"
#include "DeltaKinematics.h"
#include "StepperController.h"
#include "Data.h"

class Motion{
  public:
    Motion();
    void G0(float x, float y, float z);
    void G1(float x, float y, float z);
    void G2(float x, float y, float i, float j);
    void G3(float x, float y, float i, float j);
    void G4(float p);
    void G28();
    double fSpeed;  // mm / min
  private:
    ui8 segmentNumber;
    bool LinearInterpolation(void);
    bool CircularInterpolation(float i, float j, bool CW);
    void SetHomePosition();
    double _realSteps[3];
    double _stepsError[3];
    int32_t _stepsToTake[3];
};

extern Motion motion;

#endif