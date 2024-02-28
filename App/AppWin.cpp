#include "AppWin.h"
#include <iostream>

AppWin::AppWin()
{
}

AppWin::~AppWin()
{
}

void AppWin::onCreate()
{
	Win::onCreate();
	GraphicsEngine::get()->init();
	m_SC = GraphicsEngine::get()->createSwapChain();

	RECT rect = this->getClientWindowRect();

	m_SC->init(m_hWnd, rect.right - rect.left, rect.bottom - rect.top);

}

void AppWin::onUpdate()
{
	Win::onUpdate();
	GraphicsEngine::get()->getImmDevC()->clearRenderTargetColor(m_SC, .1f, .1f, .1f, 1);

	RECT rect = this->getClientWindowRect();
	
	m_SC->present(false);
}

void AppWin::onDestroy()
{
	Win::onDestroy();

	m_SC->release();

	GraphicsEngine::get()->release();
}

HWND AppWin::getHWnd()
{
	return m_hWnd;
}
