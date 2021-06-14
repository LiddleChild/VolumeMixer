#include "Device.h"

#pragma region PRIVATE Device::getAllProcess(): void

void Device::getAllProcess() {
	log("Device.cpp", "Getting all processes");
	clearProcess();

	IAudioSessionManager2* pManager = NULL;
	hr = pDevice->Activate(IID_IAudioSessionManager2, CLSCTX_ALL, NULL, (void**) &pManager);
	error(hr, "(Device.cpp::getAllProcess) IMMDevice::Activate(IAudioSessionManager2)");

	IAudioSessionEnumerator* pSessionEnum;
	hr = pManager->GetSessionEnumerator(&pSessionEnum);
	error(hr, "(Device.cpp::getAllProcess) IAudioSessionManager2::GetSessionEnumerator");

	hr = pSessionEnum->GetCount(&processCount);
	error(hr, "(Device.cpp::getAllProcess) IAudioSessionEnumerator::GetCount");

	log("Device.cpp", std::to_string(processCount - 1) + " processes");

	for (int i = 0; i < processCount; i++) {
		IAudioSessionControl* pControl = nullptr;
		hr = pSessionEnum->GetSession(i, &pControl);
		error(hr, "(Device.cpp::getAllProcess) IAudioSessionControl::GetSession");

		IAudioSessionControl2* pControl2 = nullptr;
		hr = pControl->QueryInterface(IID_IAudioSessionControl2, (void**) &pControl2);
		error(hr, "(Device.cpp::getAllProcess) IAudioSessionControl2::QueryInterface(IAudioSessionControl2)");

		DWORD pid;
		hr = pControl2->GetProcessId(&pid);
		error(hr, "(Device.cpp::getAllProcess) IAudioSessionControl2::GetProcessId");

		//Ignore system sound
		if (pid == 0) continue;

		//Add to dict
		std::wstring pname = getProcessName(pid);
		std::wstring newpname = nameDict(pname);
		
		//Merge same process name into one process
		if (processes[newpname] != nullptr)
			processes[newpname]->addProcess(pControl);
		else {
			Process* p = new Process(newpname);
			p->addProcess(pControl);
			
			processes[newpname] = p;
		}

		pControl2->Release();

		if (pname == newpname) wprintf(L"\t\t| (%5d) %s\n", pid, pname.c_str());
		else wprintf(L"\t\t| (%5d) %s (%s)\n", pid, pname.c_str(), newpname.c_str());
	}

	pManager->Release();
	pSessionEnum->Release();
}

#pragma endregion
#pragma region PRIVATE Device::getDeviceName(): PCWSTR

PCWSTR Device::getDeviceName() {
	IPropertyStore* pStore = nullptr;
	hr = pDevice->OpenPropertyStore(STGM_READ, &pStore);
	error(hr, "(Device.cpp::getDeviceName) IPropertyStore::OpenPropertyStore");

	PROPVARIANT prop;
	PropVariantInit(&prop);
	hr = pStore->GetValue(PKEY_Device_FriendlyName, &prop);
	error(hr, "(Device.cpp::getDeviceName) IPropertyStore::GetValue");

	return PropVariantToStringWithDefault(prop, L"(missing)");
}

#pragma endregion
#pragma region PRIVATE Device::getDeviceID(): LPWSTR

LPWSTR Device::getDeviceID() {
	LPWSTR id;
	hr = pDevice->GetId(&id);
	error(hr, "(Device.cpp::getDeviceID) IMMDevice::GetId");

	return id;
}

#pragma endregion
#pragma region PRIVATE Device::clearProcess(): void

void Device::clearProcess() {
	for (auto itr : processes) delete itr.second;
	processes.clear();
}

#pragma endregion
#pragma region PRIVATE Device::nameDict(std::wstring): std::wstring

std::wstring Device::nameDict(std::wstring name) {
	std::map<std::wstring, std::wstring> dict;
	dict[L"Discord.exe"] = L"Discord (Group)";

	return dict[name] == L"" ? name : dict[name];
	//return Config::get().processNameDicts[name] == L"" ? name : Config::get().processNameDicts[name];
}

#pragma endregion


#pragma region PUBLIC Device::Device()

Device::Device(IMMDevice* device) {
	this->pDevice = device;

	this->name = getDeviceName();
	this->id = getDeviceID();

	//Get endpoint volume controller
	hr = pDevice->Activate(IID_IAudioEndpointVolume, CLSCTX_ALL, NULL, (void**) &pVolume);
	error(hr, "(Device.cpp::Device) IMMDevice::Activate(IAudioEndpointVolume)");

	//Get all process and register them in dict
	getAllProcess();
}

#pragma endregion
#pragma region PUBLIC Device::~Device()

Device::~Device() {
	clearProcess();

	pDevice->Release();
	pVolume->Release();
}

#pragma endregion

#pragma region PUBLIC Device::updateProcess(): void

void Device::updateProcess() {
	log("Device.cpp", "Updating processes");
	getAllProcess();
	log("Device.cpp", "Processes updated");
}

#pragma endregion
#pragma region PUBLIC Device::getVolume(): void

float Device::getVolume() {
	float v;
	hr = pVolume->GetMasterVolumeLevelScalar(&v);
	error(hr, "(Device.cpp::getVolume) IAudioEndpointVolume::GetMasterVolumeLevelScalar");

	return v;
}

#pragma endregion
#pragma region PUBLIC setVolume(float): void

void Device::setVolume(float v) {
	hr = pVolume->SetMasterVolumeLevelScalar(v, NULL);
	error(hr, "(Device.cpp::setVolume) IAudioEndpointVolume::SetMasterVolumeLevelScalar");
}

#pragma endregion
#pragma region PUBLIC adjustVolume(float): void

void Device::adjustVolume(float amount) {
	float v = getVolume() + amount;
	if (v > 1.f) v = 1.f;
	if (v < 0) v = 0;
	setVolume(v);
}

#pragma endregion
#pragma region PUBLIC increment(): void

void Device::increment() { adjustVolume(adjustAmount); }

#pragma endregion
#pragma region PUBLIC decrement(): void

void Device::decrement() { adjustVolume(-adjustAmount); }

#pragma endregion
#pragma region PUBLIC getProcessCount(): int

int Device::getProcessCount() {
	return processes.size();
}

#pragma endregion
#pragma region PUBLIC getProcess(): Process*

Process* Device::getProcess(int index) {
	int i = 0;
	for (auto itr : processes) {
		if (index == i) return itr.second;
		
		i++;
	}

	return NULL;
}

#pragma endregion