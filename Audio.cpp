#include "Audio.h"

#pragma region PRIVATE Audio::logCurrentDevice(): void

void Audio::logCurrentDevice() {
    wprintf(L"[Audio.cpp] Current Device \u001b[32m%s\u001b[0m\n\n", devices[currentDeviceID]->name);
}

#pragma endregion
#pragma region PRIVATE Audio::onDefaultDeviceChanged(LPWSTR): void

void Audio::onDefaultDeviceChanged(LPWSTR id) {
    if (devices[id] == NULL) getDefaultDevice();
    else currentDeviceID = id;
       
    logCurrentDevice();
}

#pragma endregion
#pragma region PRIVATE Audio::getDefaultDevice(): void

void Audio::getDefaultDevice() {
    IMMDevice* current;
    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &current);
    error(hr, "(Audio.cpp::getDefaultDevice) IMMDeviceEnumerator::GetDefaultAudioEndpoint");

    hr = current->GetId(&currentDeviceID);
    error(hr, "(Audio.cpp::getDefaultDevice) IMMDevice::GetId");

    Device* d = new Device(current);
    devices[currentDeviceID] = d;
}

#pragma endregion

#pragma region PUBLIC Audio::Audio()

Audio::Audio() {
    //Initizlize mmDeviceEnumerator
    hr = CoInitialize(NULL);
    hr = CoCreateInstance(CLSID_MMDeviceEnumerator, 0, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**) &pEnumerator);
    error(hr, "(Audio.cpp::Audio) CoCreateInstance");

    //Register device changed callback
    hr = pEnumerator->RegisterEndpointNotificationCallback(this);
    error(hr, "(Audio.cpp::Audio) RegisterEndpointNotificationCallback");

    //Get default device
    getDefaultDevice();
    logCurrentDevice();
}

#pragma endregion
#pragma region PUBLIC Audio::~Audio()

Audio::~Audio() {
    for (auto itr : devices) delete itr.second;
    devices.clear();

    pEnumerator->Release();
}

#pragma endregion

#pragma region PUBLIC Audio::updateProcess(): void

void Audio::updateProcess() {
    log("Audio.cpp", "Updating processes");
    getCurrentDevice()->updateProcess();
    log("Audio.cpp", "Processes updated");
}

#pragma endregion
#pragma region PUBLIC Audio::increment(): void

void Audio::increment() { devices[currentDeviceID]->increment(); }

#pragma endregion
#pragma region PUBLIC Audio::decrement(): void

void Audio::decrement() { devices[currentDeviceID]->decrement(); }

#pragma endregion
#pragma region PUBLIC Audio::getCurrentDevice(): Device*

Device* Audio::getCurrentDevice() {
    return devices[currentDeviceID];
}

#pragma endregion