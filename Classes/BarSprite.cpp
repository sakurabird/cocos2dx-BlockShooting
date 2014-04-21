//
//  Bar.cpp
//  BrockShooting
//
//  Created by sakura on 2014/04/21.
//
//

#include "BarSprite.h"

using namespace cocos2d;

BarSprite::BarSprite()
{
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
        return false;
    }

    CCSprite::setTag(TAG_BAR);
    CCLOG("bar.width: %f, height: %f",
          CCSprite::getContentSize().width, CCSprite::getContentSize().height);

    return true;
}
