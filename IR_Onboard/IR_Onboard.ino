#include <IRremote.h>
#include <stdio.h>

#define IR_IN_PIN 2 //IR input pin
#define IR_OUT_PIN 3 //IR output pin

IRrecv irIn(IR_IN_PIN);

decode_results results;
decode_results gotVal;

decode_results lastVal;

void setup()
{
	//Serial.begin(9600);
	Serial.begin(250000);

	pinMode(IR_IN_PIN, INPUT);

	pinMode(IR_OUT_PIN, OUTPUT);
	digitalWrite(IR_OUT_PIN, LOW);   //Make sure LED starts "off"

	irIn.enableIRIn(); // Start the receiver

	irIn.blink13(1); //Sets pin 13 to blink every time an IR signal is being decoded
}

void loop()
{
	//Send data only when you receive data:
	//if (Serial.available() > 0)
	//{
	//	// read the incoming byte:
	//	char incomingByte = Serial.read();

	//	if (incomingByte == 'P')
	//	{
	//		Serial.println(0xC00C80, HEX);    //Sky
	//		delay(70); //Required delay between commands
	//		Serial.println(0x402E817, HEX);   //Mute
	//		delay(70);
	//	}
	//	else if (incomingByte == 'S')
	//	{
	//		int kHz = 0;
	//		int NumIRsignals = 0;
	//		int IRsignal[200];

	//		getEverything(&NumIRsignals, &kHz, IRsignal);

	//		//delay(200);

	//		for (int i = 0; i < NumIRsignals; i += 2) //Loop through all of the IR timings
	//		{
	//			pulseIR(IRsignal[i] * 10, kHz);            //Flash IR LED at 38khz for the right amount of time
	//			delayMicroseconds(IRsignal[i + 1] * 10); //Then turn it off for the right amount of time
	//		}

	//	}//incomingByte == 'S'
	//}//if (Serial.available() > 0)
	
	results.rawlen;
	//if(irIn.decodeHash(&results))
	if (irIn.decode(&results))
	{
		if (results.value != 0xFFFFFFFF
			&& results.value != 0x4CB0FADD
			&& results.value != 0xFCABFFBF
			&& results.value != 0xDDE32E9D
			&& results.value != 0x22AE7A28
			&& results.value != 0x9716BE3F
			&& results.value != 0x7A6AD1F)
		{
			gotVal = results;
		}

		//Sends the data to C++ program
		Serial.println(gotVal.value, HEX);

		//Faster if changing vol:
		if (gotVal.value == lastVal.value
			&& (gotVal.value == 0x4028877
				|| gotVal.value == 0x40248B7
				|| gotVal.value == 0x490
				|| gotVal.value == 0xC90))
		{
			delay(20);
		}
		else
		{
			delay(300);
		}

		lastVal = gotVal;

		irIn.resume(); // Receive the next value
	}
}

void getEverything(int *NumIRsignals, int *kHz, int IRsignal[])
{
	char chBuff[5] = { '0', '0', '0', '0', '0' };
	int chPointer = 4;
	int area = 0;

	int signalPointer = 0;

	while (area < 3)
	{
		if (Serial.available() > 0)
		{
			// read the incoming byte:
			char incomingByte = Serial.read();

			if (incomingByte == '#') // Preps for next value
			{
				if (area == 0)       *kHz = atoi(chBuff);
				else if (area == 1)  *NumIRsignals = atoi(chBuff);

				//Resets chBuff for next value
				for (int i = 0; i < 5; i++)
				{
					chBuff[i] = '0';
				}
				chPointer = 4;
				//Resets chBuff for next value

				area++; //Moves on to collect next value
			}
			else if (incomingByte == ',')
			{
				IRsignal[signalPointer] = atoi(chBuff);

				//Resets chBuff for next value
				for (int i = 0; i < 5; i++)
				{
					chBuff[i] = '0';
				}
				chPointer = 4;
				//Resets chBuff for next value

				signalPointer++;
			}
			else
			{
				if (chPointer >= 0)
				{
					chBuff[chPointer] = incomingByte;
					chPointer--;
				}
			}
		}
	}
}

// This function allows us to PWM the IR LED at about 38khz/40khz for the sensor
// Borrowed from Adafruit!
void pulseIR(long microsecs, int kHz)
{
	int freq = 1000 / kHz;
	int dely = (freq - (3 * 2)) / 2;

	cli();  //Turns off background interrupts

	while (microsecs > 0) // we'll count down from the number of microseconds we are told to wait
	{
		// 38 kHz is about 13 microseconds high and 13 microseconds low
		digitalWrite(IR_OUT_PIN, HIGH);  // this takes about 3 microseconds to happen
		delayMicroseconds(dely);         // hang out for 10 microseconds, you can also change this to 9 if its not working
		digitalWrite(IR_OUT_PIN, LOW);   // this also takes about 3 microseconds
		delayMicroseconds(dely);         // hang out for 10 microseconds, you can also change this to 9 if its not working

										 // so 26 microseconds altogether
		microsecs -= freq;
	}

	sei();  //Turns background interrupts back on
}