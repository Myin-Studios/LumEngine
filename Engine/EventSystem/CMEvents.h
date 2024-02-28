#pragma once
#include "CMTypes.h"
#include <Windows.h>

//mouse events

static constexpr CMUInt CMMouseMove = 0x0001;

CMUInt CMMouseLButtonDown = GetKeyState(VK_LBUTTON) & 0x8000;
static constexpr CMUInt CMMouseRButtonDown = 0x0003;

static constexpr CMUInt CMMouseLButtonUp = 0x0004;
static constexpr CMUInt CMMouseRButtonUp = 0x0005;

static constexpr CMUInt CMMouseLButtonDoubleClick = 0x0006;
static constexpr CMUInt CMMouseRButtonDoubleClick = 0x0007;

CMUInt getKey(CMUInt key)
{
	if (key > 0 && key < 256)
		if (GetKeyState(key) & 0x8001)
			return key;
}

struct CMKeyboardEvent
{
	static CMUInt A;
	static CMUInt B;
	static CMUInt C;
	static CMUInt D;
	static CMUInt E;
	static CMUInt F;
	static CMUInt G;
	static CMUInt H;
	static CMUInt I;
	static CMUInt J;
	static CMUInt K;
	static CMUInt L;
	static CMUInt M;
	static CMUInt N;
	static CMUInt O;
	static CMUInt P;
	static CMUInt Q;
	static CMUInt R;
	static CMUInt S;
	static CMUInt T;
	static CMUInt U;
	static CMUInt V;
	static CMUInt W;
	static CMUInt X;
	static CMUInt Y;
	static CMUInt Z;
	
	static CMUInt Space;
	static CMUInt Esc;
	static CMUInt BackSpace;
	
	static CMUInt Num1;
	static CMUInt Num2;
	static CMUInt Num3;
	static CMUInt Num4;
	static CMUInt Num5;
	static CMUInt Num6;
	static CMUInt Num7;
	static CMUInt Num8;
	static CMUInt Num9;
	static CMUInt Num0;

	static CMUInt NumPad1;
	static CMUInt NumPad2;
	static CMUInt NumPad3;
	static CMUInt NumPad4;
	static CMUInt NumPad5;
	static CMUInt NumPad6;
	static CMUInt NumPad7;
	static CMUInt NumPad8;
	static CMUInt NumPad9;
	static CMUInt NumPad0;
};