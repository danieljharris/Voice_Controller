#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <windows.h>
#include <algorithm>

using namespace std;

class FileRead
{
protected:
	string location;
	ifstream inputfile;

public:
	void reDirect(string location);
	int getkHz();
	vector<int> getSignals();
};