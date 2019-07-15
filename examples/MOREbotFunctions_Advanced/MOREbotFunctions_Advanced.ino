// MOREbot_Library - Version: Latest 
#include <MOREbot.h>

//Set which motor ports each motor is plugged into
#define leftMotor 4 //M4
#define rightMotor 1 //M1

//Create the MOREbot object with those settings
MOREbot bot(leftMotor, rightMotor);


//Function that runs only once when the robot turns on
void setup() {
  //Start the different parts of the robot (connects to the motors, ultrasonic, and bluetooth)
  bot.setup();
  
  //Talk to the computer over the usb cable
  Serial.begin(9600);
}

//Function that runs continuously when the robot is on
void loop() {
  //Drives the robot forward at 100% speed
  bot.forward(100);
  
  //Waits 500ms (1/2 of a second)
  delay(500);
  
  //Drives the robot right at 50% speed
  bot.right(50);
  
  //Waits 100ms (1/10 of a second)
  delay(100);
  
  //Drives the robot left at 50% speed
  bot.left(50);
  
  //Waits 100ms (1/5 of a second)
  delay(200);
  
  //Drives the robot backward at 100% speed
  bot.backward(100);
  
  //Waits 100ms (1/2 of a second)
  delay(500);

  //Stops the robot
  bot.stop();
  
  //Waits 1000ms (1 second)
  delay(1000);
}
