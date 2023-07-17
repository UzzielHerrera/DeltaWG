#include "State.h"

StateLed::StateLed(){
  _blinkCycleTime = TIME_BETWEEN_BLINK_CYCLES;
  _blinkLapse = BLINK_LAPSE;
  _canBlink = true;
  _lastBlink = 0;
  _lastBlinkCycle = 0;
  _currentBlink = 0;
  _lastState = false;
  blinksPerCycle = 0;
  pinMode(RED_LED_PIN, OUTPUT);
  digitalWrite(RED_LED_PIN, 1);
  pinMode(GREEN_LED_PIN, OUTPUT);
  digitalWrite(GREEN_LED_PIN, 1);
  pinMode(EXECUTING_ISR_CODE, OUTPUT);
  digitalWrite(EXECUTING_ISR_CODE, 0);
  pinMode(MODE_LED_PIN, OUTPUT);
  digitalWrite(MODE_LED_PIN, 0);
  PORT.println("State Led Module initialized.");
}

void StateLed::Green(){
  digitalWrite(RED_LED_PIN, 1);
  digitalWrite(GREEN_LED_PIN, 0);
}

void StateLed::Red(){
  digitalWrite(GREEN_LED_PIN, 1);
  digitalWrite(RED_LED_PIN, 0);
}

void StateLed::isr_on(){
  digitalWrite(EXECUTING_ISR_CODE, 1);
}

void StateLed::isr_off(){
  digitalWrite(EXECUTING_ISR_CODE, 0);
}

void StateLed::BlinkUpdate(){
  if(blinksPerCycle!=0){
    if(!_canBlink && millis() - _lastBlinkCycle >= _blinkCycleTime){
      //PORT.println("Resetting blink.");
      _lastBlinkCycle = millis();
      _canBlink = true;
      //PORT.println((String) "_canBlink: " + _canBlink);
    }

    if(_canBlink && millis() - _lastBlink >= _blinkLapse){
      _lastBlink = millis();
      if(_currentBlink < blinksPerCycle){
        //PORT.println("blinking");
        digitalWrite(MODE_LED_PIN, !_lastState);
        if(_lastState) _currentBlink++;
        _lastState = !_lastState;
      } else {
        _currentBlink = 0;
        _canBlink = false;
      }
    }
  } else {
    if(!_lastState){
      digitalWrite(MODE_LED_PIN, 1);
    }
  }
}
