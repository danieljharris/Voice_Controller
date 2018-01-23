#include <stdio.h>
#include <tchar.h>
#include "SerialClass.h"	// Library described above
#include <string>
#include <iostream>

// application reads from the specified serial port and reports the collected data
void main()
{
	printf("Welcome to the serial test app!\n");

	Serial* SP = new Serial("\\\\.\\COM9");    // adjust as needed

	//if (SP->IsConnected())
		//printf("We're connected\n\n");

	char incomingData[256] = "";			// don't forget to pre-allocate memory
											//printf("%s\n",incomingData);
	int dataLength = 255;
	int readResult = 0;

	while (SP->IsConnected())
	{
		readResult = SP->ReadData(incomingData, dataLength);
		// printf("Bytes read: (0 means no data available) %i\n",readResult);
		incomingData[readResult] = 0;

		//printf("%s", incomingData);
		//std::cout << std::string(incomingData);

		bool match = true;
		char toMatch[] = "4028877";
		for (int i = 0; i < 7 && match == true; i++)
		{
			if (incomingData[i] != toMatch[i])
			{
				match = false;

				//std::cout << "At location " << i  << " the value: " << incomingData[i];
			}
		}

		if (match)
		{
			std::cout << "YES!\n";
		}
		Sleep(100);
	}

	system("pause");
}