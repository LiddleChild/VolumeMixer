#pragma once

#include <map>
#include <vector>
#include <set>

#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <functiondiscoverykeys_devpkey.h>
#include <propvarutil.h>
#include <audiopolicy.h>

#include "Util.h"
#include "Debugger.h"
#include "Process.h"
#include "Config.h"

class Device {

private:

	//CONSTANT
	const IID IID_IAudioEndpointVolume = __uuidof(IAudioEndpointVolume);
	const IID IID_IAudioSessionManager2 = __uuidof(IAudioSessionManager2);
	const IID IID_IAudioSessionControl2 = __uuidof(IAudioSessionControl2);

	//VARIABLE
	IMMDevice* pDevice = NULL;
	IAudioEndpointVolume* pVolume = NULL;
	std::map<std::wstring, Process*> processes;
	int processCount = 0;

	HRESULT hr = NULL;

	float adjustAmount = 0.02f;

	//FUNCTION
	PCWSTR getDeviceName();
	LPWSTR getDeviceID();
	void getAllProcess();
	void clearProcess();
	std::wstring nameDict(std::wstring);

public:
	//VARIABLE
	PCWSTR name = NULL;
	LPWSTR id = NULL;

	//FUNCTION
	Device(IMMDevice*);
	~Device();

	void updateProcess();
	float getVolume();
	void setVolume(float);
	void adjustVolume(float);
	void increment();
	void decrement();
	int getProcessCount();
	Process* getProcess(int);

};