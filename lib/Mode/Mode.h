#ifndef Mode_h
#define Mode_h

#include "Constants.h"
#include "Arduino.h"

enum modeEnum{
  CONTROLLED_INCOMING_MOVEMENTS,
  CONTROLLED_STORAGED_MOVEMENTS,
  MANUAL_ARM1,
  MANUAL_ARM2,
  MANUAL_ARM3,
};

class Mode{
  public:
    Mode(modeEnum initialState);
    void checkModeChange();
    modeEnum currentMode;
  private:
    uint16_t counter;
    bool _lastState;
};

extern Mode mode;

#endif