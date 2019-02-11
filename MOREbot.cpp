#include "MOREbot.h"
#include "Arduino.h"
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

motor::motor(int MX){
  M = *AFMS.getMotor(MX);
}

void motor::forward(int speed){
  if(speed < 0) backward(-speed);
  if(speed > 100) speed = 100;
  speed = map(speed, 0, 100, 0, 255);
  M.setSpeed(speed);
  M.run(FORWARD);
}

void motor::backward(int speed){
  if(speed < 0) forward(-speed);
  if(speed > 100) speed = 100;
  speed = map(speed, 0, 100, 0, 255);
  M.setSpeed(speed);
  M.run(BACKWARD);
}

void motor::stop(){
  M.setSpeed(0);
  M.run(RELEASE);
}

ultrasonic::ultrasonic(int trig, int echo){
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  _trig = trig;
  _echo = echo;
}

float ultrasonic::getDistance(){
  float distance[5];
  int duration = 0;
  for(int i = 0; i < 5; i++){
    
    digitalWrite(_trig, LOW);
    delayMicroseconds(2);
    
    digitalWrite(_trig, HIGH);
    delayMicroseconds(10);
    
    digitalWrite(_trig, LOW);
    
    duration = pulseIn(_echo, HIGH);
    
    distance[i]= duration*0.034/2;
    delayMicroseconds(5);
  }
  
  return (distance[0] + distance[1] + distance[2] + distance[3] + distance[4])/5.0;
}


MOREbot::MOREbot(int LM, int RM) : _LM(LM), _RM(RM) {}

MOREbot::MOREbot(int LM, int RM, int trig, int echo) : _LM(LM), _RM(RM), us(trig, echo) {}

void MOREbot::forward(int speed){
  _LM.forward(speed);
  _RM.forward(speed);
}

void MOREbot::backward(int speed){
  _LM.backward(speed);
  _RM.backward(speed);
}

void MOREbot::left(int speed){
  _LM.backward(speed);
  _RM.forward(speed);
}

void MOREbot::right(int speed){
  _LM.forward(speed);
  _RM.backward(speed);
}

void MOREbot::stop(){
  _LM.stop();
  _RM.stop();
}

float MOREbot::getDistance(){
  return us.getDistance();
}