#include "GExecuter.h"
#include "Motion.h"

GExecuter::GExecuter(){
  ResetValue();
  isRunning = false;
  keyValueQueue.clear();
  PORT.println("GExecuter Module initialized.");
}

void GExecuter::Run(){
  WhenFinishMove();
  if(isRunning == true || gReceiver.gCodeQueue.count()==0)
    return;
  String gCodeToRun = gReceiver.gCodeQueue.pop();

  getKeyValues(gCodeToRun);
  //PORT.println(gCodeToRun);

  KeyValue firstKeyValue;
  int _counter = 0;
  while(keyValueQueue.count() > 0){
    KeyValue currentKeyValue = keyValueQueue.pop();
    if(_counter == 0){
      firstKeyValue = currentKeyValue;      
      _counter++;
    }  
    switch (currentKeyValue.Key){
      case 'X':
        X = currentKeyValue.Value;
        //PORT.println((String) "X: " + X );
        break;
      case 'Y':
        Y = currentKeyValue.Value;
        //PORT.println((String) "Y: " + Y );
        break;
      case 'Z':
        Z = currentKeyValue.Value;
        //PORT.println((String) "Z: " + Z );
        break;
      case 'E':
        E = currentKeyValue.Value;
        //PORT.println((String) "E: " + E );
        break;
      case 'S':
        S = currentKeyValue.Value;
        //PORT.println((String) "S: " + S );
        break;
      case 'A':
        A = currentKeyValue.Value;
        //PORT.println((String) "A: " + A );
        break;
      case 'I':
        I = currentKeyValue.Value;
        //PORT.println((String) "I: " + I );
        break;
      case 'J':
        J = currentKeyValue.Value;
        //PORT.println((String) "J: " + J );
        break;
      case 'F':
        F = currentKeyValue.Value;
        //PORT.println((String) "F: " + F );
        break;
      case 'P':
        P = currentKeyValue.Value;
        //PORT.println((String) "P: " + P );
        break;
      case 'R':
        R = currentKeyValue.Value;
        //PORT.println((String) "R: " + R );
        break;
      case 'Q':
        Q = currentKeyValue.Value;
        //PORT.println((String) "Q: " + Q );
        break;
      case 'W':
        W = currentKeyValue.Value;
        //PORT.println((String) "W: " + W );
        break;
      default:
        break;
    }
  }
  MotionRunner(firstKeyValue);
  isRunning = true;
}

void GExecuter::WhenFinishMove(){
  if(isRunning){
    if(data.isSleeping){
      if(!tool.Sleep()) return;
    }
    if(data.isExecutedGCode == true){
      PORT.println("ok");
      isRunning = false;
      data.isExecutedGCode = false;
    }
  }
}

void GExecuter::MotionRunner(KeyValue keyValue){
  switch(keyValue.Key){
    case 'G':
      switch((int)keyValue.Value){
        case 0:
          if(X == NULL_NUMBER) X = data.currentPoint.x;
          if(Y == NULL_NUMBER) Y = data.currentPoint.y;
          if(Z == NULL_NUMBER) Z = data.currentPoint.z;
          if(F != NULL_NUMBER) motion.fSpeed = F;
          //PORT.println((String) "Running G0 to X:" + X + ", Y:" + Y + ", Z:" + Z + "." );
          motion.G0(X, Y, Z);
          break;
        case 1:
          if(X == NULL_NUMBER) X = data.currentPoint.x;
          if(Y == NULL_NUMBER) Y = data.currentPoint.y;
          if(Z == NULL_NUMBER) Z = data.currentPoint.z;
          if(F != NULL_NUMBER) motion.fSpeed = F;
          //PORT.println((String) "Running G1 to X:" + X + ", Y:" + Y + ", Z:" + Z + "." );
          motion.G1(X, Y, Z);
          break;
        case 2:
          if(I == NULL_NUMBER) break;
          if(J == NULL_NUMBER) break;
          if(X == NULL_NUMBER) X = data.currentPoint.x;
          if(Y == NULL_NUMBER) Y = data.currentPoint.y;
          if(Z == NULL_NUMBER) Z = data.currentPoint.z;
          if(F != NULL_NUMBER) motion.fSpeed = F;
          //PORT.println((String) "Running G2 to X:" + X + ", Y:" + Y + " with: I:" + I + ", J:" + J + ".");
          motion.G2(X, Y, I, J);
          break;
        case 3:
          if(I == NULL_NUMBER) break;
          if(J == NULL_NUMBER) break;
          if(X == NULL_NUMBER) X = data.currentPoint.x;
          if(Y == NULL_NUMBER) Y = data.currentPoint.y;
          if(Z == NULL_NUMBER) Z = data.currentPoint.z;
          if(F != NULL_NUMBER) motion.fSpeed = F;
          //PORT.println((String) "Running G3 to X:" + X + ", Y:" + Y + " with: I:" + I + ", J:" + J + ".");
          motion.G3(X, Y, I, J);
          break;
        case 4:
          if(P != NULL_NUMBER){
            motion.G4(P);
          }
          break;
        case 28:
          if(F != NULL_NUMBER) motion.fSpeed = F;
          //PORT.println("Running G28.");
          motion.G28();
          break;
      }
      break;
  }
  ResetValue();
}

void GExecuter::getKeyValues(String code){
  String splitWord = "";
	code += " ";
	for (uint16_t i = 0; i < code.length(); i++){
		if (code[i] == ' '){
			if (splitWord == "")
				continue;
			KeyValue keyValue;
			keyValue.Key = splitWord[0];
			keyValue.Value = splitWord.substring(1).toFloat();
			keyValueQueue.push(keyValue);
			splitWord = "";
			continue;
		}
		splitWord += String(code[i]);
	}
}

void GExecuter::ResetValue(){
  X = NULL_NUMBER;
	Y = NULL_NUMBER;
	Z = NULL_NUMBER;
	E = NULL_NUMBER;
	S = NULL_NUMBER;
	A = NULL_NUMBER;
	I = NULL_NUMBER;
	J = NULL_NUMBER;
	F = NULL_NUMBER;
	P = NULL_NUMBER;
	R = NULL_NUMBER;
	Q = NULL_NUMBER;
	W = NULL_NUMBER;
}
