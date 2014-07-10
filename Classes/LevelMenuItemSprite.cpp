//
//  LevelMenuItemSprite.cpp
//  BrockShooting
//
//  Created by sakura on 2014/07/10.
//
//

#include "LevelMenuItemSprite.h"
#include "GHelper.h"
#include "Config.h"

USING_NS_CC;

LevelMenuItemSprite::LevelMenuItemSprite()
: m_orgTarget(NULL)
, m_orgSelector(NULL)
{
}

LevelMenuItemSprite::~LevelMenuItemSprite()
{
}

LevelMenuItemSprite * LevelMenuItemSprite::create(CCNode *normalSprite, CCNode *selectedSprite, CCObject *target, SEL_MenuHandler selector)
{
    LevelMenuItemSprite *pRet = new LevelMenuItemSprite();
    if (pRet && pRet->initWithNormalSprite(normalSprite, normalSprite, NULL, target, menu_selector(LevelMenuItemSprite::onTapThis)))
    {
        pRet->autorelease();
        pRet->setValuables(target, selector);
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

void LevelMenuItemSprite::setValuables(CCObject *target, SEL_MenuHandler selector)
{
    m_orgTarget = target;
    m_orgSelector = selector;
}

void LevelMenuItemSprite::setting(int index, float scaleY)
{
    setScaleX(scaleY);
    setScaleY(scaleY);
    setTag(index);
}

void LevelMenuItemSprite::onTapThis(CCObject *sender)
{
    //メニューのどこかが押された
    LevelMenuItemSprite *sprite = (LevelMenuItemSprite *)sender;
    if (!sprite) return;
    if (gLevelState[0][sprite->getTag()] != 1) {
        return;
    }

    if (sprite->m_orgTarget && sprite->m_orgSelector) {
        // 元のセレクタを呼び出す
        (sprite->m_orgTarget->*sprite->m_orgSelector)(sender);
    }
}