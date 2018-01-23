#include "FileRead.h"

void FileRead::reDirect(string location)
{
	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	string::size_type pos = string(buffer).find_last_of("\\/");
	string local = string(buffer).substr(0, pos);
	local += "\\IR_Signals\\";
	local += location;
	local += ".txt";

	this->location = local;
}

int FileRead::getkHz()
{
	inputfile.open(location); //Opens file

	string lineIn;
	getline(inputfile, lineIn); //Starts off search
	while (lineIn != "kHz:") getline(inputfile, lineIn); //Searches for "kHz:"

	getline(inputfile, lineIn); //Get wanted input
	int kHz = stoi(lineIn); //Converts string to int

	inputfile.close(); //Closes file
	return kHz;
}

vector<int> FileRead::getSignals()
{
	inputfile.open(location); //Opens file

	vector<int> signal;

	string lineIn;
	getline(inputfile, lineIn); //Starts off search
	while (lineIn != "Array:") getline(inputfile, lineIn); //Searches for "Array:"

	while (inputfile.good())
	{
		getline(inputfile, lineIn); //Get wanted input
		signal.push_back(stoi(lineIn)); //Converts string to int
	}

	inputfile.close(); //Closes file

	return signal;
}
