#include "pokecan/MotorControl.h"

MotorControl::MotorControl(I2C& _i2c, uint8_t address = 0x80) : i2c(_i2c), pca(_i2c, address) {
	pca.set_frequency(500);
	for(uint8_t i = 0; i < 16; ++i) stop(i);
}

void MotorControl::stop(uint8_t channel) {
	pca.set_pwm(channel, 0, 0);
}

void MotorControl::forward(uint8_t channel, uint8_t percentage) {
	float p = ((float) percentage) / 100;
	pca.set_pwm(channel, 0, p * 4095);
}

void MotorControl::backward(uint8_t channel, uint8_t percentage) {
	float p = ((float) percentage) / 100;
	pca.set_pwm(channel, 0, p * 4095);
}