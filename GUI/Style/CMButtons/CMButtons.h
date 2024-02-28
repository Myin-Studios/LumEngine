#pragma once
#include "CMItems.h"
#include <Windows.h>
#include <string>
#include <iostream>

class CMBaseButton : public CMItem
{
public:
	CMBaseButton(CMItem parent, int flag);
	~CMBaseButton();
	void setColor(COLORREF col);
	void setColor(int r, int g, int b);
	void setPressedColor(COLORREF col);
	void setPressedColor(int r, int g, int b);
	void setIcon(std::string path);

	HWND m_BButton = HWND();
	COLORREF color;
	COLORREF pressedColor;
	HBITMAP m_Icon = NULL;

	static LRESULT CALLBACK s_ButtonProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK ButtonProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	void loadIcon();
	void unloadIcon();

	bool loadImg = false;
	COLORREF backupColor;
	std::string iconPath;
	HDC deviceContext;
	HBITMAP oldBitmap;
};