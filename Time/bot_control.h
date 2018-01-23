#include "tserial.h"
#include <iostream>

class serial{

  private:
        // private attributes
		Tserial *com;
  public:


	serial() {
			
		 }
	
	bool startDevice(char *port,int speed)
	{
		com = new Tserial();
		if (com!=0)
		{
			while(com->connect(port, speed, spNONE))
			{
				printf("Not Connected...\n");

				Sleep(500);
			}
			
			printf("Connected..\n");

			return TRUE;
		}
		else
			return FALSE; 
	}

	void stopDevice()
	{
		com->disconnect();
        // ------------------
        delete com;
        com = 0;
	}

	void send_data(unsigned char data)
	{
		//	unsigned char data = 0;


		com->sendChar(data);
		printf("%c", data);
	}
	void send_data_array(char* data, int length)
	{
		//	unsigned char data = 0;


		com->sendArray(data, length);
		//printf("%c", data);
	}

	unsigned char get_data()
	{
		unsigned char data = com->getChar();
		//printf("%c", data);

		return data;
	}

	void get_data_array(char buffer[])
	{
		Sleep(100);

		char incomingData[256] = "";			// don't forget to pre-allocate memory

		int dataLength = com->getNbrOfBytes();

		if (dataLength > 7)
		{
			//Sleep(100);
			com->getArray(incomingData, com->getNbrOfBytes());

			for (int i = 0; i < 7; i++)
			{
				buffer[i] = incomingData[i];
			}

			buffer[7] = '\0';
		}
	}

	bool compair_get_data(char toMatch[])
	{
		char incomingData[256] = "";			// don't forget to pre-allocate memory

		//Sleep(50);
		com->getArray(incomingData, com->getNbrOfBytes());

		//incomingData[readResult] = 0;

		bool match = true;
		for (int i = 0; i < 7 && match == true; i++)
		{
			if (incomingData[i] != toMatch[i])
			{
				match = false;

				//std::cout << "At location " << i  << " the value: " << incomingData[i];
			}
		}

		return match;
	}
};
