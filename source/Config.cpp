#include "pokecan/Config.h"

#include <string>
#include <sstream>

using namespace std;

Config::Config() {
	threshold = 80;
	can_height = lift_height = 0;
}

void Config::load() {
	/*FILE *fp = fopen("/local/config.txt", "r");
	if(!fp) {
		// create file
		save();
		fp = fopen("/local/config.txt", "r");
	}
	char line[256];

	while(fgets(line, sizeof(line), file)) {
		string str(line);
		stringstream ss(str);
		string name;
		int value;
		ss >> name >> value;
		if(name == "threshold") threshold = value;
		else if(name == "can_height") can_height = value;
		else if(name == "lift_height") lift_height = value;
	}

	fclose(file);*/
}

void Config::save() {
	/*FILE *fp = fopen("/local/config.txt", "w");
	fprintf(fp, "threshold %d\n", threshold);
	fprintf(fp, "can_height %d\n", can_height);
	fprintf(fp, "lift_height %d\n", lift_height);
	fflush(fp);
	fclose(fp);*/
}