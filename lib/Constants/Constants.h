#ifndef Constants_h
#define Constants_h
#include "Arduino.h"

typedef struct{
  float x;
  float y;
  float z;
} Point;

typedef struct{
  float x;
  float y;
  float z;
} Vector;

typedef struct{
  float theta1;
  float theta2;
  float theta3;
} Angle;

#define ui8 uint8_t
#define ui32 uint32_t

#define RXD2 16
#define TXD2 17

#define PORT Serial
#define BAUDRATE 115200

#define S2US 1000000

// Pin Definition.
#define ARM1_STEP_PIN 4
#define ARM1_DIR_PIN 2
#define ARM2_STEP_PIN 18
#define ARM2_DIR_PIN 5
#define ARM3_STEP_PIN 23
#define ARM3_DIR_PIN 19

#define BTN_MODE 35
#define BTM_MODE_COOLDOWN 3000

#define ARM1_SWITCH 34
#define ARM2_SWITCH 39
#define ARM3_SWITCH 36

#define MANUAL_ENCODER_COOLDOWN 3000
#define MANUAL_ENCODER_A 13
#define MANUAL_ENCODER_B 12
#define MANUAL_ENCODER_SW 32
#define MANUAL_ENCODER_SW_COOLDOWN 5000
#define ENCODER_SCALER 10

#define MAX_STEPPER_SPEED 2000 //uSec for step

#define RED_LED_PIN 25
#define GREEN_LED_PIN 26
#define EXECUTING_ISR_CODE 27
#define NAN_LED_PIN 14
#define MODE_LED_PIN 33

#define BLINK_LAPSE 150
#define TIME_BETWEEN_BLINK_CYCLES 1500

#define MAX_STEPPERS 3
#define MAX_DESIGNER_MOVES 100
#define MAX_GCODES 60
#define MAX_GPARAMETERS 14

#define DEFAULT_FEED_SPEED 3000.0

#define MOVING_AREA_X 150.0
#define MOVING_AREA_Y 150.0
#define MOVING_AREA_Z 225.0

#define MOVING_AREA_LARGEST_DIAMETER 300

#define UPPER_BASE_OFFSET 21.5
#define LOWER_BASE_OFFSET 10.0
#define END_EFFECTOR_OFFSET 15.0
#define THETA1_HOME_POSITION 17.00
#define THETA2_HOME_POSITION 17.00
#define THETA3_HOME_POSITION 17.00

#define MAX_THETA1 20.0
#define MAX_THETA2 20.0
#define MAX_THETA3 20.0

//#define LCD_SDA 21
//#define LCD_SCL 22
#define LCD_DIRECTION 39
#define LCD_COLUMNS 20
#define LCD_ROWS 4

// Geometry.
#define DELTA_E 100.0             //mm
#define DELTA_F 230.0             //mm
#define DELTA_RE 158.0            //mm
#define DELTA_RF 70.0             //mm

#define MM_PER_LINEAR_SEGMENT 2.0  //mm

#define THETA1_STEPS_PER_2PI 2048
#define THETA2_STEPS_PER_2PI 2048
#define THETA3_STEPS_PER_2PI 2048

#define M_PI 3.14159265358979323846
#define M_PI_2 1.57079632679489661923

#endif