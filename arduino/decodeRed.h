#pragma once
#include "windows.h"
#include <string>

using namespace std;

class decodeRed
{
	public:
		string decode(int code);
		int encode(string command, bool sky);
};