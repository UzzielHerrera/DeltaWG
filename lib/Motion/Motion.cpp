#include "Motion.h"
#include "Tools.h"

Motion::Motion(){
  data.homeAngle.theta1 = THETA1_HOME_POSITION;
  data.homeAngle.theta2 = THETA2_HOME_POSITION;
  data.homeAngle.theta3 = THETA3_HOME_POSITION;
  //Angle transitionAngle = tool.Convert2Angle(-THETA1_HOME_POSITION, -THETA2_HOME_POSITION, -THETA3_HOME_POSITION);
  //data.homePoint = DKinematics.FowardCalculations(transitionAngle);
  //data.homePoint.z -= data.zOffset;
  data.homePoint = tool.Convert2Point(0, 0, 98.05);
  //tool.PrintPoint(data.homePoint, "homePoint");
  PORT.println("Motion Module initialized.");
  fSpeed = DEFAULT_FEED_SPEED;
} 

void Motion::G0(float x, float y, float z){
  Point bufferPoint = tool.Convert2Point(x, y, z);
  MoveDesigner md;
  md = DKinematics.InverseCalculations(bufferPoint, md);
  md.moveDuration = 0.01;
  StepperControl.addMotionMoveDesigner(md);
  Angle bufferAngle;
  DKinematics.InverseKinematics(bufferPoint, bufferAngle);
  data.currentPoint = bufferPoint;
  data.currentAngle = bufferAngle;
  data.isExecutedGCode = true;
}

void Motion::G1(float x, float y, float z){
  Point bufferPoint = tool.Convert2Point(x, y, z + data.zOffset);
  //tool.PrintPoint(bufferPoint, "DesiredPoint");
  if(tool.ChekingDesiredPoint(bufferPoint) == false) return;
  //PORT.println("Entering Linear");
  if(LinearInterpolation() == false) return;
  data.isExecutedGCode = true;
  segmentNumber = 0;
}

void Motion::G2(float x, float y, float i, float j){
  Point bufferPoint = tool.Convert2Point(x, y, data.currentPoint.z);
  if(!tool.ChekingDesiredPoint(bufferPoint)) return;
  if(!CircularInterpolation(i, j, false)) return;
  data.isExecutedGCode = true;
  segmentNumber = 0;
}

void Motion::G3(float x, float y, float i, float j){
  Point bufferPoint = tool.Convert2Point(x, y, data.currentPoint.z);
  if(!tool.ChekingDesiredPoint(bufferPoint)) return;
  if(!CircularInterpolation(i, j, true)) return;
  data.isExecutedGCode = true;
  segmentNumber = 0;
}

void Motion::G4(float p){
  tool.setSleepTime(p);
}

void Motion::G28(){
  MoveDesigner md;
  md.addMove(0, (int32_t) (THETA1_STEPS_PER_2PI * data.homeAngle.theta1 / 360.0));
  md.addMove(1, (int32_t) (THETA2_STEPS_PER_2PI * data.homeAngle.theta2 / 360.0));
  md.addMove(2, (int32_t) (THETA3_STEPS_PER_2PI * data.homeAngle.theta3 / 360.0));
  //PORT.println("G28: HomeBatch added.");
  data.currentPoint = data.homePoint;
  data.currentAngle = data.homeAngle;
  StepperControl.addMoveDesigner(md);
  data.isExecutedGCode = true;
}

bool Motion::LinearInterpolation(void){
  float distance2Point = tool.DistanceBetweenPoint(data.currentPoint, data.desiredPoint);
  if (distance2Point == 0) {
    data.isExecutedGCode = true;
    return false;
  }

  Angle _angle;
  DKinematics.InverseKinematics(data.desiredPoint, _angle);
  if(!tool.ChekingDesiredAngle(_angle)){
    //data.isExecutedGCode = true;
    return false;
  }

  segmentNumber = floorf(distance2Point/MM_PER_LINEAR_SEGMENT);
  if (segmentNumber < 1)
    segmentNumber = 1;
  
  float tbuffer;
  Angle lastAngle = data.currentAngle;
  Angle currentAngle;
  float mm_per_segment = distance2Point / segmentNumber;

  for(uint16_t i = 1; i <= segmentNumber; i++){
    tbuffer = (float) i / segmentNumber;
    Point pointBuffer = tool.GetPointInLine(data.currentPoint, data.desiredPoint, tbuffer);
    MoveDesigner md;
    md.moveDuration = mm_per_segment / (fSpeed / 60);
    //PORT.println((String) "mm_per_segment: " +  mm_per_segment);
    //PORT.println((String) "md.duration: " + md.moveDuration );
    DKinematics.InverseKinematics(pointBuffer, currentAngle);
    md = DKinematics.InverseCalculations(pointBuffer, md);
    StepperControl.addMotionMoveDesigner(md);
    //tool.PrintMoveDesigner(md, "pushed");
    //PORT.println("md pushed.");
  }
  data.currentPoint = data.desiredPoint;
  data.currentAngle = currentAngle;
  return true;
}

bool Motion::CircularInterpolation(float i, float j, bool CW){
  float distance2Point = tool.DistanceBetweenPoint(data.currentPoint, data.desiredPoint);
  if (distance2Point == 0) {
    data.isExecutedGCode = true;
    return false;
  }
  if(i==0 && j==0){
    data.isExecutedGCode = true;
    return false;
  }
  float o_x = data.currentPoint.x + i,
        o_y = data.currentPoint.y + j,
        radius = sqrt(pow(i, 2) + pow(j, 2)),
        currentAngle, desiredAngle;
  //PORT.println((String) "o_x: " + o_x ); 
  //PORT.println((String) "o_y: " + o_y ); 
  //PORT.println((String) "radius: " + radius ); 

  currentAngle = acosf(-i/ radius);
  if(j>0) currentAngle = -currentAngle;
  desiredAngle = acosf((data.desiredPoint.x - o_x) / radius);
  if(data.desiredPoint.y - o_y <= 0) desiredAngle = -desiredAngle;
  float travelAngle = desiredAngle - currentAngle;
  if(travelAngle < 0) travelAngle += 2.0 * PI;
  if(CW) travelAngle -= 2.0 * PI;
  if(travelAngle > -0.01 && travelAngle < 0.01) travelAngle = 0;
  if(travelAngle == 0 && abs(data.currentPoint.x - data.desiredPoint.x) < 0.4 && abs(data.currentPoint.y - data.desiredPoint.y) < 0.4){
    if(CW) travelAngle = -2.0 * PI;
    else travelAngle = 2.0 * PI;
  }

  float flat_mm = radius * abs(travelAngle);
  segmentNumber = floorf(flat_mm / MM_PER_LINEAR_SEGMENT);
  if(segmentNumber < 7 && segmentNumber > 3) segmentNumber = floorf(flat_mm * 2 / MM_PER_LINEAR_SEGMENT);
  else if(segmentNumber <= 3) segmentNumber = floorf(flat_mm * 4 / MM_PER_LINEAR_SEGMENT);
  if(segmentNumber < 7) return false;
  float thetaPerSegment = travelAngle / segmentNumber;
  float mm_per_segment = flat_mm / segmentNumber;
  Angle lastAngle = data.currentAngle;
  Angle currAngle;

  Point endPointInCircle = tool.GetPointInCircle(o_x, o_y, radius, currentAngle + travelAngle);
  distance2Point = tool.DistanceBetweenPoint(endPointInCircle, data.desiredPoint);
  if(distance2Point > 0.5) return false;
  
  for(uint16_t i = 1; i < segmentNumber; i++){
    Point bufferPoint = tool.GetPointInCircle(o_x, o_y, radius, currentAngle + (float) i * thetaPerSegment);
    if(!DKinematics.InverseKinematics(bufferPoint, currAngle)) return false;
    MoveDesigner md;
    md.moveDuration = mm_per_segment / (fSpeed/60);
    md = DKinematics.InverseCalculations(bufferPoint, md);
    StepperControl.addMotionMoveDesigner(md);
    //tool.PrintMoveDesigner(md, "pushed");
  }

  if(!DKinematics.InverseKinematics(data.desiredPoint, currAngle)) return false;
  MoveDesigner md;
  md.moveDuration = mm_per_segment / (fSpeed/60);
  md = DKinematics.InverseCalculations(data.desiredPoint, md);
  StepperControl.addMotionMoveDesigner(md);
  data.currentPoint = data.desiredPoint;
  data.currentAngle = currAngle;
  return true;
  //tool.PrintMoveDesigner(md, "pushed");
}
