#pragma once

#include "Device.h"

/*
* REF
*	https://github.com/omriharel/deej/blob/master/pkg/deej
*	- session_finder_windows.go
*/

class Audio : public IMMNotificationClient {

private:
	//CONSTANT
	const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
	const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

	//VARIABLE
	HRESULT hr = NULL;

	IMMDeviceEnumerator* pEnumerator = NULL;

	std::map<LPWSTR, Device*> devices;
	LPWSTR currentDeviceID = NULL;

	//FUNCTION
	void onDefaultDeviceChanged(LPWSTR);
	void getDefaultDevice();
	void logCurrentDevice();

public:
	//FUNCTION
	Audio();
	~Audio();

	void updateProcess();
	Device* getCurrentDevice();
	void increment();
	void decrement();

#pragma region Notification

	ULONG STDMETHODCALLTYPE AddRef() { return 0; }
	ULONG STDMETHODCALLTYPE Release() { return 0; }
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, VOID** ppvInterface) { return S_OK; }
	HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR pwstrDeviceId) { return S_OK; }
	HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR pwstrDeviceId) { return S_OK; }
	HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState) { return S_OK; }
	HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key) { return S_OK; }
	HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId) {
		std::wstring id(pwstrDeviceId);
		onDefaultDeviceChanged(&id[0]);
		return S_OK;
	}

#pragma endregion

};