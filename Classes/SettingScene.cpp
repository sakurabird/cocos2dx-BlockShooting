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
#include "UserSettings.h"
#include "GHelper.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
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

		scene->addChild(layer, 100, kTagLayer);
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
    _visibleSize = CCDirector::sharedDirector()->getVisibleSize();

    // BGM再生
    if (UserSettings::getMusicSetting() &&
        !SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MP3_BG, true);

    makeMusicButton();

    makeSEButton();

    makeBackButton();

    setKeypadEnabled(true);

    return  true;
}

void SettingScene::makeMusicButton()
{
    CCNode* b = this->getChildByTag(kTagMusicButtton);
    if (b) {
        this->removeChild(b, true);
    }

    CCString* fileName = NULL;

    //BackGround Music on/off
    if (UserSettings::getMusicSetting())
    {
        fileName = CCString::create(PNG_MUSIC_OFF);
    }else{
        fileName = CCString::create(PNG_MUSIC_ON);
    }

    CCMenuItemImage* button1 = CCMenuItemImage::create(fileName->getCString(), fileName->getCString(), this, menu_selector(SettingScene::onTapMusicOnOFFButton));
    button1->setPosition(ccp(_visibleSize.width / 2, _visibleSize.height * 0.55));

    CCMenu* menu = CCMenu::create(button1, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, kTagMusicButtton);
}

void SettingScene::makeSEButton()
{
    CCNode* b = this->getChildByTag(kTagSEButtton);
    if (b) {
        this->removeChild(b, true);
    }

    CCString* fileName = NULL;

    //Sound Effect on/off
    if (UserSettings::getSESetting())
    {
        fileName = CCString::create(PNG_SE_OFF);
    }else{
        fileName = CCString::create(PNG_SE_ON);
    }

    CCMenuItemImage* button = CCMenuItemImage::create(fileName->getCString(), fileName->getCString(), this, menu_selector(SettingScene::onTapSEOnOFFButton));
    button->setPosition(ccp(_visibleSize.width / 2, _visibleSize.height * 0.45));
    CCMenu* menu = CCMenu::create(button, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, kTagSEButtton);
}

void SettingScene::makeBackButton()
{
    CCMenuItemImage *item = CCMenuItemImage::create(
                                                    PNG_BACK,
                                                    PNG_BACK,
                                                    this,
                                                    menu_selector(SettingScene::onTapBackButton));

    if (!item) return;
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    item->setPosition(GHelper::convI720toCC(20, visibleSize.height * 0.1));
    CCMenu* menu = CCMenu::create(item, NULL);
    menu->setPosition(CCPointZero);
    if (!menu) return;
    this->addChild(menu, kZOrderTop, kTagBack);
}

void SettingScene::onTapMusicOnOFFButton()
{
    UserSettings::setMusicSetting(!UserSettings::getMusicSetting());

    makeMusicButton();

    if (UserSettings::getMusicSetting()){

        if (!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()) {
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MP3_BG, true);
        }
    }else{

        if (SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()) {
            SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
        }
    }
}

void SettingScene::onTapSEOnOFFButton()
{
    UserSettings::setSESetting(!UserSettings::getSESetting());

    makeSEButton();


    if (!UserSettings::getSESetting()){

        SimpleAudioEngine::sharedEngine()->stopAllEffects();
    }
}

void SettingScene::onTapBackButton()
{
    CCDirector::sharedDirector()->popScene();
}

void SettingScene::keyBackClicked()
{
    CCDirector::sharedDirector()->popScene();
}

