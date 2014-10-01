//
//  LevelSelectScene.cpp
//  BrockShooting
//
//  Created by sakura on 2014/05/10.
//
//

#include "LevelSelectScene.h"
#include "LevelMenuItemSprite.h"
#include "TopScene.h"
#include "Config.h"
#include "UserSettings.h"
#include "GameScene.h"
#include "GHelper.h"
#include "SimpleAudioEngine.h"
#include "MyRenderer.h"

USING_NS_CC;
using namespace CocosDenshion;

const char *activeFile[16] = {
    PNG_STAGE1_A,PNG_STAGE2_A,PNG_STAGE3_A,PNG_STAGE4_A,
    PNG_STAGE5_A,PNG_STAGE6_A,PNG_STAGE7_A,PNG_STAGE8_A,
    PNG_STAGE9_A,PNG_STAGE10_A,PNG_STAGE11_A,PNG_STAGE12_A,
    PNG_STAGE13_A,PNG_STAGE14_A,PNG_STAGE15_A,PNG_STAGE16_A};

const char *inactiveFile[16] = {
    PNG_STAGE1_D,PNG_STAGE2_D,PNG_STAGE3_D,PNG_STAGE4_D,
    PNG_STAGE5_D,PNG_STAGE6_D,PNG_STAGE7_D,PNG_STAGE8_D,
    PNG_STAGE9_D,PNG_STAGE10_D,PNG_STAGE11_D,PNG_STAGE12_D,
    PNG_STAGE13_D,PNG_STAGE14_D,PNG_STAGE15_D,PNG_STAGE16_D};

LevelSelectScene::LevelSelectScene()
{
	MyRenderer::showAd(2);	//astrON();
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

    if (!CCLayerGradient::initWithColor( ccc4(254, 255, 255, 255), ccc4(160, 216, 239, 255))) {
        return false;
    }
    // BGM再生
    if (UserSettings::getMusicSetting() &&
        !SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MP3_BG, true);

    makeBackButton();
    
    makeLabel();

    setKeypadEnabled(true);

    return  true;
}

LevelMenuItemSprite* item_a[16];

void LevelSelectScene::makeLabel()
{
    //タイトル
    CCLabelBMFont* title = CCLabelBMFont::create("Touch Stage", FONT_TITLE);
    title->setScale(1.3);
    title->setPosition(g_visibleSize.width / 2, g_visibleSize.height - 80);
    addChild(title);

    CCLabelBMFont* t1 = CCLabelBMFont::create("Your Level", FONT_DISC1);
    t1->setScale(0.9);
    t1->setPosition(g_visibleSize.width / 3, title->getPositionY() - title->getContentSize().height / 2 - 30);
    addChild(t1);

    CCLabelBMFont* t2 = CCLabelBMFont::create("High Score", FONT_DISC1);
    t2->setScale(0.9);
    t2->setPosition(g_visibleSize.width / 1.45, t1->getPositionY());
    addChild(t2);

    //16個のラベルを作成
    float height = title->getPositionY() + title->getContentSize().height / 2;
    height = (height - 350) / 16;

    CCSprite* s = cocos2d::CCSprite::createWithSpriteFrameName(PNG_STAGE1_A);
    float scaleY = height / s->getContentSize().height;

    //レベル
    for(int i = 0; i < 16; i++){
        CCSprite *sp;
        if (g_LevelState[0][i] == 1) {
            sp = CCSprite::createWithSpriteFrameName(activeFile[i]);
        } else {
            sp = CCSprite::createWithSpriteFrameName(inactiveFile[i]);
        }
        LevelMenuItemSprite *item = LevelMenuItemSprite::create(sp, sp, this, menu_selector(LevelSelectScene::onTapLevel));
        if (!item) continue;
        item->setting(i, scaleY);
        item_a[i] = item;
    }

    //ハイスコア
    CCString* string;
    CCLabelBMFont* sc[16];
    float f = 0.21;
    for(int i = 0; i < 16; i++){
        string = CCString::createWithFormat("%d", g_LevelState[1][i]);
        sc[i] = CCLabelBMFont::create(string->getCString(), FONT_DISC1);
    }

    //配置する

    int itemHeight = item_a[0]->getContentSize().height / 2 + 15;
    for(int i = 0; i < 16; i++){
    	if (i == 0) {
    	    item_a[i]->setPosition(t1->getPositionX(), t1->getPositionY() - t1->getContentSize().height - 10);
            CCMenu* menu = CCMenu::create(item_a[i], NULL);
            menu->setPosition(CCPointZero);
            if (!menu) return;
            this->addChild(menu);
    	    sc[0]->setPosition(t2->getPositionX(), item_a[0]->getPositionY());
    	    this->addChild(sc[0]);
    	    continue;
		}
    	//ラベル
        item_a[i]->setPosition(item_a[i - 1]->getPositionX(), item_a[i - 1]->getPositionY() - itemHeight);
        CCMenu* menu = CCMenu::create(item_a[i], NULL);
        menu->setPosition(CCPointZero);
        if (!menu) return;
        this->addChild(menu);

        //ハイスコア
        sc[i]->setPosition(t2->getPositionX(), item_a[i]->getPositionY());
        this->addChild(sc[i]);
    }
}

//void LevelSelectScene::makeLabel()
//{
//    //タイトル
//    CCLabelBMFont* title = CCLabelBMFont::create("Touch Stage", FONT_TITLE);
//    title->setScale(1.3);
//    title->setPosition(GHelper::convI720toCC(g_visibleSize.width / 2, g_visibleSize.height * 0.1));
//    addChild(title);
//
//    //16個のラベルを作成
//    float height = title->getPositionY() + title->getContentSize().height / 2;
//    height = (height - 350) / 16;
//
//    CCSprite *s = CCSprite::create(PNG_STAGE1_A);
//    float scaleY = height / s->getContentSize().height;
//
//    for(int i = 0; i < 16; i++){
//        CCSprite *sp;
//        if (g_LevelState[0][i] == 1) {
//            sp = CCSprite::create(activeFile[i]);
//        } else {
//            sp = CCSprite::create(inactiveFile[i]);
//        }
//        LevelMenuItemSprite *item = LevelMenuItemSprite::create(sp, sp, this, menu_selector(LevelSelectScene::onTapLevel));
//        if (!item) continue;
//        item->setting(i, scaleY);
//        item_a[i] = item;
//    }
//
//    CCMenu* menu = CCMenu::create(
//                               item_a[0],item_a[1],item_a[2],item_a[3],
//                               item_a[4],item_a[5],item_a[6],item_a[7],
//                               item_a[8],item_a[9],item_a[10],item_a[11],
//                               item_a[12],item_a[13],item_a[14],item_a[15], NULL );
//    if (!menu) return;
//    menu->alignItemsVerticallyWithPadding(5.0);
//    menu->setPosition(GHelper::convI720toCC(g_visibleSize.width / 3, g_visibleSize.height * 0.55));
//    this->addChild( menu );
//
//    CCLabelBMFont* t1 = CCLabelBMFont::create("Your Level", FONT_DISC1);
//    t1->setScale(0.8);
//    t1->setPosition(GHelper::convI720toCC(g_visibleSize.width / 3, g_visibleSize.height * 0.15));
//    addChild(t1);
//
//    CCLabelBMFont* t2 = CCLabelBMFont::create("High Score", FONT_DISC1);
//    t2->setScale(0.8);
//    t2->setPosition(GHelper::convI720toCC(g_visibleSize.width / 1.6, g_visibleSize.height * 0.15));
//    addChild(t2);
//
//    CCString* string;
//    CCLabelBMFont* sc;
//    float f = 0.21;
//    for(int i = 0; i < 16; i++){
//        string = CCString::createWithFormat("%d", g_LevelState[1][i]);
//        sc = CCLabelBMFont::create(string->getCString(), FONT_DISC1);
//        sc->setScale(0.9);
//        sc->setPosition(GHelper::convI720toCC(t2->getPositionX() - 20, g_visibleSize.height * f));
//        f += 0.045;
//        addChild(sc);
//    }
//}

void LevelSelectScene::makeBackButton()
{
	CCSprite* button = CCSprite::createWithSpriteFrameName(PNG_BACK);
	CCMenuItemSprite *item = CCMenuItemSprite::create(
			button, button, this, menu_selector(LevelSelectScene::onTapBackButton));

    if (!item) return;
	item->setPosition(g_visibleSize.width * 0.09, g_visibleSize.height - 60);
    CCMenu* menu = CCMenu::create(item, NULL);
    menu->setPosition(CCPointZero);
    if (!menu) return;
    this->addChild(menu, kZOrderTop, kTagBack);
}

void LevelSelectScene::onTapLevel(CCObject *sender)
{
    LevelMenuItemSprite *sprite = (LevelMenuItemSprite *)sender;
    if (!sprite) return;
    int level = sprite->getTag();
    UserSettings::setSelectedLevel(level);
    CCScene* scene = (CCScene*)GameScene::create();
    CCTransitionSplitRows* tran = CCTransitionSplitRows::create(1, scene);
    CCDirector::sharedDirector()->popScene();
    CCDirector::sharedDirector()->pushScene(tran);
}

void LevelSelectScene::onTapBackButton()
{
    CCScene* scene = (CCScene*)TopScene::create();
    CCTransitionSplitRows* tran = CCTransitionSplitRows::create(1, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
}

void LevelSelectScene::keyBackClicked()
{
    CCScene* scene = (CCScene*)TopScene::create();
    CCTransitionSplitRows* tran = CCTransitionSplitRows::create(1, scene);
    CCDirector::sharedDirector()->replaceScene(tran);
}

