// Window.cpp : Определяет точку входа для приложения.

#include "stdafx.h"
#include "Window.h"

typedef int(*CYScreen)(void);

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
HWND Edit1, Edit2;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOW));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOW));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)CreateSolidBrush( RGB(230, 230, 255));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOW);
	wcex.lpszClassName = L"ClassName";
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(L"ClassName", L"var 8 by Valerka", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0,400,300, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_COMMAND:
		switch (wParam)
		{
		case 1:
		{
			auto hinstLib = LoadLibrary(TEXT("SGT.dll"));
			if (hinstLib)
			{
				CYScreen cyscreen = (CYScreen)GetProcAddress(hinstLib, "CYScreen");
				if (cyscreen)
				{
					WCHAR t[255];
					int x = cyscreen();
					wsprintf(t, L"Основное задание:\r\n\r\n\r\n\"Определить высоту экрана(в пикселях)\"\r\n\r\nВысота экрана:  %d", x);
					SendMessage(Edit1, EM_REPLACESEL, TRUE, (LPARAM)t);


					wsprintf(t, L"Дополнительное задание:\r\n\r\n\"Определить наличие поддержки технологии SSE3\"\r\n\r\nВысота экрана:  %d", x);
					SendMessage(Edit2, EM_REPLACESEL, TRUE, (LPARAM)t);
				}
				else
					MessageBox(hWnd, L"Не удалось найти функцию", 0, MB_OK);

			}
			else
				MessageBox(hWnd, L"Ошибка открытия DLL", 0, MB_OK);
			FreeLibrary(hinstLib);
		}
			break;
		case 2:
			DestroyWindow(hWnd);
			break;
		default:
			break;
		}
		break;
	case WM_CREATE:
	{
		HWND hwndButton = CreateWindow(L"BUTTON",L"Запуск", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			10,225,100,25,hWnd,(HMENU)1,hInst, NULL);
		HWND hwndButton1 = CreateWindow(L"BUTTON", L"Закрыть", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			270, 225, 100, 25, hWnd, (HMENU)2, hInst, NULL);
		Edit1 = CreateWindow(L"EDIT", L"", ES_LEFT | ES_MULTILINE | ES_READONLY | WS_VISIBLE | WS_CHILD,
			10, 10, 175, 200, hWnd, (HMENU)3, hInst, NULL);
		Edit2 = CreateWindow(L"EDIT", L"", ES_LEFT | ES_MULTILINE | ES_READONLY | WS_VISIBLE | WS_CHILD,
			200, 10, 175, 200, hWnd, (HMENU)4, hInst, NULL);
	}
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
