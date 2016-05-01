#pragma once

#include "adafruit-ssd1306/Adafruit_SSD1306.h"

class Screen {
public:
	Screen(Adafruit_SSD1306_I2c& _display, State& _state) : display(_display), state(_state) {
	}

	virtual ~Screen(){}

	virtual void render() = 0;
	virtual int8_t key(uint8_t) = 0;

	static const int8_t SCR_SPLASH;
	static const int8_t SCR_MAIN;
	static const int8_t SCR_MENU;
	static const int8_t SCR_SETTINGS;
	static const int8_t SCR_ABOUT;
	static const int8_t SCR_NETWORK;
	
protected:
	Adafruit_SSD1306_I2c& display;
	State& state;
};