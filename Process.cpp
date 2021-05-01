#include "Process.h"

#pragma region PRIVATE Process::getProcessId(): DWORD
DWORD Process::getProcessId() {
	DWORD id;
	hr = pControl2->GetProcessId(&id);
	error(hr, "(Process.cpp::getProcessId) IAudioSessionControl2::GetProcessId");
	
	return id;
}
#pragma endregion
#pragma region PRIVATE Process::getProcessName(): std::wstring
std::wstring Process::getProcessName() {
	DWORD pid = this->id;

	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE) {
		printf("\u001b[31mError\u001b[0m \n");
		exit(EXIT_FAILURE);
	}

	for (BOOL bok = Process32First(processesSnapshot, &processInfo);bok; bok = Process32Next(processesSnapshot, &processInfo)) {
		if (pid == processInfo.th32ProcessID) {
			CloseHandle(processesSnapshot);
			return processInfo.szExeFile;
		}
	}

	CloseHandle(processesSnapshot);
	return std::wstring();
}
#pragma endregion

#pragma region PUBLIC Process::Process()

Process::Process(IAudioSessionControl* control) {
	this->pControl = control;

	hr = pControl->QueryInterface(IID_IAudioSessionControl2, (void**)&pControl2);
	error(hr, "(Process.cpp::Process) IAudioSessionControl::QueryInterface(IAudioSessionControl2)");

	//Get process volume control
	hr = control->QueryInterface(IID_ISimpleAudioVolume, (void**) &this->pVolume);
	error(hr, "(Process.cpp::Process) IAudioSessionControl::QueryInterface(ISimpleAudioVolume)");

	this->id = getProcessId();
	this->name = getProcessName();
}

#pragma endregion
#pragma region PUBLIC Process::~Process()

Process::~Process() {
	pControl->Release();
	pControl2->Release();
	pVolume->Release();
}

#pragma endregion

#pragma region PUBLIC Process::getVolume(): void

float Process::getVolume() {
	float v;
	hr = pVolume->GetMasterVolume(&v);
	error(hr, "(Process.cpp::getVolume) ISimpleAudioVolume::GetMasterVolume");

	return v;
}

#pragma endregion
#pragma region PUBLIC Process::setVolume(float): void

void Process::setVolume(float v) {
	hr = pVolume->SetMasterVolume(v, NULL);
	error(hr, "(Process.cpp::setVolume) ISimpleAudioVolume::SetMasterVolume");
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