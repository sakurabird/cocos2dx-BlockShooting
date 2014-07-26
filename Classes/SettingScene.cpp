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
#include "PopupLayer.h"

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

    // BGM再生
    if (UserSettings::getMusicSetting() &&
        !SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MP3_BG, true);

    makeLabels();

    makeBackButton();

    setKeypadEnabled(true);

    return  true;
}

void SettingScene::makeLabels()
{
    //タイトル
    CCLabelBMFont* title = CCLabelBMFont::create("Setting", FONT_BLUE);
    title->setScale(0.8);
    title->setPosition( ccp(g_visibleSize.width / 2, g_visibleSize.height * 0.8));
    this->addChild(title);

    //BackGround Music on/off
    CCLabelBMFont* disc1 = CCLabelBMFont::create("Music", FONT_WHITE);
    disc1->setScale(0.5);
    disc1->setAnchorPoint(CCPointZero);
    disc1->setPosition(ccp(g_visibleSize.width * 0.2, g_visibleSize.height * 0.65));
    this->addChild(disc1);

    CCString* fileName = NULL;

    if (UserSettings::getMusicSetting())
    {
        fileName = CCString::create(PNG_OFF);
    }else{
        fileName = CCString::create(PNG_ON);
    }

    CCMenuItemImage* button1 = CCMenuItemImage::create(fileName->getCString(), fileName->getCString(), this, menu_selector(SettingScene::onTapMusicOnOFFButton));
    button1->setScale(2);
    button1->setPosition(ccp(g_visibleSize.width * 0.75,
                             disc1->getPositionY() + disc1->getContentSize().height / 3));

    //Sound Effect on/off
    CCLabelBMFont* disc2 = CCLabelBMFont::create("Sound Effect", FONT_WHITE);
    disc2->setScale(0.5);
    disc2->setAnchorPoint(CCPointZero);
    disc2->setPosition(ccp(disc1->getPositionX(),
                           disc1->getPositionY()  - 100));
    this->addChild(disc2);

    if (UserSettings::getSESetting())
    {
        fileName = CCString::create(PNG_OFF);
    }else{
        fileName = CCString::create(PNG_ON);
    }
    CCMenuItemImage* button2 = CCMenuItemImage::create(fileName->getCString(), fileName->getCString(), this, menu_selector(SettingScene::onTapSEOnOFFButton));
    button2->setScale(2);
    button2->setPosition(ccp(g_visibleSize.width * 0.75,
                             disc2->getPositionY() + disc2->getContentSize().height / 3));


    //Clear User Data
    CCLabelBMFont* disc3 = CCLabelBMFont::create("Clear User Data", FONT_WHITE);
    disc3->setScale(0.5);
    disc3->setAnchorPoint(CCPointZero);
    // TODO
    disc3->setPosition(disc1->getPositionX(), disc2->getPositionY()  - 100);
    this->addChild(disc3);

    CCMenuItemImage* button3 = CCMenuItemImage::create(PNG_ON, PNG_ON, this, menu_selector(SettingScene::onTapClearUserData));
    button3->setScale(2);
    button3->setPosition(ccp(g_visibleSize.width * 0.75,
                             disc3->getPositionY() + disc3->getContentSize().height / 3));

    CCMenu* menu = CCMenu::create(button1, button2, button3, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
}


void SettingScene::makeBackButton()
{
    CCMenuItemImage *item = CCMenuItemImage::create(
                                                    PNG_BACK,
                                                    PNG_BACK,
                                                    this,
                                                    menu_selector(SettingScene::onTapBackButton));

    if (!item) return;
    item->setPosition(GHelper::convI720toCC(20, g_visibleSize.height * 0.1));
    CCMenu* menu = CCMenu::create(item, NULL);
    menu->setPosition(CCPointZero);
    if (!menu) return;
    this->addChild(menu, kZOrderTop, kTagBack);
}

void SettingScene::onTapMusicOnOFFButton()
{
    UserSettings::setMusicSetting(!UserSettings::getMusicSetting());
    if (UserSettings::getMusicSetting()){

        if (!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()) {
            SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MP3_BG, true);
        }
    }else{

        if (SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying()) {
            SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
        }
    }
    CCScene* scene = (CCScene*)SettingScene::create();
    CCDirector::sharedDirector()->replaceScene(scene);

}

void SettingScene::onTapSEOnOFFButton()
{
    UserSettings::setSESetting(!UserSettings::getSESetting());
    if (!UserSettings::getSESetting()){

        SimpleAudioEngine::sharedEngine()->stopAllEffects();
    }
    CCScene* scene = (CCScene*)SettingScene::create();
    CCDirector::sharedDirector()->replaceScene(scene);
}

void SettingScene::onTapBackButton()
{
    CCScene* scene = (CCScene*)TopScene::create();
    CCTransitionSplitRows* tran = CCTransitionSplitRows::create(1, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
}

void SettingScene::onTapClearUserData()
{
    // ポップアップを表示して確認
    PopupLayer* pl = PopupLayer::create(PNG_POPUP_BG);
    pl->setContentSize(CCSizeMake(g_visibleSize.width * 0.7, 400));
    pl->setTitle("Delete", 35);
    pl->setContentText("Are you sure, you want to delete user data and restore default setting?",
                       30, 60, 250);
    pl->setCallbackFunc(this, callfuncN_selector(SettingScene::popupCallback));
    pl->addButton(PNG_OK, PNG_OK, "", 0);
    pl->addButton(PNG_CANCEL, PNG_CANCEL, "", 1);
    this->addChild(pl);
}

void SettingScene::popupCallback(CCNode *sender)
{
    CCLOG("popupClickedCallback tag: %d", sender->getTag());
    if (sender->getTag() == 0) { //okが押された
        UserSettings::setMusicSetting(true);
        UserSettings::setSESetting(true);
        for(int i = 0; i < 16; i++){
            g_LevelState[0][i] = 0;
            g_LevelState[1][i] = 0;
        }
        UserSettings::saveLevelState();
        sender->removeFromParent();

        CCScene* scene = (CCScene*)SettingScene::create();
        CCDirector::sharedDirector()->replaceScene(scene);
    }
}

void SettingScene::keyBackClicked()
{
    CCScene* scene = (CCScene*)TopScene::create();
    CCTransitionSplitRows* tran = CCTransitionSplitRows::create(1, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
}

