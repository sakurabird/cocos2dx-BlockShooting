//
//  HelpScene.cpp
//  BrockShooting
//
//  Created by sakura on 2014/05/15.
//
//

#include "HelpScene.h"
#include "TopScene.h"
#include "Config.h"
#include "UserSettings.h"
#include "GHelper.h"
#include "SimpleAudioEngine.h"
#include "MyRenderer.h"

USING_NS_CC;
using namespace CocosDenshion;

HelpScene::~HelpScene()
{
	MyRenderer::showAd(1);	//astrON();
}

HelpScene::HelpScene()
{
}

CCScene* HelpScene::scene()
{
	CCScene * scene = NULL;
	do
	{
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		HelpScene *layer = HelpScene::create();
		CC_BREAK_IF(! layer);

		scene->addChild(layer, 100, kTagLayer);
	} while (0);
	return scene;
}

bool HelpScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    if (!CCLayerGradient::initWithColor( ccc4(254, 255, 255, 255), ccc4(160, 216, 239, 255))) {
        return false;
    }

    // BGM再生
    if (UserSettings::getMusicSetting() &&
        !SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MP3_BG, true);

    makeLabels();

    makeBackButton();

    setKeypadEnabled(true);

    return  true;
}

void HelpScene::makeLabels()
{
    //Helpタイトル
    CCLabelBMFont* title = CCLabelBMFont::create("Help", FONT_TITLE);
    title->setScale(1.5);
    title->setPosition( GHelper::convI720toCC(g_visibleSize.width / 2, g_visibleSize.height * 0.2));
    this->addChild(title);

    //item説明タイトル
    CCLabelBMFont* title2 = CCLabelBMFont::create("Bonus Items", FONT_DISC1);
    title2->setScale(1.1);
    title2->setPosition( GHelper::convI720toCC(g_visibleSize.width / 2, g_visibleSize.height * 0.25));
    this->addChild(title2);

    float itemScale = 0.2;
    //青
    CCSprite* image1 = CCSprite::create(PNG_P_BLUE);
    image1->setScale(itemScale);
    image1->setAnchorPoint(CCPointZero);
    image1->setPosition( GHelper::convI720toCC(g_visibleSize.width / 4.0, g_visibleSize.height * 0.35));
    this->addChild(image1);

    CCLabelBMFont* disc1 = CCLabelBMFont::create("Quick Ball", FONT_DISC1);
    disc1->setScale(1.0);
    disc1->setAnchorPoint(CCPointZero);
    disc1->setPosition( ccp(image1->getPositionX() + image1->getContentSize().width * itemScale + 30,
                            image1->getPositionY() - image1->getContentSize().height * itemScale / 2.5));
    this->addChild(disc1);

    //緑
    CCSprite* image2 = CCSprite::create(PNG_P_GREEN);
    image2->setScale(itemScale);
    image2->setAnchorPoint(CCPointZero);
    image2->setPosition( GHelper::convI720toCC(g_visibleSize.width / 4.0, g_visibleSize.height * 0.4));
    this->addChild(image2);

    CCLabelBMFont* disc2 = CCLabelBMFont::create("Long Bar", FONT_DISC1);
    disc2->setScale(1.0);
    disc2->setAnchorPoint(CCPointZero);
    disc2->setPosition( ccp(disc1->getPositionX(),
                            image2->getPositionY() - image2->getContentSize().height * itemScale / 2.5));
    this->addChild(disc2);

    //赤
    CCSprite* image3 = CCSprite::create(PNG_P_RED);
    image3->setScale(itemScale);
    image3->setAnchorPoint(CCPointZero);
    image3->setPosition( GHelper::convI720toCC(g_visibleSize.width / 4.0, g_visibleSize.height * 0.45));
    this->addChild(image3);

    CCLabelBMFont* disc3 = CCLabelBMFont::create("Multiple Ball", FONT_DISC1);
    disc3->setScale(1.0);
    disc3->setAnchorPoint(CCPointZero);
    disc3->setPosition( ccp(disc1->getPositionX(),
                            image3->getPositionY() - image3->getContentSize().height * itemScale / 2.5));
    this->addChild(disc3);

    //紫
    CCSprite* image4 = CCSprite::create(PNG_P_VIOLET);
    image4->setScale(itemScale);
    image4->setAnchorPoint(CCPointZero);
    image4->setPosition( GHelper::convI720toCC(g_visibleSize.width / 4.0, g_visibleSize.height * 0.5));
    this->addChild(image4);

    CCLabelBMFont* disc4 = CCLabelBMFont::create("Bonus Score", FONT_DISC1);
    disc4->setScale(1.0);
    disc4->setAnchorPoint(CCPointZero);
    disc4->setPosition( ccp(disc1->getPositionX(),
                            image4->getPositionY() - image4->getContentSize().height * itemScale / 2.5));
    this->addChild(disc4);

    //黃
    CCSprite* image5 = CCSprite::create(PNG_P_YELLOW);
    image5->setScale(itemScale);
    image5->setAnchorPoint(CCPointZero);
    image5->setPosition( GHelper::convI720toCC(g_visibleSize.width / 4.0, g_visibleSize.height * 0.55));
    this->addChild(image5);

    CCLabelBMFont* disc5 = CCLabelBMFont::create("Add Ramain Ball", FONT_DISC1);
    disc5->setScale(1.0);
    disc5->setAnchorPoint(CCPointZero);
    disc5->setPosition( ccp(disc1->getPositionX(),
                            image5->getPositionY() - image5->getContentSize().height * itemScale / 2.5));
    this->addChild(disc5);

    //Thanks to Terasurware
    CCLabelBMFont* title3 = CCLabelBMFont::create("Thanks to...", FONT_DISC1);
    title3->setScale(1.2);
    title3->setPosition( GHelper::convI720toCC(g_visibleSize.width / 2, g_visibleSize.height * 0.7));
    this->addChild(title3);

    CCLabelBMFont* disc6 = CCLabelBMFont::create("Terasurware", FONT_DISC1);
    disc6->setScale(1.2);
    disc6->setPosition( GHelper::convI720toCC(g_visibleSize.width / 2,  g_visibleSize.height * 0.8));
    this->addChild(disc6);
}

void HelpScene::makeBackButton()
{
    CCMenuItemImage *item = CCMenuItemImage::create(
                                                    PNG_BACK,
                                                    PNG_BACK,
                                                    this,
                                                    menu_selector(HelpScene::onTapBackButton));

    if (!item) return;
    item->setPosition(GHelper::convI720toCC(g_visibleSize.width * 0.08, g_visibleSize.height * 0.1));
    CCMenu* menu = CCMenu::create(item, NULL);
    menu->setPosition(CCPointZero);
    if (!menu) return;
    this->addChild(menu, kZOrderTop, kTagBack);
}


void HelpScene::onTapBackButton()
{
    CCScene* scene = (CCScene*)TopScene::create();
    CCTransitionSplitRows* tran = CCTransitionSplitRows::create(1, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
}

void HelpScene::keyBackClicked()
{
    CCScene* scene = (CCScene*)TopScene::create();
    CCTransitionSplitRows* tran = CCTransitionSplitRows::create(1, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
}

