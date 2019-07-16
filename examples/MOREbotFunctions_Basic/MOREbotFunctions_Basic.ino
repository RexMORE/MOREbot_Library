#include <MOREbot.h>


#define leftMotor 1 //M4
#define rightMotor 4 //M1

#define trig A0
#define echo A1

//Setup robot with motors and ultrasonic
MOREbot bot(BotName, leftMotor, rightMotor, trig, echo);

void setup() {
  //Connect to motors
  bot.setup();
  
  //Talk to the computer over the usb cable
  Serial.begin(9600);
}

//Function that runs continuously when the robot is on
void loop() {
  float dist = bot.readDistance();
  
  bot.forward(50);
  delay(1000);
  
  bot.right(40);
  delay(200);
  
  bot.stop();
  delay(1000);
}
