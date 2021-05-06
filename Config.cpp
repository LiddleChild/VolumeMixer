#include "Config.h"

#pragma region PUBLIC Config::loadConfig(): void

void Config::loadConfig() {
    std::ifstream exist(CONFIG_FILE);

    if (exist) {
        std::ifstream config(CONFIG_FILE);

        while (!config.eof()) {
            char l[25];
            config.getline(l, 25);

            std::string line(l);

            for (int i = 0; i < line.size(); i++) {
                if (*(line.begin() + i) == '=') {
                    std::string setting(line.begin(), line.begin() + i);
                    std::string value(line.begin() + i + 1, line.end());

                    if (setting == "PORT") port = value;
                    else if (setting == "SHOW_CONSOLE") showConsole = std::atoi(value.c_str());
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
