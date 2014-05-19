
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

    if (!CCLayerColor::initWithColor( ccc4(0,0,0,0) )) {
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
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

    //タイトル
    CCLabelBMFont* title = CCLabelBMFont::create("GAME OVER", FONT_BIG2);
    title->setScale(1.2);
    title->setPosition( ccp(visibleSize.width / 2, visibleSize.height * 0.7));
    this->addChild(title);

    //スコア
    CCLabelBMFont* scoreLabel1 = CCLabelBMFont::create("Score", FONT_BLUE);
    scoreLabel1->setScale(0.4);
    scoreLabel1->setPosition( ccp(visibleSize.width / 3, visibleSize.height * 0.5));
    this->addChild(scoreLabel1);

    CCLabelBMFont* scoreLabel2 = CCLabelBMFont::create("", FONT_WHITE);
    scoreLabel2->setScale(0.4);
    scoreLabel2->setPosition( ccp(scoreLabel1->getPositionX() + scoreLabel1->getContentSize().width, scoreLabel1->getPositionY()));
    scoreLabel2->setString(ccsf("%d", UserSettings::getScore()));
    this->addChild(scoreLabel2, kTagGameOverScoreLavel);


    //ハイスコア
    if (UserSettings::getScore() != 0)
    {
        if (UserSettings::getScore() > UserSettings::getHighScore())
        {
            // ハイスコアを更新する
            UserSettings::setHighScore(UserSettings::getScore());
        }
    }
    CCLabelBMFont* highScoreLabel1 = CCLabelBMFont::create("High Score", FONT_YELLOW);
    highScoreLabel1->setScale(0.4);
    highScoreLabel1->setPosition( ccp(scoreLabel1->getPositionX(), scoreLabel1->getPositionY() - 30));
    this->addChild(highScoreLabel1);

    CCLabelBMFont* highScoreLabel2 = CCLabelBMFont::create("", FONT_WHITE);
    highScoreLabel2->setScale(0.4);
    highScoreLabel2->setPosition( ccp(scoreLabel2->getPositionX(), highScoreLabel1->getPositionY()));
    highScoreLabel2->setString(ccsf("%d", UserSettings::getHighScore()));
    this->addChild(highScoreLabel2, kTagGameOverHighScoreLavel);

    //OKボタン
    CCLabelBMFont* okLabel = CCLabelBMFont::create("OK", FONT_ORANGE);
    okLabel->setScale(1);
    CCMenuItemLabel* item1 = CCMenuItemLabel::create(okLabel, this, menu_selector(GameOverScene::onTapOKButton));
    item1->setPosition( ccp(visibleSize.width / 2, visibleSize.height * 0.2));
    item1->runAction(Animation::topLabelAction());

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
