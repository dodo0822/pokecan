#pragma once

#include "mbed-drivers/mbed.h"
#include "pca9685/PCA9685.h"

#include <vector>

using std::vector;

class MotorControl {
public:
	MotorControl(I2C&, uint8_t = 0x80);

	void stop(uint8_t);
	void forward(uint8_t, uint8_t);
	void backward(uint8_t, uint8_t);

private:
	I2C& i2c;
	PCA9685 pca;

	uint8_t channels = 1;

	PinName pins[2] = { YOTTA_CFG_HARDWARE_PINS_SPI_SCK, YOTTA_CFG_HARDWARE_PINS_SPI_CS };
	vector<DigitalOut> outs;
};