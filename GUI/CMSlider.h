//
// Created by darkd on 15/03/2024.
//

#ifndef CMENGINE_CMSLIDER_H
#define CMENGINE_CMSLIDER_H

#endif //CMENGINE_CMSLIDER_H

#include "CMItem.h"

class CMSlider : public CMItem
{
public:
    explicit CMSlider(const CMItem&);
    ~CMSlider() = default;

    void show();

private:
    HWND _sliderHWND = NULL;
    HINSTANCE hInst;
};

LRESULT CALLBACK SliderProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_CREATE:
        {
            CMLog::Succeed("Slider created.");
            break;
        }
        case WM_DESTROY:
        {
            CMLog::Succeed("Slider destroyed.");
            PostQuitMessage(0);
            break;
        }
        default: break;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

CMSlider::CMSlider(const CMItem& _parentItem) : CMItem(150, 30, 30)
{
    hInst = GetModuleHandle(NULL);

    if (hInst)
    {
        HBRUSH br = CreateSolidBrush(RGB(13, 26, 59));

        WNDCLASSEX wc = {};
        wc.lpfnWndProc = SliderProc;
        wc.cbClsExtra = NULL;
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.cbWndExtra = NULL;
        wc.hCursor = LoadCursor(hInst, IDC_ARROW);
        wc.hIcon = NULL;
        wc.hIconSm = NULL;
        wc.hInstance = hInst;
        wc.hbrBackground = br;
        wc.lpszClassName = "CMSlider";
        wc.lpszMenuName = "";
        wc.style = CS_VREDRAW | CS_HREDRAW;

        if (!RegisterClassEx(&wc))
            CMLog::Error("Error during slider class registration. Error n.: " + to_string(GetLastError()));

        this->_sliderHWND = CreateWindowEx(NULL, "CMSlider", "", WS_CHILD | WS_VISIBLE, 30, 30, width, height, _parentItem.getHWND(), NULL, hInst, NULL);

        if (!this->_sliderHWND)
            CMLog::Error("Error during slider window creation. Error n.: " + to_string(GetLastError()));

        SetParent(this->_sliderHWND, _parentItem.getHWND());
    }
    else CMLog::Error("Cannot initialize slider: instance handler is undefined.");
}

void CMSlider::show() {
    ShowWindow(this->_sliderHWND, SW_SHOW);
    UpdateWindow(this->_sliderHWND);

    MSG msg;

    while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    Sleep(0);
}
