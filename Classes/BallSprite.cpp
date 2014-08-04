//
//  BallSprite.cpp
//  BrockShooting
//
//  Created by sakura on 2014/04/17.
//
//

#include "BallSprite.h"
#include "SimpleAudioEngine.h"
#include "Config.h"
#include "UserSettings.h"

USING_NS_CC;
using namespace CocosDenshion;

BallSprite::BallSprite()
:m_vx(0)
,m_vy(0)
{
    srand((unsigned int)time(NULL));
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
	int level = UserSettings::getSelectedLevel() * 2;
    float vx = 10 + level;
    float vy = 15 + level;
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
bool BallSprite::bounceBall(CCSize visibleSize)
{
    bool b = false;
    CCPoint ballPoint = getPosition();

    float vx = getVelocityX();
    float vy = getVelocityY();
    float z = 0;
    int v = rand();
    if (v > 0) {
        z = v * 0.01;
    }
    // TODO
    float friction = BALL_FRICTION + z;

    // 当たった時の処理、速度を入れ替える
    if(ballPoint.x > visibleSize.width - getContentSize().width / 2)
    {
        if (UserSettings::getSESetting())
            SimpleAudioEngine::sharedEngine()->playEffect(MP3_BOUNDWALL);
        vx = vx * -1 * BALL_FRICTION;
//        vx = vx * -1 * friction;
        setVelocityX(vx);
        setPositionX(visibleSize.width - getContentSize().width / 2 );
        b = true;
    }
    else if( ballPoint.x < 0 )
    {
        if (UserSettings::getSESetting())
            SimpleAudioEngine::sharedEngine()->playEffect(MP3_BOUNDWALL);
        vx = vx * -1 * BALL_FRICTION;
//        vx = vx * -1 * friction;
        setVelocityX(vx);
        setPositionX(0);
        b = true;
    }

    if( ballPoint.y > visibleSize.height - getContentSize().height /2 )
    {
        if (UserSettings::getSESetting())
            SimpleAudioEngine::sharedEngine()->playEffect(MP3_BOUNDWALL);
        vy = vy * -1 * BALL_FRICTION;
//        vy = vy * -1 * friction;
        setVelocityY(vy);
        setPositionY(visibleSize.height - getContentSize().height);
        b = true;
    }
    return  b;
}

//バーやブロックに当たった時の処理
bool BallSprite::bounceBall(cocos2d::CCRect rect, kTag tag)
{
    bool b = false;
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
        b = true;
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
        b = true;
    }

    setVelocityX(vx);
    setVelocityY(vy);
    setPosition(ccp(getPositionX() + vx, getPositionY() + vy));

    return b;
}

//　速度を加算する
void BallSprite::addVelocity(int velocity){

    float vx = getVelocityX();
    float vy = getVelocityY();

    if (vx < 0) {
        vx -= velocity;
    }else{
        vx += velocity;
    }

    if (vy < 0) {
        vy -= velocity;
    }else{
        vy += velocity;
    }

    setVelocityX(vx);
    setVelocityY(vy);
}
