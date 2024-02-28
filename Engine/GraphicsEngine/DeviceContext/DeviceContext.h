#pragma once
#include <d3d11.h>

class SwapChain;

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* devC);
	~DeviceContext();

	bool clearRenderTargetColor(SwapChain* sc, float r, float g, float b, float a);

	bool release();

private:
	ID3D11DeviceContext* m_DevC;
};

