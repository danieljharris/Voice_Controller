/*
 Name:		RGB_PC.ino
 Created:	3/7/2017 9:56:08 PM
 Author:	Daniel's Gaming PC
*/

#include "RGBClass.h"

#define SEGMENT_SELECT(pin, state) state == true ? digitalWrite(pin, LOW) : digitalWrite(pin, HIGH)
#define REDUCE(start, amount) start >= amount ? start - amount : 0

const byte red = 9;
const byte green = 10;
const byte blue = 11;

RGB* SegStorage[9];

RGB colorConverter(unsigned long hexValue)
{
	RGB rgbColor;

	rgbColor.r = ((hexValue >> 16) & 0xFF);  // Extract the RR byte
	rgbColor.g = ((hexValue >> 8 ) & 0xFF);  // Extract the GG byte
	rgbColor.b = ((hexValue >> 0 ) & 0xFF);  // Extract the BB byte
	
	return rgbColor;
}

void color(RGB decodedColor)
{
	//INVERTER -----------------------------------------
	decodedColor.r = 255 - decodedColor.r;
	decodedColor.g = 255 - decodedColor.g;
	decodedColor.b = 255 - decodedColor.b;
	//INVERTER -----------------------------------------


	analogWrite(red, decodedColor.r);
	analogWrite(green, decodedColor.g);
	analogWrite(blue, decodedColor.b);

	//Serial.println(decodedColor.r);
	//Serial.println(decodedColor.g);
	//Serial.println(decodedColor.b);
	//Serial.println(" ");
}

void color(unsigned long hex)
{
	analogWrite(red,	((hex >> 16) & 0xFF));
	analogWrite(green,	((hex >> 8)  & 0xFF));
	analogWrite(blue,	((hex >> 0)  & 0xFF));
}

void color(RGB decodedColor, byte dim)
{
	RGB dimColor;

	if (dim != 0)
	{
		dimColor.r = decodedColor.r / dim;
		dimColor.g = decodedColor.g / dim;
		dimColor.b = decodedColor.b / dim;
	}
	else
	{
		dimColor.r = decodedColor.r;
		dimColor.g = decodedColor.g;
		dimColor.b = decodedColor.b;
	}

	color(dimColor);
}

void color(unsigned long hex, byte dim)
{
	//analogWrite(red, ((hex >> 16) & 0xFF) / dim);
	//analogWrite(green, ((hex >> 8) & 0xFF) / dim);
	//analogWrite(blue, ((hex >> 0) & 0xFF) / dim);

	analogWrite(red,   REDUCE(((hex >> 16) & 0xFF), dim));
	analogWrite(green, REDUCE(((hex >> 8)  & 0xFF), dim));
	analogWrite(blue,  REDUCE(((hex >> 0)  & 0xFF), dim));

	//Serial.println(REDUCE(((hex >> 16) & 0xFF), dim));
	//Serial.println(REDUCE(((hex >> 8) & 0xFF), dim));
	//Serial.println(REDUCE(((hex >> 0) & 0xFF), dim));
	//Serial.println(" ");
}

void colorSegment(byte segment, unsigned long hex, byte dim)
{
	color(colorConverter(hex), dim);

	SEGMENT_SELECT(segment, true);
	delay(1000);
	SEGMENT_SELECT(segment, false);
}

void fade(RGB rgbStart, RGB rgbDest, byte dim)
{
	bool redReached = false;
	bool greenReached = false;
	bool blueReached = false;

	while (!redReached || !greenReached || !blueReached)
	{
		//Red
		if (rgbStart.r == rgbDest.r) redReached = true;
		else if (rgbStart.r < rgbDest.r) rgbStart.r++;
		else if (rgbStart.r > rgbDest.r) rgbStart.r--;

		//Green
		if (rgbStart.g == rgbDest.g) greenReached = true;
		else if (rgbStart.g < rgbDest.g) rgbStart.g++;
		else if (rgbStart.g > rgbDest.g) rgbStart.g--;

		//Blue
		if (rgbStart.b == rgbDest.b) blueReached = true;
		else if (rgbStart.b < rgbDest.b) rgbStart.b++;
		else if (rgbStart.b > rgbDest.b) rgbStart.b--;

		color(rgbStart, dim);

		delay(10);
	}
}

void fade(unsigned long rgbStart, unsigned long rgbDest, byte dim)
{
	fade(colorConverter(rgbStart), colorConverter(rgbDest), dim);
}

void readSignalFromPC()
{
	//Send data only when you receive data:
	//if (Serial.available() > 0)
	//{
		//Reads the incoming byte:
		char incomingByte = Serial.read();

		if (incomingByte == 'C')
		{
			byte segment = 0;
			unsigned long colorData = 0;

			incomingByte = Serial.read(); //Read new byte

			while (incomingByte != ',')
			{
				if (Serial.available() > 0)
				{
					//Fits new byte in to signle char (Needs to be char array for some reason)
					char chSeg[1] = { '0' };
					chSeg[0] = incomingByte;

					//Moves char to int
					segment = atoi(chSeg);

					segment = segment;

					incomingByte = Serial.read(); //Read new byte
				}
			}

			char chColor[8] = { '0', 'x', '0', '0', '0', '0', '0', '0' };
			byte chPointer = 2;

			while (incomingByte != '#')
			{
				if (Serial.available() > 0)
				{
					incomingByte = Serial.read(); //Read new byte

					//Fits new byte in to signle char (Needs to be char array for some reason)
					chColor[chPointer] = incomingByte;
					chPointer++;
				}
			}

			//Moves char to int
			colorData = strtol(chColor, NULL, 16);


			//Serial.print("Segment: ");
			//Serial.println(segment);

			//Serial.print("Color: ");
			//Serial.println(colorData, HEX);


			//colorSegment(segment, colorData, 10);
			*SegStorage[segment] = colorConverter(colorData);
		}
	//}
}

void animationFadeLine()
{
	SEGMENT_SELECT(2, true);
	SEGMENT_SELECT(3, true);
	SEGMENT_SELECT(4, true);
	SEGMENT_SELECT(5, true);

	int dim = 0;
	//int dim = 10;

	//Red
	fade(0x000000, 0xFF0000, dim);
	fade(0xFF0000, 0x000000, dim);

	//Green
	fade(0x000000, 0x00FF00, dim);
	fade(0x00FF00, 0x000000, dim);

	//Blue
	fade(0x000000, 0x0000FF, dim);
	fade(0x0000FF, 0x000000, dim);
}

//void animationSegSwap()
//{
//	int tDelay = 5;
//
//	//color(myColor);
//
//	//color(0x90C3D4, 10);
//	//color(0xFFFFFF, 10);
//
//	color(0xFF0000, 10);
//
//	SEGMENT_SELECT(2, true);
//	delay(tDelay);
//	SEGMENT_SELECT(2, false);
//
//	color(0x00FF00, 10);
//
//	SEGMENT_SELECT(3, true);
//	delay(tDelay);
//	SEGMENT_SELECT(3, false);
//
//	color(0x0000FF, 10);
//
//	SEGMENT_SELECT(4, true);
//	delay(tDelay);
//	SEGMENT_SELECT(4, false);
//
//	color(0xFFFFFF, 10);
//
//	SEGMENT_SELECT(5, true);
//	delay(tDelay);
//	SEGMENT_SELECT(5, false);
//}

void animationSegSwapNew()
{
	byte tDelay = 1;

	SEGMENT_SELECT(2, true);
	delay(tDelay);
	color(0x00FF00);
	SEGMENT_SELECT(2, false);

	SEGMENT_SELECT(3, true);
	delay(tDelay);
	color(0x0000FF);
	SEGMENT_SELECT(3, false);

	SEGMENT_SELECT(4, true);
	delay(tDelay);
	color(0xFFFFFF);
	SEGMENT_SELECT(4, false);

	SEGMENT_SELECT(5, true);
	delay(tDelay);
	color(0xFF0000);
	SEGMENT_SELECT(5, false);
}

void setup()
{
	Serial.begin(115200);

	pinMode(red, OUTPUT);
	pinMode(green, OUTPUT);
	pinMode(blue, OUTPUT);

	digitalWrite(red, LOW);
	digitalWrite(green, LOW);
	digitalWrite(blue, LOW);

	pinMode(2, OUTPUT);
	pinMode(3, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(5, OUTPUT);


	SEGMENT_SELECT(2, false);
	SEGMENT_SELECT(3, false);
	SEGMENT_SELECT(4, false);
	SEGMENT_SELECT(5, false);


	for (byte i = 0; i < 10; i++)
	{
		SegStorage[i] = new RGB;
	}



	//*SegStorage[2] = colorConverter(0x00E9FF);
	//*SegStorage[3] = colorConverter(0x00E9FF);
	//*SegStorage[4] = colorConverter(0x00E9FF);
	//*SegStorage[5] = colorConverter(0xFF0000);


	////TIME_DELAY = 798;
	//*SegStorage[2] = colorConverter(0x0F0F0F);
	//*SegStorage[3] = colorConverter(0x0F0F0F);
	//*SegStorage[4] = colorConverter(0x0F0F0F);
	//*SegStorage[5] = colorConverter(0xFFFFFF);




	//*SegStorage[2] = colorConverter(0x629c00);
	//*SegStorage[3] = colorConverter(0x629c00);
	//*SegStorage[4] = colorConverter(0x629c00);



	//*SegStorage[2] = colorConverter(0xec1200);



	//*SegStorage[2] = colorConverter(0x629c00);
	//*SegStorage[4] = colorConverter(0x0F0F0F);


	*SegStorage[2] = colorConverter(0x57a700);
}

//const int TIME_DELAY = 798;
//const int TIME_DELAY = 605;
const int TIME_DELAY = 900;
const int DIM = 10;

void loop()
{
	animationFadeLine();

	//animationSegSwap();
	//animationSegSwapNew();
	//animationFadeLine();

	//while (true)
	//{
	//	for (byte i = 2; i <= 5; i++)
	//	{
	//		//unsigned long start = micros();
	//		//color(*SegStorage[i], DIM);
	//		//unsigned long end = micros();

	//		//SEGMENT_SELECT(i, true);

	//		//if (Serial.available() > 0)
	//		//{
	//		//	//unsigned long end = micros();

	//		//	//unsigned long readStart = micros();
	//		//	readSignalFromPC();
	//		//	//unsigned long readEnd = micros();

	//		//	//delayMicroseconds(TIME_DELAY - (end - start) - (readEnd - readStart));
	//		//}
	//		//else
	//		//{
	//			//unsigned long end = micros();
	//			//Serial.println(end-start);

	//			//delayMicroseconds(TIME_DELAY - (end - start));
	//			//delayMicroseconds(TIME_DELAY);
	//			//delay(3);
	//		//}

	//		
	//		//delay(6);




	//		//SEGMENT_SELECT( ((i-1)==1?5:i-1) , false);


	//		//if (Serial.available() > 0) readSignalFromPC();

	//		//unsigned long start = micros();
	//		//color(*SegStorage[i]);
	//		//unsigned long end = micros();


	//		SEGMENT_SELECT(i, false);
	//		SEGMENT_SELECT(i, true);

	//		unsigned long start = micros();
	//		if (Serial.available() > 0) readSignalFromPC();
	//		unsigned long end = micros();

	//		delayMicroseconds(2000 - (end - start));
	//		//delayMicroseconds(2000);

	//		color(*SegStorage[((i + 1) == 6 ? 2 : i + 1)]);
	//		delayMicroseconds(2000);

	//		SEGMENT_SELECT(i, false);
	//	}
	//}

	//fade(0x010101, 0xFFFFFF, 10);
	//fade(0xFFFFFF, 0x010101, 10);
}