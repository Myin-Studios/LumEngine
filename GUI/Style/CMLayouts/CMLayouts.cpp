#include "CMLayouts.h"

CMLayoutH* pLayout = nullptr;

LRESULT CALLBACK HLayoutProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		std::cout << "HLayout created." << std::endl;

		pLayout->setInstance();
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		HBRUSH col = CreateSolidBrush(RGB(35, 35, 35));
		// All painting occurs here, between BeginPaint and EndPaint.

		FillRect(hdc, &ps.rcPaint, (HBRUSH)col);

		EndPaint(hWnd, &ps);

		break;
	}
	case WM_DESTROY:
	{
		std::cout << "HLayout destroyed." << std::endl;
		::PostQuitMessage(0);
		break;
	}
	default:
	{
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}
	}

	return (LRESULT)NULL;
}

CMLayoutH::CMLayoutH(HWND wParent, int flag) : CMItem(width, height, 0, CMW_NOPROPS, 0, x, y, CMW_NOPROPS)
{
	WNDCLASSEX hLWC = {};
	hLWC.cbSize = sizeof(WNDCLASSEX);
	hLWC.hInstance = NULL;
	hLWC.lpszClassName = L"HLayout";
	hLWC.cbClsExtra = NULL;
	hLWC.cbWndExtra = NULL;
	hLWC.hbrBackground = (HBRUSH)COLOR_WINDOW;
	hLWC.hCursor = LoadCursor(NULL, IDC_ARROW);
	hLWC.hIcon = NULL;
	hLWC.hIconSm = NULL;
	hLWC.lpszMenuName = L"";
	hLWC.style = NULL;
	hLWC.lpfnWndProc = HLayoutProc;

	if (!RegisterClassEx(&hLWC))
		std::cout << "HLAYOUT REGISTER CLASS ERROR" << std::endl;

	if (!pLayout)
		pLayout = this;

	m_HLayout = CreateWindowEx(NULL, L"HLayout", L"", WS_CHILD | WS_VISIBLE, x, y, width, height, wParent, NULL, NULL, NULL);

	this->setHWND();

	if (!m_HLayout)
		std::cout << "Error during CreateWindowEx (HLayout)." << std::endl;

	MSG msg;

	while (::PeekMessage(&msg, m_HLayout, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	ShowWindow(m_HLayout, SW_SHOW);
}

CMLayoutH::~CMLayoutH()
{
}

void CMLayoutH::addItem(CMItem item, int row, int column, int flags)
{
	int pWidth = this->width, pHeight = this->height;

	switch (flags)
	{
	case CMW_EXTENDTOBOUNDSH:
	{
		if (row > 0 && column > 0)
			SetWindowPos(item.m_ItemHWND, NULL, item.x + pWidth / column, item.y + pHeight / row, pWidth, item.height, SWP_FRAMECHANGED);

		break;
	}
	case CMW_EXTENDTOBOUNDSV:
	{
		if (row > 0 && column > 0)
			SetWindowPos(item.m_ItemHWND, NULL, item.x + pWidth / column, item.y + pHeight / row, item.width, pHeight, SWP_FRAMECHANGED);

		break;
	}
	case CMW_NOPROPS:
	{
		if (row > 0 && column > 0)
			SetWindowPos(item.m_ItemHWND, NULL, item.x + pWidth / column, item.y + pHeight / row, item.width, item.height, SWP_FRAMECHANGED);

		break;
	}
	default:
		break;
	}
}

void CMLayoutH::setInstance()
{
	pLayout = this;
}
