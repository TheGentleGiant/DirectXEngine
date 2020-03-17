#pragma comment(lib, "d3d11.lib")
#include <Windows.h>
#include <dxgi.h>
#include <d3d11.h>

#pragma region Fields
LPCTSTR WndClassName = TEXT("mainWindow");
HWND hwnd = NULL;
const int windowHeight = 720;
const int windowWidth = 1280;

/********************SWAP CHAIN*************************************/
IDXGISwapChain* swapChain;
ID3D11Device* d3d11Device;
ID3D11DeviceContext* d3d11DeviceContext;
ID3D11RenderTargetView* d3d11RenderTargetView;

float red = 0.0f;
float green = 0.0f;
float blue = 0.0f;
int colorModR = 1;
int colorModG = 1;
int colorModB = 1;


/*******************************************************************/

#pragma endregion
#pragma region Methods
/*Window Initialization*/
bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed);
int MessageLoop();
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

/********************SWAP CHAIN*************************************/
bool InitializeDirect3d11App(HINSTANCE hInstance, int width, int height, HWND outputWindow);
void ReleaseObject();
bool InitScene();
void UpdateScene();
void DrawScene();
#pragma endregion

int _stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,_In_ int nShowCmd)
{
	if (!InitializeWindow(hInstance, nShowCmd, windowWidth, windowHeight, true)) {//INITIALIZE WINDOW
		MessageBox(0, TEXT("Window Initialization failed!"), TEXT("Error"), MB_OK);
		return 0;
	}
	if (!InitializeDirect3d11App(hInstance, windowWidth, windowHeight, hwnd))//INITIALIZE DIRECT 3D
	{
		MessageBox(0, TEXT("Direct3D Initialization failed!"), TEXT("Error"), MB_OK);
		return 0;
	}
	if (!InitScene())//INITIALIZE SCENE
	{
		MessageBox(0, TEXT("Scene Initialization failed!"), TEXT("Error"), MB_OK);
		return 0;
	}
	MessageLoop();
	ReleaseObject();
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
			UpdateScene();
			DrawScene();
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

/********************SWAP CHAIN*************************************/
bool InitializeDirect3d11App(HINSTANCE hInstance, int width, int height, HWND outputWindow)
{
	HRESULT hResult;

	//DESCRIBE BUFFER
	DXGI_MODE_DESC bufferDescription;

	ZeroMemory(&bufferDescription, sizeof(DXGI_MODE_DESC));

	bufferDescription.Width = width;
	bufferDescription.Height = height;
	bufferDescription.RefreshRate.Numerator = 60;
	bufferDescription.RefreshRate.Denominator = 1;
	bufferDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDescription.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDescription.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	//DESCRIBE SWAPCHAIN
	DXGI_SWAP_CHAIN_DESC swapChainDescription;

	ZeroMemory(&swapChainDescription, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDescription.BufferDesc = bufferDescription;
	swapChainDescription.SampleDesc.Count = 1;
	swapChainDescription.SampleDesc.Quality = 0;
	swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDescription.BufferCount = 1;
	swapChainDescription.OutputWindow = outputWindow;
	swapChainDescription.Windowed = TRUE;
	swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


	//CREATE SWAPCHAIN
	hResult = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &swapChainDescription, &swapChain, &d3d11Device, NULL, &d3d11DeviceContext);

	//CREATE BECK BUFFER
	ID3D11Texture2D* backBuffer;
	hResult = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

	//CREATE RENDER TARGET
	hResult = d3d11Device->CreateRenderTargetView(backBuffer, NULL, &d3d11RenderTargetView);
	backBuffer->Release();

	//SET RENDER TARGET
	d3d11DeviceContext->OMSetRenderTargets(1, &d3d11RenderTargetView, NULL);

	return true;
}

void ReleaseObject()
{
	//RELEASE THE COM OBJECTS CREATED IN "InitializeDirect3d11App"
	swapChain->Release();
	d3d11Device->Release();
	d3d11DeviceContext->Release();
}

bool InitScene()
{
	return true;
}

void UpdateScene()
{
	red += colorModR * 0.00005f;
	green += colorModG * 0.00002f;
	blue += colorModB * 0.00001f;

	if (red >= 1.0f || red <= 0.0f)
	{
		colorModR *= -1;
	}
	if (green >= 1.0f || green <= 0.0f)
	{
		colorModG *= -1;
	}
	if (blue >= 1.0f || blue <= 0.0f)
	{
		colorModB *= -1;
	}
}

void DrawScene()
{
	//CLEAR BACK BUFFER TO THE UPDATED COLOR
	float bgColor[4]{ red, green, blue, 1.0f };

	d3d11DeviceContext->ClearRenderTargetView(d3d11RenderTargetView, bgColor);
	//PRESENT THE BACK BUFFER TO THE SCREEN
	swapChain->Present(0, 0);
}
#pragma endregion

