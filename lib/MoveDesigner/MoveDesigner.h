#ifndef MoveDesigner_h
#define MoveDesigner_h
#include "Constants.h"
#include "Arduino.h"

struct MoveCommand{
  bool isActive;
  int32_t position;
};

class MoveDesigner{
  public:
    MoveDesigner();
    void reset();
    void addMove(ui8 id, int32_t pos);
    MoveCommand Designer[MAX_STEPPERS];
    bool isActive;
    double moveDuration;
};

#endif