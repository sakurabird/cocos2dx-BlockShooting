#include "TopScene.h"
#include "Animations.h"
#include "Config.h"
#include "UserSettings.h"
#include "LevelSelectScene.h"
#include "SettingScene.h"
#include "HelpScene.h"
#include "GameOverScene.h"
#include "GHelper.h"
#include "SimpleAudioEngine.h"
#include "Utils.h"
#include "MyRenderer.h"

USING_NS_CC;
using namespace CocosDenshion;
CCSize g_visibleSize;

TopScene::TopScene()
{
	MyRenderer::showAd(1);	//astrON();
}

TopScene::~TopScene()
{
}

CCScene* TopScene::scene()
{
    CCScene * scene = NULL;
    do
    {
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        TopScene *layer = TopScene::create();
        CC_BREAK_IF(! layer);

        scene->addChild(layer);
    } while (0);

    return scene;
}

bool TopScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    if (!CCLayerGradient::initWithColor( ccc4(254, 255, 255, 255), ccc4(160, 216, 239, 255))) {
        return false;
    }

    g_visibleSize = CCDirector::sharedDirector()->getVisibleSize();

    // BGM再生
    if (UserSettings::getMusicSetting() &&
        !SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MP3_BG, true);

    // global settings
    UserSettings::loadLevelState();

    makeLabel();

    setKeypadEnabled(true);

    return  true;
}

void TopScene::makeLabel()
{
    CCLOG("g_visibleSize w:%f h:%f", g_visibleSize.width, g_visibleSize.height);
    //タイトル
    CCLabelBMFont* title = CCLabelBMFont::create("Block Shooting!", FONT_TITLE);
    title->setScale(2.0);
    title->setPosition(ccp(g_visibleSize.width / 2, g_visibleSize.height * 0.8));
    addChild(title);

    //Playボタン
    CCLabelBMFont* startLabel1 = CCLabelBMFont::create("Play", FONT_DISC1);
    startLabel1->setScale(1.7);
    CCMenuItemLabel* item1 = CCMenuItemLabel::create(startLabel1, this, menu_selector(TopScene::onTapStartButton));
    item1->runAction(Animations::topLabelAction());

    //Settingボタン
    CCLabelBMFont* startLabel2 = CCLabelBMFont::create("Setting", FONT_DISC1);
    startLabel2->setScale(1.5);
    CCMenuItemLabel* item2 = CCMenuItemLabel::create(startLabel2, this, menu_selector(TopScene::onTapSettingButton));

    //Helpボタン
    CCLabelBMFont* startLabel3 = CCLabelBMFont::create("Help", FONT_DISC1);
    startLabel3->setScale(1.5);
    CCMenuItemLabel* item3 = CCMenuItemLabel::create(startLabel3, this, menu_selector(TopScene::onTapHelpButton));

    //Quitボタン
    CCLabelBMFont* startLabel4 = CCLabelBMFont::create("Quit", FONT_DISC1);
    startLabel4->setScale(1.5);
    CCMenuItemLabel* item4 = CCMenuItemLabel::create(startLabel4, this, menu_selector(TopScene::menuCloseCallback));

    CCMenu* menu = CCMenu::create( item1, item2, item3, item4, NULL );
    menu->alignItemsVerticallyWithPadding(30.0);

    // 画面表示時のアクション
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    int i=0;
    CCNode* child;
    CCArray * pArray = menu->getChildren();
    CCObject* pObject = NULL;
    CCARRAY_FOREACH(pArray, pObject)
    {
        if(pObject == NULL)
            break;

        child = (CCNode*)pObject;

        CCPoint dstPoint = child->getPosition();
        int offset = (int) (s.width/2 + 50);
        if( i % 2 == 0)
            offset = -offset;

        child->setPosition( ccp( dstPoint.x + offset, dstPoint.y) );
        child->runAction(
                         CCEaseElasticOut::create(CCMoveBy::create(2, ccp(dstPoint.x - offset,0)), 0.35f)
                         );
        i++;
    }

    this->addChild( menu );

    //女の子のフレームアニメーション
    CCSprite *obj = CCSprite::create(PNG_MINI1_1);
    obj->setPosition(ccp(g_visibleSize.width  * 0.7, g_visibleSize.height * 0.2));
    obj->runAction(Animations::topMiniAnimation());
    this->addChild(obj);
}

void TopScene::onTapStartButton()
{
    CCScene* scene = (CCScene*)LevelSelectScene::create();
    CCTransitionSplitRows* tran = CCTransitionSplitRows::create(1, scene);
    CCDirector::sharedDirector()->pushScene(tran);
}

void TopScene::onTapSettingButton()
{
    CCScene* scene = (CCScene*)SettingScene::create();
    CCDirector::sharedDirector()->pushScene(scene);
}

void TopScene::onTapHelpButton()
{
    CCScene* scene = (CCScene*)HelpScene::create();
    CCDirector::sharedDirector()->pushScene(scene);
}

void TopScene::menuCloseCallback(CCObject* pSender)
{
    Utils::quit();
}

void TopScene::keyBackClicked()
{
    Utils::quit();
}

