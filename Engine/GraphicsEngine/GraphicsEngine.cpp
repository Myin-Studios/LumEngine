#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include <d3d11.h>

GraphicsEngine::GraphicsEngine()
{
}

bool GraphicsEngine::init()
{
    D3D_DRIVER_TYPE driverType[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE
    };

    D3D_FEATURE_LEVEL features[] =
    {
        D3D_FEATURE_LEVEL_11_0
    };

    UINT driverTypeArraySize = ARRAYSIZE(driverType);
    UINT featuresArraySize = ARRAYSIZE(features);
    HRESULT res = 0;

    ID3D11DeviceContext* m_DevContext = nullptr;

    for (UINT driverType_Index = 0; driverType_Index < driverTypeArraySize; driverType_Index++)
    {
        res = D3D11CreateDevice(NULL, driverType[driverType_Index], NULL, NULL, features,
            featuresArraySize, D3D11_SDK_VERSION, &m_D3D11Device, &m_Features, &m_DevContext);

        if (SUCCEEDED(res))
            break;
    }

    if (FAILED(res))
        return false;

    m_ImmDevC = new DeviceContext(m_DevContext);

    m_D3D11Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_DXGIDevice);
    m_DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_DXGIAdapter);
    m_DXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_DXGIFactory);

    return true;
}

bool GraphicsEngine::release()
{
    m_DXGIDevice->Release();
    m_DXGIAdapter->Release();
    m_DXGIFactory->Release();

    m_ImmDevC->release();
    m_D3D11Device->Release();

    return true;
}

GraphicsEngine::~GraphicsEngine()
{
}

GraphicsEngine* GraphicsEngine::get()
{
    static GraphicsEngine engine;
    return &engine;
}

SwapChain* GraphicsEngine::createSwapChain()
{


    return new SwapChain();
}

DeviceContext* GraphicsEngine::getImmDevC()
{
    return this->m_ImmDevC;
}
