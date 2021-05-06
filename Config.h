#pragma once

#include <string>
#include <fstream>
#include <stdio.h>

#include "Debugger.h"

#define CONFIG_FILE "./res/config.txt"

#define PORT "COM5"
#define SHOW_CONSOLE 1
#define MASTER_VOLUME_STEP 2
#define PROCESS_VOLUME_STEP 5

class Config {

private:
	static Config instance;

public:
	//VARIABLE
	std::string port;
	int showConsole;

	float masterVolumeStep;
	float processVolumeStep;

	//METHOD
	void loadConfig();

	static Config& get() {
		return instance;
	}

};