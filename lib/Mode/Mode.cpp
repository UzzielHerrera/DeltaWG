#include "Mode.h"
#include "ManualController.h"
#include "StepperController.h"
#include "State.h"

Mode::Mode(modeEnum initialState){
  currentMode = initialState;
  counter = 0;
  pinMode(BTN_MODE, INPUT);
  _lastState = digitalRead(BTN_MODE);
  stateled.blinksPerCycle = 1;
  stateled._blinkLapse = 200;
  stateled._blinkCycleTime = 500;
  PORT.println("Mode Module initialized.");
}

void Mode::checkModeChange(){
  if(counter < BTM_MODE_COOLDOWN){
    counter++;
    return;
  }
  if(_lastState != digitalRead(BTN_MODE)) return;
  if(digitalRead(BTN_MODE)==false && _lastState==false && counter == BTM_MODE_COOLDOWN){
    _lastState = true;
  }
  if(counter == BTM_MODE_COOLDOWN && digitalRead(BTN_MODE)==true && _lastState==true){
    counter = 0;
    _lastState = false;
    switch(currentMode){
      case CONTROLLED_INCOMING_MOVEMENTS:
        StepperControl.cancelMoves();
        StepperControl.repeat = true;
        currentMode = CONTROLLED_STORAGED_MOVEMENTS;
        stateled.blinksPerCycle = 0;
        PORT.println("currentMode: CONTROLLED_STORAGED_MOVEMENTS");
        break;
      case CONTROLLED_STORAGED_MOVEMENTS:
        StepperControl.cancelMoves();
        currentMode = MANUAL_ARM1;
        stateled.blinksPerCycle = 1;
        stateled._blinkLapse = 250;
        stateled._blinkCycleTime = 1500;
        PORT.println("currentMode: MANUAL_ARM1");
        break;
      case MANUAL_ARM1:
        manualControl.cancelMoves();
        currentMode = MANUAL_ARM2;
        stateled.blinksPerCycle = 2;
        stateled._blinkLapse = 200;
        stateled._blinkCycleTime = 1500;
        PORT.println("currentMode: MANUAL_ARM2");
        break;
      case MANUAL_ARM2:
        manualControl.cancelMoves();
        currentMode = MANUAL_ARM3;
        stateled.blinksPerCycle = 3;
        stateled._blinkLapse = 200;
        stateled._blinkCycleTime = 2000;
        PORT.println("currentMode: MANUAL_ARM3");
        break;
      case MANUAL_ARM3:
        manualControl.cancelMoves();
        StepperControl.repeat = false;
        currentMode = CONTROLLED_INCOMING_MOVEMENTS;
        stateled.blinksPerCycle = 1;
        stateled._blinkLapse = 200;
        stateled._blinkCycleTime = 500;
        PORT.println("currentMode: CONTROLLED_INCOMING_MOVEMENTS");
        break;
    }
    
  }
}