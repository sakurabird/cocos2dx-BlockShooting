//
//  GameScene.cpp
//  BrockShooting
//
//  Created by sakura on 2014/04/22.
//
//

#include "GameScene.h"
#include "UserSettings.h"
#include "SimpleAudioEngine.h"
#include "CCPlaySE.h"
#include "AppMacros.h"
#include "GHelper.h"
#include "BallSprite.h"
#include "BarSprite.h"
#include "BlockSprite.h"
#include "GameOverScene.h"
#include "Animations.h"

USING_NS_CC;
using namespace CocosDenshion;

bool isBallActive;

GameScene::GameScene()
:m_blocks(NULL)
,m_balls(NULL)
,m_blocksDestroyed(0)
,m_score(0)
{
    srand((unsigned int)time(NULL));
}

GameScene::~GameScene()
{
    CCLOG("~GameScene!");

    releaseObject();
}

CCScene* GameScene::scene()
{
	CCScene * scene = NULL;
	do
	{
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		GameScene *layer = GameScene::create();
		CC_BREAK_IF(! layer);

		scene->addChild(layer);
	} while (0);
	return scene;
}

bool GameScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }

    if (!CCLayerColor::initWithColor( ccc4(0,0,0,0) )) {
        return false;
    }

    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);

    // Android バックキー・メニューキーイベントを取得する
    setKeypadEnabled(true);

    // BGM再生
    if (UserSettings::getMusicSetting() &&
        !SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic(MP3_BG, true);

    initForVariables();

    showBackground();

    createBalls();

    makeBar();

    makeBlock();

    showStartLabel();

    makeRetryButton();

    makeHomeButton();

    setBall();

	this->schedule( schedule_selector(GameScene::updateGame) );

    return true;
}

//ゲームループ
void GameScene::updateGame(float dt)
{
    if (m_blocksDestroyed >= BLOCK_COLUMN * BLOCK_ROW) {
        this->win();
        return;
    }

    //ボール移動
    updateBall();

    // ブロックの当たり判定
    updateBlocks();

    // バーの当たり判定
    updateBar();
}

void GameScene::showStartLabel()
{
    CCLabelBMFont* startLabel = CCLabelBMFont::create("Touch to Start", FONT_TOUCH);
    startLabel->setPosition(ccp(_visibleSize.width * 0.5, _visibleSize.height * 0.5));
    startLabel->setTag(kTagStartLabel);
    this->addChild(startLabel);
    startLabel->runAction(Animation::topLavelAction());
}

void GameScene::initForVariables()
{
    _visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    _origin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCLOG("Hello visibleSize.width: %f, height: %f",_visibleSize.width,_visibleSize.height);
    CCLOG("Hello origin.x: %f, origin.y: %f",_origin.x,_origin.y);

    this->setBallRemain(BALL_REMAIN);

    isBallActive = false;
}

void GameScene::createBalls()
{
	m_balls = new CCArray;

    for (int i = 0; i < BALL_REMAIN; i++)
    {
        BallSprite* ball = BallSprite::createWithBallScale(0.7);
        //    CCSprite* projectile = CCSprite::createWithSpriteFrameName("Projectile.png");//テクスチャアトラスを使用
        m_balls->addObject(ball);
    }

    //レベル
    CCString* levelString = NULL;
    CCString* color = NULL;
    switch (UserSettings::getLevelSetting()) {
        case LEVEL_EASY:
            levelString = CCString::create("Easy");
            color = CCString::create(FONT_GREEN);
            break;
        case LEVEL_NORMAL:
            levelString = CCString::create("Normal");
            color = CCString::create(FONT_YELLOW);
            break;
        case LEVEL_HARD:
            levelString = CCString::create("Hard");
            color = CCString::create(FONT_BLUE);
            break;
    }
    CCLabelBMFont* label1 = CCLabelBMFont::create(levelString->getCString(), color->getCString());
    label1->setScale(0.5);
    label1->setPosition(GHelper::convI720toCC(_visibleSize.width  * 0.7, _visibleSize.height * 0.01));
    label1->setTag(kTagLevel);
    this->addChild(label1);

    //残りボール数
    CCLabelBMFont* label2 = CCLabelBMFont::create("Balls:", FONT_WHITE);
    label2->setScale(0.5);
    label2->setPosition(label1->getPositionX(),
                        label1->getPositionY() - label1->getContentSize().height * 0.5);
    label2->setTag(kTagBallRemainLabel);
    this->addChild(label2);

    //スコア
    CCLabelBMFont* label3 = CCLabelBMFont::create("Score:", FONT_WHITE);
    label3->setScale(0.5);
    label3->setPosition(label2->getPositionX(),
                        label2->getPositionY() - label2->getContentSize().height * 0.5);
    label3->setTag(kTagScoreLabel);
    this->addChild(label3);

    showBallRemain();
    showScore();
}

void GameScene::setBall()
{
    if (m_balls->count() <= 0) {
        return;
    }
    BallSprite* ball = dynamic_cast<BallSprite*>(m_balls->lastObject());
    CCNode *bar = this->getChildByTag(kTagBar);
	ball->setPosition( ccp(bar->getPositionX(), bar->getPositionY()+ bar->getContentSize().height) );
	this->addChild(ball);

    m_balls->removeLastObject();
}

void GameScene::showBallRemain()
{
    //残りボール数
    CCLabelBMFont* remain = dynamic_cast<CCLabelBMFont*>(this->getChildByTag(kTagBallRemainCount));
    if (remain) {
        this->removeChild(remain);
    }

    CCLabelBMFont* label = dynamic_cast<CCLabelBMFont*>(this->getChildByTag(kTagBallRemainLabel));

    CCString* ballRemain = CCString::createWithFormat("%d", m_ballRemain);
    remain = CCLabelBMFont::create(ballRemain->getCString(), FONT_ORANGE);
    remain->setScale(0.5);
    remain->setPosition(label->getPositionX() + label->getContentSize().width * 0.5,
                        label->getPositionY());
    remain->setTag(kTagBallRemainCount);
    this->addChild(remain);
}

void GameScene::showScore()
{
    //スコア
    CCLabelBMFont* score = dynamic_cast<CCLabelBMFont*>(this->getChildByTag(kTagScore));
    if (score) {
        this->removeChild(score);
    }

    CCLabelBMFont* label = dynamic_cast<CCLabelBMFont*>(this->getChildByTag(kTagScoreLabel));

    CCString* gameScore = CCString::createWithFormat("%d", getScore());
    score = CCLabelBMFont::create(gameScore->getCString(), FONT_ORANGE);
    score->setScale(0.5);
    score->setPosition(label->getPositionX() + label->getContentSize().width * 0.5,
                       label->getPositionY());
    score->setTag(kTagScore);
    this->addChild(score);
}

void GameScene::makeBar()
{
    float w = _visibleSize.width / 4;
    float h = w / 6;
//    CCLOG("bar.w: %f, height: %f",w,h);

    BarSprite* bar = BarSprite::createWithBarSize(w, h);

    //        CCSprite* player = CCSprite::createWithSpriteFrameName("Player.png");//テクスチャアトラスを使用

    bar->setPosition(GHelper::convI720toCC(_visibleSize.width / 2, _visibleSize.height * 0.85));
    this->addChild(bar);
}


void GameScene::makeBlock()
{
	m_blocks = new CCArray;

    float width = _visibleSize.width / 16.0;
    float height = width * 0.3;
    float margin = (_visibleSize.width - (width * BLOCK_COLUMN)) / (BLOCK_COLUMN + 1);
//    CCLOG("block.size: %f, margin: %f",width, margin);

    //    CCSprite *block = NULL;
    BlockSprite *block = NULL;

    int number = 0;
    int y = _visibleSize.height * 0.6;

    for (int i = 0; i < BLOCK_ROW; i++)
    {
        int x = 0;
        for (int j = 0; j < BLOCK_COLUMN; j++)
        {
            block  = BlockSprite::createWithBlockSize(width, height, number++);
            block->setPosition(GHelper::convI720toCC(x + block->getContentSize().width / 2,
                                                     y + block->getContentSize().height / 2));

            this->addChild(block);
            m_blocks->addObject(block);

            x += block->getContentSize().width + margin;
        }
        y -= block->getContentSize().height + margin;
    }
}

void GameScene::showBackground()
{
    // 背景を生成
    int n = rand() % PNG_BG_MAX + 1;
    CCString* fileName = CCString::createWithFormat("bg/bg%d.png",n);
    m_background = CCSprite::create(fileName->getCString());
    if (!m_background) return;

    m_background->setPosition(GHelper::convI720toCC(_visibleSize.width / 2, _visibleSize.height / 2));
    addChild(m_background, kZOrderBackground, kTagBackground);
}

void GameScene::onBallLost(CCNode* sender)
{
    if (UserSettings::getSESetting())
        SimpleAudioEngine::sharedEngine()->playEffect(MP3_BALLLOST);

    isBallActive = false;
    
    //奈落に落ちたボールを削除
    BallSprite *ball = dynamic_cast<BallSprite*>(sender);
	this->removeChild(ball, true);

    int remain = this->getBallRemain() - 1;
    this->setBallRemain(remain);

    showBallRemain();

    //ゲームオーバー判定
    if (remain <= 0) {
        this->gameOver();
        return;
    }
    setBall();
}

bool GameScene::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    if (m_blocksDestroyed >= BLOCK_COLUMN * BLOCK_ROW) {
        return false;
    }

    CCNode* lavel = dynamic_cast<CCNode*>(this->getChildByTag(kTagStartLabel));
    if (lavel) {
        this->removeChild(lavel, true);
    }

    //現在ボールが飛んでいなければボールを飛ばす
    if (!isBallActive) {
        if (UserSettings::getSESetting())
            SimpleAudioEngine::sharedEngine()->playEffect(MP3_BALLPUSH);
        isBallActive = true;
        return true;
    }

	CCPoint location = touch->getLocation();
    CCSprite *bar = dynamic_cast<CCSprite*>(this->getChildByTag(kTagBar));
    if (!bar) return false;

    //バーの横幅以内がタップされた場合のみタップイベントを有効にする
    bool b = false;
    CCRect rect = bar->boundingBox();
//    CCLOG("bar  getMaxX: %f,  getMidX: %f,  getMinX: %f",rect.getMaxX(),rect.getMidX(),rect.getMinX());
    if (!rect.containsPoint(location))
    {
        b = true;
    }else if (location.x >= rect.getMinX() && location.x <= rect.getMaxX())
    {
        b = true;
    }
    return b;

}

void GameScene::ccTouchMoved(CCTouch *touch, CCEvent *event)
{
    // バーを動かす
    moveBar(touch);

}

void GameScene::ccTouchEnded(CCTouch *touch, CCEvent* event)
{
}

void GameScene::updateBall()
{
    if (!isBallActive) {
        return;
    }
    BallSprite *ball = dynamic_cast<BallSprite*>(this->getChildByTag(kTagBall));
    if (!ball) return;

    CCPoint ballPoint = ball->getPosition();
    float vx = ball->getVelocityX();
    float vy = ball->getVelocityY();

    // ボールの移動
    ball->setPosition(ccp(ball->getPositionX() + vx, ball->getPositionY() + vy));

    if( ballPoint.y < 0 )
    {
        //ユーザーがボールを奈落に落とした
        onBallLost(ball);
        return;
    }
    //壁との衝突判定
    ball->bounceBall(_visibleSize);
}

void GameScene::moveBar(CCTouch* touch)
{
	CCPoint location = touch->getLocation();
    CCSprite *bar = dynamic_cast<CCSprite*>(this->getChildByTag(kTagBar));
    if (!bar) return;
    bar->setPositionX(location.x);
}

void GameScene::updateBlocks()
{
    BallSprite *ball = dynamic_cast<BallSprite*>(this->getChildByTag(kTagBall));
    if (!ball) return;

    CCRect ballRect = ball->boundingBox();

    CCObject* jt = NULL;
    CCArray* blocksToDelete = new CCArray;

    CCARRAY_FOREACH(m_blocks, jt)
    {
        CCSprite* block = dynamic_cast<CCSprite*>(jt);
        //            CCLOG("updateGame target.x: %f, target.y: %f, tag: %d",target->getContentSize().width, target->getContentSize().height, target->getTag());
        CCRect blockRect = block->boundingBox();

        //衝突判定
        if (ballRect.intersectsRect(blockRect))
        {
            // ボールは跳ね返す
            ball->bounceBall(blockRect, kTagBlock);

            blocksToDelete->addObject(block);

            //スコア加算
            m_score += 100;
            showScore();
        }
    }

    // 当たったブロックを消す
    CCARRAY_FOREACH(blocksToDelete, jt)
    {
        CCSprite *block = dynamic_cast<CCSprite*>(jt);
        m_blocks->removeObject(block);
        this->removeChild(block, true);

        m_blocksDestroyed++;
    }
    blocksToDelete->release();
}

void GameScene::updateBar()
{
    BallSprite *ball = dynamic_cast<BallSprite*>(this->getChildByTag(kTagBall));
    if (!ball) return;

    CCRect ballRect = ball->boundingBox();

    BarSprite *bar = dynamic_cast<BarSprite*>(this->getChildByTag(kTagBar));
    if (!bar) return;
    CCRect barRect = bar->boundingBox();

    //衝突判定
    if (ballRect.intersectsRect(barRect))
    {
        // ボールは跳ね返す
        ball->bounceBall(barRect, kTagBar);
    }
}

void GameScene::win()
{
    //クリア時の処理
    if (UserSettings::getSESetting())
        SimpleAudioEngine::sharedEngine()->playEffect(MP3_CLEAR);

	this->unschedule( schedule_selector(GameScene::updateGame) );
    isBallActive = false;

    //ボールとバーのオブジェクトを取り除く
    BallSprite* ball = dynamic_cast<BallSprite*>(this->getChildByTag(kTagBall));
    this->removeChild(ball, true);

    BarSprite* bar = dynamic_cast<BarSprite*>(this->getChildByTag(kTagBar));
    this->removeChild(bar, true);

    //パーティクル表示
    CCParticleSystem* emitter = CCParticleExplosion::create();
    emitter->retain();
    this->addChild(emitter, kZOrderEmmit);

    emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage(PNG_RECT1) );

    emitter->setAutoRemoveOnFinish(true);

    emitter->setPosition( ccp(_visibleSize.width / 2, _visibleSize.height / 2) );

    //クリアのラベル表示
    CCLabelBMFont* label = CCLabelBMFont::create("CLEAR!", FONT_BIG1);
    label->setPosition( ccp(_visibleSize.width / 2, _visibleSize.height * 0.5));
    label->runAction(Animation::gameClearAction());
    addChild(label, kZOrderLabel);
}

void GameScene::gameOver()
{
    if (UserSettings::getSESetting())
        SimpleAudioEngine::sharedEngine()->playEffect(MP3_GAMEOVER);

	this->unschedule( schedule_selector(GameScene::updateGame) );

    GameOverScene *gameOverScene = GameOverScene::create();
    GameOverLayer *gameOverLayer = gameOverScene->getLayer();

    gameOverLayer->setResult(m_blocksDestroyed);

    CCDirector::sharedDirector()->replaceScene(gameOverScene);
}

void GameScene::makeRetryButton()
{
    //リトライボタンを作成する
    CCMenuItemImage *item = CCMenuItemImage::create(
                                                          PNG_REFRESH,
                                                          PNG_REFRESH,
                                                          this,
                                                          menu_selector(GameScene::onTapRetryButton));
    if (!item) return;
    item->setPosition(GHelper::convI720toCC(_visibleSize.width * 0.2, 20));
    //メニューを作成する
    CCMenu* menu = CCMenu::create(item, NULL);
    if (!menu) return;
    //上でリトライボタンの位置を設定したためここはCCPointZeroとする必要がある
    menu->setPosition(CCPointZero);
    this->addChild(menu, kZOrderTop, kTagRetry);
}

//リトライボタンタップ時の処理
void GameScene::onTapRetryButton(CCNode* node)
{
    CCScene* gameScene = (CCScene*)GameScene::create();
    CCTransitionTurnOffTiles* tran = CCTransitionTurnOffTiles::create(1, gameScene);
    CCDirector::sharedDirector()->replaceScene(tran);
}


void GameScene::makeHomeButton()
{
    CCMenuItemImage *item = CCMenuItemImage::create(
                                                          PNG_HOME,
                                                          PNG_HOME,
                                                          this,
                                                          menu_selector(GameScene::onTapHomeButton));

    //        CCSprite* closeNormal = CCSprite::createWithSpriteFrameName("CloseNormal.png");
    //        CCSprite* closeSelected = CCSprite::createWithSpriteFrameName("CloseSelected.png");
    //        CCMenuItemSprite *pCloseItem = CCMenuItemSprite::create(closeNormal,
    //                                                                closeSelected,
    //                                                                this,
    //                                                                menu_selector(TopScene::menuCloseCallback));

    if (!item) return;
    item->setPosition(GHelper::convI720toCC(_visibleSize.width * 0.1, 20));
    CCMenu* menu = CCMenu::create(item, NULL);
    menu->setPosition(CCPointZero);
    if (!menu) return;
    this->addChild(menu, kZOrderTop, kTagHome);
}

void GameScene::releaseObject()
{
	if (m_blocks)
	{
		m_blocks->release();
		m_blocks = NULL;
	}

	if (m_balls)
	{
		m_balls->release();
		m_balls = NULL;
	}
}

void GameScene::onTapHomeButton(CCNode* node)
{
    CCDirector::sharedDirector()->popScene();
}

void GameScene::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority + 1, true);
}

// Androidバックキーイベント
void GameScene::keyBackClicked()
{
    CCDirector::sharedDirector()->popScene();
}

