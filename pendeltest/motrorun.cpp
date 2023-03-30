#include "motrorun.h"

Motrorun::Motrorun(int motorPins[])
: pwmPin(motorPins[0])
, CwPin(motorPins[1])
, CcwPin(motorPins[2])
, Kp(1)


{
  
}

void Motrorun::runMotorPWM(float errPos, float posMotor, float setPosMotor){
  //errPos = errPos - 0.1*(setPosMotor-posMotor);
  pwm = 48 + abs(Kp*errPos);
  if (pwm > 254){
    pwm = 255;
  }
  if (errPos < 0){
    digitalWrite(CwPin, 1);
    digitalWrite(CcwPin, 0);
    analogWrite(pwmPin, pwm);
  }
  else if (errPos > 0){
    digitalWrite(CwPin, 0);
    digitalWrite(CcwPin, 1);
    analogWrite(pwmPin, pwm);
  }
  else{
    digitalWrite(CwPin, 0);
    digitalWrite(CcwPin, 0);
    analogWrite(pwmPin, 0);
  }
}

void Motrorun::getUp(int gain, bool derection){
    digitalWrite(CwPin, derection);
    digitalWrite(CcwPin, !derection);
    analogWrite(pwmPin, gain);
}
