#pragma once
#include <Windows.h>
#include "CMItems.h"
#include <iostream>
#include <string>

class CMFrame : public CMItem
{
public:
	CMFrame(CMItem parent, int flag);
	~CMFrame();
	void setInstance();

	HWND m_Parent = HWND();
	HWND m_Frame = HWND();
	COLORREF color;

	void setColor(COLORREF col);
	void setColor(int r, int g, int b);

	static LRESULT CALLBACK s_FrameProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT FrameProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static BOOL CALLBACK FProc(HWND hWnd, LPARAM lParam);
private:

};