#include "MainWindow.h"

using namespace std;

MainWin* pMainScene = nullptr;

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        pMainScene->onCreate();

        cout << "Main window created." << endl;

        BOOL USE_DARK_MODE = true;
        BOOL SET_IMMERSIVE_DARK_MODE_SUCCESS = SUCCEEDED(DwmSetWindowAttribute(
            hWnd, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
            &USE_DARK_MODE, sizeof(USE_DARK_MODE)));

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
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        HBRUSH col = CreateSolidBrush(RGB(15, 15, 15));

        //BOOL bSetCC = true;
        //BOOL SET_CAPTION_COLOR = SUCCEEDED(DwmSetWindowAttribute(
        //    hWnd, DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR,
        //    &bSetCC, sizeof(col)));

        // All painting occurs here, between BeginPaint and EndPaint.
        FillRect(hdc, &ps.rcPaint, (HBRUSH)col);
        EndPaint(hWnd, &ps);

        break;
    }
    //case WM_NCHITTEST:
    //{
    //    LRESULT result = DefWindowProc(hWnd, msg, wParam, lParam);
    //    if (result == HTCLIENT)
    //        return HTCAPTION;
    //
    //    break;
    //}
    case WM_SIZE:
    {
        EnumChildWindows(hWnd, CMItem::ItemProc, lParam);

        break;
    }
    case WM_DESTROY:
    {
        pMainScene->onDestroy();
        cout << "Main window destroyed." << endl;
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

MainWin::MainWin() : CMItem(1024, 768, radius, properties, boundsOffset, CW_USEDEFAULT, CW_USEDEFAULT, radiusFlags)
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
    wc.lpszClassName = L"MainWin";
    wc.lpszMenuName = L"";
    wc.style = NULL;
    wc.lpfnWndProc = MainWndProc;

    if (!::RegisterClassEx(&wc))
        cout << "ERROR RCEX (MainWin)." << endl;

    if (!pMainScene)
    {
        pMainScene = this;
    }

    m_hWnd = ::CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"MainWin", L"Segreto di Statooo",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
        NULL, NULL, NULL, NULL);

    if (!m_hWnd)
    {
        cout << "Error during CreateWindowEx (MainWin)." << endl;
    }
    else this->setHWND(m_hWnd);

    ::ShowWindow(m_hWnd, SW_SHOW);
    ::UpdateWindow(m_hWnd);

    m_isRunning = true;
}

MainWin::~MainWin()
{
}

bool MainWin::init()
{
    RECT r = getClientWindowRect();
    //RECT sr = scene->getClientWindowRect();

    //scene->init(m_hWnd, (r.right / 2 - 700 / 2), r.bottom / 2 - 550 / 2, 700, 550);

    return true;
}

bool MainWin::release()
{
    if (::DestroyWindow(m_hWnd))
        return false;

    return true;
}

void MainWin::broadcast()
{
    while (m_isRunning)
    {
        MSG msg;

        while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        Sleep(0);
    }
}

bool MainWin::isRunning()
{
    return m_isRunning;
}

RECT MainWin::getClientWindowRect()
{
    RECT new_Rect;

    ::GetClientRect(this->m_hWnd, &new_Rect);
    return new_Rect;
}

void MainWin::onCreate()
{
}

void MainWin::onUpdate()
{
}

void MainWin::onDestroy()
{
    m_isRunning = false;
}

HWND MainWin::getHWnd()
{
    return m_hWnd;
}
