#include "CMItems.h"

int CMItem::realWidth = 0;
int CMItem::realHeigth = 0;
bool CMItem::m_msg = false;

CMItem* pItem = nullptr;
std::list<CMItem>* CMItem::items = new std::list<CMItem>();

CMItem::CMItem(int wWidth, int wHeight, int wRadius, int wProperties, int wBoundsOffset, int xPos, int yPos, int wRadiusFlags)
{
	width = wWidth;
	height = wHeight;
	radius = wRadius;
	properties = wProperties;
	boundsOffset = wBoundsOffset;
	x = xPos;
	y = yPos;
	radiusFlags = wRadiusFlags;
	pItem = this;
}

CMItem::~CMItem()
{
}

void CMItem::setCaption(bool flag)
{
	isCaption = flag;
}

void CMItem::setSize(int wWidth, int wHeight)
{
	width = wWidth;
	height = wHeight;

	SetWindowPos(m_ItemHWND, NULL, 0, 0, wWidth, wHeight, SWP_ASYNCWINDOWPOS);
}

void CMItem::setRadius(int rad)
{
	radius = rad;
	rSet = true;
}

void CMItem::setHWND(HWND hWnd)
{
	std::cout << "Setting HWND..." << std::endl;

	m_ItemHWND = hWnd;

	if (hWnd)
		std::cout << "HWND successfully set." << std::endl << std::endl;
	else std::cout << "Error during HWND set." << std::endl << std::endl;
}

CMItem CMItem::getItem()
{
	return *this;
}

HWND CMItem::getHWND()
{
	return m_ItemHWND;
}

void CMItem::setRealSize(int w, int h)
{
	realWidth = w;
	realHeigth = h;
}

void CMItem::setPos(int xPos, int yPos)
{
	x = xPos;
	y = yPos;

	SetWindowPos(m_ItemHWND, NULL, x, y, width, height, SWP_ASYNCWINDOWPOS);
}

void CMItem::setGeometry(int xPos, int yPos, int wWidth, int wHeight)
{
	width = wWidth;
	height = wHeight;
	x = xPos;
	y = yPos;

	SetWindowPos(m_ItemHWND, NULL, x, y, width, height, SWP_ASYNCWINDOWPOS);
}

CMItem CMItem::s_getItem()
{
	return pItem->getItem();
}

int CMItem::getID()
{
	return item_ID;
}

void CMItem::setID()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> dist(0, 1000000);
	int random_number = dist(mt);

	item_ID = random_number;

	std::cout << "Item ID: " << item_ID << std::endl;
}

bool CMItem::getRadBool()
{
	return rSet;
}

void CMItem::releaseUI()
{
	UnregisterClass(TEXT("Frame"), NULL);
}

bool CMItem::getSizeBool()
{
	return sSet;
}

void CMItem::setAnchors(int props)
{
	anchorProps = props;
}

void CMItem::setResponsiveVals(int xVal, int yVal)
{
	responsiveMulX = xVal;
	responsiveMulY = yVal;
}

bool CMItem::getPosBool()
{
	return pSet;
}

BOOL CALLBACK CMItem::ItemProc(HWND hWnd, LPARAM lParam)
{
	for (std::list<CMItem>::iterator it = CMItem::items->begin(); it != CMItem::items->end(); ++it)
	{
		SendMessage(it->getHWND(), WM_SIZE, NULL, lParam);
	}

	return TRUE;
}

CMProps::CMProps()
{
}

CMProps::~CMProps()
{
}
