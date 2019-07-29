#include <MOREbot.h>

//Setup robot with motors and ultrasonic
MOREbot bot(1, 4);

void setup() {
  
  bot.setup(); //Connect to motors

  bot.forward(80); //Drive robot forward at 80% speed
  
  delay(1000); //Wait 1 second

  bot.stop(); //Stop robot
}

void loop() {
  
}
