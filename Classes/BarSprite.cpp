//
//  BarSprite.cpp
//  BrockShooting
//
//  Created by sakura on 2014/04/21.
//
//

#include "BarSprite.h"

USING_NS_CC;

bool isScaled;

BarSprite::BarSprite()
{
    isScaled = false;
}

BarSprite::~BarSprite()
{
}


BarSprite* BarSprite::createWithBarSize(float width, float height)
{
    BarSprite *pRet = new BarSprite();
    if (pRet && pRet->initWithBarSize(width, height))
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

bool BarSprite::initWithBarSize(float width, float height)
{

    if (!CCSprite::initWithFile(PNG_BAR, CCRectMake(0, 0, width, height))) {
//    if (!CCSprite::initWithSpriteFrameName(PNG_BAR)) {
        return false;
    }

    setTag(kTagBar);
//    float scaleX = width / getContentSize().width;
//    float scaleY = height / getContentSize().height;
//    setScaleX(scaleX);
//    setScaleY(scaleY);
//    setAnchorPoint(ccp(0, 0));
//    setPosition(CCPointZero);
//    setContentSize(ccp(width, height));

    CCLOG("bar.width: %f, height: %f",
          getContentSize().width, getContentSize().height);

    return true;
}

void BarSprite::setScaleLonger()
{
    if (!isScaled) {
        setScaleX(2.0);
        isScaled = true;
    }
}

void BarSprite::restoreScale()
{
    setScaleX(1.0);
    isScaled = false;
}
