#include "DeviceContext.h"
#include "SwapChain.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* devC) : m_DevC(devC)
{
	
}

DeviceContext::~DeviceContext()
{
}

bool DeviceContext::clearRenderTargetColor(SwapChain* sc, float r, float g, float b, float a)
{
	FLOAT clearColor[] = {r, g, b, a};
	m_DevC->ClearRenderTargetView(sc->m_RTV, clearColor);
	return true;
}

bool DeviceContext::release()
{
	m_DevC->Release();

	delete this;

	return true;
}
