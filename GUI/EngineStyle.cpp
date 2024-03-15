//#include "EngineStyle.h"
//#include <iostream>
//#include "CMSliders.h"
//
//Style::Style()
//{
//}
//
//Style::~Style()
//{
//}
//
//void Style::setStyle(CMItem appItem)
//{
//	CMScene* mainScene = new CMScene(appItem);
//
//	CMFrame* upperFrame = new CMFrame(appItem, CMProps::extendToBoundsH);
//	upperFrame->setColor(RGB(0, 0, 0));
//	upperFrame->setGeometry(0, 0, appItem.width, 30);
//	upperFrame->setCaption(true);
//	upperFrame->setResponsiveVals(0, 13);
//	upperFrame->setAnchors(CMProps::anchorTop);
//	
//	CMFrame* ufSeparator = new CMFrame(upperFrame->getItem(), CMProps::extendToBoundsH);
//	ufSeparator->setGeometry(0, upperFrame->height - 3, upperFrame->width, 3);
//	ufSeparator->setColor(0, 159, 255);
//	ufSeparator->setResponsiveVals(0, 15);
//	ufSeparator->setAnchors(CMProps::anchorBottom);
//
//	CMLayoutH* lh = new CMLayoutH(appItem.getHWND(), CMW_NOPROPS);
//	lh->addItem(upperFrame->getItem(), 2, 2, CMW_NOPROPS);
//
//	//CMFrame* contentBrowserFrame = new CMFrame(appItem, CMW_EXTENDTOBOUNDSV);
//	//contentBrowserFrame->setGeometry(0, upperFrame->height, 300, appItem.height - upperFrame->height);
//	//contentBrowserFrame->setColor(0, 0, 0);
//	//contentBrowserFrame->setResponsiveVals(5, 0);
//	//contentBrowserFrame->setAnchors(CMProps::anchorLeft);
//	//
//	//CMFrame* sceneContentsFrame = new CMFrame(appItem, CMProps::extendToBoundsV);
//	//sceneContentsFrame->setGeometry(appItem.width - 300, upperFrame->height, 300, appItem.height - upperFrame->height);
//	//sceneContentsFrame->setColor(0, 0, 0);
//	//sceneContentsFrame->setResponsiveVals(5, 0);
//	//sceneContentsFrame->setAnchors(CMProps::anchorRight);
//}
