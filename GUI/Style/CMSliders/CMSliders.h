#pragma warning(suppress : 6387)
#pragma once
#include <Windows.h>
#include <iostream>
#include <CommCtrl.h>
#include "CMLayouts.h"
#include "CMItems.h"

//Slider properties defines:
#define CMS_UPDATERADIUS 1
#define CMS_NORADIUS 5
#define CMS_CIRCULARRADIUS 6

class CMSlider : public CMItem
{
public:
	CMSlider(CMItem parent, int flag = 0);
	~CMSlider();

	void addBoundsOffset(int wOffset = 10, int flag = CMW_OFFSETHV);
	void setPos(int xPos, int yPos);
	void setSize(int wWidth, int wHeight);
	void setRadius(int wRadius, int radFlags);
	int getRadius();
	void setGeometry(int xPos, int yPos, int wWidth, int wHeight);
	void setInstance();

	static HWND wParent;
	static bool isClicked;
	static HWND m_Slider;
	static POINT offset;

	class CMSlidable
	{
	public:
		CMSlidable();
		~CMSlidable();
		void setInstance();

		void createSlidable();
		static HWND m_Slidable;
	};

private:
	void updateFlags(int& flag, int val);
	void updateProps(int& prop, int val);
};