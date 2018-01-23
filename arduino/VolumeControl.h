#pragma once
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <iostream>

class VolumeControl
{
public:
	bool ChangeVolume(double nVolume);

	double GetVolume();
};