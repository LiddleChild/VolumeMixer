#include "Process.h"

#pragma region PUBLIC Process::Process()

Process::Process(std::wstring name) {
	this->name = name;
}

#pragma endregion
#pragma region PUBLIC Process::~Process()

Process::~Process() {
	for (int i = 0; i < datas.size(); i++) {
		datas[i]->pControl->Release();
		datas[i]->pControl2->Release();
		datas[i]->pVolume->Release();

		delete datas[i];
	}
}

#pragma endregion

#pragma region PUBLIC Process::getVolume(): void

float Process::getVolume() {
	float v;
	hr = datas[0]->pVolume->GetMasterVolume(&v);
	error(hr, "(Process.cpp::getVolume) ISimpleAudioVolume::GetMasterVolume");

	return v;
}

#pragma endregion
#pragma region PUBLIC Process::setVolume(float): void

void Process::setVolume(float v) {
	for (int i = 0; i < datas.size(); i++) {
		hr = datas[i]->pVolume->SetMasterVolume(v, NULL);
		error(hr, "(Process.cpp::setVolume) ISimpleAudioVolume::SetMasterVolume");
	}
}

#pragma endregion
#pragma region PUBLIC Process::adjustVolume(float): void

void Process::adjustVolume(float amount) {
	float v = getVolume() + amount;
	if (v > 1.f) v = 1.f;
	if (v < 0) v = 0;
	setVolume(v);
}

#pragma endregion
#pragma region PUBLIC Process::increment(): void

void Process::increment() { adjustVolume(adjustAmount); }

#pragma endregion
#pragma region PUBLIC Process::decrement(): void

void Process::decrement() { adjustVolume(-adjustAmount); }

#pragma endregion
#pragma region PUBLIC Process::addProcess(): void

void Process::addProcess(IAudioSessionControl* control) {
	ProcessData* data = new ProcessData;

	data->pControl = control;

	hr = data->pControl->QueryInterface(IID_IAudioSessionControl2, (void**) &data->pControl2);
	error(hr, "(Process.cpp::Process) IAudioSessionControl::QueryInterface(IAudioSessionControl2)");

	//Get process volume control
	hr = control->QueryInterface(IID_ISimpleAudioVolume, (void**) &data->pVolume);
	error(hr, "(Process.cpp::Process) IAudioSessionControl::QueryInterface(ISimpleAudioVolume)");

	data->id = getProcessId(data->pControl2);
	data->name = getProcessName(data->id);

	datas.push_back(data);
}

#pragma endregion
