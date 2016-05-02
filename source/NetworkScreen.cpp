#include "pokecan/NetworkScreen.h"

void NetworkScreen::render() {
	display.clearDisplay();
	display.display();
}

int8_t NetworkScreen::key(uint8_t keycode) {
	return Screen::SCR_MENU;
}