#include "tserial.h"
#include "bot_control.h"
#include <iostream>
#include <list>
#include <string>
#include <time.h>

using namespace std;

static serial comm; //serial is a class type defined in these files, used for referring to the communication device

void send(int Hour, int Minute); //Sends IR signals to arduino to send
void intToCharArray(int value, char chArray[]); //Swaps int to char array

void main() {

	char comNum[] = "COM4";
	char tempNum;
	cout << "Welcome to Daniel's Time Display!\n\nEnter COM number to get started:";
	cin >> tempNum;
	cout << "\n";

	comNum[3] = tempNum;
	
	comm.startDevice(comNum, 9600);
	
	//Reading && Writeing
	while (true)
	{
		time_t theTime = time(NULL);
		struct tm *aTime = localtime(&theTime);

		int dayOfWeek = aTime->tm_wday;
		int day = aTime->tm_mday;
		int month = aTime->tm_mon + 1; // Month is 0 – 11, add 1 to get a jan-dec 1-12 concept
		int year = aTime->tm_year + 1900; // Year is # years since 1900
		int hour = aTime->tm_hour;
		int min = aTime->tm_min;
		int sec = aTime->tm_sec;

		if (hour > 12)
		{
			hour -= 12;
		}

		send(hour, min);
		//send(12, 34);

		Sleep(60000);
	}


	cout << "\nShutting down...";
	Sleep(1000);
	comm.stopDevice(); //The device is closed down
}


void send(int Hour, int Minute)
{
	const char END = '#';

	char chBuff[5] = { NULL,NULL }; //Must allways null to start!


	comm.send_data('T'); //Starts sending beeps (Info and char array)

	intToCharArray(Hour, chBuff);
	//Sends kHz || And cleans
	for (int i = 1; i >= 0; i--)
	{
		if (chBuff[i] != NULL)
		{
			comm.send_data(chBuff[i]);
			chBuff[i] = NULL;
		}
	}
	comm.send_data(END);

	intToCharArray(Minute, chBuff);
	//Sends length of array || And cleans
	for (int i = 1; i >= 0; i--)
	{
		if (chBuff[i] != NULL)
		{
			comm.send_data(chBuff[i]);
			chBuff[i] = NULL;
		}
	}
	comm.send_data(END);
}
void intToCharArray(int value, char chArray[]) //Swaps int to char array
{
	string s = to_string(value);
	char const *pchar = s.c_str();

	strcpy(chArray, pchar);
}