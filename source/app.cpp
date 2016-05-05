#include <cstdlib>
#include <ctime>

#include "mbed-drivers/mbed.h"

#define YOTTA_CFG_MBED_TRACE
#include "mbed-trace/mbed_trace.h"
#define TRACE_GROUP  "main"

#include "adafruit-ssd1306/Adafruit_SSD1306.h"
#include "mcp230xx/MCP230xx.h"

#include "pokecan/Pins.h"

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
#include "pokecan/CalibrationScreen.h"
#include "pokecan/RouteScreen.h"

#include "pokecan/Ultrasonic.h"
#include "pokecan/NetworkManager.h"

static Screen *scr;
static State *state;
static MCP23008 *mcp_keys;
static Adafruit_SSD1306_I2c *oled;
static bool transition = false;
static Ultrasonic ultrasonic(YOTTA_CFG_HARDWARE_PINS_D11, YOTTA_CFG_HARDWARE_PINS_D10, 0.3, 0.3);
static I2C i2c(YOTTA_CFG_HARDWARE_TEST_PINS_I2C_SDA, YOTTA_CFG_HARDWARE_TEST_PINS_I2C_SCL);
static NetworkManager *nm;

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
		case Screen::SCR_CALIBRATION:
			scr = new CalibrationScreen(*oled, *state);
			break;
		case Screen::SCR_ROUTE:
			scr = new RouteScreen(*oled, *state);
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
	float previous = state->distance;
	float read = ultrasonic.get_distance();
	if(read > 30) {
		read = 30;
	}
	if(read < 0) return;
	float delta = previous - read;
	if(delta > 10 || delta < -10) {
		if(state->distance_change++ == 5) {
			state->distance_change = 0;
			state->distance = read;
			state->level = (30 - read) / 30 * 100;
		} else {
			read = previous - (delta * state->distance_change / 5);
			state->distance = read;
			state->level = (30 - read) / 30 * 100;
		}
	} else {
		state->distance_change = 0;
		state->distance = read;
		state->level = (30 - read) / 30 * 100;
	}
}

void app_start(int, char**) {
	srand(time(NULL));

	mbed_trace_init();

	tr_info("Application start");
	i2c.frequency(400000);
	state = new State(i2c);
	oled = new Adafruit_SSD1306_I2c(i2c, YOTTA_CFG_HARDWARE_PINS_D2);
	mcp_keys = new MCP23008(i2c);
	for(uint8_t i = 0; i < 4; ++i) {
		mcp_keys->setup(i, MCP23008::IN);
		mcp_keys->pullup(i, true);
	}

	ultrasonic.start_measure();
	
	scr = new SplashScreen(*oled, *state);
	scr->render();

	wait_ms(100);
	nm = new NetworkManager(PIN_ESP_TX, PIN_ESP_RX, 9600);
	state->nm = nm;

	minar::Scheduler::postCallback(leave_splash).delay(minar::milliseconds(500));
	minar::Scheduler::postCallback(poll_key).period(minar::milliseconds(50));
	minar::Scheduler::postCallback(screen_render).period(minar::milliseconds(100));
	minar::Scheduler::postCallback(read_distance).period(minar::milliseconds(300));

	tr_info("Start process completed");
}