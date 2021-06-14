#pragma once

#include <audiopolicy.h>
#include <vector>

#include "Debugger.h"
#include "Config.h"
#include "Util.h"

class Process {

private:
	//CONSTANT
	const IID IID_ISimpleAudioVolume = __uuidof(ISimpleAudioVolume);
	const IID IID_IAudioSessionControl2 = __uuidof(IAudioSessionControl2);

	//VARIABLE
	typedef struct {
		IAudioSessionControl* pControl;
		IAudioSessionControl2* pControl2;
		ISimpleAudioVolume* pVolume;

		DWORD id;
		std::wstring name;
	} ProcessData;

	HRESULT hr = NULL;

	float adjustAmount = 0.05f;

public:
	//VARIABLE
	std::vector<ProcessData*> datas;
	std::wstring name;

	//FUNCTION
	Process(std::wstring);
	~Process();

	float getVolume();
	void setVolume(float);
	void adjustVolume(float);
	void increment();
	void decrement();
	void addProcess(IAudioSessionControl*);
};