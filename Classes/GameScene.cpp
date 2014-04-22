//
//  GameScene.cpp
//  BrockShooting
//
//  Created by sakura on 2014/04/22.
//
//

#include "GameScene.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"
#include "AppMacros.h"
#include "BallSprite.h"
#include "BarSprite.h"
#include "BlockSprite.h"

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

    if (!CCLayerColor::initWithColor( ccc4(255,255,255,255) )) {
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

	this->schedule( schedule_selector(GameScene::updateGame) );

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);

    return true;
}

//ゲームループ
void GameScene::updateGame(float dt)
{

    if (_blocksDestroyed >= BLOCK_COLUMN * BLOCK_ROW) {
        //クリア
        this->gameOver();
        return;
    }
    //壁の当たり判定
    updateWalls();

    // ブロックの当たり判定
    updateBlocks();

    // バーの当たり判定
    updateBar();
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
    float marginBottom = _visibleSize.height / 15;
    CCLOG("Hello bar.w: %f, height: %f",w,h);

    BarSprite* bar = BarSprite::createWithBarSize(w, h);

    //        CCSprite* player = CCSprite::createWithSpriteFrameName("Player.png");//テクスチャアトラスを使用

    bar->setPosition( ccp(_visibleSize.width / 2,
                          _origin.y + bar->getContentSize().height / 2 + marginBottom) );
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
    int y = _visibleSize.height - (this->getChildByTag(TAG_BAR)->getPositionY());

    for (int i = 0; i < BLOCK_ROW; i++)
    {
        int x = margin;
        for (int j = 0; j < BLOCK_COLUMN; j++)
        {
            block  = BlockSprite::createWithBlockSize(width, height, number++);
            block->setPosition(ccp(x + block->getContentSize().width * 0.5,
                                   y + block->getContentSize().height * 0.5));
            this->addChild(block);
            _targets->addObject(block);

            x += block->getContentSize().width + margin;
        }
        y -= block->getContentSize().height + margin;
    }


}

void GameScene::showBackground()
{
    // 背景を生成
    m_background = CCSprite::create(PNG_BACKGROUND);
//    m_background = CCSprite::create(PNG_BACKGROUND,
//                                    CCRectMake(0, 0, _visibleSize.width, _visibleSize.height));
    if (!m_background) {
        return;
    }
    m_background->setPosition(ccp(_visibleSize.width / 2, _visibleSize.height / 2));
    addChild(m_background, kZOrderBackground, kTagBackground);
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
    //現在ボールが飛んでいなければボールを出す
    if (!this->getChildByTag(TAG_BALL)) {
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
    BallSprite* ball = BallSprite::createWithBallScale(0.6f);
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
    const float gosa = 0.05;

    BallSprite *ball = dynamic_cast<BallSprite*>(this->getChildByTag(TAG_BALL));
    if (!ball) {
        return;
    }

    CCPoint ballPoint = ball->getPosition();
    float vx = ball->getVelocityX();
    float vy = ball->getVelocityY();

    // ボールの移動
    ball->setPosition(ccp(ball->getPositionX() + vx, ball->getPositionY() + vy));

    // 壁に当たった時の処理、速度を入れ替える
    if(ballPoint.x > _visibleSize.width - ball->getContentSize().width / 2)
    {
        vx = vx * -1 + gosa;
        ball->setVelocityX(vx);
        ball->setPositionX(_visibleSize.width - ball->getContentSize().width / 2 );
    }
    else if( ballPoint.x < 0 )
    {
        vx = vx * -1 + gosa;
        ball->setVelocityX(vx);
        ball->setPositionX(0);
    }

    if( ballPoint.y < 0 )
    {
        //ユーザーがボールを奈落に落とした
        onBallLost(ball);
        return;

    }else if( ballPoint.y > _visibleSize.height - ball->getContentSize().height /2 )
    {
        vy = vy * -1 + gosa;
        ball->setVelocityY(vy);
        ball->setPositionY(_visibleSize.height - ball->getContentSize().height);
    }
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
        float vx = ball->getVelocityX();
        float vy = ball->getVelocityY();

        if ( ballRect.getMaxX() < barRect.getMinX() ||
            barRect.getMaxX() < ballRect.getMinX()) {
            vx *= -1;
        }
        else
        if ( ballRect.getMaxY() < barRect.getMinY() ||
            barRect.getMaxY() < ballRect.getMaxY()) {
            vy *= -1;
        }

        ball->setVelocityX(vx);
        ball->setVelocityY(vy);
        ball->setPosition(ccp(ball->getPositionX() + vx, ball->getPositionY() + vy));
    }
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

