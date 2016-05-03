#include "pokecan/MotorControl.h"

MotorControl::MotorControl(I2C& _i2c, uint8_t address) : i2c(_i2c), pca(_i2c, address) {
	pca.set_frequency(500);
	for(uint8_t i = 0; i < 16; ++i) stop(i);
	for(uint8_t i = 0; i < channels; ++i) {
		outs.push_back(DigitalOut(pins[i*2]));
		outs.push_back(DigitalOut(pins[i*2+1]));
	}
}

void MotorControl::stop(uint8_t channel) {
	if(channel > channels) return;
	pca.set_pwm(channel, 0, 0);
	outs[channel*2] = 0;
	outs[channel*2+1] = 0;
}

void MotorControl::forward(uint8_t channel, uint8_t percentage) {
	if(channel > channels) return;
	float p = ((float) percentage) / 100;
	pca.set_pwm(channel, 0, p * 4095);
	outs[channel*2] = 0;
	outs[channel*2+1] = 1;
}

void MotorControl::backward(uint8_t channel, uint8_t percentage) {
	if(channel > channels) return;
	float p = ((float) percentage) / 100;
	pca.set_pwm(channel, 0, p * 4095);
	outs[channel*2] = 1;
	outs[channel*2+1] = 0;
}