#include <MOREbot.h>

//Setup robot with motors
MOREbot bot(1, 4);

void setup() {
  
  bot.setup(6, 7); //Connect to motors and motor encoders

  bot.forward(80, 15); //Drive robot 15 inches forward at 80% speed
  
  delay(1000); //Wait 1 second

  bot.stop(); //Stop robot
}

void loop() {
  
}
