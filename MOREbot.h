#ifndef MOREbot_h
#define MOREbot_h

#include "Arduino.h"
#include "Wire.h"
#include <Adafruit_MotorShield.h>
#include <SoftwareSerial.h>

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
	int speed, button, slider, sliderValue;
	float direction;
	int modeButton = 2;
	String _name, text;
	bool mode = false;
	SoftwareSerial ble;
	
  public:
    bluetooth(int rx, int tx);
    bluetooth(String name, int rx, int tx);
	
	void setup();
	void processData();
	bool getModeButton();
	int getSpeed();
	float getDirection();
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
	MOREbot(String name, int LM, int RM, int trig, int echo, int rx, int tx);
	
	void setup();

	void btSetup();
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