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
,_projectilesDestroyed(0)
,_projectilesRemain(15)
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

    // プレイヤーを作成する
    makePlayer();

    //ブロックを作成する
    makeBlock();

	this->setTouchEnabled(true);

	this->schedule( schedule_selector(HelloWorld::updateGame) );

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);

    return true;
}

void HelloWorld::makePlayer()
{
    float w = _visibleSize.width / 13.0;
    float h = _visibleSize.height / 11.5;
    CCLOG("Hello player.w: %f, height: %f",w,h);
    CCSprite *player = CCSprite::create("Player.png", CCRectMake(0, 0, w, h) );

    //        CCSprite* player = CCSprite::createWithSpriteFrameName("Player.png");//テクスチャアトラスを使用

    CCLOG("Hello player.width: %f, height: %f",player->getContentSize().width,player->getContentSize().height);

    player->setPosition( ccp(_visibleSize.width/2,
                             _origin.y + player->getContentSize().height / 2) );
    player->setTag(TAG_PLAYER);
    this->addChild(player);
}


void HelloWorld::makeBlock()
{
    CCLOG("makeBlock1");
	_targets = new CCArray;

    int column = 7;
    int row = 5;
    float size = _visibleSize.width / 14.0;
    float margin = (_visibleSize.width - (size * column)) / (column + 1);
    CCLOG("Hello block.size: %f, margin: %f",size, margin);

    CCSprite *target = NULL;

    int tag = 0;
    int y = _visibleSize.height - (this->getChildByTag(TAG_PLAYER)->getContentSize().height);

    for (int i = 0; i < row; i++) {
        int x = margin;
        for (int j = 0; j < column; j++) {
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

    _projectilesRemain--;
    //ゲームオーバー判定
    if (_projectilesRemain <= 0) {
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
    //ボールを出す
    float size = _visibleSize.width / 17.0;

	CCTouch* touch = (CCTouch*)( touches->anyObject() ); //マルチタップなので配列になっている
	CCPoint location = touch->getLocation();

	CCLog("++++++++after  x:%f, y:%f", location.x, location.y);

    CCSprite *ball = CCSprite::create("Projectile.png", CCRectMake(0, 0, size, size));

    //    CCSprite* projectile = CCSprite::createWithSpriteFrameName("Projectile.png");//テクスチャアトラスを使用

	ball->setPosition( ccp(_visibleSize.width / 2,
                                 this->getChildByTag(TAG_PLAYER)->getContentSize().height) );
	ball->setTag(TAG_BALL);
	this->addChild(ball);

	CCPoint realDest = ccp(location.x, _visibleSize.height);

	float offRealX = location.x - ball->getPosition().x;
	float offRealY = _visibleSize.height - ball->getPosition().y;
	float length = sqrtf((offRealX * offRealX) + (offRealY*offRealY));
	float velocity = 1200/1; // 1200pixels/1sec
	float realMoveDuration = length/velocity;

	ball->runAction( CCSequence::create(CCMoveTo::create(realMoveDuration, realDest),
                                              CCCallFuncN::create(this,
                                                                  callfuncN_selector(HelloWorld::spriteMoveFinished)),
                                              NULL) );
}

void HelloWorld::updateGame(float dt)
{
    CCObject* jt = NULL;

    CCNode *ball = this->getChildByTag(TAG_BALL);
    if (ball) {
		CCRect ballRect = CCRectMake(
                                    ball->getPosition().x - (ball->getContentSize().width/2),
                                    ball->getPosition().y - (ball->getContentSize().height/2),
                                    ball->getContentSize().width,
                                    ball->getContentSize().height);

        CCArray* targetsToDelete = new CCArray;

        CCARRAY_FOREACH(_targets, jt)
		{
			CCSprite *target = dynamic_cast<CCSprite*>(jt);
            //            CCLOG("updateGame target.x: %f, target.y: %f, tag: %d",target->getContentSize().width, target->getContentSize().height, target->getTag());

			CCRect targetRect = CCRectMake(
                                           target->getPosition().x - (target->getContentSize().width/2),
                                           target->getPosition().y - (target->getContentSize().height/2),
                                           target->getContentSize().width,
                                           target->getContentSize().height);

            //衝突判定
            if (ballRect.intersectsRect(targetRect))
			{
				targetsToDelete->addObject(target);
			}
		}

        CCARRAY_FOREACH(targetsToDelete, jt)
		{
			CCSprite *target = dynamic_cast<CCSprite*>(jt);
			_targets->removeObject(target);
			this->removeChild(target, true);

			_projectilesDestroyed++;
		}

		targetsToDelete->release();

        //壁の当たり判定
    }
}


void HelloWorld::gameOver(CCSprite *sprite)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->end();
    _targets->removeObject(sprite);

    GameOverScene *gameOverScene = GameOverScene::create();
    GameOverLayer *gameOverLayer = gameOverScene->getLayer();
    gameOverLayer->setResult(_projectilesDestroyed);

    CCDirector::sharedDirector()->replaceScene(gameOverScene);
}

void HelloWorld::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}
