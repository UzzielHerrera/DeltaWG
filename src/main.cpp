#include <Arduino.h>
#include "Constants.h"
#include "Mode.h"
#include "GReceiver.h"
#include "DeltaKinematics.h"
#include "Stepper.h"
#include "StepperController.h"
#include "ManualController.h"
#include "Motion.h"
#include "Data.h"
#include "Tools.h"
#include "MoveDesigner.h"
#include "GExecuter.h"
#include "Encoder.h"
#include "State.h"

// --- Module's Initialize.
GReceiver gReceiver;
GExecuter gExecuter;
Tools tool;
Datas data;
DeltaKinematics DKinematics;
StepperController StepperControl;
ManualController manualControl;
Motion motion;
Mode mode(CONTROLLED_INCOMING_MOVEMENTS);
StateLed stateled;

// --- Stepper creation.
Stepper stepperArm1(ARM1_STEP_PIN, ARM1_DIR_PIN, ARM1_SWITCH, 0);
Stepper stepperArm2(ARM2_STEP_PIN, ARM2_DIR_PIN, ARM2_SWITCH, 1);
Stepper stepperArm3(ARM3_STEP_PIN, ARM3_DIR_PIN, ARM3_SWITCH, 2);

// --- Encoder creation.
Encoder Encoder1(MANUAL_ENCODER_A, MANUAL_ENCODER_B);
int16_t encoderLastCount = 0;
int difference;

// --- Common MoveDesigners.
MoveDesigner manualDesigner;
MoveDesigner calibrationManualDesigner;

// --- Timer.
hw_timer_t *timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
volatile SemaphoreHandle_t timerSemaphore;

void ManualMove(ui8 id);
void CalibrationProcess(void);
void StepperCalibrate(ui8 id);
void TestProcess(void);
void configuraInitialDatas(void);

void IRAM_ATTR onTimer(){
  stateled.isr_on();
  stateled.BlinkUpdate();
  mode.checkModeChange();
  switch(mode.currentMode){
    case MANUAL_ARM1:
      ManualMove(0);
      break;
    case MANUAL_ARM2:
      ManualMove(1);
      break;
    case MANUAL_ARM3:
      ManualMove(2);
      break;
    case CONTROLLED_INCOMING_MOVEMENTS:
      gReceiver.PORTCheck();
    case CONTROLLED_STORAGED_MOVEMENTS:
      portENTER_CRITICAL_ISR(&timerMux);
      StepperControl.update();
      portEXIT_CRITICAL_ISR(&timerMux);
      break;
  }
  xSemaphoreGiveFromISR(timerSemaphore, NULL);
  stateled.isr_off();
}

void setup() {
  timerSemaphore = xSemaphoreCreateBinary();

  StepperControl.attach(&stepperArm1);
  StepperControl.attach(&stepperArm2);
  StepperControl.attach(&stepperArm3);
  manualControl.attach(&stepperArm1);
  manualControl.attach(&stepperArm2);
  manualControl.attach(&stepperArm3);
  
  // --- Calibration process.
  CalibrationProcess();
  StepperControl.setSteppersToZero();
  data.currentPoint = data.homePoint;
  data.currentAngle = data.homeAngle;
  tool.PrintAngle(data.currentAngle, "Current");
  tool.PrintPoint(data.currentPoint, "Current");

  // --- Stepper initial test.
  //TestProcess();

  // Initialize timer with 100 usecs period.
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 100, true);
  timerAlarmEnable(timer);

  // --- Current mode print;
  PORT.println("Main Routine Setup finished.");
  PORT.println("Current Mode: CONTROLLED_INCOMING_MOVEMENTS.");
}

void loop() {
  gExecuter.Run();
  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE){
      int32_t pos = 0;
      portENTER_CRITICAL(&timerMux);
      pos = stepperArm1.currentPosition;
      portEXIT_CRITICAL(&timerMux);
      delay(10);
  }
}

void ManualMove(ui8 id){
  Encoder1.update();
  difference = Encoder1.count - encoderLastCount;
  if(difference != 0){
    //PORT.println((String) "Difference: " + difference );
    //PORT.println((String) "Encoder1.count: " + Encoder1.count );
    //PORT.println((String) "encoderLastCount: " + encoderLastCount );
    manualDesigner.addMove(id, difference*Encoder1.preScaler);
    manualControl.addMoveDesigner(manualDesigner);
    manualDesigner.reset();
  }
  encoderLastCount = Encoder1.count;
  manualControl.update();
}

void CalibrationProcess(void){
  bool latch[3] = {false, false, false};
  int armSwitches[3] = {ARM1_SWITCH, ARM2_SWITCH, ARM3_SWITCH};
  
  PORT.println("Calibration process initialized...");

  while(!latch[0] || !latch[1] || !latch[2]){

    for(uint8_t i = 0; i < MAX_STEPPERS; i++){
      if(digitalRead(armSwitches[i])) latch[i] = true;
      if(!latch[i]) calibrationManualDesigner.addMove(i, 1);
    }

    manualControl.addMoveDesigner(calibrationManualDesigner);
    do {
      manualControl.update();
    } while(manualControl._isMoving);
    calibrationManualDesigner.reset();
    delay(3);
  }
  PORT.println("Calibration process finished.");
}

void StepperCalibrate(ui8 id){
  calibrationManualDesigner.addMove(id, 1);
  manualControl.addMoveDesigner(calibrationManualDesigner);
  do {
    manualControl.update();
  } while(manualControl._isMoving);
  calibrationManualDesigner.reset();
}

void TestProcess(void){
  PORT.println("------\r\nTest Process load init.");
  Point buffer;
  MoveDesigner md;
  Angle abuf;
  /*
  buffer = tool.Convert2Point(0.0, 0.0, 150.0);
  md.moveDuration = 0.25;
  DKinematics.InverseKinematics(buffer, abuf);
  md = DKinematics.InverseCalculations(buffer, md);
  StepperControl.addMoveDesigner(md);
  tool.PrintAngle(abuf, "pushed angle");

  buffer = tool.Convert2Point(0.0, 0.0, 170.0);
  md.moveDuration = 0.25;
  DKinematics.InverseKinematics(buffer, abuf);
  md = DKinematics.InverseCalculations(buffer, md);
  StepperControl.addMoveDesigner(md);
  tool.PrintAngle(abuf, "pushed angle");

  buffer = tool.Convert2Point(0.0, 0.0, 190.0);
  md.moveDuration = 0.25;
  DKinematics.InverseKinematics(buffer, abuf);
  md = DKinematics.InverseCalculations(buffer, md);
  StepperControl.addMoveDesigner(md);
  tool.PrintAngle(abuf, "pushed angle");
  */
  buffer = tool.Convert2Point(0.0, 0.0, 200.0);
  md.moveDuration = 0.25;
  DKinematics.InverseKinematics(buffer, abuf);
  md = DKinematics.InverseCalculations(buffer, md);
  StepperControl.addMoveDesigner(md);
  tool.PrintAngle(abuf, "pushed angle");
  PORT.println("Test Process load finish.\r\n------");
}
