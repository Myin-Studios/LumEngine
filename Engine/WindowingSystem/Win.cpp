#include "Win.h"
#include <iostream>
#include <WinUser.h>
#include "EngineStyle.h"
#include "MainWindow.h"
#include <wingdi.h>

using namespace std;

Win* pScene = nullptr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        pScene->onCreate();
        cout << "Scene window created." << endl;
        break;
    }
    case WM_SIZE:
    {
        RECT wRect;

        GetWindowRect(hWnd, &wRect);

        HRGN hRgn = ::CreateRoundRectRgn(0, 0, 700, 550, 30, 30);
        ::SetWindowRgn(hWnd, hRgn, TRUE);
        ::DeleteObject(hRgn);
        break;
    }
    case WM_DESTROY:
    {
        cout << "Scene window destroyed." << endl;
        ::PostQuitMessage(0);
        break;
    }
    default:
    {
        return ::DefWindowProc(hWnd, msg, wParam, lParam);
        break;
    }
    }

    return NULL;
}

Win::Win() : CMItem(700, 500, radius, properties, boundsOffset, x, y, radiusFlags)
{
}

void Win::init(HWND parent, int xPos, int yPos, int wWidth, int wHeight)
{
    WNDCLASSEX wc = {};
    wc.cbClsExtra = NULL;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbWndExtra = NULL;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = NULL;
    wc.lpszClassName = L"Scene";
    wc.lpszMenuName = L"";
    wc.style = NULL;
    wc.lpfnWndProc = WndProc;

    if (!::RegisterClassEx(&wc))
        return;

    if (!pScene)
    {
        pScene = this;
    }

    m_hWnd = CreateWindowEx(NULL, L"Scene", L"Scene",
        NULL, xPos, yPos, wWidth, wHeight,
    parent, NULL, (HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE), NULL);

    if (!m_hWnd)
    {
        cout << "Error during CreateWindowEx." << endl;
        return;
    }

    ::ShowWindow(m_hWnd, SW_SHOW);
    ::UpdateWindow(m_hWnd);

    //SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) & ~( WS_DLGFRAME | WS_THICKFRAME));
    
    m_isRunning = true;

    setHWND(m_hWnd);
    SetParent(m_hWnd, parent);
}

bool Win::release()
{
    if (::DestroyWindow(m_hWnd))
        return false;

    return true;
}

bool Win::broadcast()
{
    MSG msg;

    pScene->onUpdate();

    while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Sleep(0);

    return true;
}

bool Win::isRunning()
{
    return m_isRunning;
}

RECT Win::getClientWindowRect()
{
    RECT new_Rect;

    ::GetClientRect(this->getHWND(), &new_Rect);
    return new_Rect;
}

void Win::onCreate()
{
    pScene = this;
}

void Win::onUpdate()
{
}

void Win::onDestroy()
{
    m_isRunning = false;
}

Win::~Win()
{
}
