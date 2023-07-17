#include "Data.h"
#include "Tools.h"

Datas::Datas(){
    currentPoint = tool.Convert2Point(0.0, 0.0, 0.0);
    desiredPoint = tool.Convert2Point(0.0, 0.0, 0.0);
    homePoint = tool.Convert2Point(0.0, 0.0, 0.0);

    currentAngle = tool.Convert2Angle(0.0, 0.0, 0.0);
    desiredAngle = tool.Convert2Angle(0.0, 0.0, 0.0);
    homeAngle = tool.Convert2Angle(THETA1_HOME_POSITION, THETA2_HOME_POSITION, THETA3_HOME_POSITION);
    
    lastSleepSet = millis();
    isSleeping = false;
    sleepTime = 0;

    zOffset = UPPER_BASE_OFFSET - END_EFFECTOR_OFFSET - LOWER_BASE_OFFSET;
    isExecutedGCode = false;
    PORT.println("Data Module initialized.");
}
