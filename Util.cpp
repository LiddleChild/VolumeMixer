#include "Util.h"

static std::wstring s2ws(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

static std::string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

static DWORD getProcessId(IAudioSessionControl2* pControl2) {
	DWORD id;
	HRESULT hr = pControl2->GetProcessId(&id);
	error(hr, "(Process.cpp::getProcessId) IAudioSessionControl2::GetProcessId");

	return id;
}

static std::wstring getProcessName(DWORD pid) {
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE) {
		printf("\u001b[31mError\u001b[0m \n");
		exit(EXIT_FAILURE);
	}

	for (BOOL bok = Process32First(processesSnapshot, &processInfo); bok; bok = Process32Next(processesSnapshot, &processInfo)) {
		if (pid == processInfo.th32ProcessID) {
			CloseHandle(processesSnapshot);
			return processInfo.szExeFile;
		}
	}

	CloseHandle(processesSnapshot);
	return std::wstring();
}