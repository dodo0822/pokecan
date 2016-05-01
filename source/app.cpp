#include "mbed-drivers/mbed.h"

#include "adafruit-ssd1306/Adafruit_SSD1306.h"

#include "pokecan/Screen.h"
#include "pokecan/MainScreen.h"
#include "pokecan/SplashScreen.h"

static Screen *scr;
static State *state;
static Adafruit_SSD1306_I2c *oled;
static bool transition = false;

static void screen_render(void) {
	if(transition) return;
	scr->render();
}

static void leave_splash(void) {
	transition = true;
	delete scr;
	scr = new MainScreen(*oled);
	transition = false;
	screen_render();
}

void app_start(int, char**) {
	static I2C i2c(YOTTA_CFG_HARDWARE_TEST_PINS_I2C_SDA, YOTTA_CFG_HARDWARE_TEST_PINS_I2C_SCL);
	i2c.frequency(400000);
	oled = new Adafruit_SSD1306_I2c(i2c, YOTTA_CFG_HARDWARE_PINS_D2);
	
	/*oled.clearDisplay();
	oled.printf("hello!");
	oled.display();*/
	
	scr = new SplashScreen(*oled);
	scr->render();

	minar::Scheduler::postCallback(leave_splash).delay(minar::milliseconds(500));
	
	minar::Scheduler::postCallback(screen_render).period(minar::milliseconds(1000));
}