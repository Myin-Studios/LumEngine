#pragma once

#include <iostream>
#include <Windows.h>
#include "../Engine/LogSystem/LogSystem.h"

using namespace std;

class CMItem
{
public:
	CMItem(int, int, int);
	~CMItem();

	HWND getHWND() const { return this->_itemHWND; }

	const int width = 0, height = 0, radius = 0;

private:
	HWND _itemHWND = NULL;
};

CMItem::CMItem(int _width, int _height, int _radius) : width(_width), height(_height), radius(_radius)
{
}

CMItem::~CMItem()
{
}