#include "CMSliders.h"

HWND CMSlider::wParent = HWND();
HWND CMSlider::m_Slider = HWND();
HWND CMSlider::CMSlidable::m_Slidable = HWND();
bool CMSlider::isClicked = false;
POINT CMSlider::offset = { 0, 0 };

CMSlider* pSlider = nullptr;
CMSlider::CMSlidable* pSlidable = nullptr;

LRESULT CALLBACK SliderProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	RECT r;

	GetWindowRect(hWnd, &r);

	int h = (r.bottom - r.top);

	switch (msg)
	{
	case WM_CREATE:
	{
		std::cout << "Slidable created." << std::endl;
		pSlidable->setInstance();
		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		// All painting occurs here, between BeginPaint and EndPaint.

		RECT wRect;

		GetWindowRect(hWnd, &wRect);

		HBRUSH col = CreateSolidBrush(RGB(150, 200, 255));
		FillRect(hdc, &ps.rcPaint, (HBRUSH)col);

		EndPaint(hWnd, &ps);

		return 0;
	}

	case WM_LBUTTONDOWN:
	{
		CMSlider::isClicked = true;

		GetCursorPos(&CMSlider::offset);

		ScreenToClient(hWnd, &CMSlider::offset);

		return 0;
	}
	case WM_MOUSEMOVE:
	{
		RECT wRect;
		RECT wParentRect;
		GetWindowRect(GetConsoleWindow(), &wRect);
		GetWindowRect(CMSlider::m_Slider, &wParentRect);

		POINT mousePos;
		GetCursorPos(&mousePos);
		ScreenToClient(CMSlider::m_Slider, &mousePos);

		if (mousePos.x - CMSlider::offset.x + h <= (wParentRect.right - wParentRect.left) && mousePos.x - CMSlider::offset.x >= 0)
		{
			if (CMSlider::isClicked == true)
			{
				MoveWindow(hWnd, mousePos.x - pSlider->offset.x, 0, h, h, TRUE);
			}
		}
		else
		{
			CMSlider::isClicked = false;
		}

		return 0;
	}
	case WM_LBUTTONUP:
	{
		CMSlider::isClicked = false;

		return 0;
	}
	case WM_DESTROY:
	{
		std::cout << "Slidable destroyed." << std::endl;
		::PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return 0;
	}
	}

	return DefSubclassProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK SlideableProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT r;

	GetWindowRect(hWnd, &r);

	int h = (r.bottom - r.top);

	switch (msg)
	{
	case WM_CREATE:
	{
		std::cout << "Slideable created." << std::endl;
		SetParent(hWnd, pSlider->m_Slider);
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		// All painting occurs here, between BeginPaint and EndPaint.

		RECT wRect;

		GetWindowRect(hWnd, &wRect);

		HBRUSH col = CreateSolidBrush(RGB(150, 200, 255));
		FillRect(hdc, &ps.rcPaint, (HBRUSH)col);

		EndPaint(hWnd, &ps);

		return 0;
	}

	case WM_LBUTTONDOWN:
	{
		CMSlider::isClicked = true;

		GetCursorPos(&CMSlider::offset);

		ScreenToClient(hWnd, &CMSlider::offset);

		return 0;
	}
	case WM_MOUSEMOVE:
	{
		RECT wRect;
		RECT wParentRect;
		GetWindowRect(GetConsoleWindow(), &wRect);
		GetWindowRect(CMSlider::m_Slider, &wParentRect);

		POINT mousePos;
		GetCursorPos(&mousePos);
		ScreenToClient(CMSlider::m_Slider, &mousePos);

		if (mousePos.x - CMSlider::offset.x + h <= (wParentRect.right - wParentRect.left) && mousePos.x - CMSlider::offset.x >= 0)
		{
			if (CMSlider::isClicked == true)
			{
				MoveWindow(hWnd, mousePos.x - pSlider->offset.x, 0, h, h, TRUE);
			}
		}
		else
		{
			CMSlider::isClicked = false;
		}

		return 0;
	}
	case WM_LBUTTONUP:
	{
		CMSlider::isClicked = false;

		return 0;
	}
	case WM_DESTROY:
	{
		std::cout << "Slidable destroyed." << std::endl;
		::PostQuitMessage(0);
		return 0;
	}
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK SliderBGProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT r;

	GetWindowRect(hWnd, &r);

	int h = (r.bottom - r.top);

	HWND slideable = HWND();

	switch (msg)
	{
	case WM_CREATE:
	{
		pSlider->setInstance();

		WNDCLASSEX wc_slideable = {};
		wc_slideable.cbClsExtra = NULL;
		wc_slideable.cbSize = sizeof(WNDCLASSEX);
		wc_slideable.cbWndExtra = NULL;
		wc_slideable.hbrBackground = (HBRUSH)COLOR_WINDOW;
		wc_slideable.hCursor = LoadCursor(NULL, IDC_HAND);
		wc_slideable.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc_slideable.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		wc_slideable.hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
		wc_slideable.lpszClassName = L"Slideable";
		wc_slideable.lpszMenuName = L"";
		wc_slideable.style = NULL;
		wc_slideable.lpfnWndProc = SlideableProc;

		std::cout << "Slider created." << std::endl;

		if (!::RegisterClassEx(&wc_slideable))
			std::cout << "SLIDABLE REGISTER CLASS ERROR" << std::endl;

		pSlidable->m_Slidable = CreateWindowEx(NULL, L"Slideable", L"", WS_CHILD | WS_VISIBLE, 0, 0, pSlider->height, pSlider->height, hWnd, NULL, NULL, NULL);

		if (!pSlidable->m_Slidable)
			std::cout << "Error during CreateWindowEx (Slidable). Error n.: " << GetLastError() << std::endl;

		SetParent(pSlidable->m_Slidable, pSlider->m_Slider);

		ShowWindow(pSlidable->m_Slidable, SW_SHOW);

		return 0;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		HBRUSH col = CreateSolidBrush(RGB(30, 70, 150));
		// All painting occurs here, between BeginPaint and EndPaint.
		
		FillRect(hdc, &ps.rcPaint, (HBRUSH)col);

		EndPaint(hWnd, &ps);

		return 0;
	}
	case WM_SIZE:
	{
		RECT wRect;

		GetWindowRect(hWnd, &wRect);

		if (pSlider->radiusFlags == CMS_UPDATERADIUS)
		{
			if (pSlider->radius > pSlider->height)
			{
				HRGN hBigRgn = CreateRoundRectRgn(0, 0, pSlider->width, pSlider->height, pSlider->height, pSlider->height);
				HRGN smRgn = CreateEllipticRgn(0, 0, pSlider->height, pSlider->height);

				SetWindowRgn(pSlider->m_Slider, hBigRgn, TRUE);
				SetWindowRgn(slideable, smRgn, TRUE);
				
				DeleteObject(smRgn);
				DeleteObject(hBigRgn);
			}
			else if (pSlider->radius <= pSlider->height)
			{
				HRGN rrRgn = CreateRoundRectRgn(0, 0, pSlider->width, pSlider->height, pSlider->radius, pSlider->radius);
				HRGN rrRgnSlidable = CreateRoundRectRgn(0, 0, pSlider->height, pSlider->height, pSlider->radius, pSlider->radius);

				SetWindowRgn(pSlider->m_Slider, rrRgn, TRUE);
				SetWindowRgn(slideable, rrRgnSlidable, TRUE);

				DeleteObject(rrRgnSlidable);
				DeleteObject(rrRgn);
			}
		}

		UpdateWindow(slideable);
		UpdateWindow(hWnd);
		return 0;
	}
	case WM_DESTROY:
	{
		std::cout << "Slider destroyed." << std::endl;
		::PostQuitMessage(0);
		return 0;
	}
	default:
	{
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
	}
	}

	return (LRESULT)NULL;
}



CMSlider::CMSlider(CMItem parent, int flag) : CMItem(100, 20, 100000, flag, 10, 0, 0, CMW_NOPROPS)
{
	WNDCLASSEX wc = {};
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(wc);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = (HINSTANCE)GetWindowLongPtr(parent.m_ItemHWND, GWLP_HINSTANCE);
	wc.lpszClassName = L"Slider";
	wc.lpszMenuName = L"";
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = SliderBGProc;

	if (!::RegisterClassEx(&wc))
		std::cout << "SLIDER REGISTER CLASS ERROR" << std::endl;

	switch (properties)
	{
	case CMW_EXTENDTOBOUNDSH:
	{
		width = parent.width;
		x = 0;
		break;
	}
	case CMW_EXTENDTOBOUNDSV:
	{
		height = parent.height;
		y = 0;
		break;
	}
	case CMW_EXTENDTOBOUNDSHV:
	{
		width = parent.width;
		x = 0;
		height = parent.height;
		y = 0;
		break;
	}
	default:
		break;
	}

	if (!pSlider)
		pSlider = this;

	m_Slider = CreateWindowEx(NULL, L"Slider", L"",
		WS_CLIPCHILDREN | WS_CHILD | WS_VISIBLE,
		x, y, width, height, parent.m_ItemHWND, 0, (HINSTANCE)GetWindowLongPtr(parent.m_ItemHWND, GWLP_HINSTANCE), 0);

	if (!m_Slider)
	{
		std::cout << "Error during CreateWindowEx (Slider)." << std::endl;
	}

	this->setHWND(m_Slider);

	ShowWindow(m_Slider, SW_SHOW);

	MSG msg;

	while (::PeekMessage(&msg, m_Slider, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Sleep(0);

	SetParent(m_Slider, parent.m_ItemHWND);

	//CMSlidable* slidable = nullptr;
	//slidable->createSlidable();
}

CMSlider::~CMSlider()
{
}

void CMSlider::addBoundsOffset(int wOffset, int flag)
{
	boundsOffset = wOffset;
	switch (flag)
	{
	case CMW_OFFSETH:
	{
		width -= wOffset * 2;
		SetWindowPos(m_Slider, NULL, wOffset, y, width, height, SWP_FRAMECHANGED);
		UpdateWindow(m_Slider);
		break;
	}
	case CMW_OFFSETV:
	{
		height -= wOffset * 2;
		SetWindowPos(m_Slider, NULL, x, wOffset, width, height, SWP_FRAMECHANGED);
		UpdateWindow(m_Slider);
		break;
	}
	default:
		break;
	}
}

void CMSlider::setPos(int xPos, int yPos)
{
	x = xPos;
	y = yPos;

	SetWindowPos(m_Slider, NULL, x, y, width, height, SWP_FRAMECHANGED);
	UpdateWindow(m_Slider);
}

void CMSlider::setSize(int wWidth, int wHeight)
{
	width = wWidth;
	height = wHeight;

	SetWindowPos(m_Slider, NULL, x, y, width, height, SWP_FRAMECHANGED);
	UpdateWindow(m_Slider);
}

void CMSlider::setRadius(int wRadius, int radFlags)
{
	updateFlags(radiusFlags, radFlags);
	updateProps(pSlider->radius, wRadius);
	updateProps(radius, wRadius);
}

int CMSlider::getRadius()
{
	return radius;
}

void CMSlider::setGeometry(int xPos, int yPos, int wWidth, int wHeight)
{
	x = xPos;
	y = yPos;
	width = wWidth;
	height = wHeight;

	SetWindowPos(m_Slider, NULL, x, y, width, height, SWP_FRAMECHANGED);
	UpdateWindow(m_Slider);
}

void CMSlider::setInstance()
{
	pSlider = this;
}

//void CMSlider::CMSlidable::createSlidable()
//{
//	WNDCLASSEX wc_slidable = {};
//	wc_slidable.cbClsExtra = NULL;
//	wc_slidable.cbSize = sizeof(WNDCLASSEX);
//	wc_slidable.cbWndExtra = NULL;
//	wc_slidable.hbrBackground = (HBRUSH)COLOR_WINDOW;
//	wc_slidable.hCursor = LoadCursor(NULL, IDC_HAND);
//	wc_slidable.hIcon = LoadIcon(NULL, IDI_APPLICATION);
//	wc_slidable.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
//	wc_slidable.hInstance = (HINSTANCE)GetWindowLongPtr(m_Slider, GWLP_HINSTANCE);
//	wc_slidable.lpszClassName = L"Slidable";
//	wc_slidable.lpszMenuName = L"";
//	wc_slidable.style = NULL;
//	wc_slidable.lpfnWndProc = NULL;
//
//	if (!::RegisterClassEx(&wc_slidable))
//		std::cout << "SLIDABLE REGISTER CLASS ERROR" << std::endl;
//
//	m_Slidable = CreateWindowEx(NULL, L"Slidable", L"", WS_CHILD | WS_VISIBLE,
//		0, 0, height, height, m_Slider, NULL, NULL, NULL);
//
//	if (!m_Slidable)
//	{
//		std::cout << "Error during CreateWindowEx (Slidable). Error n.: " << GetLastError() << std::endl;
//	}
//	
//	ShowWindow(m_Slidable, SW_SHOW);
//
//	MSG msg;
//
//	while (::PeekMessage(&msg, m_Slidable, 0, 0, PM_REMOVE) > 0)
//	{
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//	}
//
//	SetParent(m_Slidable, m_Slider);
//}

void CMSlider::updateFlags(int& flag, int val)
{
	flag = val;
}

void CMSlider::updateProps(int& prop, int val)
{
	prop = val;
}

CMSlider::CMSlidable::CMSlidable()
{
}

CMSlider::CMSlidable::~CMSlidable()
{
}

void CMSlider::CMSlidable::setInstance()
{
	pSlidable = this;
}
