//
//  Utils.cpp
//  BrockShooting
//
//  Created by sakura on 2014/05/10.
//
//

#include "Utils.h"
#include "Config.h"
#include "UserSettings.h"
#include "GHelper.h"
#include "MyRenderer.h"

USING_NS_CC;

void Utils::setGirlBackground(CCLayer* target)
{
	CCSprite* background = CCSprite::createWithSpriteFrameName(PNG_BG);
	if (!background)
		return;

	float h = background->getContentSize().height;
	float sc = g_visibleSize.height / h;
	background->setScale(sc);

	background->setPosition(
			GHelper::convI720toCC(g_visibleSize.width / 2,
					g_visibleSize.height / 2));
	target->addChild(background, kZOrderBackground, kTagBackground);
}

void Utils::setGirlFilter(CCLayer* target)
{
	float cellWidth = g_visibleSize.width / 4;
	float cellHeight = g_visibleSize.height / 4;

	float x = 0;
	float y = g_visibleSize.height;

	CCDrawNode* node = CCDrawNode::create();
	target->addChild(node);

	ccColor4F color = ccc4f(0, 0, 0, 0.7);

	int index = 0;
	for (int i = 0; i < 4; i++) {
		x = 0;
		for (int j = 0; j < 4; j++) {
			if (g_LevelState[0][index] != 1) {
				CCPoint verts[] = { ccp(x, y), ccp(x + cellWidth, y),
						ccp(x + cellWidth, y - cellHeight),
						ccp(x , y - cellHeight) };
				node->drawPolygon(verts, 4, color, 0, color);
			}
			x += cellWidth;
			index++;
		}
		y -= cellHeight;
	}
}

void Utils::createBackButton(CCLayer* target, cocos2d::SEL_MenuHandler selector, float rate_x, float rate_y)
{
	CCSprite* button = CCSprite::createWithSpriteFrameName(PNG_BACK);

	CCMenuItemSprite *item = CCMenuItemSprite::create(
			button, button, target, selector);

    if (!item) return;
    item->setPosition(ccp(g_visibleSize.width * rate_x, g_visibleSize.height * rate_y));
    CCMenu* menu = CCMenu::create(item, NULL);
    menu->setPosition(CCPointZero);
    if (!menu) return;
    target->addChild(menu, kZOrderTop, kTagBack);
}

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
