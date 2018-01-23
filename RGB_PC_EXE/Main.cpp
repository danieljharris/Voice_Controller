#include "Windows.h"
#include "tserial.h"
#include "bot_control.h"
#include "signal.h"
#include "FileRead.h"
#include "RGBClass.h"

#include <iostream>
#include <vector>
#include <stdio.h>
#include <conio.h>
#include <list>
#include <string>
#include <stdint.h>
#include <iomanip>
#include <sstream>

using namespace std;

static serial comm; //serial is a class type defined in these files, used for referring to the communication device

int getCPU_Temp(); //Gets the average CPU temp


void sendSegmentSignal(int segment, RGB color); //Sends color signals to arduino
void sendSegmentSignal(int segment, unsigned long color); //Sends color signals to arduino
void intToCharArray(int value, char chArray[]); //Swaps int to char array
void uLongToCharArray(unsigned long value, char chArray[]); //Swaps unsigned long to char array

RGB temperatureToRGB(int temp);
RGB colorConverter(unsigned long hexValue);
unsigned long colorConverter(RGB color);



void main()
{
	char comNum[] = "COM3";
	char tempNum;
	cout << "Welcome to Daniel's RGB PC!\n\nEnter COM number to get started:";
	//cin >> tempNum;
	//cout << "\n";

	//comNum[3] = tempNum;

	comm.startDevice(comNum, 115200);


	//Segment 5 color RED
	//Sleep(1000);
	//sendSegmentSignal(5, 0xFF0000);
	//Sleep(1000);




	/*cout << "Average CPU Temp: " << averageTemp << "\n";
	cout << "CPU Color Red: " << temperatureToRGB(averageTemp).r << "\n";
	cout << "CPU Color Green: " << temperatureToRGB(averageTemp).g << "\n";
	cout << "CPU Color Blue: " << temperatureToRGB(averageTemp).b << "\n";*/

	while (true)
	{
		/*cout << "Average CPU Temp: " << averageTemp << "\n";
		cout << "CPU Color Red: " << temperatureToRGB(averageTemp).r << "\n";
		cout << "CPU Color Green: " << temperatureToRGB(averageTemp).g << "\n";
		cout << "CPU Color Blue: " << temperatureToRGB(averageTemp).b << "\n";*/


		int averageTemp = getCPU_Temp();
		RGB color = temperatureToRGB(averageTemp);
		cout << "Average temp: " << averageTemp << "\n";

		sendSegmentSignal(2, color);




		Sleep(1000);
	}



	cout << "\n\n\n";
	system("pause");

	comm.stopDevice(); //The device is closed down
}

int getCPU_Temp()
{
	FileRead inFile;

	vector<Hardware_Data> CPU_Temp = inFile.getHardwareData(CPU, Temperature);

	int averageTemp = 0;
	for (vector<Hardware_Data>::iterator itr = CPU_Temp.begin(); itr != CPU_Temp.end(); itr++)
	{
		averageTemp += (*itr).data;
	}

	averageTemp = averageTemp / 4;

	return averageTemp;
}

void sendSegmentSignal(int segment, RGB color)
{
	sendSegmentSignal(segment, colorConverter(color));
}
void sendSegmentSignal(int segment, unsigned long color)
{
	//0xFFFFFF (6 Char long)
	//Max 9 segments (1 Char long)

	char charArray[] = "C5,FF0000#";
	int charPtr = 0;

	const char END = '#';
	const char SEPERATOR = ',';

	//comm.send_data('C'); //Starts sending beeps (Info and char array)
	charArray[charPtr] = 'C'; charPtr++;

	//Sends Segment || And cleans
	char chBuffSegment[2] = { NULL }; //Must allways null to start!

	intToCharArray(segment, chBuffSegment);

	//comm.send_data(chBuffSegment[0]);
	charArray[charPtr] = chBuffSegment[0]; charPtr++;

	//cout << "Array: " << charArray[0] << charArray[1] << "\n";

	//comm.send_data(SEPERATOR);
	charArray[charPtr] = SEPERATOR; charPtr++;


	//Sends HEX || And cleans
	char chBuffColor[7] = { NULL, NULL, NULL, NULL, NULL, NULL }; //Must allways null to start!
	uLongToCharArray(color, chBuffColor);

	for (int i = 0; i < 6; i++)
	{
		//comm.send_data(chBuffColor[i]);
		charArray[charPtr] = chBuffColor[i]; charPtr++;
		
		chBuffColor[i] = NULL;
	}


	//comm.send_data(END);
	charArray[charPtr] = END; charPtr++;
	//charArray[charPtr] = '\0'; charPtr++;

	comm.send_data_array(charArray, 11);
	//comm.send_data_array("C5,FF0000#", 11);
}
void intToCharArray(int value, char chArray[]) //Swaps int to char array
{
	string s = to_string(value);
	char const *pchar = s.c_str();

	strcpy(chArray, pchar);
}

void uLongToCharArray(unsigned long toBeHex, char chArray[]) //Swaps unsigned long to char array
{
	//uLong to string
	stringstream stream;
	stream	<< setfill('0')
			<< setw((sizeof(unsigned long) * 2) - 2)
			<< hex << toBeHex;

	//String to char array
	string s = stream.str();
	char const *pchar = s.c_str();
	strcpy(chArray, pchar);
}

RGB temperatureToRGB(int temp)
{
	//Temperature information:
	const int MIN = 20;
	const int MAX = 90;

	temp = temp - MIN;
	
	double percentTemp = (MAX - MIN);
	percentTemp = temp / percentTemp;
	percentTemp = percentTemp * 100;

	RGB color;
	color.r = (percentTemp / 100) * 255;
	color.g = 255 - ( (percentTemp / 100) * 255 );

	return color;
}

RGB colorConverter(unsigned long hexValue)
{
	RGB rgbColor;

	rgbColor.r = ((hexValue >> 16) & 0xFF) - 10;  // Extract the RR byte
	rgbColor.g = ((hexValue >> 8) & 0xFF) - 10;  // Extract the GG byte
	rgbColor.b = ((hexValue >> 0) & 0xFF) - 5;  // Extract the BB byte

	return rgbColor;
}
unsigned long colorConverter(RGB color)
{
	return ((color.r & 0xff) << 16) + ((color.g & 0xff) << 8) + (color.b & 0xff);
}
