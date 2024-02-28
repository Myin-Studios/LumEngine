#include "SwapChain.h"
#include "GraphicsEngine.h"

SwapChain::SwapChain()
{
}

bool SwapChain::init(HWND hWnd, UINT width, UINT height)
{
	ID3D11Device* dev = GraphicsEngine::get()->m_D3D11Device;
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1;
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hWnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE;

	HRESULT hr = GraphicsEngine::get()->m_DXGIFactory->CreateSwapChain(dev, &desc, &m_SC);

	if (FAILED(hr))
		return false;

	ID3D11Texture2D* bbTex2D;
	hr = m_SC->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&bbTex2D);

	if (FAILED(hr))
		return false;

	hr = dev->CreateRenderTargetView(bbTex2D, NULL, &m_RTV);

	bbTex2D->Release();

	if (FAILED(hr))
		return false;

	return true;
}

bool SwapChain::present(bool vsync)
{
	m_SC->Present(vsync, NULL);
	return true;
}

bool SwapChain::release()
{
	m_SC->Release();
	delete this;

	return true;
}

SwapChain::~SwapChain()
{
}
