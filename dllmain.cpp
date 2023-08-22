#pragma execution_character_set("utf-8")

#include <Windows.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

typedef PVOID(*t_StrMapFunc)(PVOID, LPSTR);
t_StrMapFunc o_strMapFunc = NULL;

#include "StringMap.h"

PVOID Hook_StrMapFunc(PVOID lpData, LPSTR lpString)
{
	auto cur = cht_mapping.find(lpString);
	if (cur != cht_mapping.end()) {
		lpString = (LPSTR)cur->second;
	}
	return o_strMapFunc(lpData, lpString);
}

void HijackDLL();
void HookFunction();

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hInstance);
		HijackDLL();
		HookFunction();
	}
	return TRUE;
}

FARPROC fpAddr[17];

void HijackDLL()
{
	WCHAR pszDllPath[MAX_PATH];
	GetSystemDirectory(pszDllPath, MAX_PATH);
	lstrcat(pszDllPath, L"\\VERSION.dll");
	HMODULE hModule = LoadLibrary(pszDllPath);
	if (hModule == NULL)
		return;
	fpAddr[0] = GetProcAddress(hModule, "GetFileVersionInfoA");
	fpAddr[1] = GetProcAddress(hModule, "GetFileVersionInfoByHandle");
	fpAddr[2] = GetProcAddress(hModule, "GetFileVersionInfoExA");
	fpAddr[3] = GetProcAddress(hModule, "GetFileVersionInfoExW");
	fpAddr[4] = GetProcAddress(hModule, "GetFileVersionInfoSizeA");
	fpAddr[5] = GetProcAddress(hModule, "GetFileVersionInfoSizeExA");
	fpAddr[6] = GetProcAddress(hModule, "GetFileVersionInfoSizeExW");
	fpAddr[7] = GetProcAddress(hModule, "GetFileVersionInfoSizeW");
	fpAddr[8] = GetProcAddress(hModule, "GetFileVersionInfoW");
	fpAddr[9] = GetProcAddress(hModule, "VerFindFileA");
	fpAddr[10] = GetProcAddress(hModule, "VerFindFileW");
	fpAddr[11] = GetProcAddress(hModule, "VerInstallFileA");
	fpAddr[12] = GetProcAddress(hModule, "VerInstallFileW");
	fpAddr[13] = GetProcAddress(hModule, "VerLanguageNameA");
	fpAddr[14] = GetProcAddress(hModule, "VerLanguageNameW");
	fpAddr[15] = GetProcAddress(hModule, "VerQueryValueA");
	fpAddr[16] = GetProcAddress(hModule, "VerQueryValueW");
}

#include "detours/detours.h"

void HookFunction()
{
	WCHAR pszLibPath[MAX_PATH];
	GetModuleFileName(NULL, pszLibPath, MAX_PATH);
	if (StrStrI(pszLibPath, L"HoloCure.exe") == NULL)
		return;

	MEMORY_BASIC_INFORMATION memInfo;
	PBYTE lpBaseAddr = (PBYTE)GetModuleHandle(NULL);
	VirtualQuery(lpBaseAddr, &memInfo, sizeof(memInfo));

	PBYTE lpAddr = lpBaseAddr + memInfo.RegionSize;
	VirtualQuery(lpAddr, &memInfo, sizeof(memInfo));
	if (memInfo.RegionSize <= 0x1000)
		return;

	for (size_t i = 0; i < memInfo.RegionSize - 32; i++) {
		if (memcmp(lpAddr + i, "\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\xF9\x48\x8B\xDA\xB9\x10\x00\x00\x00", 21) == 0) {

			o_strMapFunc = (t_StrMapFunc)(lpAddr + i);
			DetourTransactionBegin();
			DetourAttach(&o_strMapFunc, Hook_StrMapFunc);
			DetourTransactionCommit();

			break;
		}
	}
}

void ExpFunc001() { fpAddr[0](); }
void ExpFunc002() { fpAddr[1](); }
void ExpFunc003() { fpAddr[2](); }
void ExpFunc004() { fpAddr[3](); }
void ExpFunc005() { fpAddr[4](); }
void ExpFunc006() { fpAddr[5](); }
void ExpFunc007() { fpAddr[6](); }
void ExpFunc008() { fpAddr[7](); }
void ExpFunc009() { fpAddr[8](); }
void ExpFunc010() { fpAddr[9](); }
void ExpFunc011() { fpAddr[10](); }
void ExpFunc012() { fpAddr[11](); }
void ExpFunc013() { fpAddr[12](); }
void ExpFunc014() { fpAddr[13](); }
void ExpFunc015() { fpAddr[14](); }
void ExpFunc016() { fpAddr[15](); }
void ExpFunc017() { fpAddr[16](); }
