#ifndef Tools_h
#define Tools_h

#include "Constants.h"
#include "Arduino.h"
#include "Data.h"
#include "MoveDesigner.h"

class Tools{
  public:
    Tools();
    Point Convert2Point(float x, float y, float z);
    Angle Convert2Angle(float theta1, float theta2, float theta3);
    Vector GetVector(Point initialPoint, Point endPoint);
    void PrintVector(Vector vector, String vectorName);
    void PrintPoint(Point point, String pointName);
    void PrintAngle(Angle angle, String angleName);
    void PrintMoveDesigner(MoveDesigner md, String moveDesignerName);
    bool ChekingDesiredPoint(Point point);
    bool ChekingDesiredAngle(Angle angle);
    Point GetPointInLine(Point initialPoint, Point endPoint, float t);
    Point GetPointInCircle(float ox, float oy, float radius, float angle);
    bool CheckFuplicatione(Point initialPoint, Point endPoint);
    float DistanceBetweenPoint(Point initialPoint, Point endPoint);
    bool Sleep(void);
    void setSleepTime(ui32 time);
};

extern Tools tool;

#endif