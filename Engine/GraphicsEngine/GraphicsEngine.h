#pragma once
#include <d3d11.h>

class SwapChain;
class DeviceContext;

class GraphicsEngine
{
public:
	GraphicsEngine();

	//Initialization of the Graphics Engine with DirectX11 Graphics API.
	bool init();

	//Release all the source loaded.
	bool release();

	~GraphicsEngine();

	static GraphicsEngine* get();

	SwapChain* createSwapChain();
	DeviceContext* getImmDevC();

private:
	DeviceContext* m_ImmDevC = nullptr;

	ID3D11Device* m_D3D11Device = nullptr;
	D3D_FEATURE_LEVEL m_Features;

	IDXGIDevice* m_DXGIDevice = nullptr;
	IDXGIAdapter* m_DXGIAdapter = nullptr;
	IDXGIFactory* m_DXGIFactory = nullptr;

	friend class SwapChain;
};

