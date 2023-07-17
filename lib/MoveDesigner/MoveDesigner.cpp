#include "Constants.h"
#include "Arduino.h"
#include "MoveDesigner.h"

MoveDesigner::MoveDesigner(){
  isActive = false;
  moveDuration = 1.0;
  for(ui8 i = 0; i < MAX_STEPPERS; i++){
    Designer[i] = {false, 0};
  }
}

void MoveDesigner::addMove(uint8_t id, int32_t pos){
  isActive = true;
  if(id < MAX_STEPPERS){
    Designer[id] = {true, pos};
  }
}

void MoveDesigner::reset(){
  for(ui8 i = 0; i < MAX_STEPPERS; i++){
    Designer[i] = {false, 0};
  }
}