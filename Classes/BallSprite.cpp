//
//  BallSprite.cpp
//  BrockShooting
//
//  Created by sakura on 2014/04/17.
//
//

#include "BallSprite.h"

using namespace cocos2d;

BallSprite::BallSprite()
{
}

BallSprite::~BallSprite()
{
}


BallSprite* BallSprite::createWithBallSize(float size)
{
    BallSprite *pRet = new BallSprite();
    if (pRet && pRet->initWithBallSize(size))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}


bool BallSprite::initWithBallSize(float size)
{
    if (!CCSprite::initWithFile(PNG_BALL, CCRectMake(0, 0, size, size))) {
        return false;
    }

    CCSprite::

    CCSprite::setTag(TAG_BALL);

    return true;
}
