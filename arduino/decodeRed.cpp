#include "decodeRed.h"

string decodeRed::decode(int code)
{
	string command = "void";

	switch (code)
	{
		case 16754775: case 67274871: case 1168:
			command = "volume_up";
			break;

		case 16769055: case 67258551: case 3216:
			command = "volume_down";
			break;

		case 12586016:
			command = "channel_up";
			break;

		case 12586017:
			command = "channel_down";
			break;

		case 67299351: case 656:
			command = "mute";
			break;

		case 12586076:
			command = "select";
			break;

		case 12586187:
			command = "information";
			break;

		case 12586020:
			command = "pause";
			break;

		case 12586046:
			command = "play";
			break;

		case 12586045:
			command = "backward";
			break;

		case 12586024:
			command = "forward";
			break;

		case 12585996:
			command = "power";
			break;

		case 12586116:
			command = "tv";
			break;
			
		case 12586112:
			command = "sky";
			break;

		case 12586110:
			command = "services";
			break;

		case 12586072:
			command = "up";
			break;

		case 12586073:
			command = "down";
			break;

		case 12586074:
			command = "left";
			break;

		case 12586075:
			command = "right";
			break;

		case 12586044:
			command = "text";
			break;

		case 12586113:
			command = "help";
			break;

		case 12586093:
			command = "red";
			break;

		case 12586094:
			command = "green";
			break;

		case 12586095:
			command = "yellow";
			break;

		case 12586096:
			command = "blue";
			break;
	}

	return command;
};


int decodeRed::encode(string command, bool sky)
{
	int code = 0;

	if (command == "volume_up") code == (sky ? 16754775 : 67274871);
	else if (command == "volume_down") code == (sky ? 16769055 : 67258551);
	//else if (command == "volume_up") code == (sky ? 16754775 : 67274871);

	return code;
};