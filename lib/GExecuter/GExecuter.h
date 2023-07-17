#ifndef GExecuter_h
#define GExecuter_h

#include "Constants.h"
#include "Arduino.h"
#include "Data.h"
#include "GReceiver.h"
#include "Motion.h"

#define NULL_NUMBER 98765

class KeyValue{
  public:
    char Key;
    float Value;
};

class GExecuter{
  public:
    GExecuter();
    void Run();
    void WhenFinishMove();
    bool isRunning;
    float X, Y, Z, E, S, A, I, J, F, P, R, Q, W;

  private:
    void getKeyValues(String code);
    Queue<KeyValue> keyValueQueue = Queue<KeyValue>(MAX_GPARAMETERS);
    void MotionRunner(KeyValue keyValue);
    void ResetValue();
};

extern GExecuter gExecuter;

#endif