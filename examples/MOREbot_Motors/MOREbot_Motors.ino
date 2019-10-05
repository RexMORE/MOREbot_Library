#include <MOREbot.h>

//Setup robot with motors
MOREbot bot(1, 4);

void setup() {
  
  bot.setup(); //Connect to motors

  bot.leftMotor(50); //Spin left motor clockwise at 50% speed
  bot.rightMotor(-40); //Spin right motor counter-clockwise at 40% speed
  
  delay(1000); //Wait 1 second

  bot.stop(); //Stop both motors
}

void loop() {
  
}
