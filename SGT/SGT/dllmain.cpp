// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "stdafx.h"
#include <intrin.h>

void cpuid(int regs[4], int func)
{
	int ieax, iebx, iecx, iedx;
	__asm
	{
		mov eax, func
		cpuid
		mov ieax, eax
		mov iebx, ebx
		mov iecx, ecx
		mov iedx, edx
	}
	regs[0] = ieax;
	regs[1] = iebx;
	regs[2] = iecx;
	regs[3] = iedx;
}

extern "C"
{
	_declspec(dllexport) int CYScreen(void)
	{
		return GetSystemMetrics(SM_CYSCREEN);				//Получение высоты экрана
	}

	_declspec(dllexport) bool CheckSSE3(void)
	{
		int CPUInfo[4] = { -1 };
		
		cpuid(CPUInfo, 0);									//Проверка поддержки cpuid
		int nIds = CPUInfo[0];

		if (nIds >= 0x00000001) 
		{
			cpuid(CPUInfo, 0x00000001);						//Полчение информации о процессоре
			return (CPUInfo[2] & ((int)1 << 0)) != 0;		//Проверка SSE3
		}
		return false;
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

