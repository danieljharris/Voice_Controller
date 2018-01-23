#pragma once
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <algorithm>

using namespace std;

enum Hardware { CPU, GPU, RAM, Motherboard, HDD };
enum SensorType { Temperature, Load, Power, Clocks };

class Hardware_Data
{
public:
	Hardware hw;
	SensorType st;

	string name;
	int data;
};

class FileRead
{
protected:
	string location = "C:/Stuff/";
	ifstream inputfile;

public:
	//int getkHz();
	vector<Hardware_Data> getHardwareData(Hardware hw, SensorType st);
};