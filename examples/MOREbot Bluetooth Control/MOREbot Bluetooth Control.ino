// MOREbot_Library - Version: Latest 
#include <MOREbot.h>

#define btname "MOREbot"
#define leftMotor 4
#define rightMotor 1
#define trig 3
#define echo 4
#define rx 8
#define tx 9

MOREbot bot(btname, leftMotor, rightMotor, trig, echo, rx, tx);

void setup() {
  bot.setup();
  Serial.begin(9600);
}

void loop() {
  bot.btControl();
}
