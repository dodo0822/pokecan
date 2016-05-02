#include "pokecan/MainScreen.h"

void MainScreen::render() {
	display.clearDisplay();
	display.setTextSize(2);
	display.printfPos(20, 0, "Pokecan");
	if(state.status == State::IDLE) {
		display.printfPos(0, 40, "Idle");
	}
	display.setTextSize(1);
	display.drawFastHLine(0, 15, 128, WHITE);
	for(int i = 0; i < 50; ++i) {
		for(int j = 0; j < 37; ++j) {
			display.drawPixel(80 + j, 17 + i, trashcan[37*i + j]);
		}
	}
	display.display();
}

int8_t MainScreen::key(uint8_t keycode) {
	return Screen::SCR_MENU;
}