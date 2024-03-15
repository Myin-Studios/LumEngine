#pragma once

#include "CMItem.h"

class CMWindow : public CMItem
{
public:
	CMWindow();
	~CMWindow();

    void run();

private:
	HWND _WndHWND = NULL;
	HINSTANCE hInst;
};

LRESULT CALLBACK MWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lparam)
{
    switch (msg) {
        case WM_CREATE:
        {
            CMLog::Succeed("Main window created.");

            HANDLE wIcon = LoadImage(0, TEXT("Assets/CMEIcon.ico"), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);

            if (wIcon)
            {
                SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)wIcon);
                SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)wIcon);

                SendMessage(GetWindow(hWnd, GW_OWNER), WM_SETICON, ICON_SMALL, (LPARAM)wIcon);
                SendMessage(GetWindow(hWnd, GW_OWNER), WM_SETICON, ICON_BIG, (LPARAM)wIcon);
            }

            break;
        }
        case WM_DESTROY:
        {
            CMLog::Succeed("Main window destroyed.");
            PostQuitMessage(0);
            break;
        }
        default: break;
    }

    return DefWindowProc(hWnd, msg, wParam, lparam);
}

CMWindow::CMWindow() : CMItem(800, 500, 0)
{
    hInst = GetModuleHandle(NULL);

	HBRUSH br = CreateSolidBrush(RGB(25, 25, 25));

	if (hInst)
	{
		WNDCLASSEX wc = {};
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.lpszClassName = "MainWin";
		wc.cbClsExtra = NULL;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.hbrBackground = br;
		wc.cbWndExtra = NULL;
		wc.hCursor = LoadCursor(hInst, IDC_ARROW);
		wc.hIcon = NULL;
		wc.hIconSm = NULL;
		wc.hInstance = hInst;
		wc.lpszMenuName = "CryoMoon Engine";
        wc.lpfnWndProc = MWinProc;

        if (!RegisterClassEx(&wc))
            CMLog::Error("Error during window class registration. Error n.: " + to_string(GetLastError()));

        _WndHWND = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, "MainWin", "", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
                                  NULL, NULL, hInst, NULL);

        if (!_WndHWND)
            CMLog::Error("Error during window creation. Error n.:" + to_string(GetLastError()));

        ShowWindow(_WndHWND, SW_SHOW);
        UpdateWindow(_WndHWND);
    }
	else CMLog::Error("HINSTANCE is not initialized.");
}

CMWindow::~CMWindow()
{
}

void CMWindow::run()
{
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Sleep(0);
}
