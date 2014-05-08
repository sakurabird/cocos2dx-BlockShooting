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
    CCString* fileName = NULL;
    int n = rand() % 5;
    switch (n) {
        case 1:
            fileName = CCString::create(PNG_BLOCK_BLUE);
            break;
        case 2:
            fileName = CCString::create(PNG_BLOCK_GREEN);
            break;
        case 3:
            fileName = CCString::create(PNG_BLOCK_RED);
            break;
        case 4:
            fileName = CCString::create(PNG_BLOCK_VIOLET);
            break;
        case 5:
            fileName = CCString::create(PNG_BLOCK_YELLOW);
            break;

        default:
            fileName = CCString::create(PNG_BLOCK_BLUE);
    }

    if (!CCSprite::initWithFile(fileName->getCString(), CCRectMake(0, 0, width, height))) {
        return false;
    }
//    if (!CCSprite::initWithFile(fileName->getCString())) {
//        return false;
//    }

    CCSprite::setTag(kTagBlock);

    setNumber(number);

    return true;
}
