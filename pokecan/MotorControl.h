#pragma once

#include "mbed-drivers/mbed.h"
#include "pca9685/PCA9685.h"

class MotorControl {
public:
	MotorControl(I2C&, uint8_t);

	void stop(uint8_t);
	void forward(uint8_t, uint8_t);
	void backward(uint8_t, uint8_t);

private:
	I2C& i2c;
	PCA9685 pca;
};