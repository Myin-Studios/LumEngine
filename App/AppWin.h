#pragma once
#include "Win.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"

class AppWin : public Win
{
public:
	AppWin();
	~AppWin();

	// Ereditato tramite Win
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;

	HWND getHWnd();
private:
	SwapChain* m_SC = nullptr;
};

