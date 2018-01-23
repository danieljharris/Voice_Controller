#pragma once
const int readInterval = 80;
const int lightInterval = 10;

unsigned long currentMillis = 0;    // stores the value of millis() in each iteration of loop()
unsigned long previousReadMillis = 0;
unsigned long previousLightMillis = 0;

void read();
void light();

void loop()
{
	currentMillis = millis();

	read();
	light();
}

void read()
{
	if (millis() - previousReadMillis >= readInterval)
	{



		previousReadMillis += readInterval;
	}
}

void light()
{
	if (millis() - previousLightMillis >= lightInterval)
	{



		previousLightMillis += lightInterval;
	}
}