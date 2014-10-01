//
//  Utils.cpp
//  BrockShooting
//
//  Created by sakura on 2014/05/10.
//
//

#include "Utils.h"
#include "UserSettings.h"
#include "MyRenderer.h"

USING_NS_CC;

void Utils::quit()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");

#else
    UserSettings::saveLevelState();
	MyRenderer::end();
//    CCDirector::sharedDirector()->end();
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    exit(0);
//#endif
#endif
}
