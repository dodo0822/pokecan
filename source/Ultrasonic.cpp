#include "pokecan/Ultrasonic.h"

Ultrasonic::Ultrasonic(PinName _trig, PinName _echo, float _speed, float _timeout) : trig(_trig), echo(_echo), speed(_speed), timeout(_timeout) {
	done = false;
}

uint16_t Ultrasonic::get_distance() {
	return distance;
}

void Ultrasonic::echo_rise() {
	if(timer.read() > 600) timer.reset();
	start = timer.read_us();
}

void Ultrasonic::echo_fall() {
	end = timer.read_us();
	done = true;
	distance = (end - start) / 6;
	tout.detach();
	tout.attach(this, &Ultrasonic::start_measure, speed);
}

void Ultrasonic::start_measure(void) {
	tout.detach();
	trig = 1;
	wait_us(10);
	done = false;

	echo.rise(this, &Ultrasonic::echo_rise);
	echo.fall(this, &Ultrasonic::echo_fall);
	echo.enable_irq();

	tout.attach(this, &Ultrasonic::start_measure, timeout);
	trig = 0;
}