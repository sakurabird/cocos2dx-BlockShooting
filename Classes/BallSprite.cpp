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
    setVelocityX(10);
    setVelocityY(10);
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

    CCSprite::setTag(TAG_BALL);

    initVelocity();

    return true;
}

void BallSprite::initVelocity()
{
    float vx = 18;
    float vy = 18;
    if (rand() % 2 == 0){
        vx *= -1;
    }
    int v = rand() % 10;
    if (v > 0) {
        float z = v * 0.1;
        vx += z;
    }
    v = rand() % 10;
    if (v > 0) {
        float z = v * 0.1;
        vy += z;
    }
    setVelocityX(vx);
    setVelocityY(vy);
}
