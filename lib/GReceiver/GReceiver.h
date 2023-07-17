#ifndef GReceiver_h
#define GReceiver_h

#include "Constants.h"
#include "Arduino.h"
#include "Queue.h"

class GReceiver{
  public:
    GReceiver();
    void PORTCheck();
    Queue<String> gCodeQueue = Queue<String>(MAX_GCODES);
  private:
    String _receiveString;
    String _stringBackUp;
    bool _isStringComplete;
    
};

extern GReceiver gReceiver;

#endif
