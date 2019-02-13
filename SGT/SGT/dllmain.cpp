// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "stdafx.h"

extern "C"
{
	_declspec(dllexport) int CYScreen(void)
	{
		return GetSystemMetrics(SM_CYSCREEN);
	}

	_declspec(dllexport) int function2(void)
	{
		return GetSystemMetrics(SM_CYSCREEN);
	}

}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

