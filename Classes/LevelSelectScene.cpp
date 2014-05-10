//
//  LevelSelectScene.cpp
//  BrockShooting
//
//  Created by sakura on 2014/05/10.
//
//

#include "LevelSelectScene.h"
#include "Config.h"
#include "UserSettings.h"
#include "GameScene.h"
#include "GHelper.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

LevelSelectScene::LevelSelectScene()
{
}

LevelSelectScene::~LevelSelectScene()
{
}

CCScene* LevelSelectScene::scene()
{
    CCScene * scene = NULL;
    do
    {
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        LevelSelectScene *layer = LevelSelectScene::create();
        CC_BREAK_IF(! layer);

        scene->addChild(layer);
    } while (0);

    return scene;
}

bool LevelSelectScene::init()
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

    makeBackButton();
    
    makeLabel();

    return  true;
}

void LevelSelectScene::makeLabel()
{
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

    //タイトル
    CCLabelBMFont* title = CCLabelBMFont::create("Select Level", FONT_RED);
    title->setScale(1.0);
    title->setPosition( ccp(visibleSize.width / 2, visibleSize.height * 0.8));
    addChild(title);

    //Easyボタン
    CCLabelBMFont* startLabel1 = CCLabelBMFont::create("EASY", FONT_GREEN, 30);
    startLabel1->setScale(0.7);
    CCMenuItemLabel* item1 = CCMenuItemLabel::create(startLabel1, this, menu_selector(LevelSelectScene::onTapEasyButton));

    //Normalボタン
    CCLabelBMFont* startLabel2 = CCLabelBMFont::create("NORMAL", FONT_YELLOW, 30);
    startLabel2->setScale(0.7);
    CCMenuItemLabel* item2 = CCMenuItemLabel::create(startLabel2, this, menu_selector(LevelSelectScene::onTapNormalButton));

    //Hardボタン
    CCLabelBMFont* startLabel3 = CCLabelBMFont::create("HARD", FONT_BLUE, 30);
    startLabel3->setScale(0.7);
    CCMenuItemLabel* item3 = CCMenuItemLabel::create(startLabel3, this, menu_selector(LevelSelectScene::onTapHardButton));

    CCMenu* menu = CCMenu::create( item1, item2, item3, NULL );
    menu->alignItemsVerticallyWithPadding(30.0);
    addChild( menu );
}

void LevelSelectScene::makeBackButton()
{
    CCMenuItemImage *item = CCMenuItemImage::create(
                                                    PNG_BACK,
                                                    PNG_BACK,
                                                    this,
                                                    menu_selector(LevelSelectScene::onTapBackButton));

    if (!item) return;
    item->setPosition(GHelper::convI720toCC(30, 20));
    CCMenu* menu = CCMenu::create(item, NULL);
    menu->setPosition(CCPointZero);
    if (!menu) return;
    this->addChild(menu, kZOrderTop, kTagBack);
}

void LevelSelectScene::onTapEasyButton()
{
    this->onLevelSelected(LEVEL_EASY);
}

void LevelSelectScene::onTapNormalButton()
{
    this->onLevelSelected(LEVEL_NORMAL);
}

void LevelSelectScene::onTapHardButton()
{
    this->onLevelSelected(LEVEL_HARD);
}

void LevelSelectScene::onLevelSelected(int level)
{
    UserSettings::setLevelSetting(level);
    CCScene* scene = (CCScene*)GameScene::create();
    CCTransitionSplitRows* tran = CCTransitionSplitRows::create(1, scene);
    CCDirector::sharedDirector()->pushScene(tran);
}

void LevelSelectScene::onTapBackButton()
{
    CCDirector::sharedDirector()->popScene();
}

void LevelSelectScene::keyBackClicked()
{
    CCDirector::sharedDirector()->popScene();
}

