// MOREbot_Library - Version: Latest 
#include <MOREbot.h>

//Set the robot name
#define BotName "MOREbot"

//Set which motor ports each motor is plugged into
#define leftMotor 4 //M4
#define rightMotor 1 //M1

//Set which pins the Ultrasonic module is plugged into
#define trig A0
#define echo A1

//Create the MOREbot object with those settings
MOREbot bot(BotName, leftMotor, rightMotor, trig, echo);


//Function that runs only once when the robot turns on
void setup() {
  //Start the different parts of the robot (connects to the motors, ultrasonic, and bluetooth)
  bot.setup();
  
  //Talk to the computer over the usb cable
  Serial.begin(9600);
}

//Function that runs continuously when the robot is on
void loop() {
  //Reads distance from the ultrasonic sensor
  float dist = bot.readDistance();

  //Sends that distance to the computer
  Serial.println(dist);
  delay(20);

  //Drives the robot forward at 100% speed
  bot.forward(100);
  
  //Waits 100ms (1/10 of a second)
  delay(100);
  
  //Drives the robot right at 50% speed
  bot.right(50);
  
  //Waits 100ms (1/10 of a second)
  delay(100);
  
  //Drives the robot left at 50% speed
  bot.left(50);
  
  //Waits 100ms (1/10 of a second)
  delay(100);
  
  //Drives the robot backward at 100% speed
  bot.backward(100);
  
  //Waits 100ms (1/10 of a second)
  delay(100);

  //Stops the robot
  bot.stop();
  
  //Waits 80ms
  delay(80);
}
