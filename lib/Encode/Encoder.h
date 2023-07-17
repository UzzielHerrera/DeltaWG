#ifndef Encoder_h
#define Encoder_h

#include "Constants.h"
#include "Arduino.h"

enum SCALER{
  MEDIO,
  UNO,
  DOS,
  CINCO,
};

class Encoder
{
  public:
    Encoder(int pinSignalA, int pinSignalB);
    void update();
    int32_t count;
    int currentRotation;
    ui8 preScaler;
    SCALER nextState;
    // currentRotation:
    // 0 -> rotating CCW
    // 1 -> rotating CW
  private:
    bool _btnState;
    uint16_t _cooldownCounter;
    int _pinSignalA;
    int _pinSignalB;
    int _state;
    int _lastState;
};

#endif