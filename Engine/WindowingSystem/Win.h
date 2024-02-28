#pragma once
#include <Windows.h>
#include <uxtheme.h>
#include "CMItems.h"

class Win : public CMItem
{
public:
	Win();
	
	//Init the window.
	void init(HWND parent, int xPos, int yPos, int wWidth, int wHeight);
	
	//Release window.
	bool release();

	bool broadcast();

	//Check if the window is running.
	bool isRunning();

	RECT getClientWindowRect();
	void setHWND(HWND hWnd) { CMItem::setHWND(hWnd); };

	//onCreate event.
	virtual void onCreate();

	//onUpdate event.
	virtual void onUpdate();
	
	//onDestroy event.
	virtual void onDestroy();

	~Win();

	HWND m_hWnd = NULL;

protected:
	bool m_isRunning = false;
};

