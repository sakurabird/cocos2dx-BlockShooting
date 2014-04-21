#include "HelloWorldScene.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"
#include "AppMacros.h"
#include "BallSprite.h"
#include "BarSprite.h"

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
:_targets(NULL)
,_blocksDestroyed(0)
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

    initForVariables();

    // バーを作成する
    makeBar();

    //ブロックを作成する
    makeBlock();

	this->schedule( schedule_selector(HelloWorld::updateGame) );

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);

    return true;
}

void HelloWorld::initForVariables()
{
    _visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    _origin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCLOG("Hello visibleSize.width: %f, height: %f",_visibleSize.width,_visibleSize.height);
    CCLOG("Hello origin.x: %f, origin.y: %f",_origin.x,_origin.y);

    setBallRemain(BALL_REMAIN);
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

    float size = _visibleSize.width / 14.0;
    float margin = (_visibleSize.width - (size * BLOCK_COLUMN)) / (BLOCK_COLUMN + 1);
    CCLOG("Hello block.size: %f, margin: %f",size, margin);

    CCSprite *target = NULL;

    int tag = 0;
    int y = _visibleSize.height - (this->getChildByTag(TAG_BAR)->getPositionY());

    for (int i = 0; i < BLOCK_ROW; i++) {
        int x = margin;
        for (int j = 0; j < BLOCK_COLUMN; j++) {
            target = CCSprite::create("block.png", CCRectMake(0, 0, size, size) );
            target->setPosition(ccp(x + target->getContentSize().width * 0.5,
                                y + target->getContentSize().height * 0.5));
            this->addChild(target);
            target->setTag(tag++);
            _targets->addObject(target);

            x += target->getContentSize().width + margin;
        }
        y -= target->getContentSize().height + margin;
    }


}

void HelloWorld::spriteMoveFinished(CCNode* sender)
{
    //奈落に落ちたボールを削除
	BallSprite *ball = (BallSprite*)sender;
	this->removeChild(ball, true);

    int remain = getBallRemain() - 1;
    setBallRemain(remain);
    //ゲームオーバー判定
    if (remain <= 0) {
        this->gameOver(ball);
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
        CCLog("ccTouchBegan bar null");
        return false;
    }
    //バーがタップされた場合のみタップイベントを有効にする
    CCLog("ccTouchBegan bar touched? %c",bar->boundingBox().containsPoint(location));
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
    _vx = 18;
    _vy = 18;
    if (rand() % 2 == 0){
        _vx *= -1;
    }
    int v = rand() % 10;
    if (v > 0) {
        float z = v * 0.1;
        _vx += z;
    }
    v = rand() % 10;
    if (v > 0) {
        float z = v * 0.1;
        _vy += z;
    }

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

void HelloWorld::updateGame(float dt)
{

    //壁の当たり判定
    updateWalls();

    // ブロックの当たり判定
    updateBlocks();

    // バーの当たり判定
    updateBar();
}

void HelloWorld::updateBlocks()
{
    CCNode *ball = this->getChildByTag(TAG_BALL);

    if (!ball) {
        return;
    }

    CCObject* jt = NULL;

    CCRect ballRect = CCRectMake(
                                 ball->getPosition().x - (ball->getContentSize().width / 2),
                                 ball->getPosition().y - (ball->getContentSize().height / 2),
                                 ball->getContentSize().width,
                                 ball->getContentSize().height);

    CCArray* targetsToDelete = new CCArray;

    CCARRAY_FOREACH(_targets, jt)
    {
        CCSprite *target = dynamic_cast<CCSprite*>(jt);
        //            CCLOG("updateGame target.x: %f, target.y: %f, tag: %d",target->getContentSize().width, target->getContentSize().height, target->getTag());

        CCRect targetRect = CCRectMake(
                                       target->getPosition().x - (target->getContentSize().width / 2),
                                       target->getPosition().y - (target->getContentSize().height / 2),
                                       target->getContentSize().width,
                                       target->getContentSize().height);

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
    CCNode *ball = this->getChildByTag(TAG_BALL);

    if (!ball) {
        return;
    }


    CCPoint ballPoint = ball->getPosition();
//    CCLog("updateWalls ball x:%f, y:%f", ballPoint.x, ballPoint.y);

    // ボールの移動
    ball->setPosition(ball->getPositionX() + _vx, ball->getPositionY() + _vy);
    // 壁に当たった時の処理、速度を入れ替える
    if(ballPoint.x > _visibleSize.width - ball->getContentSize().width / 2){
        _vx *= -1;
        ball->setPositionX(_visibleSize.width - ball->getContentSize().width / 2 );
    }else if( ballPoint.x < 0 ){
        _vx *= -1;
        ball->setPositionX(0);
    }

    if( ballPoint.y < 0 ){
        spriteMoveFinished(ball);
        return;
    }else if( ballPoint.y > _visibleSize.height - ball->getContentSize().height /2 ){
        _vy *= -1;
        ball->setPositionY(_visibleSize.height - ball->getContentSize().height);
    }
}

void HelloWorld::updateBar()
{
    CCNode *ball = this->getChildByTag(TAG_BALL);

    if (!ball) {
        return;
    }
    CCRect ballRect = CCRectMake(
                                ball->getPosition().x - (ball->getContentSize().width / 2),
                                ball->getPosition().y - (ball->getContentSize().height / 2),
                                ball->getContentSize().width,
                                ball->getContentSize().height);

    CCNode *bar = this->getChildByTag(TAG_BAR);
    if (!bar) {
        CCLog("updateBar null");
        return;
    }

    CCRect barRect = CCRectMake(
                                   bar->getPosition().x - (bar->getContentSize().width / 2),
                                   bar->getPosition().y - (bar->getContentSize().height / 2),
                                   bar->getContentSize().width,
                                   bar->getContentSize().height);

    //衝突判定
    if (ballRect.intersectsRect(barRect))
    {
        _vy *= -1;
        ball->setPosition(ball->getPositionX() + _vx, ball->getPositionY() + _vy);
    }

}
void HelloWorld::gameOver(CCSprite *sprite)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->end();
    _targets->removeObject(sprite);

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
