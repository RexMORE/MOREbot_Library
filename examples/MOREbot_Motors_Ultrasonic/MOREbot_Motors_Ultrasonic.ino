#include <MOREbot.h>

//Setup robot with motors and ultrasonic
MOREbot bot(1, 4);

void setup() {
  
  bot.setup(); //Connect to motors
  
}

void loop() {
  float distance = bot.readDistance(); //Get distance from ultrasonic module
  
  if(distance > 0){
    if(distance < 30){ //Compare distance to the number 30
      
      bot.leftMotor(50); //Spin left motor clockwise at 50% speed
      
    }else{
      
      bot.leftMotor(0); //Stop left motor
      
    }
  }
}
