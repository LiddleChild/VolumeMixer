#include <chrono>
#include <thread>
#include <iostream>

#include "Debugger.h"
#include "Window.h"
#include "SerialClass.h"
#include "Audio.h"
#include "Config.h"

#pragma comment (lib, "Dwmapi.lib")
#pragma comment(lib, "propsys")

HWND consoleWindow;

Serial* sp = NULL;
Audio* audio = NULL;
Window* window = NULL;

Config* config = Config::get();

int windowTimer = -1, windowTimerMax = 50;
int sel = 0, maxsel = 1;
int windowVisible = 0;

//Window event
void onWindowOpen() {
    log("main.cpp", "\u001b[32mWindow Open\u001b[0m");

    //Update all processes
    log("main.cpp", "Updating Core Audio API");
    audio->updateProcess();
    log("main.cpp", "Update window");
    window->loadDevice(audio->getCurrentDevice());
    maxsel = audio->getCurrentDevice()->getProcessCount() + 1;
}

//Window event
void onWindowClose() {
    log("main.cpp", "\u001b[32mWindow Close\u001b[0m\n");

    //Select 0
    sel = 0;
    window->select(sel);
}

#pragma region Init Serial
void initSerial() {
    log("main.cpp", "Initializing serial connection");
    const char* port = config->port.c_str();

    do {
        sp = new Serial(port);
        Sleep(2000);
    } while (!sp->IsConnected());

    if (sp->IsConnected()) printf("%s Serial connection initialized on \u001b[32m%s\u001b[0m\n\n", "[main.cpp]", port);
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
    ShowWindow(consoleWindow, config->showConsole);
}

#pragma endregion

void updateAudioProcess() {
    audio->updateProcess();
    window->loadDevice(audio->getCurrentDevice());
    maxsel = audio->getCurrentDevice()->getProcessCount() + 1;
}

int main() {
    config->loadConfig();

    hideConsole();

    initSerial();
    initWASAPI();
    initWindow();

    std::string str("init");
    sp->WriteData(str.c_str(), str.size());

    log("main.cpp", "\u001b[32mREADY\u001b[0m\n");

    char data[256];
    while (sp->IsConnected()) {
        int i = sp->ReadData(data, 256);

        data[i] = 0;
        if (strlen(data) > 0) {
            if (window->isVisible()) {
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
            }

            if (!window->isVisible()) onWindowOpen();

            //Update interface
            window->updateVolume(audio->getCurrentDevice());
            window->select(sel);

            if (windowTimer == -1) windowTimer = 0;
            else windowTimer = 1;
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

    return 0;
}
