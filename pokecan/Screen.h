#pragma once

#include "adafruit-ssd1306/Adafruit_SSD1306.h"

class Screen {
public:
	Screen(Adafruit_SSD1306_I2c& _display, State& _state) : display(_display), state(_state) {
	}

	virtual ~Screen(){}

	virtual void render() = 0;
	virtual void key(uint8_t) = 0;
	
protected:
	Adafruit_SSD1306_I2c& display;
	State& state;
};