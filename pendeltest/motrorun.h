#ifndef motrorun_h
#define motrorun_h

#include "Arduino.h"
#include "Servo.h"


class Motrorun{
  public:
    Motrorun(int motorPins[3]);
    void runMotorPWM(float errPos, float posMotor, float setPosMotor);
    void getUp(int gain, bool derection);
 
  private:
    int pwmPin;
    int CwPin;
    int CcwPin;
    int pwm;
    float Kp;
};

#endif
