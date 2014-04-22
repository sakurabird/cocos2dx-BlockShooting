#include "HelloWorldScene.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"
#include "AppMacros.h"
#include "BallSprite.h"
#include "BarSprite.h"
#include "BlockSprite.h"

HelloWorld::~HelloWorld()
{
    CCLOG("~HelloWorld!");

	if (_targets)
	{
		_targets->release();
		_targets = NULL;
	}
}

HelloWorld::HelloWorld()
{
}

CCScene* HelloWorld::scene()
{
	CCScene * scene = NULL;
	do
	{
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		HelloWorld *layer = HelloWorld::create();
		CC_BREAK_IF(! layer);

		scene->addChild(layer);
	} while (0);
	return scene;
}

bool HelloWorld::init()
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

    // バックキー・メニューキーイベントを取得する
    setKeypadEnabled(true);

    initForVariables();

    showBackground();

    // バーを作成する
    makeBar();

    //ブロックを作成する
    makeBlock();

	this->schedule( schedule_selector(HelloWorld::updateGame) );

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);

    return true;
}

void HelloWorld::updateGame(float dt)
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

void HelloWorld::initForVariables()
{
    _visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    _origin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCLOG("Hello visibleSize.width: %f, height: %f",_visibleSize.width,_visibleSize.height);
    CCLOG("Hello origin.x: %f, origin.y: %f",_origin.x,_origin.y);

    _targets = NULL;
    _blocksDestroyed = 0;

    this->setBallRemain(BALL_REMAIN);
}

void HelloWorld::makeBar()
{
    float w = _visibleSize.width / 4;
    float h = w / 4;
    float marginBottom = _visibleSize.height / 15;
    CCLOG("Hello bar.w: %f, height: %f",w,h);

    BarSprite* bar = BarSprite::createWithBarSize(w, h);

    //        CCSprite* player = CCSprite::createWithSpriteFrameName("Player.png");//テクスチャアトラスを使用

    bar->setPosition( ccp(_visibleSize.width / 2,
                             _origin.y + bar->getContentSize().height / 2 + marginBottom) );
    this->addChild(bar);
}


void HelloWorld::makeBlock()
{
	_targets = new CCArray;

    float size = _visibleSize.width / 16.0;
    float width = _visibleSize.width / 16.0;
    float height = width * 0.75;
    float margin = (_visibleSize.width - (size * BLOCK_COLUMN)) / (BLOCK_COLUMN + 1);
    CCLOG("Hello block.size: %f, margin: %f",size, margin);

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

void HelloWorld::showBackground()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    // 背景を生成
    m_background = CCSprite::create(PNG_BACKGROUND);
    if (!m_background) {
        CCLOG("Hello background null");
        return;
    }
    m_background->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    addChild(m_background, kZOrderBackground, kTagBackground);
}

void HelloWorld::onBallLost(CCNode* sender)
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

bool HelloWorld::ccTouchBegan(CCTouch *touch, CCEvent *event)
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

void HelloWorld::ccTouchMoved(CCTouch *touch, CCEvent *event)
{
    // バーを動かす
    moveBar(touch);

}

void HelloWorld::ccTouchEnded(CCTouch *touch, CCEvent* event)
{
}

void HelloWorld::pushBall(CCTouch *touch)
{
    float size = _visibleSize.width / 17.0;
    BallSprite* ball = BallSprite::createWithBallSize(size);
    //    CCSprite* projectile = CCSprite::createWithSpriteFrameName("Projectile.png");//テクスチャアトラスを使用

    CCNode *bar = this->getChildByTag(TAG_BAR);
	ball->setPosition( ccp(bar->getPositionX(),
                            this->getChildByTag(TAG_BAR)->getPositionY()
                           + this->getChildByTag(TAG_BAR)->getContentSize().height) );
	this->addChild(ball);
}

void HelloWorld::moveBar(CCTouch* touch)
{
	CCPoint location = touch->getLocation();
    CCSprite *bar = dynamic_cast<CCSprite*>(this->getChildByTag(TAG_BAR));
    if (!bar) {
        CCLog("moveBar null");
        return;
    }
    bar->setPositionX(location.x);
}

void HelloWorld::updateBlocks()
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

void HelloWorld::updateWalls()
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

    // 壁に当たった時の処理、速度を入れ替える
    if(ballPoint.x > _visibleSize.width - ball->getContentSize().width / 2)
    {
        vx *= -1;
        ball->setVelocityX(vx);
        ball->setPositionX(_visibleSize.width - ball->getContentSize().width / 2 );
    }
    else if( ballPoint.x < 0 )
    {
        vx *= -1;
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
        vy *= -1;
        ball->setVelocityY(vy);
        ball->setPositionY(_visibleSize.height - ball->getContentSize().height);
    }
}

void HelloWorld::updateBar()
{
    BallSprite *ball = dynamic_cast<BallSprite*>(this->getChildByTag(TAG_BALL));
    if (!ball) {
        return;
    }
    CCRect ballRect = ball->boundingBox();

    BarSprite *bar = dynamic_cast<BarSprite*>(this->getChildByTag(TAG_BAR));
    if (!bar) {
        CCLog("updateBar null");
        return;
    }
    CCRect barRect = bar->boundingBox();

    //衝突判定
    if (ballRect.intersectsRect(barRect))
    {
        float vx = ball->getVelocityX();
        float vy = ball->getVelocityY();
        vy *= -1;
        ball->setVelocityY(vy);
        ball->setPosition(ccp(ball->getPositionX() + vx, ball->getPositionY() + vy));
    }
}

void HelloWorld::gameOver()
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->end();

	this->unschedule( schedule_selector(HelloWorld::updateGame) );

    GameOverScene *gameOverScene = GameOverScene::create();
    GameOverLayer *gameOverLayer = gameOverScene->getLayer();
    
    gameOverLayer->setResult(_blocksDestroyed);

    CCDirector::sharedDirector()->replaceScene(gameOverScene);
}

void HelloWorld::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority + 1, true);
}

// Androidバックキーイベント
void HelloWorld::keyBackClicked()
{
    CCDirector::sharedDirector()->end();
}

