#pragma once

#include <stdint.h>

#include "pokecan/Config.h"
#include "pokecan/MotorControl.h"
#include "pokecan/NetworkManager.h"

class State {
public:
	uint8_t status;
	float distance;
	uint8_t level;
	uint8_t distance_change;

	MotorControl motor;
	Config config;
	NetworkManager* nm;

	State(I2C &_i2c) : motor(_i2c), config(_i2c) {
		status = IDLE;
		level = 0;
		distance = 10;
	}

	static const uint8_t IDLE;
	static const uint8_t MOVE;
	static const uint8_t DUMP;
};