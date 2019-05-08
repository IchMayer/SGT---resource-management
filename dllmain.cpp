// dllmain.cpp : Определяет точку входа для приложения DLL.
#include <intrin.h>
#include <windows.h>

extern "C"
{
	_declspec(dllexport) int CYScreen(void)
	{
		return GetSystemMetrics(SM_CYSCREEN);				//Получение высоты экрана
	}

	_declspec(dllexport) bool CheckSSE3(void)
	{
		bool result;
		__asm
		{
			//Проверка поддержки cpuid
			mov eax, 0
			cpuid
			cmp eax, 1
			jl fals	//Если не поддерживе

			mov eax, 1	//Проверка поддержки sse3
			cpuid
			//Рещультат -> 1 байт 3 переменной (1 - eax, 2-ebx, 3-ecx, 4-edx)
			and ecx, 1	//накладываем маску проверки	
			cmp ecx, 0	//Если этот байт ноль, значит не поддерживается
			je fals		//Если этот бит 

			tru:	//Вызод поддерживается
			mov result, 1
			jmp end		//Переход в конец

			fals: //Вызод не поддерживается или cpuid не поддерживается
			mov result, 0
			jmp end		//Переход в конец

			end:
		}
		return result;
#pragma region Код на с++

		//int CPUInfo[4] = { -1 };
		//
		//cpuid(CPUInfo, 0);									//Проверка поддержки cpuid



		//if (CPUInfo[0] >= 0x00000001)
		//{
		//	cpuid(CPUInfo, 0x00000001);						//Полчение информации о процессоре
		//	return (CPUInfo[2] & ((int)1 << 0)) != 0;		//Проверка SSE3
		//}
		//return false;
#pragma endregion
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

