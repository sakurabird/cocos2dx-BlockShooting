//
//  GirlImageScene.cpp
//  BrockShooting
//
//  Created by sakura on 2014/10/07.
//
//

#include "GirlImageScene.h"
#include "TopScene.h"
#include "Config.h"
#include "UserSettings.h"
#include "Utils.h"
#include "SimpleAudioEngine.h"
#include "MyRenderer.h"

USING_NS_CC;
using namespace CocosDenshion;

GirlImageScene::~GirlImageScene()
{
	MyRenderer::showAd(2);	//astrOFF();
}

GirlImageScene::GirlImageScene()
{
}

CCScene* GirlImageScene::scene()
{
	CCScene * scene = NULL;
	do
	{
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		GirlImageScene *layer = GirlImageScene::create();
		CC_BREAK_IF(! layer);

		scene->addChild(layer, 100, kTagLayer);
	} while (0);
	return scene;
}

bool GirlImageScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    // BGM再生
    if (UserSettings::getMusicSetting() &&
        !SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MP3_BG, true);

	Utils::setGirlBackground(this);
	Utils::setGirlFilter(this);

	Utils::createBackButton(this, menu_selector(GirlImageScene::onTapBackButton), 0.1, 0.95);

    setKeypadEnabled(true);

    return  true;
}

void GirlImageScene::onTapBackButton()
{
    CCScene* scene = (CCScene*)TopScene::create();
    CCTransitionSplitRows* tran = CCTransitionSplitRows::create(1, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
}

void GirlImageScene::keyBackClicked()
{
    CCScene* scene = (CCScene*)TopScene::create();
    CCTransitionSplitRows* tran = CCTransitionSplitRows::create(1, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
}

