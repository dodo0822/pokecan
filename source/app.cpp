#include "mbed-drivers/mbed.h"

#include "adafruit-ssd1306/Adafruit_SSD1306.h"
#include "mcp230xx/MCP230xx.h"

#include "pokecan/State.h"

#include "pokecan/Screen.h"
#include "pokecan/MainScreen.h"
#include "pokecan/SplashScreen.h"
#include "pokecan/MenuScreen.h"
#include "pokecan/SettingsScreen.h"
#include "pokecan/AboutScreen.h"
#include "pokecan/NetworkScreen.h"
#include "pokecan/DumpingThresholdScreen.h"
#include "pokecan/MotorTestScreen.h"

static Screen *scr;
static State *state;
static MCP23008 *mcp_keys;
static Adafruit_SSD1306_I2c *oled;
static bool transition = false;

static bool keys[4] = { false };

static void screen_render(void) {
	if(transition) return;
	scr->render();
}

static void poll_key(void) {
	if(transition) return;
	int8_t key_pressed = -1;
	for(uint8_t i = 0; i < 4; ++i) {
		if(!mcp_keys->input(i)) {
			if(keys[i]) continue;
			keys[i] = true;
			key_pressed = i;
		} else {
			keys[i] = false;
		}
	}
	if(key_pressed >= 0) {
		int8_t result = scr->key(key_pressed);
		if(result == -1) {
			screen_render();
			return;
		}
		transition = true;
		delete scr;
		switch(result) {
		case Screen::SCR_MAIN:
			scr = new MainScreen(*oled, *state);
			break;
		case Screen::SCR_MENU:
			scr = new MenuScreen(*oled, *state);
			break;
		case Screen::SCR_SETTINGS:
			scr = new SettingsScreen(*oled, *state);
			break;
		case Screen::SCR_ABOUT:
			scr = new AboutScreen(*oled, *state);
			break;
		case Screen::SCR_NETWORK:
			scr = new NetworkScreen(*oled, *state);
			break;
		case Screen::SCR_DUMP_TH:
			scr = new DumpingThresholdScreen(*oled, *state);
			break;
		case Screen::SCR_MOTOR_TEST:
			scr = new MotorTestScreen(*oled, *state);
			break;
		}
		transition = false;
		screen_render();
	}
}

static void leave_splash(void) {
	transition = true;
	delete scr;
	scr = new MainScreen(*oled, *state);
	transition = false;
	screen_render();
}

static void read_distance(void) {
	state->level += 3;
	if(state->level > 100) state->level -= 100;
}

void app_start(int, char**) {
	state = new State();
	static I2C i2c(YOTTA_CFG_HARDWARE_TEST_PINS_I2C_SDA, YOTTA_CFG_HARDWARE_TEST_PINS_I2C_SCL);
	i2c.frequency(400000);
	oled = new Adafruit_SSD1306_I2c(i2c, YOTTA_CFG_HARDWARE_PINS_D2);
	mcp_keys = new MCP23008(i2c);
	for(uint8_t i = 0; i < 4; ++i) {
		mcp_keys->setup(i, MCP23008::IN);
		mcp_keys->pullup(i, true);
	}
	
	scr = new SplashScreen(*oled, *state);
	scr->render();

	minar::Scheduler::postCallback(leave_splash).delay(minar::milliseconds(500));
	minar::Scheduler::postCallback(poll_key).period(minar::milliseconds(50));
	minar::Scheduler::postCallback(screen_render).period(minar::milliseconds(100));
	minar::Scheduler::postCallback(read_distance).period(minar::milliseconds(1000));
}