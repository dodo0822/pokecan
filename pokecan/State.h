#pragma once

#include <stdint.h>

#include "pokecan/Config.h"

class State {
public:
	uint8_t status;
	uint8_t distance;
	uint8_t level;

	Config config;

	State(){
		status = IDLE;
		level = 0;
		distance = 10;
		config = Config();
		config.load();
	}

	static const uint8_t IDLE;
	static const uint8_t MOVE;
	static const uint8_t DUMP;
};