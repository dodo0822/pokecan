#pragma once

#include "mbed-drivers/mbed.h"

class Config {
public:
	int threshold;
	int can_height;
	int lift_height;

	Config();

	void load();
	void save();

private:
	//LocalFileSystem local;
};