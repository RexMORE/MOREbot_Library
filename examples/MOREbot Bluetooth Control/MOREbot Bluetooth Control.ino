// MOREbot_Library - Version: Latest 
#include <MOREbot.h>

#define btname "MOREbot"
#define left_Motor 4
#define right_Motor 1
#define trig 3
#define echo 4
#define rx 8
#define tx 9

MOREbot bot(btname, left_Motor, right_Motor, trig, echo, rx, tx);

void setup() {
  bot.setup();
  Serial.begin(9600);
}

void loop() {
  bot.btControl();
}
