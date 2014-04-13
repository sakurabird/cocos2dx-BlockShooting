#include "HelloWorldScene.h"
#include "GameOverScene.h"
#include "SimpleAudioEngine.h"

HelloWorld::~HelloWorld()
{
    CCLOG("~HelloWorld!");

	if (_targets)
	{
		_targets->release();
		_targets = NULL;
	}

	if (_projectiles)
	{
		_projectiles->release();
		_projectiles = NULL;
	}
}

HelloWorld::HelloWorld()
:_targets(NULL)
,_projectiles(NULL)
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

	_targets = new CCArray;
	_projectiles = new CCArray;

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
    CCLOG("makePlayer1");
    CCSprite *player = CCSprite::create("Player.png", CCRectMake(0, 0, 95, 120) );

    //        CCSprite* player = CCSprite::createWithSpriteFrameName("Player.png");//テクスチャアトラスを使用

    CCLOG("Hello player.width: %f, height: %f",player->getContentSize().width,player->getContentSize().height);

    player->setPosition( ccp(_visibleSize.width/2,
                             _origin.y + player->getContentSize().height / 2) );
    player->setTag(TAG_PLAYER);
    this->addChild(player);
    CCLOG("makePlayer2");
}


void HelloWorld::makeBlock()
{
    CCLOG("makeBlock1");
    CCSprite *target = NULL;
    int tag = 0;
    int y = _visibleSize.height;
    for (int i = 0; i < 3; i++) {
        int x = 20;
        for (int j = 0; j < 7; j++) {
            target = CCSprite::create("Target.png", CCRectMake(0,0,80,80) );
            target->setPosition(ccp(x + target->getContentSize().width,
                                y + target->getContentSize().height ));
            this->addChild(target);
            target->setTag(tag++);
            _targets->addObject(target);

            x += target->getContentSize().width + 20;
        }
        y -= target->getContentSize().height - 20;
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
	_projectiles->removeObject(sprite);
}

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    //ボールを出す
	CCTouch* touch = (CCTouch*)( touches->anyObject() ); //マルチタップなので配列になっている
	CCPoint location = touch->getLocation();

	CCLog("++++++++after  x:%f, y:%f", location.x, location.y);


    CCSprite *projectile = CCSprite::create("Projectile.png", CCRectMake(0, 0, 30, 30));

//    CCSprite* projectile = CCSprite::createWithSpriteFrameName("Projectile.png");//テクスチャアトラスを使用

	projectile->setPosition( ccp(_visibleSize.width/2, 150) );

	this->addChild(projectile);

	// Determine where we wish to shoot the projectile to
	CCPoint realDest = ccp(location.x, _visibleSize.height);

	// Determine the length of how far we're shooting
	float offRealX = location.x - projectile->getPosition().x;
	float offRealY = _visibleSize.height - projectile->getPosition().y;
	float length = sqrtf((offRealX * offRealX) + (offRealY*offRealY));
	float velocity = 1200/1; // 1200pixels/1sec
	float realMoveDuration = length/velocity;

	// Move projectile to actual endpoint
	projectile->runAction( CCSequence::create(CCMoveTo::create(realMoveDuration, realDest),
                                              CCCallFuncN::create(this,
                                                                  callfuncN_selector(HelloWorld::spriteMoveFinished)),
                                              NULL) );

	projectile->setTag(TAG_BALL);
	_projectiles->addObject(projectile);

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
}

void HelloWorld::updateGame(float dt)
{
	CCArray *projectilesToDelete = new CCArray;
    CCObject* it = NULL;
    CCObject* jt = NULL;

	// for (it = _projectiles->begin(); it != _projectiles->end(); it++)
    CCARRAY_FOREACH(_projectiles, it)
	{
		CCSprite *projectile = dynamic_cast<CCSprite*>(it);
		CCRect projectileRect = CCRectMake(
                                           projectile->getPosition().x - (projectile->getContentSize().width/2),
                                           projectile->getPosition().y - (projectile->getContentSize().height/2),
                                           projectile->getContentSize().width,
                                           projectile->getContentSize().height);

		CCArray* targetsToDelete =new CCArray;

		// for (jt = _targets->begin(); jt != _targets->end(); jt++)
        CCARRAY_FOREACH(_targets, jt)
		{
			CCSprite *target = dynamic_cast<CCSprite*>(jt);
			CCRect targetRect = CCRectMake(
                                           target->getPosition().x - (target->getContentSize().width/2),
                                           target->getPosition().y - (target->getContentSize().height/2),
                                           target->getContentSize().width,
                                           target->getContentSize().height);

            if (projectileRect.intersectsRect(targetRect))
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
        
		if (targetsToDelete->count() > 0)
		{
			projectilesToDelete->addObject(projectile);
		}
		targetsToDelete->release();
	}
    
    CCARRAY_FOREACH(projectilesToDelete, it)
	{
		CCSprite* projectile = dynamic_cast<CCSprite*>(it);
		_projectiles->removeObject(projectile);
		this->removeChild(projectile, true);
	}
	projectilesToDelete->release();
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
