#ifndef MOREbot_h
#define MOREbot_h

#include "Arduino.h"
#include "Wire.h"
#include <Adafruit_MotorShield.h>

class motor{
  private:
    Adafruit_DCMotor M;
	
  public:
    motor();
    motor(int MX);
    void forward(int speed);
    void backward(int speed);
    void stop();
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

class bluetooth{
  private:
    int _rx;
    int _tx;
	int speed, direction, button, slider, sliderValue;
	int modeButton = 2;
	String text;
	bool mode = false;
	MOREbot _robot;
	SoftwareSerial ble;
	
  public:
    bluetooth(int rx, int tx);
    bluetooth(String name, int rx, int tx);
	
	void processData();
	bool getModeButton();
	int getSpeed();
	int getDirection();
	int getButton();
	int getSlider();
	int getSliderValue();
	String getText();
};

class MOREbot{
  private:
    motor _LM;
    motor _RM;
    ultrasonic us;
	bluetooth ble;
	String _name;
  
  public:
    MOREbot(int LM, int RM);
    MOREbot(int LM, int RM, int trig, int echo);
	MOREbot(String name, int LM, int RM, int trig, int echo, int brx, int btx);
	
	void connectBluetooth(int rx, int tx);
	void connectBluetooth(String name, int rx, int tx);
	void btControl();
	
    void forward(int speed);
    void backward(int speed);
    void left(int speed);
    void right(int speed);
    void stop();
	
    float getDistance();
	
	void bounce();
};
#endif