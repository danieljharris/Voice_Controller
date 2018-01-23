#include "Windows.h"
#include <stdio.h>
#include <conio.h>
#include "tserial.h"
#include "bot_control.h"
#include <iostream>
#include "VolumeControl.h"
#include "decodeRed.h"
#include "signal.h"
#include "FileRead.h"
#include <list>


static serial comm; //serial is a class type defined in these files, used for referring to the communication device

static int mode = 1; // 1 = Reading || 2 = Writeing (Panasonic) || 3 = GRUNDIG || 4 =  

void sendIRSignal(int kHz, int length, vector<int> IRSignal); //Sends IR signals to arduino to send
void intToCharArray(int value, char chArray[]); //Swaps int to char array
void readMode1(int value);
void readMode2(int value);

void main() {

	char comNum[] = "COM4";
	char tempNum;
	cout << "Welcome to Daniel's Infrared USB Remote!\n\nEnter COM number to get started:";
	cin >> tempNum;
	cout << "\n";

	comNum[3] = tempNum;
	
	//comm.startDevice(comNum, 9600);
	comm.startDevice(comNum, 250000);
	
	//Reading && Writeing
	while (true)
	{
		//Reads in buffer
		char buffer[8] = "";
		comm.get_data_array(buffer);

		if (buffer[0] != NULL)
		{
			//Decodes input
			int value = (int)strtol(buffer, NULL, 16);

			//Reading
			if (mode == 1)
			{
				readMode1(value);
			}
			//Writeing ()
			else if (mode == 2)
			{
				readMode2(value);
			}
		}

	}


	cout << "\nShutting down...";
	Sleep(1000);
	comm.stopDevice(); //The device is closed down
}

void readMode1(int value)
{
	decodeRed red;
	string decodedValue = red.decode(value);

	//Sets up virtual key strokes
	BYTE keyState[256];
	GetKeyboardState((LPBYTE)&keyState);

	if (decodedValue == "sky")
	{
		mode++;

		cout << "Mode: " << mode << "\n";
	}
	else if (decodedValue == "volume_up")
	{
		keybd_event(VK_VOLUME_UP,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0);
	}
	else if (decodedValue == "volume_down")
	{
		keybd_event(VK_VOLUME_DOWN,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0);
	}
	else if (decodedValue == "channel_up")
	{
		
	}
	else if (decodedValue == "channel_down")
	{
		
	}
	else if (decodedValue == "mute")
	{
		keybd_event(VK_VOLUME_MUTE,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0);
	}
	else if (decodedValue == "information")
	{
		keybd_event(0x46,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0);
	}
	else if (decodedValue == "select")
	{
		keybd_event(VK_RETURN,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0);
	}
	else if (decodedValue == "left")
	{
		keybd_event(VK_CONTROL,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0);
		keybd_event(VK_SHIFT,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0);
		keybd_event(VK_TAB,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0);
	}
	else if (decodedValue == "right")
	{
		keybd_event(VK_CONTROL,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0);
		keybd_event(VK_TAB,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0);
	}
	else if (decodedValue == "text")
	{
		keybd_event(VK_SHIFT,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0);
		keybd_event(VK_TAB,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0);
	}
	else if (decodedValue == "help")
	{
		keybd_event(VK_TAB,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0);
	}
	else if (decodedValue == "play" || decodedValue == "pause")
	{
		keybd_event(VK_SPACE,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0);
	}
	else if (decodedValue == "back")
	{
		keybd_event(VK_SHIFT,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0);
		keybd_event(0x50,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0);
	}
	else if (decodedValue == "forward")
	{
		keybd_event(VK_SHIFT,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0);
		keybd_event(0x4E,
			0x45,
			KEYEVENTF_EXTENDEDKEY | 0,
			0);
	}
	else if (decodedValue == "power")
	{
		////Shut down this program
		cout << "\nShutting down program...";
		Sleep(1000);
		comm.stopDevice(); //The device is closed down
		exit(0);

		//Shuts down PC
		//cout << "\nShutting down PC...";
		//system("shutdown.exe /s /t 00");
	}
	else
	{
		cout << "New Button 1: " << value << "\n";
	}

	Sleep(5);

	keybd_event(VK_SHIFT,
		0x45,
		KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
		0);
	keybd_event(VK_RETURN,
		0x45,
		KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
		0);
	keybd_event(VK_CONTROL,
		0x45,
		KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
		0);
}
void readMode2(int value)
{
	decodeRed red;
	string decodedValue = red.decode(value);

	FileRead readIn;
	
	bool known = true;

	if (decodedValue == "sky")
	{
		mode = 1;

		cout << "Mode: " << mode << "\n";
		known = false;
	}
	else if (decodedValue == "play")
	{
		comm.send_data('P');
		known = false;
	}
	else if (decodedValue == "services")	readIn.reDirect("Panasonic\\Menu");
	else if (decodedValue == "up")			readIn.reDirect("Panasonic\\Up");
	else if (decodedValue == "down")		readIn.reDirect("Panasonic\\Down");
	else if (decodedValue == "left")		readIn.reDirect("Panasonic\\Left");
	else if (decodedValue == "right")		readIn.reDirect("Panasonic\\Right");
	else if (decodedValue == "select")		readIn.reDirect("Panasonic\\OK");
	else
	{
		cout << "New Button 2: " << value << "\n";
		known = false;
	}

	if (known)
	{
		vector<int> signals = readIn.getSignals();
		sendIRSignal(readIn.getkHz(), signals.size(), signals);
	}
}

void sendIRSignal(int kHz, int length, vector<int> IRSignal)
{
	const char END = '#';

	char chBuff[5] = { NULL,NULL,NULL,NULL,NULL }; //Must allways null to start!


	comm.send_data('S'); //Starts sending beeps (Info and char array)

	intToCharArray(kHz, chBuff);
	//Sends kHz || And cleans
	for (int i = 4; i >= 0; i--)
	{
		if (chBuff[i] != NULL)
		{
			comm.send_data(chBuff[i]);
			chBuff[i] = NULL;
		}
	}
	comm.send_data(END);

	intToCharArray(length, chBuff);
	//Sends length of array || And cleans
	for (int i = 4; i >= 0; i--)
	{
		if (chBuff[i] != NULL)
		{
			comm.send_data(chBuff[i]);
			chBuff[i] = NULL;
		}
	}
	comm.send_data(END);

	Sleep(300);
	for (int i = 0; i < length; i++)
	{
		intToCharArray(IRSignal[i], chBuff);

		//Sends 1 beep || And cleans
		for (int i = 4; i >= 0; i--)
		{
			if (chBuff[i] != NULL)
			{
				comm.send_data(chBuff[i]);
				chBuff[i] = NULL;
			}
		}
		comm.send_data(','); //End of beep
	}
	comm.send_data(END);
}
void intToCharArray(int value, char chArray[]) //Swaps int to char array
{
	string s = to_string(value);
	char const *pchar = s.c_str();

	strcpy(chArray, pchar);
}