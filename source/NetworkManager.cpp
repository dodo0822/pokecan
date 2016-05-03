#include "pokecan/NetworkManager.h"

NetworkManager::NetworkManager(PinName tx, PinName rx, uint32_t baud) : esp(tx, rx, 256), timeout(500) {
	esp.baud(baud);
	printf("nm init\r\n");
}

void NetworkManager::get_status() {
	int ip_len = 15;
	char ip[15];
	if(!(command("ip=wifi.sta.getip();print(ip)") && execute(ip, &ip_len))) {
		printf("cant get ip!\r\n");
		return;
	}
}

bool NetworkManager::command(const char* cmd) {
	printf("command:%s\r\n", cmd);
	for(int i = 0; cmd[i]; ++i) {
		if(send_char(cmd[i]) < 0) return false;
	}
	printf("command ok\r\n");
	return true;
}

bool NetworkManager::execute(char* resp_buf, int* resp_len) {
	if(!(command("\r\n") && discard_echo())) return false;
	printf("execute ok\r\n");
	if(resp_buf && resp_len) {
		for(int i = 0; i < *resp_len; ++i) {
			int c = recv_char();
			if(c < 0) return false;
			if(c == '\r') {
				*resp_len = i;
				break;
			}
			resp_buf[i] = c;
		}
		printf("cmd response: %.*s\r\n", *resp_len, resp_buf);
	}
	return flush();
}

bool NetworkManager::discard_echo() {
	while(1) {
		int ch = recv_char();
		if(ch < 0) {
			printf("oh no\r\n");
			return false;
		}
		//printf("discarded: %c\r\n", ch);
		if(ch == '\n') return true;
	}
}

bool NetworkManager::flush() {
	while(1) {
		int ch = recv_char();
		if(ch < 0) return false;
		else if(ch == '>') return true;
	}
}

int NetworkManager::send_char(char ch) {
	Timer timer;
	timer.start();
	while(1) {
		if(esp.writeable()) {
			//printf("send %c\r\n", ch);
			return esp.putc(ch);
		}
		if(timer.read_ms() > timeout) return -1;
	}
}

int NetworkManager::recv_char() {
	Timer timer;
	timer.start();
	while(1) {
		if(esp.readable()) {
			char ch = esp.getc();
			//printf("recv:%c\r\n", ch);
			return ch;
		}
		if(timer.read_ms() > timeout) {
			printf("recv timeout!\r\n");
			return -1;
		}
	}
}