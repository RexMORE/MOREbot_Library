// MOREbot_Library - Version: Latest 
#include <MOREbot.h>

#define btname "MOREbot"
#define LM 4
#define RM 1
#define trig 3
#define echo 4
#define rx 8
#define tx 9

MOREbot bot(btname, LM, RM, trig, echo, rx, tx);

void setup() {
  bot.btSetup();
  Serial.begin(9600);
}

void loop() {
  bot.btControl();
}
