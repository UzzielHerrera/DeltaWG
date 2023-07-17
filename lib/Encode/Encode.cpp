#include "Encoder.h"

Encoder::Encoder(int pinSignalA, int pinSignalB){
  _pinSignalA = pinSignalA;
  _pinSignalB = pinSignalB;
  pinMode(_pinSignalA, INPUT);
  pinMode(_pinSignalB, INPUT);
  pinMode(MANUAL_ENCODER_SW, INPUT_PULLUP);
  count = 0;
  _lastState = digitalRead(_pinSignalA);
  currentRotation = 0;
  _cooldownCounter = 0;
  preScaler = 3;
  nextState = UNO;
}

void Encoder::update(){

  if(_cooldownCounter < MANUAL_ENCODER_COOLDOWN){
    _cooldownCounter++;
  }
  if(digitalRead(MANUAL_ENCODER_SW) && _btnState){
    _btnState = false;
  }
  if(_cooldownCounter == MANUAL_ENCODER_COOLDOWN && !digitalRead(MANUAL_ENCODER_SW) && !_btnState){
    _btnState = true;
    switch(nextState){
      case MEDIO:
        preScaler = 3;
        PORT.println("PRESCALER: X1/2");
        nextState = UNO;
        break;
      case UNO:
        preScaler = 6;
        PORT.println("PRESCALER: X1");
        nextState = DOS;
        break;
      case DOS:
        preScaler = 12;
        PORT.println("PRESCALER: X2");
        nextState = CINCO;
        break;
      case CINCO:
        preScaler = 30;
        PORT.println("PRESCALER: X5");
        nextState = MEDIO;
        break;
    }
    _cooldownCounter = 0;
  }

  _state = digitalRead(_pinSignalA);
  if(_state != _lastState){
    if(digitalRead(_pinSignalB) != _state){
      count++;
      currentRotation = 1;
    } else {
      count--;
      currentRotation = 0;
    }
    //PORT.println(count);
  }
  _lastState = _state;
}
