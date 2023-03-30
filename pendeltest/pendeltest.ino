#include <AS5600.h>
#include <Wire.h>
#include "motrorun.h"
#define A 2
#define B 3


int pwmPin = 8;
int motorCw = 6;
int motorCcw = 7;
int pinList[3] = {pwmPin, motorCw, motorCcw};
int aCounter = 0;
int bCounter = 0;
bool currentA;
bool currentB;
bool lastA;
bool lastB;
int posFlank = 0;
float posDeg = 0;
int i = 0;
float setPosMotor = 0.0;
float pendelPosBits;
float pendelPosDeg;
float errPendel;
float errPendelDPS;
float errMotorPos;
float pendelDPS;
float motorVelocety = 0.0;
float previousPos = 0.0;
float previousTime = micros();
float setVelocity = 360;
float errVelocity;
float totErrPendel;
float x;


Motrorun pendelMotor_(pinList);
AS5600 as5600;


void setup() {
  Serial.begin(9600);
  pinMode(motorCcw, OUTPUT);
  pinMode(motorCw, OUTPUT);
  pinMode(A, INPUT);
  pinMode(B, INPUT);

  as5600.begin(4);  //  set direction pin.
  as5600.setDirection(AS5600_CLOCK_WISE);  // default, just be explicit.

  Serial.println(as5600.getAddress());
 
  attachInterrupt(0, updateEncoder, CHANGE);
	attachInterrupt(1, updateEncoder, CHANGE);
}

void loop() {
  pendelAngelCalk();
  if (pendelPosDeg > 170 && pendelPosDeg < 190){
    errPendel = (179 - pendelPosDeg)*(-30);
    errPendelDPS = (0-pendelDPS)*(-0.5);
    errVelocity = (0-motorVelocety)*(-1);
    errMotorPos = (0-posDeg)*(-0.5);
    totErrPendel = errPendel + errPendelDPS+errVelocity+errMotorPos;
    pendelMotor_.runMotorPWM(totErrPendel, posDeg, setPosMotor);    
  }
  else if (pendelPosDeg > 145 && pendelPosDeg < 215){
    errPendel = (180 - pendelPosDeg)*(-90);
    errPendelDPS = (0-pendelDPS)*(-8);
    errVelocity = (0-motorVelocety)*(0);
    errMotorPos = (0-posDeg)*(0);
    totErrPendel = errPendel + errPendelDPS+errVelocity+errMotorPos;
    pendelMotor_.runMotorPWM(totErrPendel, posDeg, setPosMotor);    
  }
  else if ((pendelPosDeg > 10 && pendelPosDeg < 60) || (pendelPosDeg < -10 && pendelPosDeg > -60)){
    if (pendelDPS<0){
      pendelMotor_.getUp(80, 1);
    }
    else{
      pendelMotor_.getUp(220, 0);
    }
  }
  else if ((pendelPosDeg > 370 && pendelPosDeg < 420)|| (pendelPosDeg < 350 && pendelPosDeg > 300)){
    if (pendelDPS<0){
      pendelMotor_.getUp(220, 1);
    }
    else{
      pendelMotor_.getUp(80, 0);
    }
  }
  else{
    pendelMotor_.runMotorPWM(0, 0, 0);
  }
  
  
  if (i == 50){
    motorVelocety = (posDeg-previousPos)/(micros()-previousTime)*1000000;
    i = 0;
    previousTime = micros();
    previousPos = posDeg;
  }
  i++;
}

void updateEncoder(){
  currentA = digitalRead(A);
  currentB = digitalRead(B);
	if (currentA != lastA && currentA != currentB){
    aCounter++;
  }
  else if(currentA != lastA && currentA == currentB){
    aCounter--;
  }
  if (currentB != lastB && currentA == currentB){
    bCounter++;
  }
  else if(currentB != lastB && currentA != currentB){
    bCounter--;
  }

  lastA = currentA;
  lastB = currentB;
  posFlank = aCounter + bCounter;
  posDeg = (posFlank/(19.225*12))*360;
}

void pendelAngelCalk(){
  pendelPosBits = as5600.getCumulativePosition();
  pendelPosDeg = ((pendelPosBits+220)/4096)*360;
  pendelDPS = as5600.getAngularSpeed(AS5600_MODE_RPM)*6;
}
