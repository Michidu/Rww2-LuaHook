#include "initiate_state.h"

#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "libMinHook-x64-v141-md.lib")

HINSTANCE m_hinst_dll;
extern "C" UINT_PTR m_procs[17] = {0};

LPCSTR m_import_names[] = {
	"GetFileVersionInfoA", "GetFileVersionInfoByHandle", "GetFileVersionInfoExA", "GetFileVersionInfoExW",
	"GetFileVersionInfoSizeA", "GetFileVersionInfoSizeExA", "GetFileVersionInfoSizeExW", "GetFileVersionInfoSizeW",
	"GetFileVersionInfoW", "VerFindFileA", "VerFindFileW", "VerInstallFileA", "VerInstallFileW", "VerLanguageNameA",
	"VerLanguageNameW", "VerQueryValueA", "VerQueryValueW"
};

BOOL WINAPI DllMain(HINSTANCE hinst_dll, DWORD fdw_reason, LPVOID)
{
	if (fdw_reason == DLL_PROCESS_ATTACH)
	{
		TCHAR sys_dir[MAX_PATH];
		GetSystemDirectory(sys_dir, MAX_PATH);

		char buffer[MAX_PATH];
		sprintf_s(buffer, "%s\\version.dll", sys_dir);

		m_hinst_dll = LoadLibrary(buffer);
		if (!m_hinst_dll)
			return FALSE;

		for (int i = 0; i < 17; i++)
			m_procs[i] = reinterpret_cast<UINT_PTR>(GetProcAddress(m_hinst_dll, m_import_names[i]));

		rww2hook::InitiateStates();
	}
	else if (fdw_reason == DLL_PROCESS_DETACH)
	{
		FreeLibrary(m_hinst_dll);
	}

	return TRUE;
}

extern "C" void GetFileVersionInfoA_wrapper();
extern "C" void GetFileVersionInfoByHandle_wrapper();
extern "C" void GetFileVersionInfoExA_wrapper();
extern "C" void GetFileVersionInfoExW_wrapper();
extern "C" void GetFileVersionInfoSizeA_wrapper();
extern "C" void GetFileVersionInfoSizeExA_wrapper();
extern "C" void GetFileVersionInfoSizeExW_wrapper();
extern "C" void GetFileVersionInfoSizeW_wrapper();
extern "C" void GetFileVersionInfoW_wrapper();
extern "C" void VerFindFileA_wrapper();
extern "C" void VerFindFileW_wrapper();
extern "C" void VerInstallFileA_wrapper();
extern "C" void VerInstallFileW_wrapper();
extern "C" void VerLanguageNameA_wrapper();
extern "C" void VerLanguageNameW_wrapper();
extern "C" void VerQueryValueA_wrapper();
extern "C" void VerQueryValueW_wrapper();
