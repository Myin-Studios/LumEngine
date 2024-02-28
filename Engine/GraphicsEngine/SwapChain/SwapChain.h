#pragma once
#include <d3d11.h>

class DeviceContext;

class SwapChain
{
public:
	SwapChain();

	//SwapChain initialization.
	bool init(HWND hWnd, UINT width, UINT height);

	bool present(bool vsync);

	//SwapChain release.
	bool release();

	~SwapChain();

private:
	IDXGISwapChain* m_SC;
	ID3D11RenderTargetView* m_RTV;

	friend class DeviceContext;
};

