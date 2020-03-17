#include "DirectXInit.h"
#include <DirectXMath.h>

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
	float bgColor[4] {red, green, blue, 1.0f};

	d3d11DeviceContext->ClearRenderTargetView(d3d11RenderTargetView, bgColor);
	//PRESENT THE BACK BUFFER TO THE SCREEN
	swapChain->Present(0, 0);
}
