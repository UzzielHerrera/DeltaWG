#include "Tools.h"

Tools::Tools(){
  PORT.println("Tools Module initialized.");
}

Angle Tools::Convert2Angle(float theta1, float theta2, float theta3){
  Angle buffer;
  buffer.theta1 = theta1;
  buffer.theta2 = theta2;
  buffer.theta3 = theta3;
  return buffer;
}

Point Tools::Convert2Point(float xPos, float yPos, float zPos){
  Point buffer;
  buffer.x = xPos;
  buffer.y = yPos;
  buffer.z = zPos;
  return buffer;
}

void Tools::PrintPoint(Point point, String pointName){
  PORT.println("Point " + pointName + ": [ " + String(point.x, 2) +", " + String(point.y, 2) + ", " + String(point.z, 2) +"]");
}

void Tools::PrintMoveDesigner(MoveDesigner md, String moveDesignerName){
  PORT.print("MoveDesigner" + moveDesignerName + "{");
  for(uint8_t i = 0; i < 3; i++){
    PORT.print(" " + String(md.Designer[i].position) + " ");
  }
  PORT.println("}");
}

void Tools::PrintAngle(Angle angle, String angleName){
  PORT.print("Angle " + angleName + ": ");
  PORT.print("{Theta1: " + String(angle.theta1));
  PORT.print(",Theta2: " + String(angle.theta2));
  PORT.println(",Theta3: " + String(angle.theta3)+"}");
}

Vector Tools::GetVector(Point initialPoint, Point endPoint){
  Vector buffer;
  buffer.x = endPoint.x - initialPoint.x;
  buffer.y = endPoint.y - initialPoint.y;
  buffer.z = endPoint.z - initialPoint.z;
  return buffer;
}

void Tools::PrintVector(Vector vector, String vectorName){
  PORT.println("Vector " + vectorName + ": [ " + String(vector.x, 2) +", " + String(vector.y, 2) + ", " + String(vector.z, 2) +"]");
}

Point Tools::GetPointInLine(Point initialPoint, Point endPoint, float t){
  Point buffer;
  buffer.x = initialPoint.x - ((initialPoint.x - endPoint.x) * t);
  buffer.y = initialPoint.y - ((initialPoint.y - endPoint.y) * t);
  buffer.z = initialPoint.z - ((initialPoint.z - endPoint.z) * t);
  return buffer;
}

Point Tools::GetPointInCircle(float ox, float oy, float radius, float angle){
  Point buffer;
  buffer.x = ox + radius * cosf(angle);
  buffer.y = oy + radius * sinf(angle);
  buffer.z = data.currentPoint.z;
  return buffer;
}

bool Tools::ChekingDesiredAngle(Angle angle){
  if (angle.theta1 > MAX_THETA1)
    return false;
  if (angle.theta2 > MAX_THETA2)
    return false;
  if (angle.theta3 > MAX_THETA3)
    return false;
  return true;
}

bool Tools::ChekingDesiredPoint(Point point)  {
  if(point.z > MOVING_AREA_Z) return false;
  //PORT.println("Z Good");
  if(sqrt(pow(point.x,2) + pow(point.y,2)) > MOVING_AREA_LARGEST_DIAMETER)
    return false;
  //PORT.println("AREA Goods");
  data.desiredPoint = point;
  return true;
}

bool Tools::CheckFuplicatione(Point initialPoint, Point endPoint){
  Point buffer;
  buffer.x = initialPoint.x - endPoint.x;
  buffer.y = initialPoint.y - endPoint.y;
  buffer.z = initialPoint.z - endPoint.z;
  
  if(buffer.x == 0 && buffer.y == 0 && buffer.z == 0){
    return true;
  } else {
    return false;
  }
}

float Tools::DistanceBetweenPoint(Point initialPoint, Point endPoint){
  Vector buffer = GetVector(initialPoint, endPoint);
  float distance = sqrt( pow(buffer.x, 2) + pow(buffer.y, 2) + pow(buffer.z, 2));
  if (distance < 0.2 && distance > 0.2) distance = 0;
  return distance;
}

bool Tools::Sleep(void){
  if(millis()-data.lastSleepSet < data.sleepTime) return false;
  else {
    data.isSleeping = false;
    data.isExecutedGCode = true;
    return true;
  }
}

void Tools::setSleepTime(ui32 time){
  data.sleepTime = time;
  data.lastSleepSet = millis();
  data.isSleeping = true;
}
