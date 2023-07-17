#include "Constants.h"
#include "Arduino.h"
#include "DeltaKinematics.h"
#include "MoveDesigner.h"
#include "Tools.h"
#include "Data.h"

DeltaKinematics::DeltaKinematics(){
  _deltaE = DELTA_E;
  _deltaF = DELTA_F;
  _deltaRE = DELTA_RE;
  _deltaREpow2 = _deltaRE * _deltaRE;
  _deltaRF = DELTA_RF;
  _deltaRFpow2 = _deltaRF * _deltaRF;
  _y0 = 0.5 * tan30 * DELTA_E;
  _y1 = -0.5 * tan30 * DELTA_F;
  nanErrorOccured = false;
  pinMode(NAN_LED_PIN, OUTPUT);
  PORT.println("Delta Kinematics Module initialized.");
  for(ui8 i = 0; i < 3; i++){
    _stepsError[i] = 0;
    _realSteps[i] = 0;
    _stepsToTake[i] = 0;
  }
}

bool DeltaKinematics::AngleThetaCalculations(float x0, float y0, float z0, float &theta){
  float y1 = -0.5 * tan30 * _deltaF;
  //PORT.println((String) "y1" + y1);
	y0 -= 0.5 * tan30 * _deltaE;
  //PORT.println((String) "y0" + y0);
  float a = (x0*x0 + y0*y0 + z0*z0 + _deltaRFpow2 - _deltaREpow2 - y1*y1) / (2.0*z0);
  //PORT.println((String) "a" + a);
	float b = (y1 - y0) / z0;
  //PORT.println((String) "b" + b);
  float d = -(a + b * y1)*(a + b * y1) + b*b*_deltaRFpow2 + _deltaRFpow2;
  //PORT.println((String) "d" + d);
  if (d < 0) return false;
  float yj = (y1 - a * b - sqrt(d)) / (b*b + 1.0);
  //PORT.println((String) "yj" + yj);
	float zj = a + b * yj;
  //PORT.println((String) "zj" + zj);
  theta = atan(-zj / (y1 - yj)) * RAD_TO_DEG + ((yj > y1) ? 180.0 : 0.0);
  //PORT.println((String) "theta" + theta);
	return true;
}

void DeltaKinematics::NAN_OCCURED(){
  digitalWrite(NAN_LED_PIN, HIGH);
  nanErrorOccured = true;
  Serial.println("NaN error.");
}

MoveDesigner DeltaKinematics::InverseCalculations(Point point, MoveDesigner &md){
  Angle angle;
  if(!AngleThetaCalculations( point.x, point.y, point.z, angle.theta1)) NAN_OCCURED();
  if(!AngleThetaCalculations(point.x*cos120 + point.y*sin120, point.y*cos120 - point.x*sin120, point.z, angle.theta2)) NAN_OCCURED();
  if(!AngleThetaCalculations(point.x*cos120 - point.y*sin120, point.y*cos120 + point.x*sin120, point.z, angle.theta3)) NAN_OCCURED();

  _realSteps[0] = (THETA1_STEPS_PER_2PI * (angle.theta1 / 360))-_stepsError[0];
  _realSteps[1] = (THETA2_STEPS_PER_2PI * (angle.theta2 / 360))-_stepsError[1];
  _realSteps[2] = (THETA3_STEPS_PER_2PI * (angle.theta3 / 360))-_stepsError[2];
  
  for(ui8 i = 0; i < 3; i ++){
    _stepsToTake[i] = round(_realSteps[i]);
    //PORT.println((String) "_stepsToTake:" + _stepsToTake[i]);
    _stepsError[i] = _stepsToTake[i] - _realSteps[i];
    md.addMove(i, _stepsToTake[i]);
    //PORT.println((String) "STT: " + _stepsToTake[i] + " - RT: " + _realSteps[i] + " - E: " + _stepsError[i]);
  }
  return md;
}

bool DeltaKinematics::InverseKinematics(Point point, Angle &angle){
  if(!AngleThetaCalculations( point.x, point.y, point.z, angle.theta1)) return false;
  if(!AngleThetaCalculations(point.x*cos120 + point.y*sin120, point.y*cos120 - point.x*sin120, point.z, angle.theta2)) return false;
  if(!AngleThetaCalculations(point.x*cos120 - point.y*sin120, point.y*cos120 + point.x*sin120, point.z, angle.theta3)) return false;
  return true;
}

Point DeltaKinematics::FowardCalculations(Angle angle){
  float theta1 = DEG_TO_RAD * angle.theta1;
  float theta2 = DEG_TO_RAD * angle.theta2;
  float theta3 = DEG_TO_RAD * angle.theta3;

  float t = (DELTA_F - DELTA_E) * tan30 / 2.0;

  float y1 = -(t + DELTA_RF * cos(theta1));
  float z1 = -DELTA_RF * sin(theta1);

  float y2 = (t + DELTA_RF * cos(theta2)) * sin30;
  float x2 = y2 * tan60;
  float z2 = -DELTA_RF * sin(theta2);

  float y3 = (t + DELTA_RF * cos(theta3))*sin30;
	float x3 = -y3 * tan60;
	float z3 = -DELTA_RF * sin(theta3);

  float dnm = (y2 - y1)*x3 - (y3 - y1)*x2;

	float w1 = y1 * y1 + z1 * z1;
	float w2 = x2 * x2 + y2 * y2 + z2 * z2;
	float w3 = x3 * x3 + y3 * y3 + z3 * z3;

	// x = (a1*z + b1)/dnm
	float a1 = (z2 - z1)*(y3 - y1) - (z3 - z1)*(y2 - y1);
	float b1 = -((w2 - w1)*(y3 - y1) - (w3 - w1)*(y2 - y1)) / 2.0;

	// y = (a2*z + b2)/dnm;
	float a2 = -(z2 - z1)*x3 + (z3 - z1)*x2;
	float b2 = ((w2 - w1)*x3 - (w3 - w1)*x2) / 2.0;

  // a*z^2 + b*z + c = 0
	float a = a1 * a1 + a2 * a2 + dnm * dnm;
	float b = 2 * (a1*b1 + a2 * (b2 - y1 * dnm) - z1 * dnm*dnm);
	float c = (b2 - y1 * dnm)*(b2 - y1 * dnm) + b1 * b1 + dnm * dnm*(z1*z1 - DELTA_RE * DELTA_RE);

	// discriminant
	float d = b * b - (float)4.0*a*c;
	if (d < 0) NAN_OCCURED();

  Point point;
	point.z = ((-(float)0.5*(b + sqrt(d)) / a)) * -1;
	point.x = (a1*point.z + b1) / dnm;
	point.y = (a2*point.z + b2) / dnm;

	return point;
}
