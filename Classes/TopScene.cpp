#include "TopScene.h"
#include "HelloWorldScene.h"
#include "GHelper.h"

using namespace cocos2d;


TopScene::~TopScene()
{
}

TopScene::TopScene()
{
}

CCScene* TopScene::scene()
{
    CCScene * scene = NULL;
    do
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        TopScene *layer = TopScene::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

bool TopScene::init()
{

	bool bRet = false;
	do
	{
        CC_BREAK_IF(!CCLayerColor::initWithColor(ccc4(51, 120, 200, 255)));

        CCSize winSize = CCDirector::sharedDirector()->getWinSize();

        CCLabelTTF* startLabel = CCLabelTTF::create("Start!", "Arial", 40.0);
        startLabel->setColor(ccc3(255,192,203));
        CCMenuItemLabel* startItem = CCMenuItemLabel::create(startLabel, this, menu_selector(TopScene::tapStartButton));
        startItem->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.5));

        CCMenu* menu = CCMenu::create(startItem, NULL);
        menu->setPosition(CCPointZero);
        this->addChild(menu);

        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                                          "CloseNormal.png",
                                                          "CloseSelected.png",
                                                          this,
                                                          menu_selector(TopScene::menuCloseCallback));

//        CCSprite* closeNormal = CCSprite::createWithSpriteFrameName("CloseNormal.png");
//        CCSprite* closeSelected = CCSprite::createWithSpriteFrameName("CloseSelected.png");
//        CCMenuItemSprite *pCloseItem = CCMenuItemSprite::create(closeNormal,
//                                                                closeSelected,
//                                                                this,
//                                                                menu_selector(TopScene::menuCloseCallback));

        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

        pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2,
                                origin.y + pCloseItem->getContentSize().height/2));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);

        CC_BREAK_IF(! pMenu);
    
        this->addChild(pMenu, 1);

        CCLOG("visibleSize.width: %f, height: %f",visibleSize.width,visibleSize.height);
        CCLOG("origin.x: %f, origin.y: %f",origin.x,origin.y);

        bRet = true;
    } while (0);

    return bRet;
}

void TopScene::tapStartButton()
{
    CCScene* scene = (CCScene*)HelloWorld::create();
    CCDirector::sharedDirector()->replaceScene(scene);
}

void TopScene::menuCloseCallback(CCObject* pSender)
{
	// "close" menu item clicked
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
    CCDirector::sharedDirector()->end();
#endif
}
