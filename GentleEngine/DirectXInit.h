#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
//#pragma comment(lib, "d3dx10.lib")

#include <dxgi.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
//#include <d3dx11.h>
//#include <d3dx10.h>
//#include <xnamath.h>

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

bool InitializeDirect3d11App(HINSTANCE hInstance, int width, int height, HWND outputWindow);
void ReleaseObject();
bool InitScene();
void UpdateScene();
void DrawScene();
