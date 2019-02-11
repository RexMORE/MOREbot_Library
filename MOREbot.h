#ifndef MOREbot_h
#define MOREbot_h

#include "Arduino.h"
#include "Wire.h"
#include <Adafruit_MotorShield.h>

class motor{
  public:
    motor();
    motor(int MX);
    void forward(int speed);
    void backward(int speed);
    void stop();
    
  private:
    Adafruit_DCMotor M;
};

class ultrasonic{
  public:
    ultrasonic();
    ultrasonic(int trig, int echo);
    float getDistance();
    
  private:
    int _trig;
    int _echo;
};

class MOREbot{
  private:
    motor _LM;
    motor _RM;
    ultrasonic us;
  
  public:
    MOREbot(int LM, int RM);
    MOREbot(int LM, int RM, int trig, int echo);
    void forward(int speed);
    void backward(int speed);
    void left(int speed);
    void right(int speed);
    void stop();
    float getDistance();
};
#endif