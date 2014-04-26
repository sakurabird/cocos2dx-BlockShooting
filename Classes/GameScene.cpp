//
//  GameScene.cpp
//  BrockShooting
//
//  Created by sakura on 2014/04/22.
//
//

#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "AppMacros.h"
#include "GHelper.h"
#include "BallSprite.h"
#include "BarSprite.h"
#include "BlockSprite.h"
#include "GameOverScene.h"

enum
{
    kTagStartLabel,
    kTagLayer = 1234,
};
static const char s_stars1[]              = "stars.png";

GameScene::~GameScene()
{
    CCLOG("~GameScene!");

	if (_targets)
	{
		_targets->release();
		_targets = NULL;
	}
}

GameScene::GameScene()
{
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

    srand((unsigned int)time(NULL));

    setTouchEnabled(true);
    setTouchMode(kCCTouchesOneByOne);

    // Android バックキー・メニューキーイベントを取得する
    setKeypadEnabled(true);

    initForVariables();

    showBackground();

    // バーを作成する
    makeBar();

    //ブロックを作成する
    makeBlock();

    //スタートラベルの表示
    showStartLabel();

    makeRetryButton();

    makeCloseButton();

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);

    return true;
}

//ゲームループ
void GameScene::updateGame(float dt)
{
    if (_blocksDestroyed >= BLOCK_COLUMN * BLOCK_ROW) {
        this->win();
        return;
    }

    //壁の当たり判定
    updateWalls();

    // ブロックの当たり判定
    updateBlocks();

    // バーの当たり判定
    updateBar();
}

void GameScene::showStartLabel()
{
    CCLabelTTF* startLabel = CCLabelTTF::create("タッチしてスタート!", "Arial", 50.0);
    startLabel->setColor(ccc3(0,0,0));
    CCMenuItemLabel* startItem = CCMenuItemLabel::create(startLabel, this, menu_selector(GameScene::tapStartButton));
    startItem->setPosition(ccp(_visibleSize.width * 0.5, _visibleSize.height * 0.2));

    CCMenu* menu = CCMenu::create(startItem, NULL);
    menu->setPosition(CCPointZero);
    menu->setTag(kTagStartLabel);
    this->addChild(menu);
}

void GameScene::tapStartButton()
{
	this->schedule( schedule_selector(GameScene::updateGame) );
    CCNode* lavel = dynamic_cast<CCNode*>(this->getChildByTag(kTagStartLabel));
    this->removeChild(lavel, true);
}

void GameScene::initForVariables()
{
    _visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    _origin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCLOG("Hello visibleSize.width: %f, height: %f",_visibleSize.width,_visibleSize.height);
    CCLOG("Hello origin.x: %f, origin.y: %f",_origin.x,_origin.y);

    _targets = NULL;
    _blocksDestroyed = 0;

    this->setBallRemain(BALL_REMAIN);
}

void GameScene::makeBar()
{
    float w = _visibleSize.width / 4;
    float h = w / 6;
//    float marginBottom = _visibleSize.height / 15;
    CCLOG("Hello bar.w: %f, height: %f",w,h);

    BarSprite* bar = BarSprite::createWithBarSize(w, h);

    //        CCSprite* player = CCSprite::createWithSpriteFrameName("Player.png");//テクスチャアトラスを使用

    bar->setPosition(GHelper::convI720toCC(_visibleSize.width / 2, _visibleSize.height * 0.85));
    this->addChild(bar);
}


void GameScene::makeBlock()
{
	_targets = new CCArray;

    float width = _visibleSize.width / 16.0;
    float height = width * 0.75;
    float margin = (_visibleSize.width - (width * BLOCK_COLUMN)) / (BLOCK_COLUMN + 1);
    CCLOG("Hello block.size: %f, margin: %f",width, margin);

    //    CCSprite *block = NULL;
    BlockSprite *block = NULL;

    int number = 0;
    int y = _visibleSize.height * 0.4;

    for (int i = 0; i < BLOCK_ROW; i++)
    {
        int x = 0;
        for (int j = 0; j < BLOCK_COLUMN; j++)
        {
            block  = BlockSprite::createWithBlockSize(width, height, number++);
            block->setPosition(GHelper::convI720toCC(x + block->getContentSize().width / 2,
                                                     y + block->getContentSize().height / 2));

            this->addChild(block);
            _targets->addObject(block);

            x += block->getContentSize().width + margin;
        }
        y -= block->getContentSize().height + margin * 2;
    }

    //残りボールがゼロでない時はバーの上にボールを表示する TODO

}

void GameScene::showBackground()
{
    // 背景を生成
    int n = rand() % PNG_BG_MAX + 1;
    CCString* fileName = CCString::createWithFormat("bg/bg%d.png",n);
    m_background = CCSprite::create(fileName->getCString());
    if (!m_background) {
        return;
    }

    m_background->setPosition(GHelper::convI720toCC(_visibleSize.width / 2, _visibleSize.height / 2));
    addChild(m_background, kZOrderBackground, kTagBackground);

    //画像をぼやかすレイヤーを加える
    CCLayerColor* layer = CCLayerColor::create( ccc4(255, 255, 255, 100) );
    layer->setCascadeColorEnabled(false);
//    addChild(layer, 100, kTagLayer);


//
//    layer->runAction(
//                      CCRepeatForever::create(
//                                    CCSequence::create(
//                                                       CCTintTo::create(35, 255, 0, 255),
//                                                       CCTintTo::create(25, 255, 255, 255),
//                                                       CCDelayTime::create(1),
//                                                                 NULL)));
}

void GameScene::onBallLost(CCNode* sender)
{
    //奈落に落ちたボールを削除
    BallSprite *ball = dynamic_cast<BallSprite*>(sender);
	this->removeChild(ball, true);

    int remain = this->getBallRemain() - 1;
    this->setBallRemain(remain);

    //ゲームオーバー判定
    if (remain <= 0) {
        this->gameOver();
        return;
    }
}

bool GameScene::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
    if (_blocksDestroyed >= BLOCK_COLUMN * BLOCK_ROW) {
        return false;
    }

    //現在ボールが飛んでいなければボールを出す
    if (!this->getChildByTag(TAG_BALL)) {
        tapStartButton();
        pushBall(touch);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
    }

	CCPoint location = touch->getLocation();
    CCSprite *bar = dynamic_cast<CCSprite*>(this->getChildByTag(TAG_BAR));

    if (!bar) {
        return false;
    }
    //バーがタップされた場合のみタップイベントを有効にする
    return bar->boundingBox().containsPoint(location);

}

void GameScene::ccTouchMoved(CCTouch *touch, CCEvent *event)
{
    // バーを動かす
    moveBar(touch);

}

void GameScene::ccTouchEnded(CCTouch *touch, CCEvent* event)
{
}

void GameScene::pushBall(CCTouch *touch)
{
    BallSprite* ball = BallSprite::createWithBallScale(1);
    //    CCSprite* projectile = CCSprite::createWithSpriteFrameName("Projectile.png");//テクスチャアトラスを使用

    CCNode *bar = this->getChildByTag(TAG_BAR);
	ball->setPosition( ccp(bar->getPositionX(), bar->getPositionY()+ bar->getContentSize().height) );
	this->addChild(ball);
}

void GameScene::moveBar(CCTouch* touch)
{
	CCPoint location = touch->getLocation();
    CCSprite *bar = dynamic_cast<CCSprite*>(this->getChildByTag(TAG_BAR));
    if (!bar) {
        CCLog("moveBar null");
        return;
    }
    bar->setPositionX(location.x);
}

void GameScene::updateBlocks()
{
    BallSprite *ball = dynamic_cast<BallSprite*>(this->getChildByTag(TAG_BALL));
    if (!ball) {
        return;
    }
    CCRect ballRect = ball->boundingBox();

    CCObject* jt = NULL;
    CCArray* targetsToDelete = new CCArray;

    CCARRAY_FOREACH(_targets, jt)
    {
        CCSprite *target = dynamic_cast<CCSprite*>(jt);
        //            CCLOG("updateGame target.x: %f, target.y: %f, tag: %d",target->getContentSize().width, target->getContentSize().height, target->getTag());
        CCRect targetRect = target->boundingBox();

        //衝突判定
        if (ballRect.intersectsRect(targetRect))
        {
            // ボールは跳ね返す
            ball->bounceBall(targetRect);

            targetsToDelete->addObject(target);
        }
    }

    // 当たったブロックを消す
    CCARRAY_FOREACH(targetsToDelete, jt)
    {
        CCSprite *target = dynamic_cast<CCSprite*>(jt);
        _targets->removeObject(target);
        this->removeChild(target, true);

        _blocksDestroyed++;
    }
    targetsToDelete->release();
}

void GameScene::updateWalls()
{
    BallSprite *ball = dynamic_cast<BallSprite*>(this->getChildByTag(TAG_BALL));
    if (!ball) {
        return;
    }

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
    //衝突判定
    ball->bounceBall(_visibleSize);
}

void GameScene::updateBar()
{
    BallSprite *ball = dynamic_cast<BallSprite*>(this->getChildByTag(TAG_BALL));
    if (!ball) {
        return;
    }
    CCRect ballRect = ball->boundingBox();

    BarSprite *bar = dynamic_cast<BarSprite*>(this->getChildByTag(TAG_BAR));
    if (!bar) {
        return;
    }
    CCRect barRect = bar->boundingBox();

    //衝突判定
    if (ballRect.intersectsRect(barRect))
    {
        // ボールは跳ね返す
        ball->bounceBall(barRect);
    }
}

void GameScene::win()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->end();

	this->unschedule( schedule_selector(GameScene::updateGame) );

    BallSprite* ball = dynamic_cast<BallSprite*>(this->getChildByTag(TAG_BALL));
    this->removeChild(ball, true);

    BarSprite* bar = dynamic_cast<BarSprite*>(this->getChildByTag(TAG_BAR));
    this->removeChild(bar, true);

    CCLayer* layer = dynamic_cast<CCLayer*>(this->getChildByTag(kTagLayer));
    this->removeChild(layer, true);

    CCParticleSystem* emitter = CCParticleExplosion::create();
    emitter->retain();
    this->addChild(emitter, 1);

//    emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage(s_stars1) );
    emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage(PNG_BALL) );
    emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage(PNG_BAR) );
    emitter->setTexture( CCTextureCache::sharedTextureCache()->addImage(s_stars1) );

    emitter->setAutoRemoveOnFinish(true);

    emitter->setPosition( ccp(_visibleSize.width / 2, _visibleSize.height / 2) );
}

void GameScene::gameOver()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->end();

	this->unschedule( schedule_selector(GameScene::updateGame) );

    GameOverScene *gameOverScene = GameOverScene::create();
    GameOverLayer *gameOverLayer = gameOverScene->getLayer();

    gameOverLayer->setResult(_blocksDestroyed);

    CCDirector::sharedDirector()->replaceScene(gameOverScene);
}

void GameScene::makeRetryButton()
{
    //リトライボタンを作成する
    CCLabelTTF* retryLabel = CCLabelTTF::create("Retry", "Arial", 50.0);
    // tapRetryButton関数が呼ばれるようにする
    CCMenuItemLabel* retryItem = CCMenuItemLabel::create(retryLabel, this, menu_selector(GameScene::tapRetryButton));
    if (!retryItem) {
        return;
    }
    retryItem->setPosition(GHelper::convI720toCC(_visibleSize.width * 0.7, 20));

    //メニューを作成する
    CCMenu* menu = CCMenu::create(retryItem, NULL);
    if (!menu) {
        return;
    }
    //上でリトライボタンの位置を設定したためここはCCPointZeroとする必要がある
    menu->setPosition(CCPointZero);
    this->addChild(menu);
}

//リトライボタンタップ時の処理
void GameScene::tapRetryButton(CCNode *node)
{
    //ゲームのシーンを新しく用意します
    CCScene* gameScene = (CCScene*)GameScene::create();
    CCDirector::sharedDirector()->replaceScene(gameScene);
}


void GameScene::makeCloseButton()
{
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                                          "CloseNormal.png",
                                                          "CloseSelected.png",
                                                          this,
                                                          menu_selector(GameScene::menuCloseCallback));

    //        CCSprite* closeNormal = CCSprite::createWithSpriteFrameName("CloseNormal.png");
    //        CCSprite* closeSelected = CCSprite::createWithSpriteFrameName("CloseSelected.png");
    //        CCMenuItemSprite *pCloseItem = CCMenuItemSprite::create(closeNormal,
    //                                                                closeSelected,
    //                                                                this,
    //                                                                menu_selector(TopScene::menuCloseCallback));

    if (!pCloseItem) {
        return;
    }
    pCloseItem->setPosition(GHelper::convI720toCC(_visibleSize.width * 0.9, 20));
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    if (!pMenu) {
        return;
    }
    this->addChild(pMenu, 1);
}

void GameScene::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");

#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

void GameScene::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority + 1, true);
}
// Androidバックキーイベント
void GameScene::keyBackClicked()
{
    CCDirector::sharedDirector()->end();
}

