#include "pokecan/SplashScreen.h"

void SplashScreen::render() {
	display.clearDisplay();
	for(uint8_t i = 0; i < 44; ++i) {
		for(uint8_t j = 0; j < 128; ++j) {
			display.drawPixel(j, i, px[i*128 + j]);
		}
	}
	display.printfPos(0, 50, "by Dodo, Pierre, Sasa");
	display.display();
}

void SplashScreen::key(uint8_t keycode) {
	
}