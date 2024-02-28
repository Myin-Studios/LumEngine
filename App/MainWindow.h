#pragma once
#include <Windows.h>
#include <iostream>
#include "AppWin.h"
#include <dwmapi.h>
#include "CMItems.h"

class MainWin : public CMItem
{
public:
	MainWin();
	~MainWin();

	bool init();

	//Release window.
	bool release();

	void broadcast();

	//Check if the window is running.
	bool isRunning();

	RECT getClientWindowRect();

	//onCreate event.
	virtual void onCreate();

	//onUpdate event.
	virtual void onUpdate();

	//onDestroy event.
	virtual void onDestroy();

	void setHWND(HWND hWnd) { CMItem::setHWND(hWnd); }
	HWND getHWnd();

	//AppWin* scene = new AppWin();

	HWND m_hWnd = NULL;
protected:
	
	bool m_isRunning = false;
};