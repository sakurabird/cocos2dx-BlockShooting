//
//  LevelMenuItemSprite.h
//  BrockShooting
//
//  Created by sakura on 2014/07/10.
//
//

#ifndef __BrockShooting__LevelMenuItemSprite__
#define __BrockShooting__LevelMenuItemSprite__

#include "cocos2d.h"

class LevelMenuItemSprite : public cocos2d::CCMenuItemSprite
{
private:
    cocos2d::CCObject* m_orgTarget;
    cocos2d::SEL_MenuHandler m_orgSelector;

    void setValuables( CCObject* target, cocos2d::SEL_MenuHandler selector);

public:
    LevelMenuItemSprite();
	~LevelMenuItemSprite();

    static LevelMenuItemSprite * create(CCNode* normalSprite, CCNode* selectedSprite, CCObject* target, cocos2d::SEL_MenuHandler selector);

    void setting(int index, float scaleY);
    
    void onTapThis(cocos2d::CCObject* sender);
};

#endif /* defined(__BrockShooting__LevelMenuItemSprite__) */
