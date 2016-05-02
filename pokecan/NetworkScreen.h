#pragma once

#include "pokecan/Screen.h"

class NetworkScreen : public Screen {
public:
	NetworkScreen(Adafruit_SSD1306_I2c& _display, State& _state) : Screen(_display, _state) {
	}

	~NetworkScreen(){}

	virtual void render();
	virtual int8_t key(uint8_t);
	
private:
};
