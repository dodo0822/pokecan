#pragma once

#include <stdint.h>

class State {
public:
	uint8_t status;
	uint8_t distance;
	uint8_t level;

	State(){}

	static const uint8_t IDLE;
	static const uint8_t MOVE;
	static const uint8_t DUMP;
};