#pragma once

#include <string>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <map>

#include "Util.h"
#include "Debugger.h"

#define CONFIG_FILE "./res/config.txt"

#define PORT "COM5"
#define SHOW_CONSOLE 1
#define MASTER_VOLUME_STEP 2
#define PROCESS_VOLUME_STEP 5

class Config {

private:
	//VARIABLE
	static Config instance;

	//FUNCTION
	std::vector<std::string> split2(std::string, char);

public:
	//VARIABLE
	std::string port;
	int showConsole;

	float masterVolumeStep;
	float processVolumeStep;

	std::map<std::wstring, std::wstring> processNameDicts;

	//METHOD
	void loadConfig();

	static Config& get() {
		return instance;
	}

};