#ifndef State_h
#define State_h

#include "Constants.h"
#include "Arduino.h"
//#include "Mode.h"

class StateLed{
  public:
    StateLed();
    void Red();
    void Green();
    void isr_on();
    void isr_off();
    void BlinkUpdate();
    int blinksPerCycle;
    uint16_t _blinkCycleTime;
    uint16_t _blinkLapse;
  private:
    bool _canBlink;
    ui32 _lastBlinkCycle;
    ui32 _lastBlink;
    bool _lastState;
    int _currentBlink;
};

extern StateLed stateled;

#endif