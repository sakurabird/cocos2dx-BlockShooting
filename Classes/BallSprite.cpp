//
//  BallSprite.cpp
//  BrockShooting
//
//  Created by sakura on 2014/04/17.
//
//

#include "BallSprite.h"
#include "SimpleAudioEngine.h"
#include "UserSettings.h"

USING_NS_CC;
using namespace CocosDenshion;

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

//壁に当たった時の処理
void BallSprite::bounceBall(CCSize visibleSize)
{
    CCPoint ballPoint = getPosition();

    float vx = getVelocityX();
    float vy = getVelocityY();

    // 当たった時の処理、速度を入れ替える
    if(ballPoint.x > visibleSize.width - getContentSize().width / 2)
    {
        if (UserSettings::getSESetting())
            SimpleAudioEngine::sharedEngine()->playEffect(MP3_BOUNDWALL);
        vx = vx * -1 * BALL_FRICTION;
        setVelocityX(vx);
        setPositionX(visibleSize.width - getContentSize().width / 2 );
    }
    else if( ballPoint.x < 0 )
    {
        if (UserSettings::getSESetting())
            SimpleAudioEngine::sharedEngine()->playEffect(MP3_BOUNDWALL);
        vx = vx * -1 * BALL_FRICTION;
        setVelocityX(vx);
        setPositionX(0);
    }

    if( ballPoint.y > visibleSize.height - getContentSize().height /2 )
    {
        if (UserSettings::getSESetting())
            SimpleAudioEngine::sharedEngine()->playEffect(MP3_BOUNDWALL);
        vy = vy * -1 * BALL_FRICTION;
        setVelocityY(vy);
        setPositionY(visibleSize.height - getContentSize().height);
    }
}

//バーやブロックに当たった時の処理
void BallSprite::bounceBall(cocos2d::CCRect rect, kTag tag)
{
    CCRect ballRect = boundingBox();

    float vx = getVelocityX();
    float vy = getVelocityY();

    // 当たった時の処理、速度を入れ替える
    if ( ballRect.getMaxX() < rect.getMinX() ||
        rect.getMaxX() < ballRect.getMinX()) {

        if (UserSettings::getSESetting()){
            switch (tag) {
                case kTagBar:
                    SimpleAudioEngine::sharedEngine()->playEffect(MP3_BOUNDBAR);
                    break;
                case kTagBlock:
                    SimpleAudioEngine::sharedEngine()->playEffect(MP3_HITBLOCK);
                    break;
                default:
                    break;
            }
        }
        vx = vx * -1 * BALL_FRICTION;
    }
    if ( ballRect.getMaxY() < rect.getMinY() ||
        rect.getMaxY() < ballRect.getMaxY()) {
        if (UserSettings::getSESetting()){
            switch (tag) {
                case kTagBar:
                    SimpleAudioEngine::sharedEngine()->playEffect(MP3_BOUNDBAR);
                    break;
                case kTagBlock:
                    SimpleAudioEngine::sharedEngine()->playEffect(MP3_HITBLOCK);
                    break;
                default:
                    break;
            }
        }
        vy = vy * -1 * BALL_FRICTION;
    }

    setVelocityX(vx);
    setVelocityY(vy);
    setPosition(ccp(getPositionX() + vx, getPositionY() + vy));
}
