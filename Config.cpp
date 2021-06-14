#include "Config.h"

#pragma region PRIVATE Config::split2(std::string, char): std::vector<std::string>

std::vector<std::string> Config::split2(std::string str, char c) {
    for (int i = 0; i < str.size(); i++) {
        if (*(str.begin() + i) == c) {
            std::string str1(str.begin(), str.begin() + i);
            std::string str2(str.begin() + i + 1, str.end());

            return { str1, str2 };
        }
    }

    return {};
}

#pragma endregion

#pragma region PUBLIC Config::loadConfig(): void

void Config::loadConfig() {
    log("Config.cpp", "Loading config");

    std::ifstream exist(CONFIG_FILE);

    if (exist) {
        std::ifstream config(CONFIG_FILE);

        while (!config.eof()) {
            char l[64];
            config.getline(l, 64);

            std::string line(l);
            if (line.size() == 0) continue;

            std::vector<std::string> v = split2(line, '=');
            if (v.size() > 0) {
                std::string setting = v[0];
                std::string value = v[1];

                //PORT
                if (setting == "PORT") port = value;
                
                //SHOW_CONSOLE
                else if (setting == "SHOW_CONSOLE") showConsole = std::atoi(value.c_str());
                
                //DICT
                else if (setting == "DICT") {
                    std::vector<std::string> v = split2(value, ',');
                    if (v.size() > 0) {
                        std::string key = v[0];
                        std::string val = v[1];

                        processNameDicts[s2ws(key)] = s2ws(val);
                    }
                }
            }
        }

        log("Config.cpp", "Config loaded");

        config.close();
    }
    else {
        std::ofstream config(CONFIG_FILE);

        config << "PORT=" << PORT << "\n";
        config << "SHOW_CONSOLE=" << SHOW_CONSOLE << "\n";

        config.close();

        log("main.cpp", "Config file created");
    }

    exist.close();
}

#pragma endregion
