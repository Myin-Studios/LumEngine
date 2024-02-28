#include "CMButtons.h"

CMBaseButton* pBButton = nullptr;

CMBaseButton::CMBaseButton(CMItem parent, int flag) : CMItem(CMItem(width, height, radius, flag, boundsOffset, x, y, radiusFlags))
{
	setID();
	wchar_t* className = (wchar_t*)L"BaseButton";
	std::wstring classID = std::to_wstring(getID());

	std::wstring ws_className(className);
	ws_className += L"_" + classID;
	std::string str_className(ws_className.begin(), ws_className.end());

	std::cout << "Item class name: " << str_className << std::endl;

	WNDCLASSEX bwc = {};
	bwc.cbSize = sizeof(bwc);
	bwc.hInstance = NULL;
	bwc.lpszClassName = ws_className.c_str();
	bwc.cbClsExtra = NULL;
	bwc.cbWndExtra = NULL;
	bwc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	bwc.hCursor = LoadCursor(NULL, IDC_HAND);
	bwc.hIcon = NULL;
	bwc.hIconSm = NULL;
	bwc.lpszMenuName = L"";
	bwc.style = NULL;
	bwc.lpfnWndProc = s_ButtonProc;

	if (!RegisterClassEx(&bwc))
		std::cout << "ERROR DURING REGISTERCLASSEX (BaseButton). Error n.: " << GetLastError() << std::endl;

	if (!pBButton)
		pBButton = this;

	switch (flag)
	{
	case CMW_EXTENDTOBOUNDSH:
	{
		this->width = parent.width;
		this->x = 0;
		height = 30;
		break;
	}
	case CMW_EXTENDTOBOUNDSV:
	{
		this->height = parent.height;
		this->y = 0;
		width = 300;
		break;
	}
	case CMW_EXTENDTOBOUNDSHV:
	{
		this->width = parent.width;
		this->x = 0;
		this->height = parent.height;
		this->y = 0;
		break;
	}
	default:
	{
		break;
	}
	}

	m_BButton = CreateWindowEx(NULL, ws_className.c_str(), L"", WS_CHILD | WS_VISIBLE, x, y, width, height, parent.getHWND(), NULL, NULL, this);

	this->setHWND(m_BButton);

	if (!m_BButton)
		std::cout << "Error during CreateWindowEx (BaseButton). Error n.: " << GetLastError() << std::endl;
}

CMBaseButton::~CMBaseButton()
{
}

void CMBaseButton::setColor(COLORREF col)
{
	this->color = col;

	if (!this->color)
		std::cout << "Error during color set." << std::endl;
}

void CMBaseButton::setColor(int r, int g, int b)
{
	this->color = RGB(r, g, b);

	if (!this->color)
		std::cout << "Error during color set." << std::endl;
}

void CMBaseButton::setPressedColor(COLORREF col)
{
	this->pressedColor = col;

	if (!this->pressedColor)
		std::cout << "Error during pressed color set." << std::endl;
}

void CMBaseButton::setPressedColor(int r, int g, int b)
{
	this->pressedColor = RGB(r, g, b);

	if (!this->pressedColor)
		std::cout << "Error during pressed color set." << std::endl;
}

void CMBaseButton::setIcon(std::string path)
{
	iconPath = path;
	loadImg = true;
}


LRESULT CMBaseButton::s_ButtonProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE) {
		LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
		pBButton = (CMBaseButton*)lpcs->lpCreateParams;

		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pBButton);
	}
	else
		pBButton = (CMBaseButton*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (pBButton)
		return pBButton->ButtonProc(hWnd, msg, wParam, lParam);

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


LRESULT CMBaseButton::ButtonProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		std::cout << "BaseButton created." << std::endl;
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		HBRUSH col = CreateSolidBrush(pBButton->color);
		// All painting occurs here, between BeginPaint and EndPaint.

		if (loadImg)
		{
			loadIcon();
			StretchBlt(hdc, 0, 0, width, height, deviceContext, 0, 0, width, height, SB_NONE);
		}
		else FillRect(hdc, &ps.rcPaint, col);

		EndPaint(hWnd, &ps);

		break;
	}
	case WM_SIZE:
	{
		RECT r;
		RECT pr;

		GetClientRect(hWnd, &r);
		GetWindowRect(GetParent(hWnd), &pr);

		int cw, ch;
		cw = r.right - r.left;
		ch = r.bottom - r.top;

		int pw, ph;
		pw = pr.right - pr.left;
		ph = pr.bottom - pr.top;

		switch (this->properties)
		{
		case CMW_EXTENDTOBOUNDSH:
		{
			SetWindowPos(hWnd, NULL, 0, y, pw, height, SWP_FRAMECHANGED);
			break;
		}
		case CMW_EXTENDTOBOUNDSV:
		{
			SetWindowPos(hWnd, NULL, x, 0, width, ph, SWP_FRAMECHANGED);
			break;
		}
		case CMW_EXTENDTOBOUNDSHV:
		{
			SetWindowPos(hWnd, NULL, 0, 0, pw, ph, SWP_FRAMECHANGED);
			break;
		}
		case CM_ANCHORRIGHT:
		{
			SetWindowPos(hWnd, NULL, pw - width, y, width, height, SWP_FRAMECHANGED);
			std::cout << "right" << std::endl;
			break;
		}
		default:
			break;
		}

		if (getRadBool())
		{
			std::cout << "Setting radius..." << std::endl;

			if (radius > height)
			{
				HRGN hBigRgn = CreateRoundRectRgn(0, 0, width, height, height, height);

				SetWindowRgn(hWnd, hBigRgn, TRUE);

				DeleteObject(hBigRgn);
			}
			else if (radius <= height)
			{
				HRGN rrRgn = CreateRoundRectRgn(0, 0, width, height, radius, radius);

				SetWindowRgn(hWnd, rrRgn, TRUE);

				DeleteObject(rrRgn);
			}
		}

		break;
	}
	case WM_LBUTTONDOWN:
	{
		std::cout << "Button down" << std::endl;
		backupColor = pBButton->color;
		setColor(pBButton->pressedColor);
		//SendMessage(hWnd, WM_PAINT, wParam, lParam);
		RECT r;

		GetClientRect(hWnd, &r);

		InvalidateRect(hWnd, &r, TRUE);
		UpdateWindow(hWnd);

		break;
	}
	case WM_LBUTTONUP:
	{
		std::cout << "Button up" << std::endl;
		setColor(backupColor);
		//SendMessage(hWnd, WM_PAINT, wParam, lParam);
		RECT r;

		GetClientRect(hWnd, &r);

		InvalidateRect(hWnd, &r, TRUE);
		UpdateWindow(hWnd);

		break;
	}
	case WM_DESTROY:
	{
		std::cout << "BaseButton destroyed." << std::endl;
		::PostQuitMessage(0);
		break;
	}
	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void CMBaseButton::loadIcon()
{
	std::wstring path(iconPath.begin(), iconPath.end());

	std::cout << "Loading icon at: " << iconPath << std::endl;

	//if (!deviceContext)
	//{
	//	std::cout << "Device already loaded." << std::endl;
	//	return;
	//}

	m_Icon = (HBITMAP)LoadImageW(NULL, path.c_str(), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);

	if (!m_Icon)
	{
		std::cout << "Error during image load. Error n.: " << GetLastError() << std::endl;
		return;
	}
	else std::cout << "Image load at: " << iconPath << std::endl;

	deviceContext = CreateCompatibleDC(GetDC(getHWND()));
	oldBitmap = (HBITMAP)SelectObject(deviceContext, m_Icon);
}

void CMBaseButton::unloadIcon()
{
	if (loadImg)
		if (deviceContext)
		{
			SelectObject(deviceContext, oldBitmap);
			DeleteObject(m_Icon);
			DeleteObject(oldBitmap);
		}
}