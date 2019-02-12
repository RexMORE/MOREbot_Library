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

bluetooth::bluetooth(int rx, int tx) : ble(rx, tx) {
	ble.begin(9600);
	delay(100);
}

bluetooth::bluetooth(String name, int rx, int tx) : ble(rx, tx) {
	_name = name;
	ble.begin(9600);
}

void bluetooth::setup(){
	delay(100);
	String s = "AT+NAME" + _name + "\r\n";
	ble.print(s);
	delay(10);
	ble.print("AT+RESET\r\n");
	delay(100);
}

void bluetooth::processData() {
  if (ble.available()>0){
    int i = ble.read();
    if(i == 251){
	  while(ble.available()<=0);
      speed = ble.read();
	  while(ble.available()<=0);
      direction = (int)ble.read();
	  direction = map(((speed-49.0)/abs(speed-49.0))*direction, -99, 99, 0, 2*3.1415);
	  speed = map(abs(speed-49), 0, 50, 0, 100);
	  while(ble.available()<=0);
      ble.read();
    }
	else if(i == 252){
	  while(ble.available()<=0);
      button = ble.read();
	  while(ble.available()<=0);
      ble.read();
	  if(button == modeButton) mode = !mode;
    }
	else if(i == 253){
	  while(ble.available()<=0);
      slider = ble.read();
	  while(ble.available()<=0);
      sliderValue = ble.read();
	  while(ble.available()<=0);
      ble.read();
    }
	else if(i == 254){
      String t = "";
      while(ble.available()<=0);
      i = ble.read();
      while(250 != i){
      t.concat(char(i));
        while(ble.available()<=0);
        i = ble.read();
      }
      text = t;
    }
  }
}

bool bluetooth::getModeButton(){
	return mode;
}

int bluetooth::getSpeed(){
	return speed;
}

float bluetooth::getDirection(){
	return direction;
}

int bluetooth::getButton(){
	return button;
}

int bluetooth::getSlider(){
	return slider;
}

int bluetooth::getSliderValue(){
	return sliderValue;
}

String bluetooth::getText(){
	return text;
}

MOREbot::MOREbot(int LM, int RM) : _LM(LM), _RM(RM), ble(8, 9) {}

MOREbot::MOREbot(int LM, int RM, int trig, int echo) : _LM(LM), _RM(RM), us(trig, echo), ble(8, 9) {}

MOREbot::MOREbot(String name, int LM, int RM, int trig, int echo, int rx, int tx) : _name(name), _LM(LM), _RM(RM), us(trig, echo), ble(name, rx, tx) {}

void MOREbot::btSetup(){
	ble.setup();
}

void MOREbot::btControl(){
	ble.processData();
	bool b = ble.getModeButton();
	if(!b){
		int P = ble.getSpeed();
		int D = ble.getDirection();
		_LM.forward(P*(cos(D)-sin(D)));
		_RM.forward(P*(-cos(D)-sin(D)));
	}else{
		bounce();
	}
}

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

void MOREbot::bounce(){
}