//
//  SettingScene.cpp
//  BrockShooting
//
//  Created by sakura on 2014/04/27.
//
//

#include "SettingScene.h"
#include "TopScene.h"
#include "Config.h"
#include "GHelper.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

SettingScene::~SettingScene()
{
}

SettingScene::SettingScene()
{
}

CCScene* SettingScene::scene()
{
	CCScene * scene = NULL;
	do
	{
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		SettingScene *layer = SettingScene::create();
		CC_BREAK_IF(! layer);

		scene->addChild(layer);
	} while (0);
	return scene;
}

bool SettingScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    if (!CCLayerColor::initWithColor( ccc4(0,0,0,0) )) {
        return false;
    }
    // BGM再生
    if (!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MP3_BG, true);

    makeLabel();

    return  true;
}

void SettingScene::makeLabel()
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

    //タイトル
    CCLabelBMFont* title = CCLabelBMFont::create("Setting", FONT_ORANGE);
    title->setScale(1.0);
    title->setPosition( ccp(visibleSize.width / 2, visibleSize.height * 0.8));
    addChild(title);

    //BackGround Music on/off
    CCLabelBMFont* label1 = CCLabelBMFont::create("BackgroundMusic", FONT_WHITE, 30);
    label1->setScale(0.5);
    label1->setAlignment(kCCTextAlignmentLeft);
    label1->setPosition(ccp(visibleSize.width * 0.3, visibleSize.height * 0.75));
    addChild(label1);

    //Sound Effect on/off
    CCLabelBMFont* label2 = CCLabelBMFont::create("SoundEffect", FONT_WHITE, 30);
    label2->setScale(0.5);
    label2->setAlignment(kCCTextAlignmentLeft);
    label2->setPosition(ccp(visibleSize.width * 0.3, visibleSize.height * 0.5));
    addChild(label2);

    //Backボタン
    CCLabelBMFont* label3 = CCLabelBMFont::create("Back", FONT_VIOLET, 30);
    label3->setScale(0.5);
    label3->setAlignment(kCCTextAlignmentLeft);
//    CCMenuItemLabel* item = CCMenuItemLabel::create(label3, this, menu_selector(SettingScene::tapReturnButton));

//    CCMenu* menu = CCMenu::create(item);
    label3->setPosition(ccp(visibleSize.width * 0.3, visibleSize.height * 0.2));
    addChild(label3);
}

void SettingScene::tapReturnButton()
{
    CCScene* scene = (CCScene*)TopScene::create();
//    CCDirector::sharedDirector()->popScene();
    CCDirector::sharedDirector()->replaceScene(scene);
}

