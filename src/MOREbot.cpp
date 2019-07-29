#include "MOREbot.h"
#include "Arduino.h"
#include <Adafruit_MotorShield.h>
#include <SoftwareSerial.h>

//Global handler for motor shield at IC Address: 0x60.
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

motor::motor(int motorPort){
	//Check motor port exists
	if(motorPort < 0 && motorPort < 5) return;

	//Connect to motor
	_motor = *AFMS.getMotor(motorPort);
}

void motor::counterClockwise(int speed){
	//Reverse if speed is negative
	if(speed < 0) {
		clockwise(-speed);
		return;
	}

	//Cap speed between 0 and 100
	if(speed > 100) speed = 100;

	//Remap speed from percentage (0-100) to digital (0-255)
	speed = map(speed, 0, 100, 60, 255);
	if(speed < 65) speed = 0;

	//Send speed and direction to motor
	_motor.setSpeed(speed);
	_motor.run(FORWARD);
}

void motor::clockwise(int speed){
	//Reverse if speed is negative
	if(speed < 0) {
		counterClockwise(-speed);
		return;
	}

	//Cap speed between 0 and 100
	if(speed > 100) speed = 100;

	//Remap speed from percentage (0-100) to digital (0-255)
	speed = map(speed, 0, 100, 60, 255);
	if(speed < 65) speed = 0;

	//Send speed and direction to motor
	_motor.setSpeed(speed);
	_motor.run(BACKWARD);
}

void motor::stop(){
	//Stop motor and allow it to move more freely
	_motor.setSpeed(0);
	_motor.run(RELEASE);
}

ultrasonic::ultrasonic(int trig, int echo){
	//Check that ports given exist
	if(trig < 0 || echo < 0) return;

	//Redefine the ports to communicate with the Ultrasonic module
	pinMode(trig, OUTPUT);
	pinMode(echo, INPUT);

	//Save pins for later use
	_trig = trig;
	_echo = echo;
}

float ultrasonic::readDistance(){
	//Allocate space for 5 results
	float distance[5];

	//Create an int to recieve the response time
	int duration = 0;

	//Loops 0-5 (5 times) and saves the result in the allocated space above
	for(int i = 0; i < 5; i++){
		//Ensure that nothing is being sent to the Ultrasonic at start
		digitalWrite(_trig, LOW);
		delayMicroseconds(2);

		//Send a HIGH(1) signal to the Ultrasonic for 10 microseconds
		digitalWrite(_trig, HIGH);
		delayMicroseconds(10);

		//Send a LOW(0) signal to the Ultrasonic after those 10 microseconds
		digitalWrite(_trig, LOW);

		//Record the time it takes from sending LOW(0) to recieve a HIGH(1) from the Ultrasonic
		duration = pulseIn(_echo, HIGH);

		//Convert time (microseconds) to distance (centimeters) using the speed of sound
		distance[i]= duration*0.0343/2;

		//Wait some time to ensure all data is finished being processed
		delayMicroseconds(5);
	}

	//Take the average of the 5 measurements and return that result
	return (distance[0] + distance[1] + distance[2] + distance[3] + distance[4])/5.0;
}

bluetooth::bluetooth(int rx, int tx) : ble(rx, tx) {
	//Check that the pins used exist
	if(rx < 0 || tx < 0) return;
	
	//Start UART communication with the bluetooth module at 9600 bits/second
	ble.begin(9600);
	
	//Wait some time to ensure the data is cleared
	delay(100);
}

bluetooth::bluetooth(String name, int rx, int tx) : ble(rx, tx) {
	_rx = rx;
	_tx = tx;
	
	//Check that the pins used exist
	if(rx < 0 || tx < 0) return;

	//Check that the name is valid, and then save it
	if(name.length()>0)
	_name = name;

	//Start UART communication with the bluetooth module at 9600 bits/second
	ble.begin(9600);
}

void bluetooth::setup(){
	if(_rx < 0 || _tx < 0) return;
	
	delay(100);
	
	//Tell the bluetooth module to rename itself to the name given (default to MOREbot)
	String s = "AT+NAME " + _name + "\r\n";
	ble.print(s);
	delay(1100);
	
	//Restarts the bluetooth module so that it can connect to other devices
	ble.print("AT+RESET\r\n");
	delay(1000);
	ble.setTimeout(10);
}

char bluetooth::readData(){
	if (ble.available()>0){
		char c = ble.read();
		//Serial.print("Data in:");
		//Serial.println(c);
		//Recieve 8 bits of data as a number
		return c;
	}
	return NULL;
}

int bluetooth::processData() {
	//Check for data to recieve
	if (ble.available() > 0){
		//Serial.print("Data in:");
		//Recieve 8 bits of data as a number
		String s = ble.readString();
		
		int is = -1, ie = -1;
		
		for(int i = 0; i < s.length(); i++){
			
			char ic = s.charAt(i);
			
			if(ic == 2){ 
				is = i;
			}else if(ic == 3){
				ie = i;
			}

			if(is > -1 && is < ie && ie > -1){

				String codeword = s.substring(is + 1, ie);

				if(codeword.length() == 6){
					char c[6];

					for(int j = 0; j < 6; j++){
						c[j] = codeword.charAt(j);
					}

					int rawX = (10*(int)(c[1] - '0') + 1*(int)(c[2] - '0'));
					joyX = c[0] == '0' ? -1*rawX : rawX;

					int rawY = (int)(10*(c[4] - '0') + 1*(c[5] - '0'));
					joyY = c[3] == '0' ? -1*rawY : rawY;					
				}else if(codeword.length() == 1){
					button = codeword.charAt(0);
				}
			}
		}
		return 1;
	}
	return -1;
}

//Returns last recieved X coord
int bluetooth::getJoystickX(){
	return joyX;
}

//Returns last recieved Y coord
int bluetooth::getJoystickY(){
	return joyY;
}

//Returns last recieved button id
char bluetooth::getButton(){
	return button;
}

//Defines robot with only motors
MOREbot::MOREbot(int LM, int RM) : _LM(LM), _RM(RM), us(-1,-1), ble(-1,-1) {}


//Defines robot with only motors
MOREbot::MOREbot(String name, int LM, int RM) : _name(name), _LM(LM), _RM(RM), us(-1,-1), ble(-1,-1) {}

//Defines robot with motors and ultrasonic
MOREbot::MOREbot(int LM, int RM, int trig, int echo) : _LM(LM), _RM(RM), us(trig, echo), ble(-1,-1) {}

//Defines robot with motors and ultrasonic
MOREbot::MOREbot(String name, int LM, int RM, int trig, int echo) : _name(name), _LM(LM), _RM(RM), us(trig, echo), ble(-1,-1) {}

//Defines robot with motors, ultrasonic, and bluetooth
MOREbot::MOREbot(String name, int LM, int RM, int trig, int echo, int rx, int tx) : _name(name), _LM(LM), _RM(RM), us(trig, echo), ble(name, rx, tx) {}

//Prepares motors and bluetooth if they are being used
void MOREbot::setup(){
	AFMS.begin();
	ble.setup();
}

//Prepares motors and bluetooth and any other components if they are being used
void MOREbot::setup(int leftPin, int rightPin){
	AFMS.begin();
	ble.setup();
	
	pinMode(leftPin, INPUT);
	pinMode(rightPin, INPUT);
	
	leftEncoderPort = leftPin;
	rightEncoderPort = rightPin;
}

//Returns the motor object of the robot's left motor
motor MOREbot::getLeftMotor(){
	return _LM;
}

//Returns the motor object of the robot's right motor
motor MOREbot::getRightMotor(){
	return _RM;
}

//Returns the ultrasonic object of the robot's ultrasonic
ultrasonic MOREbot::getUltrasonic(){
	return us;
}

//Returns the motor object of the robot's bluetooth
bluetooth MOREbot::getBluetooth(){
	return ble;
}

//Commands the robot forward by having the left motor move clockwise and right counterClockwise
void MOREbot::forward(int speed){
	if(speed > 100) speed = 100;
	speed = map(speed, 0, 100, 0, 80);
	_LM.clockwise(speed);
	_RM.counterClockwise(speed);
}

//Commands the robot forward by having the left motor move clockwise and right counterClockwise
void MOREbot::forward(int speed, float dist){
	float leftCount = 0, rightCount = 0;
	
	if(leftEncoderPort > 0 && rightEncoderPort > 0){
		if(speed > 100) speed = 100;
		speed = map(speed, 0, 100, 0, 80);
		
		_LM.clockwise(speed);
		_RM.counterClockwise(speed);
		
		bool fl1 = false, fl2 = false;
		
		while((leftCount*3.1415*2.44)/8.0 < dist || (rightCount*3.1415*2.44)/8.0 < dist){
			if(digitalRead(leftEncoderPort) && fl1){
				fl1 = false;
				leftCount++;
			}else if(!digitalRead(leftEncoderPort) && !fl1) fl1 = true;
			
			if((leftCount*3.1415*2.44)/8 >= dist) _LM.clockwise(0);
			
			if(digitalRead(rightEncoderPort) && fl2){
				fl2 = false;
				rightCount++;
			}else if(!digitalRead(rightEncoderPort) && !fl2) fl2 = true;
			
			if((rightCount*3.1415*2.44)/8 >= dist) _RM.counterClockwise(0);
		}
	}
}

//Commands the robot backward by having the left motor move counterClockwise and right clockwise
void MOREbot::backward(int speed){
	if(speed > 100) speed = 100;
	speed = map(speed, 0, 100, 0, 80);
	_LM.counterClockwise(speed);
	_RM.clockwise(speed);
}

//Commands the robot forward by having the left motor move clockwise and right counterClockwise
void MOREbot::backward(int speed, float dist){
	float leftCount = 0, rightCount = 0;
	
	if(leftEncoderPort > 0 && rightEncoderPort > 0){
		if(speed > 100) speed = 100;
		speed = map(speed, 0, 100, 0, 80);
		
		_LM.counterClockwise(speed);
		_RM.clockwise(speed);
		
		bool fl1 = false, fl2 = false;
		
		while((leftCount*3.1415*2.44)/8 < dist || (rightCount*3.1415*2.44)/8 < dist){
			if(digitalRead(leftEncoderPort) && fl1){
				fl1 = false;
				leftCount++;
			}else if(!digitalRead(leftEncoderPort) && !fl1) fl1 = true;
			
			if((leftCount*3.1415*2.44)/8 >= dist) _LM.counterClockwise(0);
			
			if(digitalRead(rightEncoderPort) && fl2){
				fl2 = false;
				rightCount++;
			}else if(!digitalRead(rightEncoderPort) && !fl2) fl2 = true;
			
			if((rightCount*3.1415*2.44)/8 >= dist) _RM.clockwise(0);
		}
	}
}

//Commands the robot left by having the both motors move counterClockwise
void MOREbot::left(int speed){
	if(speed > 100) speed = 100;
	speed = map(speed, 0, 100, 0, 80);
	_LM.counterClockwise(speed);
	_RM.counterClockwise(speed);
}

//Commands the robot forward by having the left motor move clockwise and right counterClockwise
void MOREbot::left(int speed, float deg){
	float leftCount = 0, rightCount = 0;
	
	if(leftEncoderPort > 0 && rightEncoderPort > 0){
		if(speed > 100) speed = 100;
		speed = map(speed, 0, 100, 0, 80);
		
		_LM.counterClockwise(speed);
		_RM.counterClockwise(speed);
		
		bool fl1 = false, fl2 = false;
		
		while((leftCount*2.44*360)/64 < deg || (rightCount*2.44*360)/64 < deg){
			if(digitalRead(leftEncoderPort) && fl1){
				fl1 = false;
				leftCount++;
			}else if(!digitalRead(leftEncoderPort) && !fl1) fl1 = true;
			
			if((leftCount*2.44*360)/64 >= deg) _LM.counterClockwise(0);
			
			if(digitalRead(rightEncoderPort) && fl2){
				fl2 = false;
				rightCount++;
			}else if(!digitalRead(rightEncoderPort) && !fl2) fl2 = true;
			
			
			if((rightCount*2.44*360)/64 >= deg) _RM.counterClockwise(0);
		}
	}
}

//Commands the robot right by having the both motors move clockwise
void MOREbot::right(int speed){
	if(speed > 100) speed = 100;
	speed = map(speed, 0, 100, 0, 80);
	_LM.clockwise(speed);
	_RM.clockwise(speed);
}

void MOREbot::right(int speed, float deg){
	float leftCount = 0, rightCount = 0;
	
	if(leftEncoderPort > 0 && rightEncoderPort > 0){
		if(speed > 100) speed = 100;
		speed = map(speed, 0, 100, 0, 80);
		
		_LM.clockwise(speed);
		_RM.clockwise(speed);
		
		bool fl1 = false, fl2 = false;
		
		while((leftCount*2.44*360)/64 < deg || (rightCount*2.44*360)/64 < deg){
			if(digitalRead(leftEncoderPort) && fl1){
				fl1 = false;
				leftCount++;
			}else if(!digitalRead(leftEncoderPort) && !fl1) fl1 = true;
			
			if((leftCount*2.44*360)/64 >= deg) _LM.clockwise(0);
			
			if(digitalRead(rightEncoderPort) && fl2){
				fl2 = false;
				rightCount++;
			}else if(!digitalRead(rightEncoderPort) && !fl2) fl2 = true;
			
			if((rightCount*2.44*360)/64 >= deg) _RM.clockwise(0);
		}
	}
}

void MOREbot::leftMotor(int speed){
	_LM.clockwise(speed);
}

void MOREbot::rightMotor(int speed){
	_RM.counterClockwise(speed);
}

//Commands the robot to stop by having the both motors stop
void MOREbot::stop(){
	_LM.stop();
	_RM.stop();
}

//Retrieves distance data from the robot's ultrasonic
float MOREbot::readDistance(){
	//Retrieve distance data
	float dist = us.readDistance();

	//Remove noisy error regions
	if(dist < 2 || dist > 180) dist = -1;

	return dist;
}

//Basic bluetooth joystick control function
void MOREbot::btControl(){
	//Recieve bluetooth data
	if(ble.processData() == -1) return;
	
	//Retrieve speed and direction from bluetooth
	int x = ble.getJoystickX();
	int y = ble.getJoystickY();
	
	//Convert radial direction and speed to linear coordinates
	float Lpow = (x+y)/2;
	float Rpow = (x-y)/2;
	
	//Send linear speed and direction to robot
	_LM.clockwise(Lpow);
	_RM.counterClockwise(Rpow);
	
	//Allow robot to rest
	delay(50);
}

char MOREbot::btStream(){
	char c = ble.readData();
	if(c == NULL) return NULL;
	return c;

}

//Basic bounce function
void MOREbot::bounce(float targetDistance, float threshold){
	//Read distance from ultrasonic
	float currentDistance = readDistance();
	
	//Find the difference in where the robot wants to be and where it is
	float delta = targetDistance - currentDistance;
	
	//Check that difference with the range of ok errors (-: too far back, +: too far forward)
	if(delta < -threshold){
		delta *= -1;
		
		//If the distance is too far, stop moving
		if(delta > 40){
			stop();
			return;
		}
		
		//Move forward proportionally to how far the robot is off
		forward(2*delta);
		
	}else if(delta > threshold){
		//If distance is too close keep speed not too fast
		if(delta > 30){
			delta = 30;
		}
		
		//Move backward proportionally to how far the robot is off
		backward(3*delta);
	}else{
		//If the robot is within target range, stop moving
		stop();
	}
}