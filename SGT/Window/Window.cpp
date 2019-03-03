#include "stdafx.h"
#include "Window.h"

typedef int(*CYScreen)(void);					//Тип функции определение высоты экрана
typedef bool(*CheckSSE3)(void);					//Тип функции проверка SSE3

HINSTANCE hInst;                                //Текущий экземпляр
HWND Edit1, Edit2;								//Textboxs 

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    MyRegisterClass(hInstance);							// Создание класса
    if (!InitInstance (hInstance, nCmdShow))		    // Выполнить инициализацию приложения:
    {													// (Создает окно)
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

void threadfunction(HWND hWnd)
{
	auto hinstLib = LoadLibrary(TEXT("SGT.dll"));		//Загружает библиотеку
	if (hinstLib)										//Проверка на наличие библиотеки
	{
		CYScreen cyscreen = (CYScreen)GetProcAddress(hinstLib, "CYScreen");	//Обращение кк функции из библиотеки
		if (cyscreen)														//Проверка на наличие такой функции
		{
			WCHAR t[255];
			int x = cyscreen();												//Выполнение функции
			wsprintf(t, L"Основное задание:\r\n\r\n\r\n\"Определить высоту экрана(в пикселях)\"\r\n\r\nВысота экрана:  %d", x);
			SendMessage(Edit1, EM_REPLACESEL, TRUE, (LPARAM)t);				//Изменение бокса
		}
		else
			MessageBox(hWnd, L"Не удалось найти функцию определения размера экрана", 0, MB_OK);

		CheckSSE3 sse3 = (CheckSSE3)GetProcAddress(hinstLib, "CheckSSE3");
		if (sse3)
		{
			WCHAR t[255];
			bool x = sse3();
			wsprintf(t, L"Дополнительное задание:\r\n\r\n\"Определить наличие поддержки технологии SSE3\"\r\n\r\nПоддержка SSE3:  %s", x ? L"true" : L"false");
			SendMessage(Edit2, EM_REPLACESEL, TRUE, (LPARAM)t);
		}
		else
			MessageBox(hWnd, L"Не удалось найти функцию проверка SSE3", 0, MB_OK);
	}
	else
		MessageBox(hWnd, L"Ошибка открытия DLL", 0, MB_OK);
	FreeLibrary(hinstLib);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_COMMAND:
		switch (wParam)
		{
		case 1:			//Кнопка выполнить
		{
			thread t(threadfunction, hWnd);//Создает поток
			t.detach();				//Отцепляет поток
		}
			break;
		case 2:			//Закрытие по кнопку
			DestroyWindow(hWnd);
			break;
		default:
			break;
		}
		break;
	case WM_CREATE:
	{		//Создание кнопок и тектовых полей
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
