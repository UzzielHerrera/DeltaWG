#ifndef Data_h
#define Data_h

#include "Constants.h"
#include "Arduino.h"

class Datas {
  public:
    Datas();
    Point currentPoint;
    Point desiredPoint;
    Point homePoint;

    Angle currentAngle;
    Angle desiredAngle;
    Angle homeAngle;
    bool isExecutedGCode;
    float zOffset;
    
    ui32 sleepTime;
    ui32 lastSleepSet;
    bool isSleeping;
};

extern Datas data;


#endif