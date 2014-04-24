//
//  BlockSprite.cpp
//  BrockShooting
//
//  Created by sakura on 2014/04/22.
//
//

#include "BlockSprite.h"

USING_NS_CC;

BlockSprite::BlockSprite()
{
}

BlockSprite::~BlockSprite()
{
}


BlockSprite* BlockSprite::createWithBlockSize(float width, float height, int number)
{
    BlockSprite *pRet = new BlockSprite();
    if (pRet && pRet->initWithBlockSize(width, height, number))
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


bool BlockSprite::initWithBlockSize(float width, float height, int number)
{
    if (!CCSprite::initWithFile(PNG_BLOCK, CCRectMake(0, 0, width, height))) {
        return false;
    }

    CCSprite::setTag(TAG_BLOCK);

    setNumber(number);

    return true;
}
