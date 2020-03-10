#include <Windows.h>

#pragma region Fields
LPCTSTR WndClassName = TEXT("mainWindow");
HWND hwnd = NULL;
const int windowHeight = 720;
const int windowWidth = 1280;

#pragma endregion
#pragma region Methods
/*Window Initialization*/
bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed);
int MessageLoop();
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#pragma endregion

int _stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,_In_ int nShowCmd)
{
	if (!InitializeWindow(hInstance, nShowCmd, windowWidth, windowHeight, true)) {
		MessageBox(0, TEXT("Window Initialization failed!"), TEXT("Error"), MB_OK);
		return 0;
	}
	MessageLoop();
	return 0;
}

#pragma region WindowInitialization
bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WndClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, TEXT("Error Registering class"), TEXT("Error"), MB_OK | MB_ICONERROR);
		return false;
	}

	hwnd = CreateWindowEx(NULL, WndClassName, TEXT("Main Window"), WS_OVERLAPPEDWINDOW,
		METRICS_USEDEFAULT, METRICS_USEDEFAULT, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

	if (!hwnd)
	{
		MessageBox(NULL, TEXT("Error while creating Window"), TEXT("Error"), MB_OK | MB_ICONERROR);
		return false;
	}

	ShowWindow(hwnd, ShowWnd);
	UpdateWindow(hwnd);
	return true;
}
#pragma endregion

#pragma region WidnowMessage And MainLoop
int MessageLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (true)
	{
		if (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//GameCode
		}
	}
	return msg.wParam;
}
#pragma endregion

#pragma region WindowEventsCallback
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			if (MessageBox(0, TEXT("Are you sure you want to quit?"), TEXT("Error"), MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				DestroyWindow(hwnd);
			}
		}
	return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
	return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}
#pragma endregion

