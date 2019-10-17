// MOREbot_Library - Version: Latest 
#include <MOREbot.h>

//Set the robot name
#define BotName "MOREbot"

//Set which motor ports each motor is plugged into
#define leftMotor 1 //M1
#define rightMotor 4 //M4

//Set which pins the Ultrasonic module is plugged into
#define trig A0
#define echo A1

//Set which pins the Bluetooth module is plugged into
#define rx 3
#define tx 4

//Create the MOREbot object with those settings
MOREbot bot(BotName, leftMotor, rightMotor, trig, echo, rx, tx);


void setup() {
	//Start the different parts of the robot (connects to the motors, ultrasonic, and bluetooth)
	bot.setup();
}


int ButtonID = 5;
int SliderID = 7;
String CheckedText = "spin";

void loop() {
	//Looks for new data sent from the bluetooth module
	bot.btLoadData();

	//Checks if a button with the ID of 5 was the last thing sent
	if(bot.getButton() == ButtonID){
		bot.forward(40);
		delay(400);
		bot.stop();
	}
	//Checks if a slider with the ID of 7 was the last thing sent
	else if(bot.getSliderID() == SliderID){
		//Uses the value of that slider as the power for the robot going forward
		bot.forward(bot.getSliderValue());
	}
	//Checks if the string "spin" was the last thing sent
	else if(bot.getString().equalsIgnoreCase(CheckedText)){
		bot.right(70);
		delay(1000);
	}

	delay(10);
	//Stops the robot if anything else was sent
	bot.stop();
}
