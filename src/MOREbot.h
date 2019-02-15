#ifndef MOREbot_h
#define MOREbot_h

#include "Arduino.h"
#include "Wire.h"
#include <Adafruit_MotorShield.h>
#include <SoftwareSerial.h>

/** Motor class. Handles retrieving the motor from i2c and then sending the commands. */

class motor{
	
  private:
  
  /** Motor Object. The pointer object to the motor port on the Adafruit motor shield */
    Adafruit_DCMotor M;
	
  public:
  
  /** Motor constructor. Retrieves the i2c address of the specified motor port on the motor shield.
   * @param MX integer. The motor port being accessed.
   */
    motor(int MX); 
	
  /** counterClockwise motor function. Sets motor speed and direction (of the internal motor axle), handles negative values with clockwise().
   *  @param speed an integer. The speed to be sent to the motor. Contrained from -100 to 100.
   *  @see backward()
   */
    void counterClockwise(int speed);
	
  /** Backward motor function. Sets motor speed and direction (of the internal motor axle), handles negative values with counterClockwise().
   *  @param speed an integer. The speed to be sent to the motor. Contrained from -100 to 100.
   *  @see forward()
   */
    void clockwise(int speed);
	
  /** Stop motor function. Sets motor speed to zero, and releases the motor. */
    void stop();
};

/** Ultrasonic class. Handles communicating with the ultrasonic module to determine distance to the nearest object. */

class ultrasonic{
  private:
  
  /** Pin number on the arduino that the ultrasonic trig pin is connected to. */
    int _trig;
	
  /** Pin number on the arduino that the ultrasonic echo pin is connected to. */
    int _echo;

  public:
  
  /** Ultrasonic constructor. Saves the pins to communicate with the ultrasonic module.
   *  @param trig an integer. The pin on the arduino that the ultrasonic's trig pin is connected to.
   *  @param echo an integer. The pin on the arduino that the ultrasonic's echo pin is connected to.
   */
    ultrasonic(int trig, int echo);
	
  /** Function to get distance from the Ultrasonic. Activates the ultrasonic, reads the echo time, and processes that time into a distance in centimeters.
   *  @return float distance to the closest object in centimeters.
   */
    float readDistance();
};

/** Bluetooth BLE class. Handles communicating with the bluetooth module, uses ArduinoBlue data tags. */

class bluetooth{
  private:
  /** Read pin on Arduino. */
    int _rx;
	
  /** Send pin on Arduino. */
    int _tx;
	
  /** Last recieved speed value. */
	int speed = -1;
	
  /** Last recieved button id. */
	int button = -1;
	
  /** Last recieved slider id. */
	int slider = -1;
	
  /** Last recieved slider value. */
	int sliderValue = -1;
	
  /** Last recieved direction value. */
	float direction = -1;
	
  /** Mode button id. */
	int modeButton = 2;
	
  /** Last recieved text String. */
	String _name = "", text;
	
  /** Current mode status. */
	bool mode = false;
	
  /** UART communication stream with the BLE module. */
	SoftwareSerial ble;
	
  public:
  /** Basic Bluetooth constructor. Sets the pins for communication with the BLE module and starts the stream to communicate. 
   * @param rx an integer. Read pin for the arduino, connects to tx on the BLE module.
   * @param tx an integer. Send pin for the arduino, connects to rx on the BLE.
   */
    bluetooth(int rx, int tx);
	
  /** Bluetooth constructor eith renaming. Sets the pins for communication with the BLE module and starts the stream to communicate. Saves <b>name</b> to be used when renaming the module during setup().
   * @param name a String. The name that the arduino will attempt to rename the BLE before starting data communication (Does not rename if the module is already connected)
   * @param rx an integer. Read pin for the arduino, connects to tx on the BLE module.
   * @param tx an integer. Send pin for the arduino, connects to rx on the BLE.
   */
    bluetooth(String name, int rx, int tx);
	
  /** Checks that the module is responding and attempts to rename it if a name was given during construction. */
	void setup();
	
  /** Reads the BLE stream for any data that the module has recieved. Saves that data in private memory. Processes joystick data into speed and direction. */
	void processData();
	
  /** Call function for the current mode status, inverts when the mode button id (2) is recieved. 
   * @return boolean status of mode on or off.
   */
	bool getModeButton();
	
  /** Call function for sthe last speed recieved. 
   * @return integer speed, between 0 and 100.
   */
	int getSpeed(); 
	
  /** Call function for the last direction recieved. 
   * @return float direction, between 0 and 6.28 (<tt>2PI</tt>).
   */
	float getDirection();
	
  /** Call function for the last button id recieved. 
   * @return integer button id, expected between 0 and 200.
   */
	int getButton();
	
  /** Call function for the last slider id recieved. 
   * @return integer slider id, expected between 0 and 200..
   */
	int getSlider();
	
  /** Call function for the last slider value recieved. 
   * @return integer slider value, expected between 0 and 200.
   */
	int getSliderValue();
	
  /** Call function for the last text recieved. 
   * @return String text.
   */
	String getText();
};


class MOREbot{
  private:
  
  /** Left motor object */
    motor _LM;
	
  /** Right motor object */
    motor _RM;
	
  /** Ultrasonic object */
    ultrasonic us;
	
  /** Bluetooth object */
	bluetooth ble;
	
  /** Name of robot, passed to bluetooth. */
	String _name;
  
  public:
  
  /** Two motor MOREbot constructor. Creates left and right motor objects to control the motors. 
   * @param LM an integer. Port of the left motor.
   * @param RM an integer. Port of the right motor.
   */
    MOREbot(int LM, int RM);
	
  /** Two motor and ultrasonic MOREbot constructor. Creates left and right motor objects to control the motors and an ultrasonic object to sense distance. 
   * @param LM an integer. Port of the left motor.
   * @param RM an integer. Port of the right motor.
   * @param trig an integer. The pin on the arduino that the ultrasonic's trig pin is connected to.
   * @param echo an integer. The pin on the arduino that the ultrasonic's echo pin is connected to.
   */
    MOREbot(int LM, int RM, int trig, int echo);
	
  /** Two motor and ultrasonic MOREbot constructor. Creates left and right motor objects to control the motors and an ultrasonic object to sense distance. 
   * @param name a String. The name that the arduino will attempt to rename the BLE before starting data communication (Does not rename if the module is already connected)
   * @param LM an integer. Port of the left motor.
   * @param RM an integer. Port of the right motor.
   * @param trig an integer. The pin on the arduino that the ultrasonic's trig pin is connected to.
   * @param echo an integer. The pin on the arduino that the ultrasonic's echo pin is connected to.
   * @param rx an integer. Read pin for the arduino, connects to tx on the BLE module.
   * @param tx an integer. Send pin for the arduino, connects to rx on the BLE.
   */
	MOREbot(String name, int LM, int RM, int trig, int echo, int rx, int tx);
	
  /** Robot setup. Starts the motor shield i2c and bluetooth UART communication streams. */
	void setup();

  /** Call function for the left motor motor object.
   *  @return motor object, the object for the left motor port on the motor shield.
   *  @see motor
   */
	motor getLeftMotor();
	
  /** Call function for the right motor motor object.
   *  @return motor object, the object for the right motor port on the motor shield.
   *  @see motor
   */
	motor getRightMotor();
	
  /** Call function for the ultrasonic object.
   *  @return ultrasonic object.
   *  @see ultrasonic
   */
	ultrasonic getUltrasonic();
	
  /** Call function for the bluetooth object.
   *  @return bluetooth object.
   *  @see bluetooth
   */
	bluetooth getBluetooth();
	
  /** Drive function for MOREbot to drive forward. Commands the left motor clockwise and the right motor counterclockwise at the given speed.
   *  @param speed an integer, constrained between -100 and 100. 
   */
    void forward(int speed);
	
  /** Drive function for MOREbot to drive backward. Commands the left motor counterclockwise and the right motor clockwise at the given speed.
   *  @param speed an integer, constrained between -100 and 100. 
   */
    void backward(int speed);
	
  /** Drive function for MOREbot to drive left. Commands the both motors counterclockwise at the given speed.
   *  @param speed an integer, constrained between -100 and 100. 
   */
    void left(int speed);
	
  /** Drive function for MOREbot to drive right. Commands the both motors clockwise at the given speed.
   *  @param speed an integer, constrained between -100 and 100. 
   */
    void right(int speed);
	
  /** Drive function for MOREbot to stop. Commands the both motors to stop. */
    void stop();
	
  /** Distance sensing function. Calls ultasonic.readDistance() to get distance to the closest object in centimeters.
   *  @return float distance to closest object in centimeters. 
   */
    float readDistance();
	
	
  /** Operation function for full control from a BLE connection. Handles full control of the robot through the bluetooth module expecting ArduinoBlue joystick values, button id 2 changes to bounce(). */
	void btControl();
	
  /** Operation function for full control to maintain distance to object in front. Handles full control of the robot through the ultrasonic's distance value, if bluetooth is connected, ArduinoBlue button id 2 changes to btControl(). */
	void bounce();
};
#endif