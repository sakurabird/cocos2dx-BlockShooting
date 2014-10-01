
#include "GameOverScene.h"
#include "TopScene.h"
#include "Config.h"
#include "UserSettings.h"
#include "SimpleAudioEngine.h"
#include "Utils.h"
#include "Animations.h"

USING_NS_CC;
using namespace CocosDenshion;

GameOverScene::GameOverScene()
{
}

GameOverScene::~GameOverScene()
{
}

CCScene* GameOverScene::scene()
{
    CCScene * scene = NULL;
    do
    {
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        GameOverScene *layer = GameOverScene::create();
        CC_BREAK_IF(! layer);

        scene->addChild(layer);
    } while (0);

    return scene;
}


bool GameOverScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    if (!CCLayerGradient::initWithColor( ccc4(47, 47, 47, 255), ccc4(0, 0, 0, 255) )) {
        return false;
    }
    // BGM再生
    if (UserSettings::getMusicSetting() &&
        !SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MP3_BG, true);

    makeLabel();

    setKeypadEnabled(true);

    return  true;
}


void GameOverScene::makeLabel()
{
    //タイトル
    CCLabelBMFont* title = CCLabelBMFont::create("GAME OVER", FONT_BIG2);
    title->setScale(1.2);
    title->setPosition( ccp(g_visibleSize.width / 2, g_visibleSize.height * 0.7));
    this->addChild(title);

    //レベル
    CCString* string = CCString::createWithFormat("Level %d", UserSettings::getSelectedLevel() + 1);
    CCLabelBMFont* lebelLabel = CCLabelBMFont::create(string->getCString(), FONT_BLUE);
    lebelLabel->setScale(0.6);
    lebelLabel->setAnchorPoint(ccp(0, 0));
    lebelLabel->setPosition( ccp(g_visibleSize.width / 4, g_visibleSize.height * 0.55));
    this->addChild(lebelLabel);


    //スコア
    string = CCString::createWithFormat("Score %d", UserSettings::getScore());
    CCLabelBMFont* scoreLabel1 = CCLabelBMFont::create(string->getCString(), FONT_WHITE);
    scoreLabel1->setScale(0.6);
    scoreLabel1->setAnchorPoint(ccp(0, 0));
    scoreLabel1->setPosition( ccp(g_visibleSize.width / 4, g_visibleSize.height * 0.5));
    this->addChild(scoreLabel1);

    //ハイスコア
    string = CCString::createWithFormat("High Score %d", UserSettings::getHighScore());
    CCLabelBMFont* highScoreLabel1 = CCLabelBMFont::create(string->getCString(), FONT_WHITE);
    highScoreLabel1->setScale(0.6);
    highScoreLabel1->setAnchorPoint(ccp(0, 0));
    highScoreLabel1->setPosition( ccp(g_visibleSize.width / 4, g_visibleSize.height * 0.45));
    this->addChild(highScoreLabel1);

    //女の子
    CCSprite *obj = CCSprite::createWithSpriteFrameName(PNG_MINI3_1);
    obj->setPosition( ccp(g_visibleSize.width / 1.5, g_visibleSize.height * 0.33));
    obj->runAction(Animations::gameoverMiniAnimation());
    this->addChild(obj);

    //OKボタン
    CCLabelBMFont* okLabel = CCLabelBMFont::create("OK", FONT_ORANGE);
    okLabel->setScale(1);
    CCMenuItemLabel* item1 = CCMenuItemLabel::create(okLabel, this, menu_selector(GameOverScene::onTapOKButton));
    item1->setPosition( ccp(g_visibleSize.width / 2, g_visibleSize.height * 0.2));
    item1->runAction(Animations::topLabelAction());

    CCMenu* menu = CCMenu::create( item1,  NULL );
    menu->setPosition(CCPointZero);
    this->addChild(menu);
}

void GameOverScene::onTapOKButton()
{
    CCScene* scene = (CCScene*)TopScene::create();
    CCDirector::sharedDirector()->replaceScene(scene);
}

void GameOverScene::keyBackClicked()
{
    Utils::quit();
}
