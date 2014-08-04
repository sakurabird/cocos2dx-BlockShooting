//
//  GameClearPopup.cpp
//  BrockShooting
//
//  Created by sakura on 2014/07/24.
//
//

#include "GameClearPopup.h"
#include "Config.h"
#include "GHelper.h"
#include "TopScene.h"
#include "GameScene.h"
#include "Utils.h"
#include "UserSettings.h"
#include "Animations.h"

USING_NS_CC;

GameClearPopup::GameClearPopup()
{
}

GameClearPopup::~GameClearPopup(){
}

bool GameClearPopup::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    this->setTouchPriority(-128);
    this->setTouchEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);

    makeLabels();

    setKeypadEnabled(true);

    return  true;
}

void GameClearPopup::makeLabels()
{
    //背景
    CCSprite* frame = CCSprite::create(PNG_POPUP_BG2);
    frame->setScale(2.5f);
    frame->setPosition(GHelper::convI720toCC(g_visibleSize.width / 2, g_visibleSize.height / 2));
    this->addChild(frame);

    CCLOG("frame y:%f", frame->getPositionY());

    //タイトル
    CCLabelBMFont* title = CCLabelBMFont::create("You Win!", FONT_YELLOW);
    title->setScale(0.6);
    title->setPosition( GHelper::convI720toCC(g_visibleSize.width * 0.5, g_visibleSize.height * 0.3));
    this->addChild(title);

    //女の子
    CCSprite *obj = CCSprite::create(PNG_MINI2_1);
    obj->setPosition(GHelper::convI720toCC(g_visibleSize.width * 0.5, g_visibleSize.height * 0.35));
    obj->runAction(Animations::winMiniAnimation());
    this->addChild(obj);

    //Next Stage?
    CCLabelBMFont* disc1 = CCLabelBMFont::create("Next Stage?", FONT_GREEN);
    disc1->setScale(0.5);
    CCMenuItemLabel* item1 = CCMenuItemLabel::create(disc1, this, menu_selector(GameClearPopup::onTapNextStage));

    //Again?
    CCLabelBMFont* disc2 = CCLabelBMFont::create("Again?", FONT_WHITE);
    disc2->setScale(0.5);
    CCMenuItemLabel* item2 = CCMenuItemLabel::create(disc2, this, menu_selector(GameClearPopup::onTapAgain));

    //Menu
    CCLabelBMFont* disc3 = CCLabelBMFont::create("Menu", FONT_WHITE);
    disc3->setScale(0.5);
    CCMenuItemLabel* item3 = CCMenuItemLabel::create(disc3, this, menu_selector(GameClearPopup::onTapMenu));

    //Quit
    CCLabelBMFont* disc4 = CCLabelBMFont::create("Quit", FONT_WHITE);
    disc4->setScale(0.5);
    CCMenuItemLabel* item4 = CCMenuItemLabel::create(disc4, this, menu_selector(GameClearPopup::onTapQuit));

    CCMenu* menu = CCMenu::create( item1, item2, item3, item4, NULL );
    menu->setTouchPriority(-128);
    menu->alignItemsVerticallyWithPadding(1.0);
    menu->setPosition(ccp(g_visibleSize.width / 1.5, g_visibleSize.height / 2 - 50));
    this->addChild(menu);
}

void GameClearPopup::onTapNextStage(){

    int level = UserSettings::getSelectedLevel();
    if (level >= 16) {
        level = 16;
    }else{
        level++;
    }
    UserSettings::setSelectedLevel(level);
    
    CCScene* scene = (CCScene*)GameScene::create();
    CCTransitionSplitRows* tran = CCTransitionSplitRows::create(1, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
    this->removeFromParentAndCleanup(true);
}

void GameClearPopup::onTapAgain(){
    CCScene* scene = (CCScene*)GameScene::create();
    CCTransitionSplitRows* tran = CCTransitionSplitRows::create(1, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
    this->removeFromParentAndCleanup(true);
}

void GameClearPopup::onTapMenu()
{
    CCScene* scene = (CCScene*)TopScene::create();
    CCTransitionSplitRows* tran = CCTransitionSplitRows::create(1, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
    this->removeFromParentAndCleanup(true);
}

void GameClearPopup::onTapQuit()
{
    Utils::quit();
    this->removeFromParentAndCleanup(true);
}

void GameClearPopup::keyBackClicked()
{
    this->removeFromParent();
}

bool GameClearPopup::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent){
    return true;
}
