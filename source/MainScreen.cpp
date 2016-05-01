#include "pokecan/MainScreen.h"

void MainScreen::render() {
	display.clearDisplay();
	display.printfPos(20, 0, "Pokecan");
	display.drawFastHLine(0, 9, 128, WHITE);
	for(int i = 0; i < 50; ++i) {
		for(int j = 0; j < 37; ++j) {
			display.drawPixel(80 + j, 15 + i, trashcan[37*i + j]);
		}
	}
	display.display();
}

void MainScreen::key(uint8_t keycode) {
	
}