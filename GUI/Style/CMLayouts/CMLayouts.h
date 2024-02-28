#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#include <iostream>
#include "CMItems.h"


//class CMBaseLayout
//{
//public:
//	CMBaseLayout(HWND parent, int xPos = 0, int yPos = 0, int wWidth = 300, int wHeight = 720);
//	~CMBaseLayout();
//
//	static HWND m_BLayout;
//	//int width = 0, height = 0, radius = 0, properties = CMW_NOPROPS, boundsOffset = 0, x = 0, y = 0, radiusFlags = CMW_NOPROPS;
//private:
//
//};

class CMLayoutH : public CMItem
{
public:
	CMLayoutH(HWND wParent, int flag);
	~CMLayoutH();

	void addItem(CMItem item, int row, int column, int flags);

	HWND m_HLayout;
	void setHWND() { CMItem::setHWND(m_HLayout); }
	void setInstance();
};