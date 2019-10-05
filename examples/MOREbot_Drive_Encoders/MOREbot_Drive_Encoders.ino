#include <MOREbot.h>

//Setup robot with motors and ultrasonic
MOREbot bot(1, 4);

void setup() {
  
  bot.setup(6, 7); //Connect to motors and motor encoders

  bot.forward(80, 15); //Drive robot 15 inches forward at 80% speed
  
  delay(1000); //Wait 1 second

  bot.left(60, 135); //Turn 135 degrees left at 60% speed (faster speeds might be inaccurate)

  bot.stop(); //Stop robot
}

void loop() {
  
}
