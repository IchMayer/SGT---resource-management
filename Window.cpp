#include "stdafx.h"
#include "Window.h"

typedef int(*CYScreen)(void);					//Тип функции определение высоты экрана
typedef bool(*CheckSSE3)(void);					//Тип функции проверка SSE3

BOOL(WINAPI *pSetLayer)(HWND, COLORREF, BYTE, DWORD);

HINSTANCE hInst;                                //Текущий экземпляр
HWND Edit1, Edit2;								//Textboxs 

ATOM                MyRegisterClass(HINSTANCE hInstance);	//Создание класса окна
BOOL                InitInstance(HINSTANCE);				//Создание окна
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);	//Функция обработки сообзений
HICON				HICONFromCBitmap();						//Функция закрызки иконки

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,			//Точка входа
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    MyRegisterClass(hInstance);							// Создание класса
    if (!InitInstance (hInstance))		    // Выполнить инициализацию приложения:
    {													// (Создает окно)
        return FALSE;						// В случае ошибки окна завершается
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
    WNDCLASSEX wcex = {0};	//Заполняем нулями

    wcex.cbSize = sizeof(WNDCLASSEX);

	auto ic = HICONFromCBitmap();			//Загружаем иконку
	wcex.hIcon =  ic ? ic : LoadIcon(nullptr, IDI_WARNING);

    wcex.style          = CS_VREDRAW;		//Стиль окна
    wcex.lpfnWndProc    = WndProc;			//Функция обработки
    wcex.hInstance      = hInstance;		//
    wcex.hCursor        = LoadCursor(nullptr, IDC_HELP);
    wcex.hbrBackground  = (HBRUSH)CreateSolidBrush( RGB(230, 230, 255));
	wcex.lpszClassName	= "ClassName";

    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(L"ClassName", L"var 8 by Valerka", WS_OVERLAPPED  | WS_SYSMENU,
      CW_USEDEFAULT, 0,400,300, nullptr, nullptr, hInstance, nullptr);

   //Ошибка создания окна
   if (!hWnd)
   {
      return FALSE;
   }
   
   ShowWindow(hWnd, SW_SHOWNOACTIVATE);
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
			wsprintfW(t, L"Основное задание:\r\n\r\n\r\n\"Определить высоту экрана(в пикселях)\"\r\n\r\nВысота экрана:  %d", x);
			SetWindowTextW(Edit1, t);
		}
		else
			MessageBoxW(hWnd, L"Не удалось найти функцию определения размера экрана", 0, MB_OK);
		
		CheckSSE3 sse3 = (CheckSSE3)GetProcAddress(hinstLib, "CheckSSE3");
		if (sse3)
		{
			WCHAR t[255];
			bool x = sse3();										//Выполнение функции
			wsprintfW(t, L"Дополнительное задание:\r\n\r\n\"Определить наличие поддержки технологии SSE3\"\r\n\r\nПоддержка SSE3:  %s", x ? L"true" : L"false");
			SetWindowTextW(Edit2, t);
		}
		else
			MessageBoxW(hWnd, L"Не удалось найти функцию проверка SSE3", 0, MB_OK);
	}
	else
		MessageBoxW(hWnd, L"Ошибка открытия DLL", 0, MB_OK);
	FreeLibrary(hinstLib);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_COMMAND:
		switch (wParam)
		{
		case 1:												//Кнопка выполнить
		{
			thread t(threadfunction, hWnd);					//Создает поток
			t.detach();										//Отцепляет поток
		}													//Если не отцепить поток, то программа будет ждать
			break;											//его заверщения и окно будет это время висеть
		case 2:												//Закрытие по кнопке
			DestroyWindow(hWnd);							//Вызываем закрытие окна
			break;
		default:
			break;
		}
		break;
	case WM_CREATE:
	{		//Создание кнопок и тектовых полей
		HWND hwndButton = CreateWindowW(L"BUTTON",L"Запуск", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			10,225,100,25,hWnd,(HMENU)1,hInst, NULL);
		HWND hwndButton1 = CreateWindowW(L"BUTTON", L"Закрыть", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			270, 225, 100, 25, hWnd, (HMENU)2, hInst, NULL);
		Edit1 = CreateWindowW(L"EDIT", L"", ES_LEFT | ES_MULTILINE | ES_READONLY | WS_VISIBLE | WS_CHILD,
			10, 10, 175, 200, hWnd, (HMENU)3, hInst, NULL);
		Edit2 = CreateWindowW(L"EDIT", L"", ES_LEFT | ES_MULTILINE | ES_READONLY | WS_VISIBLE | WS_CHILD,
			200, 10, 175, 200, hWnd, (HMENU)4, hInst, NULL);

		HINSTANCE hMod = LoadLibrary("user32.dll");		//Полупрозрачность окна
		if (hMod)											//если удалост подгразить библиотеку
		{
			pSetLayer = (int(__stdcall*)(HWND, COLORREF, BYTE, DWORD))GetProcAddress(hMod, "SetLayeredWindowAttributes");
			if (pSetLayer)									//Если удалось найти функцию
				if (SetWindowLong(hWnd, GWL_EXSTYLE, 0x80000))
					pSetLayer(hWnd, 0, 255 - 10 * 255 / 100, 0x02);
		}

	}
		break;
	case WM_CTLCOLORSTATIC:
	{
		//Стиль для текстовых полей
		HFONT hFont = CreateFont(18, 0, 0, 0, FW_MEDIUM, TRUE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
			CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Comic sans MS"));
	
		SetBkMode((HDC)wParam, TRANSPARENT);			//Убираем зарисовку за буквами
		SelectObject((HDC)wParam, hFont);				//Подкрипляем стиль

		return (INT_PTR)GetStockObject(NULL_BRUSH);		//Цвет фона текстовых полей прозрачный
	}
	case WM_CTLCOLORBTN:
	{
		//Стиль для строк
		HFONT hFont = CreateFont(20, 0, 15, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_MODERN, TEXT("Mistral"));
		
		SetBkMode((HDC)wParam, TRANSPARENT);			//Убираем зарисовку за буквами
		SelectObject((HDC)wParam, hFont);
		
		return (INT_PTR)GetStockObject(NULL_BRUSH);		//Цвет фона прозрачный(стандартный для кнопок)
	}
    case WM_DESTROY:
        PostQuitMessage(0);								//Завершение работы 
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//Загрузка иконки из файла icon.h (находящегося в этой же папке)
HICON HICONFromCBitmap()
{
	HBITMAP hBmp = (HBITMAP)LoadImageW(NULL,
		L"icon.bmp",
		IMAGE_BITMAP, 0, 0,
		LR_LOADFROMFILE);
	BITMAP bmp;
	GetObject(hBmp, sizeof(BITMAP), (LPVOID)&bmp);

	ICONINFO ii = { 0 };
	ii.fIcon = TRUE;
	ii.hbmColor = hBmp;
	ii.hbmMask = hBmp;

	HICON hIcon = CreateIconIndirect(&ii);

	return hIcon;
}