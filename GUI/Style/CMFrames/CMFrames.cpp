#include "CMFrames.h"

CMFrame* pFrame = nullptr;

LRESULT CALLBACK CMFrame::s_FrameProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE) {
		LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
		pFrame = (CMFrame*)lpcs->lpCreateParams;
		
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pFrame);
	}
	else
		pFrame = (CMFrame*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	
	if (pFrame)
		return pFrame->FrameProc(hWnd, msg, wParam, lParam);
	
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CMFrame::FrameProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		std::cout << "Frame created." << std::endl;

		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		
		HBRUSH col = CreateSolidBrush(pFrame->color);
		// All painting occurs here, between BeginPaint and EndPaint.

		FillRect(hdc, &ps.rcPaint, col);

		EndPaint(hWnd, &ps);
		//InvalidateRect(pFrame->m_Frame, &ps.rcPaint, FALSE);

		break;
	}
	case WM_SIZE:
	{

		RECT r;
		RECT pr;

		GetClientRect(hWnd, &r);
		GetClientRect(GetParent(hWnd), &pr);

		int cw, ch;
		cw = r.right - r.left;
		ch = r.bottom - r.top;

		int pw, ph;
		pw = pr.right - pr.left;
		ph = pr.bottom - pr.top;

		if (properties == extendToBoundsV)
		{
			if (anchorProps == anchorRight)
			{
				if (responsiveMulX > 0)
				{
					if (x - width < pw / responsiveMulX)
					{
						if (responsiveMulX > 0)
							width = pw / responsiveMulX;
					}
					else
					{
						if (responsiveMulX > 0)
							width = pw / responsiveMulX;
					}
				}

				x = pw - width;
			}
			else if (anchorProps == anchorLeft)
			{
				if (responsiveMulX > 0)
				{
					if (x + width < pw / responsiveMulX)
					{
						if (responsiveMulX > 0)
							width = pw / responsiveMulX;
					}
					else
					{
						if (responsiveMulX > 0)
							width = pw / responsiveMulX;
					}
				}

				x = 0;
			}

			height = ph;

			SetWindowPos(hWnd, NULL, x, y, width, height, SWP_FRAMECHANGED);
		}
		else if (properties == extendToBoundsH)
		{
			if (anchorProps == anchorTop)
			{
				if (responsiveMulY > 0)
				{
					if (y - height < ph / responsiveMulY)
					{
						if (responsiveMulY > 0)
						{
							height = ph / responsiveMulY;
						}
					}
					else
					{
						if (responsiveMulY > 0)
						{
							height = ph / responsiveMulY;
						}
					}
				}

				y = 0;
			}
			else if (anchorProps == anchorBottom)
			{
				if (responsiveMulY > 0)
				{
					if (y + height < ph / responsiveMulY)
					{
						if (responsiveMulY > 0)
						{
							height = ph / responsiveMulY;
						}
					}
					else
					{
						if (responsiveMulY > 0)
						{
							height = ph / responsiveMulY;
						}
					}
				}

				y = ph - height;
			}

			width = pw;

			SetWindowPos(hWnd, NULL, x, y, width, height, SWP_FRAMECHANGED);
		}
		else SetWindowPos(hWnd, NULL, x, y, width, height, SWP_FRAMECHANGED);

		//else
		//{
		//	if (properties == extendToBoundsV)
		//	{
		//		if (responsiveMulX > 0)
		//			width = pw / responsiveMulX;
		//		x = pw - width;
		//		height = ph;
		//		y = 0;
		//	}
		//	else if (properties == extendToBoundsH)
		//	{
		//		width = pw;
		//		x = 0;
		//	}
		//	
		//
		//	if (properties == responsive)
		//	{
		//
		//	}
		//}
		//switch (properties)
		//{
		//case responsive + extendToBoundsV:
		//{
		//	break;
		//}
		//case responsive + extendToBoundsV:
		//{
		//	SetWindowPos(hWnd, NULL, x, y, pw - x, height, SWP_FRAMECHANGED);
		//	break;
		//}
		//case CMW_EXTENDTOBOUNDSV:
		//{
		//	std::cout << "responsive" << std::endl;
		//	SetWindowPos(hWnd, NULL, x, y, width, ph - y, SWP_FRAMECHANGED);
		//	break;
		//}
		//case CMW_EXTENDTOBOUNDSHV:
		//{
		//	SetWindowPos(hWnd, NULL, x, y, pw - x, ph - y, SWP_FRAMECHANGED);
		//	break;
		//}
		//default:
		//	break;
		//}

		break;
	}
	case WM_DESTROY:
	{
		std::cout << "Frame destroyed." << std::endl;
		::PostQuitMessage(0);
		break;
	}
	default:
	{
		return ::DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}
	}
}

void CMFrame::setColor(COLORREF col)
{
	this->color = col;

	if (!this->color)
		std::cout << "Error during color set." << std::endl;
}

void CMFrame::setColor(int r, int g, int b)
{
	this->color = RGB(r, g, b);

	if (!this->color)
		std::cout << "Error during color set." << std::endl;
}

CMFrame::CMFrame(CMItem parent, int flag) : CMItem(width, height, radius, flag, boundsOffset, x, y, radiusFlags)
{
	setID();
	wchar_t* className = (wchar_t*)L"Frame";
	std::wstring classID = std::to_wstring(getID());

	std::wstring ws_className(className);
	ws_className += L"_" + classID;
	std::string str_className(ws_className.begin(), ws_className.end());

	//wcscat_s((&className), sizeof(className), cID_wcs);

	std::cout << "Item class name: " << str_className << std::endl;

	WNDCLASSEX fwc = {};
	fwc.cbSize = sizeof(fwc);
	fwc.hInstance = NULL;
	fwc.lpszClassName = ws_className.c_str();
	fwc.cbClsExtra = NULL;
	fwc.cbWndExtra = NULL;
	fwc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	fwc.hCursor = LoadCursor(NULL, IDC_ARROW);
	fwc.hIcon = NULL;
	fwc.hIconSm = NULL;
	fwc.lpszMenuName = L"";
	fwc.style = NULL;
	fwc.lpfnWndProc = s_FrameProc;

	if (!RegisterClassEx(&fwc))
		std::cout << "ERROR DURING REGISTERCLASSEX (Frame). Error n.: " << GetLastError() << std::endl;

	if (!pFrame)
		pFrame = this;

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

	m_Frame = CreateWindowEx(NULL, ws_className.c_str(), L"", WS_CHILD | WS_VISIBLE, x, y, width, height, parent.getHWND(), NULL, NULL, this);

	this->setHWND(m_Frame);

	if (!m_Frame)
		std::cout << "Error during CreateWindowEx (Frame). Error n.: " << GetLastError() << std::endl;

	SetParent(m_Frame, parent.getHWND());

	ShowWindow(m_Frame, SW_SHOW);

	CMItem::items->push_back(this->getItem());
}

CMFrame::~CMFrame()
{
}

void CMFrame::setInstance()
{
	pFrame = this;
}
