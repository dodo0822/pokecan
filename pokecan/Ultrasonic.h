#pragma once

#include "mbed-drivers/mbed.h"

class Ultrasonic {
public:
	Ultrasonic(PinName, PinName, float, float);

	uint16_t get_distance();

private:
	void echo_rise();
	void echo_fall();
	void start_measure(void);

	DigitalOut trig;
	InterruptIn echo;

	Timer timer;
	Timeout tout;

	float timeout;
	float speed;

	uint16_t distance;

	bool done;

	int start;
	int end;
};