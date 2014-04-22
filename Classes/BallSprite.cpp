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

BallSprite* BallSprite::createWithBallScale(float scale)
{
    BallSprite *pRet = new BallSprite();
    if (pRet && pRet->initWithBallScale(scale))
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


bool BallSprite::initWithBallScale(float scale)
{
    if (!CCSprite::initWithFile(PNG_BALL)) {
        return false;
    }

    CCSprite::setTag(TAG_BALL);

    CCSprite::setScale(scale);

    initVelocity();

    return true;
}

void BallSprite::initVelocity()
{
    //ランダムで少し速度を変える
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
