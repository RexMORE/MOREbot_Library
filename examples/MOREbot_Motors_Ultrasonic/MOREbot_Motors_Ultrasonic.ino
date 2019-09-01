#include <MOREbot.h>

//Setup robot with motors in ports 1 and 4 and ultrasonic in pins A0 (trig) and A1 (echo)
MOREbot bot(1, 4, A0, A1);

void setup() {
  
  bot.setup(); //Connect to motors
  
}

void loop() {
  float distance = bot.readDistance(); //Get distance from ultrasonic module
  
  if(!(distance < 0)){
    if(distance < 30){ //Compare distance to the number 30
      
      bot.leftMotor(50); //Spin left motor clockwise at 50% speed
      
    }else{
      
      bot.leftMotor(0); //Stop left motor
      
    }
  }

  
}
