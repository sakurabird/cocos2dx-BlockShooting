//
//  BallSprite.cpp
//  BrockShooting
//
//  Created by sakura on 2014/04/17.
//
//

#include "BallSprite.h"

USING_NS_CC;

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

    CCSprite::setTag(kTagBall);

    CCSprite::setScale(scale);

    initVelocity();

    return true;
}

void BallSprite::initVelocity()
{
    //ランダムで少し速度を変える
    float vx = 22;
    float vy = 22;
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

void BallSprite::bounceBall(CCSize visibleSize)
{
    CCPoint ballPoint = getPosition();

    float vx = getVelocityX();
    float vy = getVelocityY();

    // 壁に当たった時の処理、速度を入れ替える
    if(ballPoint.x > visibleSize.width - getContentSize().width / 2)
    {
        vx = vx * -1 * BALL_FRICTION;
        setVelocityX(vx);
        setPositionX(visibleSize.width - getContentSize().width / 2 );
    }
    else if( ballPoint.x < 0 )
    {
        vx = vx * -1 * BALL_FRICTION;
        setVelocityX(vx);
        setPositionX(0);
    }

    if( ballPoint.y > visibleSize.height - getContentSize().height /2 )
    {
        vy = vy * -1 * BALL_FRICTION;
        setVelocityY(vy);
        setPositionY(visibleSize.height - getContentSize().height);
    }
}

void BallSprite::bounceBall(cocos2d::CCRect rect)
{
    //バーやブロックに当たった時の処理
    CCRect ballRect = boundingBox();

    float vx = getVelocityX();
    float vy = getVelocityY();

    if ( ballRect.getMaxX() < rect.getMinX() ||
        rect.getMaxX() < ballRect.getMinX()) {
        vx = vx * -1 * BALL_FRICTION;
    }
    if ( ballRect.getMaxY() < rect.getMinY() ||
        rect.getMaxY() < ballRect.getMaxY()) {
        vy = vy * -1 * BALL_FRICTION;
    }

    setVelocityX(vx);
    setVelocityY(vy);
    setPosition(ccp(getPositionX() + vx, getPositionY() + vy));
}
