#pragma once

#include <audiopolicy.h>
#include <TlHelp32.h>

#include "Debugger.h"
#include "Config.h"

class Process {

private:
	//CONSTANT
	const IID IID_ISimpleAudioVolume = __uuidof(ISimpleAudioVolume);
	const IID IID_IAudioSessionControl2 = __uuidof(IAudioSessionControl2);

	//VARIABLE
	IAudioSessionControl* pControl = NULL;
	IAudioSessionControl2* pControl2 = NULL;
	ISimpleAudioVolume* pVolume = NULL;

	HRESULT hr = NULL;

	DWORD getProcessId();
	std::wstring getProcessName();

	float adjustAmount = 0.05f;

public:
	//VARIABLE
	DWORD id;
	std::wstring name;

	//FUNCTION
	Process(IAudioSessionControl*);
	~Process();

	float getVolume();
	void setVolume(float);
	void adjustVolume(float);
	void increment();
	void decrement();
};