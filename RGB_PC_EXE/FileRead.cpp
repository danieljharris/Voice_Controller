#include "FileRead.h"
//int FileRead::getkHz()
//{
//	inputfile.open(location); //Opens file
//
//	string lineIn;
//	getline(inputfile, lineIn); //Starts off search
//	while (lineIn != "kHz:") getline(inputfile, lineIn); //Searches for "kHz:"
//
//	getline(inputfile, lineIn); //Get wanted input
//	int kHz = stoi(lineIn); //Converts string to int
//
//	inputfile.close(); //Closes file
//	return kHz;
//}

vector<Hardware_Data> FileRead::getHardwareData(Hardware hw, SensorType st)
{
	string newLocation = location;

	//Hardware type
	switch (hw)
	{
	case CPU:
		newLocation += "CPU";
		break;
	case GPU:
		newLocation += "GPU";
		break;
	case RAM:
		newLocation += "RAM";
		break;
	case Motherboard:
		newLocation += "Motherboard";
		break;
	case HDD:
		newLocation += "HDD";
		break;
	}
	newLocation += "_";

	//Sensor type
	switch (st)
	{
	case Temperature:
		newLocation += "Temp";
		break;
	case Load:
		newLocation += "Load";
		break;
	case Power:
		newLocation += "Power";
		break;
	case Clocks:
		newLocation += "Clocks";
		break;
	}
	newLocation += "Data.txt";


	//CPU_TempData.txt

	inputfile.open(newLocation); //Opens file

	vector<Hardware_Data> probeData;

	string lineIn;
	getline(inputfile, lineIn); //Starts off search
	while (lineIn != "Data") getline(inputfile, lineIn); //Searches for "Temperature"

	Hardware_Data newProbe;
	newProbe.hw = hw;
	newProbe.st = st;

	while (inputfile.good())
	{
		getline(inputfile, lineIn); //Get wanted input
		if (inputfile.good())newProbe.name = lineIn;

		getline(inputfile, lineIn); //Get wanted input
		if (inputfile.good()) newProbe.data = stoi(lineIn);

		if (inputfile.good()) probeData.push_back(newProbe);
	}

	inputfile.close(); //Closes file

	return probeData;
}
