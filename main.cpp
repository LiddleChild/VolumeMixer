#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <stdio.h>

#include "Debugger.h"
#include "Window.h"
#include "SerialClass.h"
#include "Audio.h"

#pragma comment (lib, "Dwmapi.lib")
#pragma comment(lib, "propsys")

#define CONFIG_FILE "./res/config.txt"
#define PORT "COM5"
#define SHOW_CONSOLE 1

std::string port(PORT);
int showConsole = SHOW_CONSOLE;

HWND consoleWindow;

Serial* sp = NULL;
Audio* audio = NULL;
Window* window = NULL;

int windowTimer = -1, windowTimerMax = 100;
int sel = 0, maxsel = 1;
int windowVisible = 0;

#pragma region Init Serial
void initSerial() {
    sp = new Serial(port.c_str());
    
    while (!sp->IsConnected()) {
        sp = new Serial(port.c_str());
        Sleep(2000);
    }

    if (sp->IsConnected()) printf("%s Serial connection initialized \u001b[42;1m%s\u001b[0m\n", "[main.cpp]", port.c_str());
}
#pragma endregion
#pragma region Init WASAPI
void initWASAPI() {
    audio = new Audio();
}
#pragma endregion
#pragma region Init Window
void initWindow() {
    window = new Window();
    window->loadDevice(audio->getCurrentDevice());
}
#pragma endregion

#pragma region Hide Console

void hideConsole() {
    AllocConsole();
    consoleWindow = FindWindowA("ConsoleWindowClass", NULL);
    SendMessage(consoleWindow, WM_KILLFOCUS, NULL, NULL);
    ShowWindow(consoleWindow, showConsole);
}

#pragma endregion

void loadConfig() {
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

//Window event
void onWindowOpen() {
    log("main.cpp", "\u001b[32mWindow Open\u001b[0m");
    //Select 0
    sel = 0;
    window->select(sel);

    log("main.cpp", "Updating all processes");
    //Update all processes
    log("main.cpp", "Update Core Audio API");
    audio->updateProcess();
    log("main.cpp", "Update window");
    window->loadDevice(audio->getCurrentDevice());
    maxsel = audio->getCurrentDevice()->getProcessCount() + 1;
}

//Window event
void onWindowClose() {
    log("main.cpp", "\u001b[32mWindow Close\u001b[0m");
}

int main() {
    loadConfig();
    hideConsole();

    initSerial();
    initWASAPI();
    initWindow();

    log("main.cpp", "\u001b[32mREADY\u001b[0m");

    char data[256];
    while (sp->IsConnected()) {
        int i = sp->ReadData(data, 256);

        data[i] = 0;
        if (strlen(data) > 0) {
            switch (data[0]) {
            case '+':
                if (sel == 0) audio->increment();
                else audio->getCurrentDevice()->getProcess(sel - 1)->increment();
                break;

            case '-':
                if (sel == 0) audio->decrement();
                else audio->getCurrentDevice()->getProcess(sel - 1)->decrement();
                break;

            case '>':
                sel = sel == maxsel - 1 ? maxsel - 1 : sel + 1;
                break;

            case '<':
                sel = sel == 0 ? 0 : sel - 1;
                break;
            }

            if (!window->isVisible()) onWindowOpen();

            window->updateVolume(audio->getCurrentDevice()); //Update all volume
            window->select(sel); //Update selector
            if (windowTimer == -1) windowTimer = 0;
        }
        //Hide window when not interacting with the amount of time
        if (windowTimer == 0) window->show();
        else if (windowTimer == windowTimerMax) { windowTimer = -1; window->hide(); onWindowClose(); }
        
        if (windowTimer != -1) windowTimer++;
        
        //Update window when window is opening
        if (window->isVisible()) {
            window->update();
        } else Sleep(100);
    }

    delete sp;
    delete audio;
    delete window;
    
    return 0;
}
