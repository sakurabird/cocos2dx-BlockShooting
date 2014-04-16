#include "HelloWorldScene.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"
#include "AppMacros.h"

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
,_ballsRemain(3)
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

    _visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    _origin = CCDirector::sharedDirector()->getVisibleOrigin();

    CCLOG("Hello visibleSize.width: %f, height: %f",_visibleSize.width,_visibleSize.height);
    CCLOG("Hello origin.x: %f, origin.y: %f",_origin.x,_origin.y);

    // バーを作成する
    makeBar();

    //ブロックを作成する
    makeBlock();

	this->setTouchEnabled(true);

	this->schedule( schedule_selector(HelloWorld::updateGame) );

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);

    return true;
}

void HelloWorld::makeBar()
{
    float w = _visibleSize.width / 7;
    float h = w / 4;
    float marginBottom = _visibleSize.height / 15;
    CCLOG("Hello bar.w: %f, height: %f",w,h);
    CCSprite *bar = CCSprite::create("bar.png", CCRectMake(0, 0, w, h) );

    //        CCSprite* player = CCSprite::createWithSpriteFrameName("Player.png");//テクスチャアトラスを使用

    CCLOG("Hello bar.width: %f, height: %f",bar->getContentSize().width,bar->getContentSize().height);

    bar->setPosition( ccp(_visibleSize.width / 2,
                             _origin.y + bar->getContentSize().height / 2 + marginBottom) );
    bar->setTag(TAG_BAR);
    this->addChild(bar);
}


void HelloWorld::makeBlock()
{
	_targets = new CCArray;

    float size = _visibleSize.width / 14.0;
    float margin = (_visibleSize.width - (size * _column)) / (_column + 1);
    CCLOG("Hello block.size: %f, margin: %f",size, margin);

    CCSprite *target = NULL;

    int tag = 0;
    int y = _visibleSize.height - (this->getChildByTag(TAG_BAR)->getPositionY());

    for (int i = 0; i < _row; i++) {
        int x = margin;
        for (int j = 0; j < _column; j++) {
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
	CCSprite *sprite = (CCSprite *)sender;
	this->removeChild(sprite, true);

    _ballsRemain--;
    //ゲームオーバー判定
    if (_ballsRemain <= 0) {
        this->gameOver(sprite);
        return;
    }
}

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    //現在ボールが飛んでいなければボールを出す
    if (!this->getChildByTag(TAG_BALL)) {
        pushBall(touches);
    }

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
}

void HelloWorld::pushBall(CCSet* touches)
{
    _vx = 10;
    _vy = 10;

    //ボールを出す
    float size = _visibleSize.width / 17.0;

	CCTouch* touch = (CCTouch*)( touches->anyObject() ); //マルチタップなので配列になっている
	CCPoint location = touch->getLocation();

//	CCLog("++++++++after  x:%f, y:%f", location.x, location.y);

    CCSprite *ball = CCSprite::create("ball.png", CCRectMake(0, 0, size, size));

    //    CCSprite* projectile = CCSprite::createWithSpriteFrameName("Projectile.png");//テクスチャアトラスを使用

//	// Determinie offset of location to projectile
//	float offX = location.x - ball->getPosition().x;
//	float offY = location.y - ball->getPosition().y;

//	// Bail out if we are shooting down or backwards
//	if (offX <= 0) return;

	ball->setPosition( ccp(_visibleSize.width / 2,
                            this->getChildByTag(TAG_BAR)->getPositionY()
                           + this->getChildByTag(TAG_BAR)->getContentSize().height) );
	ball->setTag(TAG_BALL);
	this->addChild(ball);



//	// Determine where we wish to shoot the projectile to
//    float realX = offX - (_origin.x + _visibleSize.width) / 2;
//    realX = realX + (_origin.x + _visibleSize.width) / 2;
//	float ratio = offY / offX;
//	float realY = (realX * ratio) + ball->getPosition().y;
//	CCPoint realDest = ccp(realX, realY);
//
//	// Determine the length of how far we're shooting
//	float offRealX = realX - ball->getPosition().x;
//	float offRealY = realY - ball->getPosition().y;
//	float length = sqrtf((offRealX * offRealX) + (offRealY * offRealY));
//	float velocity = 1300/1;
//	float realMoveDuration = length/velocity;
//
//	// Move projectile to actual endpoint
//	ball->runAction( CCSequence::create(
//                                              CCMoveTo::create(realMoveDuration, realDest),
//                                              CCCallFuncN::create(this,
//                                                                  callfuncN_selector(HelloWorld::spriteMoveFinished)),
//                                              NULL) );



//	CCPoint realDest = ccp(location.x, _visibleSize.height);
//
//	float offRealX = location.x - ball->getPosition().x;
//	float offRealY = _visibleSize.height - ball->getPosition().y;
//	float length = sqrtf((offRealX * offRealX) + (offRealY*offRealY));
//	float velocity = 1300/1; // 1200pixels/1sec
//	float realMoveDuration = length/velocity;
//
//	ball->runAction( CCSequence::create(CCMoveTo::create(realMoveDuration, realDest),
//                                        CCCallFuncN::create(this,
//                                                            callfuncN_selector(HelloWorld::spriteMoveFinished)),
//                                        NULL) );
}

void HelloWorld::updateGame(float dt)
{

    //壁の当たり判定
    updateWalls();

    // ブロックの当たり判定
    updateBlocks();
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
    CCLog("updateWalls ball x:%f, y:%f", ballPoint.x, ballPoint.y);

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

void HelloWorld::gameOver(CCSprite *sprite)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->end();
    _targets->removeObject(sprite);

    GameOverScene *gameOverScene = GameOverScene::create();
    GameOverLayer *gameOverLayer = gameOverScene->getLayer();
    gameOverLayer->setResult(_blocksDestroyed);

    CCDirector::sharedDirector()->replaceScene(gameOverScene);
}

void HelloWorld::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}
