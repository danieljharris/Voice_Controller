/*
 Name:		DC_TO_AC.ino
 Created:	1/2/2017 2:46:22 AM
 Author:	Daniel's Gaming PC
*/

// the setup function runs once when you press reset or power the board
void setup()
{
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
}

// the loop function runs over and over again until power down or reset
void loop()
{
	int delayTime = 10;

	digitalWrite(5, LOW);
	digitalWrite(6, LOW);

	delay(delayTime);

	digitalWrite(5, HIGH);
	digitalWrite(6, HIGH); 

	delay(delayTime);
}
