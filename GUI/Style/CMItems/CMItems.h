#pragma once
#include <Windows.h>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <random>
#include <dwmapi.h>
#include <list>

//Properties defines:
//Extension flags
#define CMW_NOPROPS 0
#define CMW_EXTENDTOBOUNDSH 1
#define CMW_EXTENDTOBOUNDSV 2
#define CMW_EXTENDTOBOUNDSHV (CMW_EXTENDTOBOUNDSH | CMW_EXTENDTOBOUNDSV)
#define CM_RESPONSIVE 9

//Offset from bounds flags
#define CMW_OFFSETH 3
#define CMW_OFFSETV 4
#define CMW_OFFSETHV (CMW_OFFSETH | CMW_OFFSETV)

#define CM_SENDMESSAGE true

#define CM_ANCHORLEFT 5
#define CM_ANCHORRIGHT 6
#define CM_ANCHORTOP 7
#define CM_ANCHORBOTTOM 8

class CMProps
{
public:
	CMProps();
	~CMProps();

	static constexpr int noProps = 0;
	static constexpr int extendToBoundsH = 1;
	static constexpr int extendToBoundsV = 2;
	static constexpr int extendToBoundsHV = (extendToBoundsH | extendToBoundsV);
	static constexpr int responsive = 3;
	static constexpr int offsetH = 4;
	static constexpr int offsetV = 5;
	static constexpr int offsetHV = (offsetH | offsetV);
	static constexpr int anchorLeft = 6;
	static constexpr int anchorRight = 7;
	static constexpr int anchorTop = 8;
	static constexpr int anchorBottom = 9;

private:

};

class CMItem : public CMProps
{
public:
	CMItem(int wWidth = 300, int wHeight = 50, int wRadius = 100000, int wProperties = 0, int wBoundsOffset = 0, int xPos = 0, int yPos = 0, int wRadiusFlags = 0);
	~CMItem();

	int width, height, radius, properties, boundsOffset, x, y, radiusFlags, responsiveMulX = 1, responsiveMulY = 1, anchorProps, isCaption = false;
	static int realWidth, realHeigth;
	static bool m_msg;
	HWND m_ItemHWND = NULL;
	static std::list<CMItem>* items;

	void setCaption(bool flag);

	///<summary>
	/// This sets width and heigth. You should call this AFTER setRadius.
	///</summary>
	void setSize(int wWidth, int wHeight);

	///<summary>
	/// This sets x and y coordinates. You should call this AFTER setRadius.
	///</summary>
	void setPos(int xPos, int yPos);

	///<summary>
	/// This sets x and y coordinates and width and heigth. You should call this AFTER setRadius.
	///</summary>
	void setGeometry(int xPos, int yPos, int wWidth, int wHeight);
	
	///<summary>
	/// This sets an arbitrary radius. You should call this BEFORE setSize, setPos or setGeometry.
	///</summary>
	void setRadius(int rad);

	void setAnchors(int props);

	void setResponsiveVals(int xVal, int yVal);

	bool getRadBool();
	bool getSizeBool();
	bool getPosBool();
	void setHWND(HWND hWnd);
	HWND getHWND();
	CMItem getItem();
	void setID();
	int getID();
	static CMItem s_getItem();
	void setRealSize(int w, int h);

	void releaseUI();

	static BOOL CALLBACK ItemProc(HWND hWnd, LPARAM lParam);
private:
	bool sSet = false;
	bool pSet = false;
	bool rSet = false;
	int item_ID = 0;
};