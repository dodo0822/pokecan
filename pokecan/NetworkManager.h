#pragma once

#include "mbed-drivers/mbed.h"

#include "pokecan/BufferedSerial.h"

class NetworkManager {
public:
	NetworkManager(PinName, PinName, uint32_t);

	void get_status();
private:
	bool command(const char*);
	bool execute(char* = 0, int* = 0);
	bool flush();
	bool discard_echo();

	int send_char(char);
	int recv_char();

	BufferedSerial esp;
	uint16_t timeout;
};