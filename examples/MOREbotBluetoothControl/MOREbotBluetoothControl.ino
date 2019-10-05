// MOREbot_Library - Version: Latest 
#include <MOREbot.h>

//Set the robot name
#define BotName "MOREbot"

//Set which motor ports each motor is plugged into
#define leftMotor 1 //M1
#define rightMotor 4 //M4

//Set which pins the Ultrasonic module is plugged into
#define trig A0
#define echo A1

//Set which pins the Bluetooth module is plugged into
#define rx 3
#define tx 4

//Create the MOREbot object with those settings
MOREbot bot(BotName, leftMotor, rightMotor, trig, echo, rx, tx);

//Function that runs only once when the robot turns on
void setup() {
  //Start the different parts of the robot (connects to the motors, ultrasonic, and bluetooth)
  bot.setup();
}

//Function that runs continuously when the robot is on
void loop() {
  bot.btControl();
}
